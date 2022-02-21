#include "clove.h"
#include "../../Include/bomberman.h"

CLOVE_TEST(BombermanNew)
{
    CLOVE_NOT_NULL(bomberman_new(0, 0, 0, 0, 0));
}

CLOVE_TEST(BombermanFree)
{
    bomberman_t *bomberman = bomberman_new(0, 0, 0, 0, 0);
    bomberman_free(bomberman);
    CLOVE_PASS();
}

CLOVE_TEST(BombermanInputNullBomberman)
{
    SDL_Event event;
    bomberman_input(NULL, &event);
    CLOVE_PASS();
}

CLOVE_TEST(BombermanInputNullEvent)
{
    bomberman_t *bomberman = bomberman_new(0, 0, 0, 0, 0);
    bomberman_input(bomberman, NULL);
    bomberman_free(bomberman);
    CLOVE_PASS();
}

CLOVE_SUITE(BombermanSuite)
{
    CLOVE_SUITE_TESTS(
        BombermanNew,
        BombermanFree,
        BombermanInputNullBomberman,
        BombermanInputNullEvent
        );
}