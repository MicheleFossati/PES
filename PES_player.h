////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#ifndef PLAYER
#define PLAYER

#include "PES_field_obj.h"

class PES_player: public PES_mov_obj
{ 

 public:
  
 PES_player(PES_vector2d pos,PES_FLOAT min = 0,PES_FLOAT max = 10,PES_FLOAT r = 0.2) : min_h(min), max_h(max), radius(r) 
  { set_pos(pos); score = 0;};
 PES_player(const std::string); //costruttore da stringa

 PES_FLOAT get_min_h()  {return min_h;}
 PES_FLOAT get_max_h()  {return max_h;}
 PES_FLOAT get_radius() {return radius;}
 int get_score()        {return score;};

 void add_points(int points){score += points;};

 private:
 PES_FLOAT min_h, max_h;
 PES_FLOAT radius; 
 int score;
};

#endif

