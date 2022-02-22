#include "clove.h"
#include "level.h"

CLOVE_TEST(LevelInitializationNULL)
{
    CLOVE_INT_EQ(level_init(NULL, 0, 0, 0, NULL), -1);
}

CLOVE_TEST(LevelInitializationZeroCols)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 0, 100, 100, NULL), -1);
}

CLOVE_TEST(LevelInitializationZeroRows)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 100, 0, 100, NULL), -1);
}

CLOVE_TEST(LevelInitializationZeroSize)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 100, 100, 0, NULL), -1);
}

CLOVE_TEST(LevelInitializationNullCells)
{
    level_t level;
    CLOVE_INT_EQ(level_init(&level, 100, 100, 10, NULL), 0);
}

CLOVE_TEST(LevelCellsNULL)
{
    level_t level;
    level_init(&level, 2, 2, 1, NULL);
    CLOVE_INT_EQ(level_cell(&level, 0, 0), -1);
}

CLOVE_TEST(LevelCellsContent)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1, map);
    CLOVE_INT_EQ(level_cell(&level, 0, 0), 1);
}

CLOVE_TEST(LevelCellsOutOfBoundsCol)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1, map);
    CLOVE_INT_EQ(level_cell(&level, 3, 0), -1);
}

CLOVE_TEST(LevelCellsOutOfBoundsRow)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1, map);
    CLOVE_INT_EQ(level_cell(&level, 1, 5), -1);
}

CLOVE_TEST(LevelCellsOutOfBounds)
{
    level_t level;
    int32_t map[] = {1, 2, 3, 4};
    level_init(&level, 2, 2, 1, map);
    CLOVE_INT_EQ(level_cell(&level, 5, 5), -1);
}

CLOVE_TEST(LevelTextureInit)
{
    level_level_texture_init();
    CLOVE_PASS();
}

CLOVE_TEST(LevelTextureFreeIsNull)
{
    level_level_texture_init();
    level_level_texture_free();
    CLOVE_NULL(get_level_texture(0));
}

CLOVE_TEST(getLevelTextureWithoutInitialization)
{
    CLOVE_NULL(get_level_texture(0));
}

CLOVE_TEST(getLevelTextureWithInitialization)
{
    level_level_texture_init();
    CLOVE_NOT_NULL(get_level_texture(0));
    level_level_texture_free();
    CLOVE_PASS();
}

CLOVE_TEST(getLevelTextureOutOfRange)
{
    level_level_texture_init();
    CLOVE_NULL(get_level_texture(100));
    level_level_texture_free();
    CLOVE_PASS();
}

CLOVE_TEST(levelLoadTextureWithoutInitialization)
{
    level_load_texture(NULL);
    CLOVE_NULL(get_level_texture(0));
}

CLOVE_TEST(levelLoadTextureWithInitializationButNullRenderer)
{
    level_level_texture_init();
    level_load_texture(NULL);
    level_level_texture_free();
    CLOVE_PASS();
}

CLOVE_SUITE(LevelSuite)
{
    CLOVE_SUITE_TESTS(
        LevelInitializationNULL,
        LevelInitializationZeroCols,
        LevelInitializationZeroRows,
        LevelInitializationZeroSize,
        LevelInitializationNullCells,
        LevelCellsNULL,
        LevelCellsContent,
        LevelCellsOutOfBoundsCol,
        LevelCellsOutOfBoundsRow,
        LevelCellsOutOfBounds,
        LevelTextureInit,
        LevelTextureFreeIsNull,
        getLevelTextureWithoutInitialization,
        getLevelTextureOutOfRange,
        levelLoadTextureWithoutInitialization,
        levelLoadTextureWithInitializationButNullRenderer
        );
}