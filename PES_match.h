////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
   MATCH
Object of type "match" contains all the infos for a single match,
Should be the only object used in main function
It contains (dynamically) the objects:
 -arena
 -event handler
 -game engine

It takes care of the main loop, calling the process functions from arena etc...

    Usually included from PES.h
*/

#include "PES_arena.h"
#include "PES_game_engine.h"
#include "PES_event_handler.h"




#ifndef MATCH
#define MATCH

class PES_match
{

public:
 
 PES_match(std::string, PES_GAME_MODE);      //    <- constructor from filepath
 PES_match(char*, PES_GAME_MODE);
 ~PES_match();

 void run();    //  <- main loop of the game
 

private:
 
 //main objects
 PES_arena_ptr arena;
 std::shared_ptr<PES_game_engine> engine;
 std::shared_ptr<PES_handler> handler;
 SDL_Renderer* renderer;

 bool quit;  //   <- set to true when the game has to exit

 PES_GAME_MODE mode; //single or two players, WASD or mouse
 std::string title;

 //time variables (Uint32)
 Uint32 current_time, last_time, dt, time_delta;
 int FPS;
 Uint32 max_time;
 int winning_score;

 //calculates dt for the next frame
 void update_time();
 void winning(int); //if a player has won, prints the winning player and quits
 int is_over_yet(); //returns the winning player, or -1 if nobody has won yet
                    //called at the end of the main loop

 //renders scores, game_time etc
 bool render(SDL_Renderer*);
 FC_Font* font_time;  //font for the time text
 FC_Font* font_score; //font for the score text
 FC_Font* font_title; //font fro the title

 //init and read
 bool INIT(std::string); 
 void CLEAN();
 bool read_from_file(std::string, int&, int&, PES_FLOAT&, PES_FLOAT&); // <- reads general match info from match.txt contained in string=folder
 bool read_fonts(std::string); // <- reads fonts

 //game stats: timing, scores, etc...
 Uint32 game_time, starting_time;
 std::vector<int> scores;  

};



#endif



