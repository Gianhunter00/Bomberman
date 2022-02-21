#include "clove.h"
#include "../../Include/parse_bmp.h"

CLOVE_TEST(loadBmpNullPath)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    CLOVE_NULL(load_bmp(NULL, "rb", &px_offset, &width, &height, &px_size, &depth));
}

CLOVE_TEST(loadBmpNullMode)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    CLOVE_NULL(load_bmp("bin\\Test.bmp", NULL, &px_offset, &width, &height, &px_size, &depth));
}

CLOVE_TEST(loadBmpNullAnyInfoParams)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    CLOVE_NULL(load_bmp("bin\\Test.bmp", "rb", NULL, &width, NULL, &px_size, &depth));
}

CLOVE_TEST(loadBmpCorrect)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    CLOVE_NOT_NULL(load_bmp("bin\\Test.bmp", "rb", &px_offset, &width, &height, &px_size, &depth));
    CLOVE_INT_EQ(px_offset, 54);
    CLOVE_INT_EQ(width, 85);
    CLOVE_INT_EQ(height, 83);
    CLOVE_INT_EQ(depth, 24);
}

CLOVE_TEST(createTextureFromBMPDataNullRenderer)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    Uint8* content = load_bmp("bin\\Test.bmp", "rb", &px_offset, &width, &height, &px_size, &depth);
    CLOVE_NULL(create_texture_from_BMP_data(NULL, content, px_offset, px_size, width, height, depth));
}

CLOVE_TEST(createTextureFromBMPDataNullContent)
{
    Uint32 px_offset;
    Uint32 width;
    Uint32 height;
    Uint32 px_size;
    Uint32 depth;
    CLOVE_NULL(create_texture_from_BMP_data(NULL, NULL, px_offset, px_size, width, height, depth));
    SDL_Quit();
    CLOVE_PASS();
}

CLOVE_SUITE(ParserSuite)
{
    CLOVE_SUITE_TESTS(
        loadBmpNullPath,
        loadBmpNullMode,
        loadBmpNullAnyInfoParams,
        loadBmpCorrect,
        createTextureFromBMPDataNullRenderer,
        createTextureFromBMPDataNullContent
    );
}