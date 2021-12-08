#include "../inc/huff.h"

int main(int argc, char **argv){
    if (argc == 1){
        fprintf(stderr, "A file is needed to start the execution.\
        \nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
        exit(BAD_ARGUMENTS);
    }
    uint8_t opt;
    for (opt = 1; opt < argc - 1; opt++){
        // Possible arguments for the execution.
        if (!strcmp(argv[opt], "-c"))
            compression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-d"))
            decompression(argv[argc - 1]);
        else if (!strcmp(argv[opt], "-h") || !strcmp(argv[opt], "--help"))
            help();
        else
            printf("Bad option.\nTry './bin/exe -h <file>' or './bin/exe --help <file>' for more information.\n");
    }
    /* With no option, the file is compressed and decompressed.
     But to execute './bin/exe -h' or './bin/exe --help' is allowed and it displays help message. */
    if (opt == 1){
        if (strcmp(argv[opt], "-h") || strcmp(argv[opt], "--help"))
            help();
        else{
            compression(argv[argc - 1]);
            decompression(argv[argc - 1]);
        }
    }
    return EXIT_SUCCESS;
}

void compression(char *file){
    // Useful to get the execution time.
    clock_t begin = clock();
    // Pointer for the number of different characters in the file to avoid complications in the returns.
    uint8_t *nb_char = calloc(1, sizeof *nb_char);
    // Initialization of a dictionary which will contains characters, their occurrences, their encoding and its size.
    Data dict[CHAR_MAX] = {{0}, {0}, {0}, {0}};
    /* As we get all the characters and their occurrences in the dictionary,
    we can build the Huffman tree of the file and that even more easily with a priority queue. */
    Tree huffTree = buildHuffmanTree(findCharFile(file, dict, nb_char));
    // With the huffman tree we can get the encoding of the character by going through the tree.
    getCharEncoding(huffTree, dict, 0, 0);
    freeTree(huffTree);
    // Finally, we can compress the file.
    makeCompressFile(dict, file, nb_char);
	free(nb_char);
    clock_t end = clock();
    printf("\nCompression completed in %ld ms\n", (end - begin) * 1000 / CLOCKS_PER_SEC);
}

PriorityQueue findCharFile(char *file, Data *dict, uint8_t *nb_char){
    unsigned char c;
    FILE *f = fopen(file, "r");
    if (!f){
        fprintf(stderr, "It seems that the file '%s' does not exist", file);
        exit(FILE_DOES_NOT_EXIST);
    }
    // Full reading of the original file to get chars and especially their occurrences.
    while (fread(&c, sizeof(unsigned char), 1, f)){
        dict[c].value = c;
        dict[c].occur++;
    }
    fclose(f);
    PriorityQueue dictQueue = NULL;
    /* Filling of the priority queue only with characters presents in the file.
    Priority queue already contains nodes of the huffman Tree to make construction easier. */
    for (int i = 0; i < CHAR_MAX; i++, dict++){
        if (dict->occur){
            dictQueue = push(dictQueue, createTree(*dict));
            (*nb_char)++;
        }
    }
    return dictQueue;
}

Tree buildHuffmanTree(PriorityQueue ptrQ){
    /* As the concept of huffman tree's construction is based on recursive grouping of smallest elements.
    The use of a priority queue allows to simply take the first two elements and add the formed tree in the
    queue and repeat this until only one element remains. */
    while (ptrQ->next){
        ptrQ = push(ptrQ, createOverTree(ptrQ->pt, ptrQ->next->pt));
        ptrQ = pull(ptrQ); ptrQ = pull(ptrQ);
    }
    // In order to free memory for the priority queue, we use temporary variable.
    Tree tmp = ptrQ->pt;
    free(ptrQ);
    ptrQ = NULL;
    return tmp;
}

void getCharEncoding(Tree pt, Data *dict, unsigned encode, uint8_t size){
    // To get the character's encoding, a pre-order traversal is sufficient.
    if(pt){
        /* It's useless to check existence of branches because by construction,
        node's value is null and leaf value is a character. */
        if(pt->data.value != '\0'){
            dict[(int) pt->data.value].encoding = encode; dict[(int) pt->data.value].size_encoding = size;
        }
        // On the left we append a zero at the end of the encoding.
        getCharEncoding(pt->left, dict, encode << 1, size + 1);
        // On the right we append a one at the end of the encoding.
        getCharEncoding(pt->right, dict, (encode << 1) + 1, size + 1);
    }
}

void makeCompressFile(Data *dict, char *file, uint8_t *nb_char){
    unsigned char c = 0;
    unsigned buffer = 0, buffer_size = 0, binary = 0;
    // These sizes are just useful to get the size of original and compressed file.
    double sizeO = 0, sizeC = 0;
    FILE *f = fopen(file,"r+");
    // Removing of the extension of the file to append .huf extension.
    removeExt(file);
    FILE *cf = fopen(strcat(file, ".huf"), "wb");
    // Writing of the number of different char, the characters and their occurrences in the header of the compressed file.
    fwrite(nb_char, sizeof(uint8_t), 1, cf);
    for (uint16_t i = 0; i < CHAR_MAX; i++){
        if (dict[i].occur){
            fwrite(&dict[i].value, sizeof(char), 1, cf);
            fwrite(&dict[i].occur, sizeof(unsigned), 1, cf);
        }
    }
    while(fread(&c, sizeof(unsigned char), 1, f)){
        // Retrieval of a byte.
        sizeO += 8;
        // Append in the buffer the encoding corresponding to the retrieved character.
        buffer <<= dict[c].size_encoding;
        buffer |= dict[c].encoding;
        // Increment the size of the buffer by the size of the added encoding.
        buffer_size += dict[c].size_encoding;
        // Writing of at least one byte obtained with the sequence of encodings.
        while(buffer_size >= 8){
            buffer_size -= 8;
            // We write the first byte from the left.
            binary = buffer >> buffer_size;
            fwrite(&binary, 1, 1, cf);
            // One byte has been written in the compressed file.
            sizeC += 8;
        }
    }
    // Processing of last bit.
    if (buffer_size){
        // Due to the last bits' number at most seven.
        buffer <<= (7 - buffer_size);
        // Another shift to get last sequence of encodings.
        binary = buffer >> 8;
        fwrite(&binary, 1, 1, cf) ;
    }
    // Determination of the best bits prefix for files' size.
    Prefix sizefiles[2] = {UNIT_PREFIX(sizeO / 8), UNIT_PREFIX(sizeC / 8)};
    printf("Original size: %.2lf %s\nCompressed size: %.2lf %s\nEfficiency: %.1lf%%\n",
                sizefiles[0].n, sizefiles[0].prf, sizefiles[1].n, sizefiles[1].prf, 100 - (100 * sizeC / sizeO));
    fclose(f); fclose(cf);
}

void decompression(char *file){
    clock_t begin = clock();
    FILE *f = fopen(file, "rb");
    uint8_t size;
    // Retrieving of the different characters' number.
    fread(&size, sizeof(uint8_t), 1, f);
    Data dict[size];
    // Filling the dictionary.
    getDictionary(f, dict, size);
    // As in the compression, we re-create the priority and then the huffman tree.
    PriorityQueue dictQueue = NULL;
    for (uint8_t i = 0; i < size; i++){
        dictQueue = push(dictQueue, createTree(dict[i]));
    }
    Tree huffTree = buildHuffmanTree(dictQueue);
    // Now decompression can be done.
    makeDecompressFile(f, huffTree, file);
    freeTree(huffTree);
    fclose(f);
    clock_t end = clock();
    printf("Decompression completed in %ld ms\n", (end - begin) * 1000 / CLOCKS_PER_SEC);

}

void getDictionary(FILE *file, Data *dict, uint8_t size){
    // Get characters and their occurrences as they have been added in the compressed file.
    for (uint8_t i = 0; i < size; i++){ 
        fread(&dict[i].value, sizeof(char), 1, file); 
        fread(&dict[i].occur, sizeof(unsigned), 1, file);
    }
}

void makeDecompressFile(FILE *file, Tree t, char *n_file){ 
    // Removing of the file's extension to append later .dcm extension in the name of decompressed file.
    removeExt(n_file);
    FILE *df = fopen(strcat(n_file, ".dcm"), "w+");
    uint8_t buffer_size = 0; int8_t i;
    unsigned char c = 0;
    // The total number of characters in the original file is represented by nb_char.
    unsigned buffer = 0, nb_char = t->data.occur;
    // We use a temporary tree to get back to the root.
    Tree tmp = t;
    while (fread(&c, sizeof(uint8_t), 1, file) == 1){
        // Retrieving one byte which contains or will complete an encoding else.
        buffer <<= 8; 
        buffer |= c;
        buffer_size += 8;
        /* Pathway of the bit sequence from the left, from the first "unused" bit 
        and until 0-th bit or until all characters have been written. */
        for (i = buffer_size - 1; i >= 0 && nb_char; i--){
            /* Get the i-th bit from the buffer.
            If it is 0 we go to left branch of the node else we go to right branch. */
            if (BIT_EXTRACTION(buffer, i)) tmp = tmp->right;
            else tmp = tmp->left;
            // If a leaf is encountered then we write to corresponding character. 
            if (!tmp->right && !tmp->left){
                fwrite(&tmp->data.value, sizeof(unsigned char), 1, df);
                buffer_size = i; 
                // One character has been written
                nb_char--;
                // We return to the root of the huffman tree.
                tmp = t;
            }
        }
        // To avoid reading again last bits which form no encoding we return to the root   
        tmp = t;
    }
    fclose(df);
}