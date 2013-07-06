#ifndef __RLE_H__
#define __RLE_H__

void rle_compress( char *input, char *output, size_t in, size_t *out );
void rle_decompress( char *input, char *output, size_t in, size_t *out );

#endif