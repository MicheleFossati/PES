////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
  GOAL


*/

#include "PES_field_obj.h"

#ifndef GOAL
#define GOAL

class PES_goal: public PES_field_obj
{
 //it's a rectangle which, if hit, changes the score
 public:
 int get_player() {return player;};
 int get_score()  {return score;};

 bool INIT(std::istringstream&);

 private:
 int player;
 int score;

};


class PES_rect_goal: public PES_goal
{
 public:
 //constructor from string
 PES_rect_goal(const std::string&);

 PES_FLOAT    get_width()   {return width;};
 PES_FLOAT    get_height()  {return height;};
 PES_vector2d get_respawn() {return respawn;};

 private:
 PES_FLOAT width,height;
 PES_vector2d respawn;
 
};


class PES_bumper: public PES_goal
{
 public:
 PES_bumper(const std::string&);

 PES_FLOAT get_radius() {return radius;};
 PES_FLOAT get_boost()  {return boost;};

 private:
 PES_FLOAT radius;
 PES_FLOAT boost;

};

#endif

