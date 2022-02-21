#pragma once
#include <SDL.h>

Uint8* load_bmp(char *path, char *mode, Uint32* pixels_offset, Uint32* width, Uint32* height, Uint32* pixels_size, Uint32* depth);
SDL_Texture* create_texture_from_BMP_data(SDL_Renderer* renderer, Uint8* content, Uint32 pixels_offset, Uint32 pixels_size, Uint32 width, Uint32 height, Uint32 depth);
SDL_Texture* create_texture_from_BMP_file(char *path, char *mode, SDL_Renderer *renderer);