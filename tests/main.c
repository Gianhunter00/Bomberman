#include "clove.h"
#include "test_level.h"
#include "test_movable.h"
#include "test_parser.h"
#include "test_bomberman.h"
#include "test_players_mgr.h"

CLOVE_RUNNER(LevelSuite, MovableSuite, ParserSuite, BombermanSuite, PlayersMgrSuite);