#include "driver.h"
#include <iostream>

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
void hebiprint(const uint8_t *bitmap) {
  int i=0;
  for (i=0;i<20;i++) {
    printf("%x ", bitmap[i]);
  }
  printf("\n");
}
int rle_blit_to_surface(const uint8_t *bitmap, SDL_Surface *surface, int dx, int dy) {
  // printf("%x %x %x %x\n", bitmap[0],bitmap[1],bitmap[2],bitmap[3]);
  // hebiprint(bitmap);
  uint i, j;
  int x, y;
  uint uiLen = 0;
  uint uiWidth = 0;
  uint uiHeight = 0;
  uint8_t T;

  // Skip the 0x00000002 in the file header.
  if (bitmap[0] == 0x02 && bitmap[1] == 0x00 &&
    bitmap[2] == 0x00 && bitmap[3] == 0x00)
  {
    bitmap += 4;
  }
  // Get the width and height of the bitmap.
  uiWidth = bitmap[0] | (bitmap[1] << 8);
  uiHeight = bitmap[2] | (bitmap[3] << 8);
  // Calculate the total length of the bitmap.
  // The bitmap is 8-bpp, each pixel will use 1 byte.
  uiLen = uiWidth * uiHeight;
  // Start decoding and blitting the bitmap.
  bitmap += 4;
  for (i = 0; i < uiLen;) {
    T = *bitmap++;
    if ((T & 0x80) && T <= 0x80 + uiWidth) {
      i += T - 0x80;
    } else {
      for (j = 0; j < T; j++) {
        //
        // Calculate the destination coordination.
        // FIXME: This could be optimized
        //
        y = (i + j) / uiWidth + dy;
        x = (i + j) % uiWidth + dx;

        //
        // Skip the points which are out of the surface.
        //
        if (x < 0) {
          j += -x - 1;
          continue;
        } else if (x >= surface->w) {
          j += x - surface->w;
          continue;
        }

        if (y < 0) {
          j += -y * uiWidth - 1;
          continue;
        } else if (y >= surface->h) {
          goto end; // No more pixels needed, break out
        }

        //
        // Put the pixel onto the surface (FIXME: inefficient).
        //
        ((uint8_t*)surface->pixels)[y * surface->pitch + x] = bitmap[j];
      }
      bitmap += T;
      i += T;
    }
  }

  end:
  return 0;
}
