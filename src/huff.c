#include "../inc/huff.h"

int main(int argc, char **argv){
    if (argc == 1){
        fprintf(stderr, "A file is needed to start the execution.\
        \nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
        exit(BAD_ARGUMENTS);
    }
    uint8_t opt;
    for (opt = 1; opt < argc - 1; opt++){
        if (!strcmp(argv[opt], "-c"))
            compression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-d"))
            ;//decompression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-h") || !strcmp(argv[opt], "--help"))
            help();
        else
            printf("Bad option.\nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
    }
    if (opt == 1){  // just file name
        compression(argv[argc - 1]);
        //decompression(concat(argv[argc - 1], ".hfm"));
    }
    return EXIT_SUCCESS;
}

void compression(char* file){
    clock_t begin = clock();
    Data dict[CHAR_MAX] = {{0}, {0}, {0}, {0}};
    Tree huffTree = buildHuffmanTree(findCharFile(file, dict));
    getCharEncoding(huffTree, dict, 0, 0);
    freeTree(huffTree);
    makeCompressFile(dict, file);
	clock_t end = clock();
    printf("\nDuration : %ld ms\n", (end - begin) * 1000 / CLOCKS_PER_SEC);
}

PriorityQueue findCharFile(char* file, Data* dict){
    for (int i = 0; i < CHAR_MAX; i++)
        dict[i].value = (char) i;
    int8_t c;
    FILE *f = fopen(file, "r");
    while ((c = getc(f)) != EOF)
        dict[c].occur++;
    fclose(f);
    PriorityQueue dictQueue = NULL;
    for (int i = 0; i < CHAR_MAX; i++, dict++){
        if (dict->occur)
            dictQueue = push(dictQueue, createTree(dict->occur, dict->value));
    }
    return dictQueue;
}

Tree buildHuffmanTree(PriorityQueue dict){
    while (dict->next){
        dict = push(dict, createOverTree(dict->pt, dict->next->pt));
        dict = pull(dict); dict = pull(dict);
    }
    Tree tmp = dict->pt;
    free(dict);
    return tmp;
}


void getCharEncoding(Tree pt, Data* dict, unsigned encode, uint8_t size){
    if(pt){
        if(pt->value!='\0'){
            dict[(int) pt->value].encoding = encode; dict[(int) pt->value].size_encoding = size;
        }
        getCharEncoding(pt->left, dict, encode << 1, size + 1);
        getCharEncoding(pt->right, dict, (encode << 1) | 1, size + 1);
    };
}

void makeCompressFile(Data* dict, char* file){
    int8_t c = 0, buff_size = 0;
    unsigned buffer = 0, binary = 0;
    float sizeO = 0, sizeC = 0;
    FILE *f = fopen(file,"r+"), *hf = fopen(strcat(file, ".huf"), "wb");
    for (short i = 0; i < CHAR_MAX; i++)
        dict[i].occur ? fprintf(hf, "%c|%u|%u|", dict[i].value, dict[i].encoding, dict[i].size_encoding): 0;
    while((c = getc(f)) != EOF){
        sizeO += 8;
        buffer <<= dict[c].size_encoding;
        buffer |= dict[c].encoding;
        buff_size += dict[c].size_encoding;
        while(buff_size >= 8){
            buff_size -= 8;
            binary = buffer >> buff_size;
            fwrite(&binary, 1, 1, hf);
            sizeC += 8;
        }
    }
    if (buff_size){
        buffer <<= (8 - buff_size);
        binary = buffer >> 8;
        fwrite(&binary, 1, 1, hf) ;
    }
    printf("Original size: %.f bits\nCompressed size: %.f bits\nEfficiency: %.2f %%", sizeO / 8, sizeC / 8, (100 * sizeC / sizeO));
    fclose(f); fclose(hf);
}