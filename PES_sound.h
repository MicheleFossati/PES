////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
   SOUND
contains sound effects and background music
implemented using SDL_mixer library

Usually included from PES_handler.h
*/

#include "PES_data_templates.h"
#include "PES_global.h"

#ifndef MIXER
#define MIXER

class PES_mixer
{
 public:
 PES_mixer(const std::string&); //contructor from match folder
 ~PES_mixer();
 void play_chunk(int, int); //chunk, repetitions
 void play_music();   //play the background music


 private:
 bool INIT(const std::string&);
 bool read_from_file(const std::string&);//reads music and music chunks specified in
                                         // audio/audio.txt

 Mix_Music* background_music;
 std::vector<Mix_Chunk*> chunks; //chunk sound vector

 void CLEAN();
};



#endif

