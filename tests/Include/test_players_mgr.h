#include "clove.h"
#include "players_mgr.h"

CLOVE_TEST(PlayersMgrInitFreeAndGetSize)
{
    players_mgr_init();
    CLOVE_INT_EQ(1, players_mgr_get_size());
    players_mgr_free();
    CLOVE_INT_EQ(0, players_mgr_get_size());
}

CLOVE_TEST(PlayersMgrGetSizeWithoutInit)
{
    CLOVE_INT_EQ(0, players_mgr_get_size());
}

CLOVE_TEST(PlayersMgrIncreaseSize)
{
    players_mgr_init();
    CLOVE_INT_EQ(2, players_mgr_increase_size_by_one());
    players_mgr_free();
    CLOVE_INT_EQ(0, players_mgr_get_size());
}

CLOVE_TEST(PlayersMgrGetByIndexWithoutInit)
{
    CLOVE_NULL(players_mgr_get_by_index(0));
}

CLOVE_TEST(PlayersMgrGetByIndexWitInitButNoPlayerInside)
{
    players_mgr_init();
    CLOVE_NULL(players_mgr_get_by_index(0));
}

CLOVE_TEST(PlayersMgrGetByIndexWitInitAndPlayerInside)
{
    players_mgr_init();
    players_mgr_add_new_player(0, 0, 0, 0, 0, NULL, 0);
    CLOVE_NOT_NULL(players_mgr_get_by_index(0));
}

CLOVE_TEST(PlayersMgrGetByIndexOutOfRange)
{
    players_mgr_init();
    players_mgr_add_new_player(0, 0, 0, 0, 0, NULL, 0);
    CLOVE_NULL(players_mgr_get_by_index(2));
    CLOVE_NULL(players_mgr_get_by_index(-1));
}

CLOVE_TEST(PlayersMgrAddNewPlayer)
{
    players_mgr_init();
    players_mgr_add_new_player(1, 2, 3, 4, 5, NULL, 6);
    bomberman_t *player_added = players_mgr_get_by_index(0);
    CLOVE_NOT_NULL(player_added);
    CLOVE_FLOAT_EQ(1, player_added->movable.x);
    CLOVE_FLOAT_EQ(2, player_added->movable.y);
    CLOVE_FLOAT_EQ(3, player_added->movable.width);
    CLOVE_FLOAT_EQ(4, player_added->movable.height);
    CLOVE_FLOAT_EQ(5, player_added->movable.speed);
    CLOVE_FLOAT_EQ(6, player_added->code);
    players_mgr_free();
    CLOVE_INT_EQ(0, players_mgr_get_size());
}

CLOVE_TEST(PlayersMgrContainCode)
{
    players_mgr_init();
    CLOVE_IS_FALSE(players_mgr_contain_code(0));
    players_mgr_add_new_player(1, 2, 3, 4, 5, NULL, 6);
    CLOVE_IS_TRUE(players_mgr_contain_code(6));
}

CLOVE_TEST(PlayersMgrAddNewPlayerWithSameCode)
{
    players_mgr_init();
    players_mgr_add_new_player(1, 2, 3, 4, 5, NULL, 6);
    CLOVE_INT_EQ(0, players_mgr_add_new_player(1, 2, 3, 4, 5, NULL, 6));
    players_mgr_free();
    CLOVE_INT_EQ(0, players_mgr_get_size());
}

CLOVE_TEST(PlayersMgrUpdatePlayer)
{
    players_mgr_init();
    players_mgr_add_new_player(1, 2, 3, 4, 5, NULL, 6);
    players_mgr_update_player(100, 100, 6);
    bomberman_t *player = players_mgr_get_by_index(0);
    CLOVE_FLOAT_EQ(100, player->movable.x);
    CLOVE_FLOAT_EQ(100, player->movable.y);
    CLOVE_FLOAT_EQ(100, player->rect_to_draw.x);
    CLOVE_FLOAT_EQ(100, player->rect_to_draw.y);
}

CLOVE_TEST(PlayersMgrUpdatePlayerNotExisting)
{
    players_mgr_init();
    CLOVE_INT_EQ(0, players_mgr_update_player(100, 100, 6));
}

CLOVE_SUITE(PlayersMgrSuite)
{
    CLOVE_SUITE_TESTS(
        PlayersMgrInitFreeAndGetSize,
        PlayersMgrGetSizeWithoutInit,
        PlayersMgrIncreaseSize,
        PlayersMgrGetByIndexWithoutInit,
        PlayersMgrGetByIndexWitInitButNoPlayerInside,
        PlayersMgrGetByIndexWitInitAndPlayerInside,
        PlayersMgrGetByIndexOutOfRange,
        PlayersMgrAddNewPlayer,
        PlayersMgrContainCode,
        PlayersMgrAddNewPlayerWithSameCode,
        PlayersMgrUpdatePlayer,
        PlayersMgrUpdatePlayerNotExisting,
        );
}