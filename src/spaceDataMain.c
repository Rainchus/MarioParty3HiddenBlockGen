#include "main.h"

#include "board_space_data/chilly_waters_spaces.c"
#include "board_space_data/deep_blooper_sea_spaces.c"
#include "board_space_data/spiny_desert_spaces.c"
#include "board_space_data/woody_woods_spaces.c"
#include "board_space_data/creepy_cavern_spaces.c"
#include "board_space_data/waluigis_island_spaces.c"

SpaceData* spacesForBoards[6] = {
    chilly_waters_spaces,
    deep_blooper_sea_spaces,
    spiny_desert_spaces,
    woody_woods_spaces,
    creepy_cavern_spaces,
    waluigis_island_spaces
};