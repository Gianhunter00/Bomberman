#include <SDL.h>
#include <stdio.h>
#include "..\Include\parse_bmp.h"

Uint8 *load_bmp(char *path, char *mode, Uint32 *pixels_offset, Uint32 *width, Uint32 *height, Uint32 *pixels_size, Uint32 *depth)
{
    if(!pixels_offset || !width || !height || !pixels_size || !depth)
        return NULL;
    SDL_RWops *file = SDL_RWFromFile(path, mode);
    if (file == NULL)
        return NULL;
    Sint64 size = SDL_RWsize(file);
    Uint8 *content = SDL_malloc(size);
    SDL_RWread(file, content, size, 1);
    SDL_RWclose(file);
    Uint32 bmp_pixels_offset;
    SDL_memcpy(&bmp_pixels_offset, content + 10, 4);
    *pixels_offset = bmp_pixels_offset;
    Uint32 image_size[2];
    SDL_memcpy(image_size, content + 18, 8);
    *width = image_size[0];
    *height = image_size[1];
    int bmp_depth = 0;
    SDL_memcpy(&bmp_depth, content + 28, 2);
    *depth = bmp_depth;
    Uint32 bmp_pixels_size;
    SDL_memcpy(&bmp_pixels_size, content + 34, 4);
    *pixels_size = bmp_pixels_size;
    return content;
}

SDL_Texture *create_texture_from_BMP_data(SDL_Renderer *renderer, Uint8 *content, Uint32 pixels_offset, Uint32 pixels_size, Uint32 width, Uint32 height, Uint32 depth)
{
    if(renderer == NULL || content == NULL) return NULL;
    const Uint32 alignment = 4;
    if(depth != 24 && depth != 32)
        return NULL;
    Uint32 row_size = width * (depth / 8);
    Uint32 padded_row_size = (row_size / alignment) * alignment;
    if (row_size % alignment != 0)
    {
        padded_row_size += alignment;
    }
    Uint32 pixel_format = depth == 32 ? SDL_PIXELFORMAT_BGRA32 : SDL_PIXELFORMAT_BGR24;
    Uint32 full_padded_size = padded_row_size * height;
    SDL_Texture *texture = SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, width, height);
    Uint8 *pixels_ptr;
    int pitch;
    SDL_LockTexture(texture, NULL, (void **)&pixels_ptr, &pitch);
    for (Uint32 y = 0; y < height; y++)
    {
        Uint8 *texture_row = pixels_ptr + y * pitch;
        Uint8 *bitmap_row = content + pixels_offset + pixels_size - (y + 1) * padded_row_size;
        SDL_memcpy(texture_row, bitmap_row, row_size);
    }
    SDL_UnlockTexture(texture);
    if (pixel_format == SDL_PIXELFORMAT_BGRA32)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_free(content);
    return texture;
}
SDL_Texture *create_texture_from_BMP_file(char *path, char *mode, SDL_Renderer *renderer)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    Uint8 *content = load_bmp(path, mode, &px_offset, &width, &height, &px_size, &depth);
    SDL_Texture *texture = create_texture_from_BMP_data(renderer, content, px_offset, px_size, width, height, depth);
    return texture;
}