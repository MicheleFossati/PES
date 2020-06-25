////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
       EVENT HANDLER
the event handler receives SDL's events and make the necessaries changes 
 -process the players' motion
 -play sound effects
 -check for key presses (es. ESC)

Usually included from PES_match.h
*/

#ifndef EVENT_HANDLER
#define EVENT_HANDLER

#include "PES_arena.h"
#include "PES_sound.h"
#include "PES_global.h"

class PES_handler
{
 public:
 
 //init
 PES_handler(){};
 PES_handler(PES_arena_ptr a, PES_FLOAT sens = 0.05, PES_GAME_MODE mode = SINGLEPLAYER_MOUSE);
 //initialize mixer from string
 bool init_mixer(const std::string&);
 
 //public functions

 //runs through all the SDL_events in the event queue, calling the needed functions:
 // sound, mouse_motion or WASD_motion, quit
 bool process(Uint32);
 bool squitting() {return quit;};
 void play_music() {mixer->play_music();};

 private:
 bool quit; //false by default
 PES_arena_ptr arena;
 PES_FLOAT mouse_sensitivity; 
 PES_GAME_MODE game_mode;

 //bitmask to encode WASD and arrows keys
 WASD_BITMASK wasd_code;

 //sound
 std::shared_ptr <PES_mixer> mixer;

 //private mehods
 void mouse_motion(PES_vector2d, Uint32);
 void WASD_motion(Uint32);             //               <-----------------------------------,
 void move_player_wasd(int, Uint8, Uint32); //assign new pos and vel to a player. called by |
                                       //the previous function  ----------------------------'

 void INIT(PES_arena_ptr);
};

#endif

