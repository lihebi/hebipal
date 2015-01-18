#include "driver.h"

int mkf_get_chunk_size(uint32_t chunkId, FILE *fp) {
  uint32_t offset = 0;
  uint32_t nextOffset = 0;
  fseek(fp, 4*chunkId, SEEK_SET);
  fread(&offset, sizeof(uint32_t), 1, fp);
  fread(&nextOffset, sizeof(uint32_t), 1, fp);
  return nextOffset - offset;
}
int mkf_read_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp) {
  uint32_t offset=0,nextOffset=0,chunkLen;
  fseek(fp, 4*chunkId, SEEK_SET);
  fread(&offset, sizeof(uint32_t), 1, fp);
  fread(&nextOffset, sizeof(uint32_t), 1, fp);
  chunkLen = nextOffset - offset;
  fseek(fp, offset, SEEK_SET);
  fread(buffer, chunkLen, 1, fp);
  return (int)chunkLen;
}

int mkf_decompress_chunk(uint8_t *buffer, uint32_t size, uint32_t chunkId, FILE *fp) {
  uint8_t *buf;
  int len;
  len = mkf_get_chunk_size(chunkId, fp);
  // if (len<=0) return len;
  buf = (uint8_t*)malloc(len);
  // if (!buf) return -3;
  mkf_read_chunk(buf, len, chunkId, fp);
  len = Decompress(buf, buffer, size);
  free(buf);
  return len;
}

int fbp_blit_to_surface(uint8_t* fbp, SDL_Surface *surface) {
  int x,y;
  uint8_t *p;
  for (y=0;y<200;y++) {
    p = (uint8_t*)(surface->pixels) + y*surface->pitch;
    for (x=0;x<320;x++) {
      *(p++) = *(fbp++);
    }
  }
  return 0;
}
