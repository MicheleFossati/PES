////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////



/*
      TYPES
here we define the variable types of PES

usually included from PES_EM_structs.h
*/

#ifndef TYPES
#define TYPES

#include "PES_data_templates.h"

typedef float PES_FLOAT, PES_EM_TYPE, PES_MASS_TYPE;
typedef int PES_INT; //so useful!
typedef PES_vector2d_template<PES_FLOAT>    PES_vector2d;
typedef PES_vector2d_template<PES_EM_TYPE>  PES_EM_vector2d;



//game modes
//we use four bits to encode general playing modes
enum PES_GAME_MODE:Uint8
{
 SINGLEPLAYER_MOUSE = 1|8,
 SINGLEPLAYER_WASD = 1|4,
 TWOPLAYERS_WASD = 2|4,

 SINGLEPLAYER = 1,
 TWOPLAYERS = 2,
 PES_WASD_MODE = 4,
 PES_MOUSE_MODE = 8
};


//WASD bitmask - 8bit
//bitmask used in PES_event_handler to handle WASD and arrows infos
enum WASD_BITMASK:Uint8
{
 W_BMSK = 1,
 S_BMSK = 2,
 A_BMSK = 4,
 D_BMSK = 8,

 UP_BMSK = 16,
 DOWN_BMSK = 32,
 LEFT_BMSK = 64,
 RIGHT_BMSK = 128,

 EVERY_KEY = 0xff,
 NO_KEY = 0x0
};

#endif




