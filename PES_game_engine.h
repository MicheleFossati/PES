////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
        GAME ENGINE
The game engine contains an arena (pointer) and performs the physical calculation


*/

#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "PES_arena.h"

class PES_game_engine 
{
 public:
  //constructors
  PES_game_engine();
  PES_game_engine(PES_arena_ptr&, PES_FLOAT); 

  //process all the physical calculations and consequently modify the objects' stat
  bool process(Uint32 t_interval);

 private:
  PES_arena_ptr arena;
  
  PES_FLOAT dump_cost; //dumping constant

  //integrates Newton's law with Lorentz's force and changes objects' stat
  //uses Runge-Kutta integration method to calculate v(t)
  bool EM_evolution(int, Uint32);

  //returns Lorentz's force
  PES_vector2d get_force(PES_vector2d, PES_vector2d, PES_EM_TYPE, PES_EM_TYPE);

  //  ---private functions called by process()
  //  |	 NOTE: these functions returns false if an error has occured, thus shutting 
  //  |  	    down the process by returning false
  //  V  	    they usually return true
  //check for score events (and eventually calls PES_arena::scored)
  bool score(int);
  //physical collisions
  bool wall_collision(int);
  bool player_collision(int, int);
  bool disk_collision(int, int);
  
  //GOALS
  bool rect_goal_collision(int, int);
  bool bumper_collision(int, int);
};



#endif











