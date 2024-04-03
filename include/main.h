#ifndef _MAIN_H_
#define _MAIN_H_

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

typedef unsigned long u32;
typedef long s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef float f32;

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct SpaceData {
/* 0x00 */ s8 unk_00;
/* 0x01 */ u8 space_type; // enum board_space_type
/* 0x02 */ s16 unk_02;
/* 0x04 */ s32 unk_04;
/* 0x08 */ Vec3f coords;
/* 0x14 */ Vec3f rot;
/* 0x20 */ void* event_list;
} SpaceData;

enum Items {
    MUSHROOM = 0,
    SKELETON_KEY = 1,
};

typedef struct Blocks {
    s16 coinBlockSpaceIndex;
    s16 starBlockSpaceIndex;
    s16 itemBlockSpaceIndex;
} Blocks;

SpaceData* spacesForBoards[6];

#define ANSI_GREEN "\x1b[32m"
#define ANSI_RESET "\x1b[0m"
#define ANSI_BLUE "\x1b[34m"

#define ANSI_BLACK "\x1b[30m"
#define ANSI_RED "\x1b[31m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_WHITE "\x1b[37m"

#endif //_MAIN_H_