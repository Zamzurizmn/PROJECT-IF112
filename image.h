#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width;
    int height;
    Pixel *data;
} Image;

typedef struct Node {
    unsigned char value;  // Pixel intensity (0-255)
    int freq;            // Frequency for this intensity
    struct Node *left;   // Left child for Huffman Tree
    struct Node *right;  // Right child for Huffman Tree
} Node;

// Image I/O
Image* load_ppm(const char *filename);
void save_ppm(const char *filename, Image *img);

// Histogram
void calculate_histogram(Image *img, int *histogram);

// Huffman Tree
Node* create_node(unsigned char value, int freq);
Node* build_huffman_tree(int *histogram, int size);
void generate_codes(Node *root, char codes[256][256], char *code, int depth);
void free_huffman_tree(Node *root);

// Compression (simple output)
void compress_image(Image *img, char codes[256][256], const char *filename);

#endif // IMAGE_H
