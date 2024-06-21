#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"

int main(void) {
    FILE *in = fopen("in.txt", "rb");
    if (in == NULL) {
        goto exit_error_in;
    }
    FILE *out = fopen("out.txt", "wb");
    if (out == NULL) {
        goto exit_error_out;
    }

    switch (getc(in)) {
    case 'd':
        if (Decode(in, out)) {
            goto exit_error_huffman;
        }
        break;
    case 'c':
        if (Encode(in, out)) {
            goto exit_error_huffman;
        }
        break;
    default:
        fprintf(stderr, "Unknown command\n");
        goto exit_error_huffman;
    }

    fclose(out);
    fclose(in);
    return EXIT_SUCCESS;

exit_error_huffman:
    fclose(out);
exit_error_out:
    fclose(in);
exit_error_in:
    return EXIT_FAILURE;
}
