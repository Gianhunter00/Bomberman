#pragma once
#include <SDL.h>

/**
 * @brief Parse bitmap given the path the mode and the settings to populate
 * 
 * @param path the path of the bitmap
 * @param mode the mode to read the bitmap
 * @param pixels_offset will be stored the offset to the start of the pixels
 * @param width will be stored the width of the bmp
 * @param height will be stored the height of the bmp
 * @param pixels_size will be stored the pixel size
 * @param depth will be stored the depth of the bmp
 * 
 * @return Uint8* content of the bmp
 */
Uint8* load_bmp(char *path, char *mode, Uint32* pixels_offset, Uint32* width, Uint32* height, Uint32* pixels_size, Uint32* depth);

/**
 * @brief Create a texture from BMP data object
 * 
 * @param renderer the renderer in use
 * @param content the content of the bmp
 * @param pixels_offset the offset to the start of the pixels
 * @param pixels_size the pixels size of the bmp
 * @param width the width of the bmp
 * @param height the height of the bmp
 * @param depth the depth of the bmp
 * 
 * @return SDL_Texture* texture created using the bmp as reference
 */
SDL_Texture* create_texture_from_BMP_data(SDL_Renderer* renderer, Uint8* content, Uint32 pixels_offset, Uint32 pixels_size, Uint32 width, Uint32 height, Uint32 depth);

/**
 * @brief Create a texture from BMP file object
 * 
 * @param path the path of the bmp
 * @param mode the mode to read the bmp
 * @param renderer the renderer in use
 * 
 * @return SDL_Texture* texture created using the bmp as reference
 */
SDL_Texture* create_texture_from_BMP_file(char *path, char *mode, SDL_Renderer *renderer);