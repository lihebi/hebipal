#ifndef __DRIVER_H__
#define __DRIVER_H__
#include <SDL.h>

int Decompress(const void *source, void *destination, int size);

int mkf_get_chunk_size(uint32_t chunkId, FILE *fp);
int mkf_read_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp);

int mkf_decompress_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp);

int fbp_blit_to_surface(uint8_t* fbp, SDL_Surface *surface);
int rle_blit_to_surface(const uint8_t *bitmap, SDL_Surface *surface, int dx, int dy);

uint rle_get_width(const uint8_t *bitmap);
uint rle_get_height(const uint8_t *bitmap);
SDL_Color *get_palette(int paletteId);


#endif /* end of include guard: __DRIVER_H__ */
