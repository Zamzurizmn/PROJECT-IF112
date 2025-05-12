#include "image.h"

// Load a P6 PPM image from file
Image* load_ppm(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("File open error");
        return NULL;
    }

    char format[3];
    fscanf(f, "%2s", format);
    if (strcmp(format, "P6") != 0) {
        fprintf(stderr, "Unsupported PPM format (must be P6)\n");
        fclose(f);
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    if (!img) {
        fclose(f);
        return NULL;
    }

    int max_val;
    fscanf(f, "%d %d %d", &img->width, &img->height, &max_val);
    fgetc(f); // consume newline

    if (max_val != 255) {
        fprintf(stderr, "Only max color value 255 supported\n");
        free(img);
        fclose(f);
        return NULL;
    }

    img->data = malloc(img->width * img->height * sizeof(Pixel));
    if (!img->data) {
        free(img);
        fclose(f);
        return NULL;
    }
    fread(img->data, sizeof(Pixel), img->width * img->height, f);
    fclose(f);
    return img;
}

// Save a P6 PPM image to file
void save_ppm(const char *filename, Image *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("File open error for writing");
        return;
    }

    fprintf(f, "P6\n%d %d\n255\n", img->width, img->height);
    fwrite(img->data, sizeof(Pixel), img->width * img->height, f);
    fclose(f);
}

// Calculate histogram of red channel intensity
void calculate_histogram(Image *img, int *histogram) {
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    for (int i = 0; i < img->width * img->height; i++) {
        unsigned char val = img->data[i].r;  // For simplicity use red channel only
        histogram[val]++;
    }
}

// Create a new Huffman tree node
Node* create_node(unsigned char value, int freq) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// A simple priority queue for building the Huffman tree:
typedef struct {
    Node **nodes;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_minheap(int capacity) {
    MinHeap *minheap = malloc(sizeof(MinHeap));
    minheap->size = 0;
    minheap->capacity = capacity;
    minheap->nodes = malloc(capacity * sizeof(Node*));
    return minheap;
}

void minheap_swap(Node **a, Node **b) {
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

// Heapify up
void minheap_insert(MinHeap *h, Node* node) {
    int i = h->size++;
    h->nodes[i] = node;

    while (i && h->nodes[(i - 1) / 2]->freq > h->nodes[i]->freq) {
        minheap_swap(&h->nodes[i], &h->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Heapify down
void minheapify(MinHeap *h, int idx) {
    int smallest = idx;
    int left = 2*idx + 1;
    int right = 2*idx + 2;

    if (left < h->size && h->nodes[left]->freq < h->nodes[smallest]->freq)
        smallest = left;
    if (right < h->size && h->nodes[right]->freq < h->nodes[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        minheap_swap(&h->nodes[smallest], &h->nodes[idx]);
        minheapify(h, smallest);
    }
}

Node* minheap_extract(MinHeap *h) {
    Node* root = h->nodes[0];
    h->nodes[0] = h->nodes[--h->size];
    minheapify(h, 0);
    return root;
}

// Build the Huffman tree given frequency histogram
Node* build_huffman_tree(int *histogram, int size) {
    MinHeap *heap = create_minheap(size);

    // Add all nodes with freq > 0
    for (int i = 0; i < size; i++) {
        if (histogram[i] > 0) {
            Node* node = create_node((unsigned char)i, histogram[i]);
            minheap_insert(heap, node);
        }
    }

    while (heap->size > 1) {
        Node* left = minheap_extract(heap);
        Node* right = minheap_extract(heap);

        Node* new_node = create_node(0, left->freq + right->freq);
        new_node->left = left;
        new_node->right = right;

        minheap_insert(heap, new_node);
    }

    Node* root = minheap_extract(heap);
    free(heap->nodes);
    free(heap);
    return root;
}

// Recursive code generator from Huffman tree
void generate_codes(Node *root, char codes[256][256], char *code, int depth) {
    if (!root)
        return;

    // Leaf node?
    if (!root->left && !root->right) {
        code[depth] = '\0';
        strcpy(codes[root->value], code);
        return;
    }

    // Traverse left child
    if (root->left) {
        code[depth] = '0';
        generate_codes(root->left, codes, code, depth + 1);
    }
    // Traverse right child
    if (root->right) {
        code[depth] = '1';
        generate_codes(root->right, codes, code, depth + 1);
    }
}

// Free the Huffman tree recursively
void free_huffman_tree(Node *root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

// Compress image by writing codes as ascii (for simplicity)
void compress_image(Image *img, char codes[256][256], const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Failed to open file for compression");
        return;
    }

    // Simple header for reconstruction (width height)
    fprintf(f, "%d %d\n", img->width, img->height);

    // Write codes for ASCII 256 characters (only code lengths)
    for (int i = 0; i < 256; i++) {
        if (codes[i][0] != '\0')
            fprintf(f, "%d %s\n", i, codes[i]);
    }
    fprintf(f, "-1\n"); // Marker for end of codes

    // Write encoded image data by replacing pixels with codes
    for (int i = 0; i < img->width * img->height; i++) {
        fputs(codes[img->data[i].r], f); // Using red channel only for compression
    }

    fclose(f);
}
