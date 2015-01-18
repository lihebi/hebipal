#ifndef __DRIVER_H__
#define __DRIVER_H__
#include <SDL.h>

int Decompress(const void *source, void *destination, int size);

int mkf_get_chunk_size(uint32_t chunkId, FILE *fp);
int mkf_read_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp);

int mkf_decompress_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp);

int fbp_blit_to_surface(uint8_t* fbp, SDL_Surface *surface);


#endif /* end of include guard: __DRIVER_H__ */
