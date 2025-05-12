#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input.ppm output.huff\n", argv[0]);
        return 1;
    }

    Image *img = load_ppm(argv[1]);
    if (!img) {
        fprintf(stderr, "Error: Failed to load image\n");
        return 1;
    }

    int histogram[256];
    calculate_histogram(img, histogram);

    Node* root = build_huffman_tree(histogram, 256);
    if (!root) {
        fprintf(stderr, "Error: Huffman tree could not be built\n");
        free(img->data);
        free(img);
        return 1;
    }

    char codes[256][256] = {{0}};
    char code[256];
    generate_codes(root, codes, code, 0);

    compress_image(img, codes, argv[2]);

    free_huffman_tree(root);
    free(img->data);
    free(img);

    printf("Compression completed successfully.\n");
    return 0;
}
