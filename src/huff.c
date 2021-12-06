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
            decompression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-h") || !strcmp(argv[opt], "--help"))
            help();
        else
            printf("Bad option.\nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
    }
    if (opt == 1){  // just file name
        compression(argv[argc - 1]);
        decompression(argv[argc - 1]);
    }
    return EXIT_SUCCESS;
}

void compression(char *file){
    clock_t begin = clock();
    uint8_t *nb_char = calloc(1, sizeof *nb_char);
    Data dict[CHAR_MAX] = {{0}, {0}, {0}, {0}};
    Tree huffTree = buildHuffmanTree(findCharFile(file, dict, nb_char));
    getCharEncoding(huffTree, dict, 0, 0);
    freeTree(huffTree);
    makeCompressFile(dict, file, nb_char);
	free(nb_char);
    clock_t end = clock();
    printf("\nCompression completed in %ld ms\n", (end - begin) * 1000 / CLOCKS_PER_SEC);
}

PriorityQueue findCharFile(char *file, Data *dict, uint8_t *nb_char){
    for (uint16_t i = 0; i < CHAR_MAX; i++)
        dict[i].value = (char) i;
    short c;
    FILE *f = fopen(file, "r");
    if (!f){
        fprintf(stderr, "It seems that the file '%s' does not exist", file);
        exit(FILE_DOES_NOT_EXIST);
    }
    while ((c = getc(f)) != EOF)
        dict[c].occur++;
    fclose(f);
    PriorityQueue dictQueue = NULL;
    for (int i = 0; i < CHAR_MAX; i++, dict++){
        if (dict->occur){
            dictQueue = push(dictQueue, createTree(*dict));
            (*nb_char)++;
        }
    }
    return dictQueue;
}

Tree buildHuffmanTree(PriorityQueue ptrQ){
    while (ptrQ->next){
        ptrQ = push(ptrQ, createOverTree(ptrQ->pt, ptrQ->next->pt));
        ptrQ = pull(ptrQ); ptrQ = pull(ptrQ);
    }
    Tree tmp = ptrQ->pt;
    free(ptrQ);
    return tmp;
}

void getCharEncoding(Tree pt, Data *dict, unsigned encode, uint8_t size){
    if(pt){
        if(pt->data.value != '\0'){
            dict[(int) pt->data.value].encoding = encode; dict[(int) pt->data.value].size_encoding = size;
        }
        getCharEncoding(pt->left, dict, encode << 1, size + 1);
        getCharEncoding(pt->right, dict, (encode << 1) | 1, size + 1);
    }
}

void makeCompressFile(Data *dict, char *file, uint8_t *nb_char){
    int8_t c = 0, buff_size = 0;
    unsigned buffer = 0, binary = 0;
    float sizeO = 0, sizeC = 0;
    FILE *f = fopen(file,"r+"), *cf = fopen(strcat(file, ".huf"), "wb");
    fwrite(nb_char, sizeof(uint8_t), 1, cf);
    for (uint16_t i = 0; i < CHAR_MAX; i++){
        if (dict[i].occur){
            fwrite(&dict[i].value, sizeof(char), 1, cf);
            fwrite(&dict[i].occur, sizeof(unsigned), 1, cf);
        }
    }
    while((c = getc(f)) != EOF){
        sizeO += 8;
        buffer <<= dict[c].size_encoding;
        buffer |= dict[c].encoding;
        buff_size += dict[c].size_encoding;
        while(buff_size >= 8){
            buff_size -= 8;
            binary = buffer >> buff_size;
            fwrite(&binary, 1, 1, cf);
            sizeC += 8;
        }
    }
    if (buff_size){
        buffer <<= (8 - buff_size);
        binary = buffer >> 8;
        fwrite(&binary, 1, 1, cf) ;
    }
    Prefix sizefiles[2] = {UNIT_PREFIX(sizeO / 8), UNIT_PREFIX(sizeC / 8)};
    printf("Original size: %.2f %s\nCompressed size: %.2f %s\nEfficiency: %.1f%%\n",
                sizefiles[0].n, sizefiles[0].prf, sizefiles[1].n, sizefiles[1].prf, 100 - (100 * sizeC / sizeO));
    fclose(f); fclose(cf);
}

void decompression(char *file){
    clock_t begin = clock();
    FILE *f = fopen(file, "rb");
    uint8_t size;
    fread(&size, sizeof(uint8_t), 1, f);
    Data dict[size];    
    getDictionary(f, dict, size);
    PriorityQueue dictQueue = NULL;
    for (uint8_t i = 0; i < size; i++){
        dictQueue = push(dictQueue, createTree(dict[i]));
    }
    Tree huffTree = buildHuffmanTree(dictQueue);
    makeDecompressFile(f, huffTree, file);
    freeTree(huffTree);
    fclose(f);
    clock_t end = clock();
    printf("Decompression completed in %ld ms\n", (end - begin) * 1000 / CLOCKS_PER_SEC);

}

void getDictionary(FILE *file, Data *dict, uint8_t size){
    for (uint8_t i = 0; i < size; i++){ 
        fread(&dict[i].value, sizeof(char), 1, file); 
        fread(&dict[i].occur, sizeof(unsigned), 1, file);
    }
}

void makeDecompressFile(FILE *file, Tree t, char *n_file){
    FILE *df = fopen(strcat(n_file, ".dcm"), "w+");
    uint8_t buffer_size = 0, c; int8_t i;
    unsigned buffer = 0, nb_char = t->data.occur;
    Tree tmp = t;
    while (fread(&c, sizeof(uint8_t), 1, file) == 1){
        buffer <<= 8; 
        buffer_size += 8;
        buffer |= c;
        for (i = buffer_size - 1; i >= 0 && nb_char; i--){
            if (BIT_EXTRACTION(buffer, i)) tmp = tmp->right;
            else tmp = tmp->left;
            if (!tmp->right && !tmp->left){
                fprintf(df, "%c", tmp->data.value);
                buffer_size = i; 
                nb_char--;
                tmp = t;
            }
        }
        tmp = t;
    }
    fclose(df);
}