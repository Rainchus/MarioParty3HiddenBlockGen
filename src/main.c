#include <stdio.h>
#include <string.h>
#include "main.h"

//probably only 4 entries but 10 for safety
u8 D_800CD0B6[] = {0xC8, 0xBD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

s32 callAmountBetweenSpace2[] = {6, 11, 16};
char* WalkSpeed[] = {
    "Fast",
    "Normal",
    "Slow"
};

#define ADV_RNG cur_rng_seed = (cur_rng_seed*0x41C64E6D)+0x3039;

u32 cur_rng_seed = 0x57DDF041; //initial starting seed D_80097650

s32 func_80035F98_36B98(s32 input) {
    s32 byteValue;
    s32 adjustedIndex;
    s32 byteIndex;
    s32 bitIndex;
    s32 mask;

    if (input < 0) {
        byteValue = D_800CD0B6[(input + 7) >> 3];
    } else {
        byteValue = D_800CD0B6[input >> 3];
    }

    if (input >= 0) {
        adjustedIndex = input;
    } else {
        adjustedIndex = input + 7;
    }

    // Calculate the byte index (divide by 8)
    byteIndex = adjustedIndex >> 3;

    // Calculate the bit index
    bitIndex = input - (byteIndex << 3);

    mask = (1 << bitIndex);

    return byteValue & mask;
}

u8 HuGetRandomByte(void) {
    cur_rng_seed = (cur_rng_seed*0x41C64E6D)+0x3039;
    return (u8)((cur_rng_seed+1) >> 16) & 0xFF;
}

s32 ScaleRandomNumber(f32 arg0) { //800EEF80 in boards, 800EFE20 in duel mode
    // HuGetRandomByte returns an unsigned byte
    u8 randomByte1 = HuGetRandomByte();
    u8 randomByte2 = HuGetRandomByte();
    s32 shiftedByte1 = (randomByte1 << 8);

    // Normalize the 16-bit number to the range [0, 1), then multiply by arg0
    s32 scaledRandom = ((randomByte2 | shiftedByte1) / 65536.0f) * arg0;
    
    return scaledRandom;
}

u8 rollDice(void) { //func_800DB148 in duel mode
    u8 randByte = HuGetRandomByte();
    return ((randByte % 10) + 1);
}

//i think this is only 8 in size, made it 16 for safety
//on chilly waters this array is blank, but on desert map it's not?
s16 D_801052B8[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

u16 chilly_waters_total_board_spaces = 0x94; //chilly waters has 0x94 spaces (D_80105210)
s16 D_801054F8 = 0x08; //in chilly waters, this is 0x08 at first (does this change?)
s16 D_801054B6 = 0; //no idea, is zero on chilly waters start

s16 D_800D03FC = 0; //total coin blocks placed
s16 D_800CE208 = 0; //total star blocks placed
s16 D_800CDD68 = 0; //total item blocks placed

SpaceData* func_800EB160_FED80(s16 arg0, s32 boardIndex) {
    SpaceData* boardSpaces = spacesForBoards[boardIndex];
    return &boardSpaces[arg0];
}

u8 D_80101468_115088[] = { //data for chilly waters (all boards?)
    0x01, 0x02, 0x04, 0x01, 0x10, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
};

s16 D_801054B8[] = { //data for chilly waters
    0x0036, 0x002F, 0x005E, 0x0012, 0x0051, 0x0060, 0x004D, 0x0029,
};

s16 func_800EB5DC_FF1FC(s32 arg0, u8 arg1, s32 numOfBoardSpaces, s32 boardIndex) {
    s32 var_s1;
    u8 var_s1_2;
    SpaceData* temp_a1;
    s32 i, j;
    s32 tempVar;
    var_s1 = 0;

    //find valid spaces block could go
    for (i = 0; i < numOfBoardSpaces; i++) {
        if ((D_80101468_115088[(func_800EB160_FED80(i, boardIndex)->space_type & 0xF)] & (u16)arg0)){
            var_s1++;
        }
    }

    var_s1_2 = var_s1 - D_801054F8;
    if ((arg1) < 5U) {
        var_s1_2 = var_s1_2 - D_801054B6;
    }

    var_s1_2 = ScaleRandomNumber((var_s1_2));

    tempVar = (arg1 & 0xFF) < 5;

    for (i = 0;; i = -(i < numOfBoardSpaces) & i) {
        temp_a1 = func_800EB160_FED80(i, boardIndex);
        for (j = 0; j < D_801054F8; j++) {
            if (D_801054B8[j] == i) {
                break;
            }
        }

        if (j != D_801054F8) {
            i++;
            continue;
        } else {
            if (tempVar != 0) {
                for (j = 0; j < D_801054B6; j++) {
                    if (D_801052B8[j] == i) {
                        break;
                    }
                }
            
                if (j != D_801054B6) {
                    i++;
                    continue;
                }            
            }
        }
        if ( !(D_80101468_115088[(temp_a1->space_type & 0xF)] & (u16)arg0)){
            i++;
            continue;
        } else if ((var_s1_2 & 0xFF) == 0) {
            break;
        }
        var_s1_2--;
        i++;
    }

    return i;
}

s16 func_800EBCD4_FF8F4(u8 arg0, s32 numOfBoardSpaces, s32 boardIndex) {
    return func_800EB5DC_FF1FC(2, arg0, numOfBoardSpaces, boardIndex);
}

void PlaceHiddenBlocksMain(Blocks* blocks, s32 numOfSpaces, s32 boardIndex) {
    if (func_80035F98_36B98(0xF) != 0) {
        while (blocks->coinBlockSpaceIndex == -1 || blocks->coinBlockSpaceIndex == blocks->starBlockSpaceIndex || blocks->coinBlockSpaceIndex == blocks->itemBlockSpaceIndex) {
            blocks->coinBlockSpaceIndex = func_800EBCD4_FF8F4(D_800D03FC, numOfSpaces, boardIndex);
        }
        while (blocks->starBlockSpaceIndex == -1 || blocks->coinBlockSpaceIndex == blocks->starBlockSpaceIndex || blocks->itemBlockSpaceIndex == blocks->starBlockSpaceIndex) {
            blocks->starBlockSpaceIndex = func_800EBCD4_FF8F4(D_800CE208, numOfSpaces, boardIndex);
        }
        
        while (blocks->itemBlockSpaceIndex == -1 || blocks->coinBlockSpaceIndex == blocks->itemBlockSpaceIndex || blocks->starBlockSpaceIndex == blocks->itemBlockSpaceIndex) {
            blocks->itemBlockSpaceIndex = func_800EBCD4_FF8F4(D_800CDD68, numOfSpaces, boardIndex);
        }
    }
}

#define CHILLY_WATERS 0
#define DEEP_BLOOPER_SEA 1
#define SPINY_DESERT 2
#define WOODY_WOODS 3
#define CREEPY_CAVERN 4
#define WALUIGIS_ISLAND 5

typedef struct SpaceThing {
    u8 startingSpaceID;
    u8 spacesToBankEncounter;
    u8 possibleSpaces[10];
} SpaceThing;

//Chilly Waters Spaces
SpaceThing SpaceIndex00 = {
    .startingSpaceID = 0,
    .spacesToBankEncounter = 0,
    .possibleSpaces = {0xB, -1, 0x21, 0x2C, -1, -1, -1, -1, -1, -1}
};

SpaceThing SpaceIndex0B = {
    .startingSpaceID = 0xB,
    .spacesToBankEncounter = 0,
    .possibleSpaces = {-1, 0x21, 0x2C, -1, -1, -1, -1, -1, -1, -1}
};

SpaceThing SpaceIndex16 = {
    .startingSpaceID = 0x16,
    .spacesToBankEncounter = 0,
    .possibleSpaces = {0x21, 0x2C, -1, -1, -1, -1, -1, -1, -1, -1}
};

SpaceThing SpaceIndex21 = {
    .startingSpaceID = 0x21,
    .spacesToBankEncounter = 0,
    .possibleSpaces = {0x2C, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

SpaceThing* ChillyWaters[] = {
    &SpaceIndex00, &SpaceIndex0B, &SpaceIndex16, &SpaceIndex21
};

SpaceThing* FindStartingSpaceIndexInList(s32 spaceIndex) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(ChillyWaters); i++) {
        if (spaceIndex == ChillyWaters[i]->startingSpaceID) {
            return ChillyWaters[i];
        }
    }
    return 0; //not found
}

void DoNumberOfPauseBuffers(s32 pauseBufferAmount) {
    s32 advancements = pauseBufferAmount * 5; //5 advancements with perfect pauses
    for (int i = 0; i< advancements; i++) {
        ADV_RNG;
    }
}

void BlockGenMain(char* argv[]) {
    u32 prevSeed;
    Blocks blockVariables;
    s32 i = 0;
    u8 spaceIndex = 0x21;

    SpaceThing* spaceArrayData = FindStartingSpaceIndexInList(spaceIndex);

    if (spaceArrayData == 0) {
        printf("Starting space index of %02X " ANSI_RED "not found!\n" ANSI_RESET, spaceIndex);
        return;
    }

    printf("Starting space index of %02X" ANSI_GREEN" found!\n" ANSI_RESET, spaceIndex);

    for (i = 0; i < 3000; i++) {
        prevSeed = cur_rng_seed;
        blockVariables.coinBlockSpaceIndex = -1;
        blockVariables.starBlockSpaceIndex = -1;
        blockVariables.itemBlockSpaceIndex = -1;
        PlaceHiddenBlocksMain(&blockVariables, chilly_waters_total_board_spaces, CHILLY_WATERS);
        for (int j = 0; j < 10; j++) {
            if (blockVariables.itemBlockSpaceIndex == spaceArrayData->possibleSpaces[j]) {
                //item block placed on valid space cpu can reach

                //printf("Found space %02X, roll of %d needed \n", blockVariables.itemBlockSpaceIndex, j + 1);
                for (int k = 0; k < 70; k++) { //hidden block placement to roll is 70 advancements
                    ADV_RNG;
                }

                s32 roll = rollDice();

                if (roll == j + 1) {
                    //cpu has rolled to land on item space
                    u32 seedBeforePauseBuffers = cur_rng_seed;

                    //try up to 3 pause buffers
                    for (int k = 0; k < 3; k++) {
                        cur_rng_seed = seedBeforePauseBuffers;
                        //try each walking speed
                        for (int l = 0; l < 3; l++) {
                            cur_rng_seed = seedBeforePauseBuffers;
                            #define ROLL_DICE_BLOCK_TIME 23

                            s32 rngAdvancements = ROLL_DICE_BLOCK_TIME + callAmountBetweenSpace2[l] * (roll);
                            //cpu walks to space
                            for (int j = 0; j < rngAdvancements - 1; j++) { //-1 because???
                                ADV_RNG;
                            }

                            u32 beforePauseBuffersTempSeed = cur_rng_seed;

                            DoNumberOfPauseBuffers(k);

                            u8 randByte = ScaleRandomNumber(100.0f) + 1;

                            if (randByte > 0x55 && randByte < 0x5B) {
                                printf("Valid seed of " ANSI_BLUE "%08lX " ANSI_RESET "found!\n", prevSeed);
                                printf("Calls: %ld\n", i);
                                printf("Before Pause Buffers: %08lX\n", beforePauseBuffersTempSeed);
                                printf("Pause Buffers: %d\n", k);
                                printf("Walk Speed: %s\n\n", WalkSpeed[l]);
                            }
                        }

                    }


                    // printf("Valid seed of %08lX found!\n", prevSeed);
                    // printf("Space: %02X, Roll: %d\n\n", blockVariables.itemBlockSpaceIndex, roll);
                    break;
                }
            }
        }
        // printf("Seed Before Placements: " ANSI_GREEN "0x%08lX\n" ANSI_RESET, prevSeed);
        // printf("Hidden Coin Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.coinBlockSpaceIndex);
        // printf("Hidden Star Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.starBlockSpaceIndex);
        // printf("Hidden Item Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.itemBlockSpaceIndex);
        // printf("\n");
        cur_rng_seed = prevSeed;
        ADV_RNG;
    }

    // for (i = 0; i < 5; i++) {
    //     prevSeed = cur_rng_seed;
    //     blockVariables.coinBlockSpaceIndex = -1;
    //     blockVariables.starBlockSpaceIndex = -1;
    //     blockVariables.itemBlockSpaceIndex = -1;
    //     PlaceHiddenBlocksMain(&blockVariables, chilly_waters_total_board_spaces, CHILLY_WATERS);
    //     printf("Seed Before Placements: " ANSI_GREEN "0x%08lX\n" ANSI_RESET, prevSeed);
    //     printf("Hidden Coin Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.coinBlockSpaceIndex);
    //     printf("Hidden Star Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.starBlockSpaceIndex);
    //     printf("Hidden Item Block Placed at " ANSI_MAGENTA "0x%02X\n" ANSI_RESET, blockVariables.itemBlockSpaceIndex);
    //     printf("\n");
    //     cur_rng_seed = prevSeed;
    //     ADV_RNG;
    // }
}

//after blocks have been placed, 70 rng calls until dice roll
//pause buffer advances rng 5 times if you pause and then unpause in pause lag

int main(int argc, char *argv[]) {
    if (strncmp("--blockgen", argv[1], sizeof("--blockgen")) == 0) {
        BlockGenMain(argv);
        return 0;
    }
}