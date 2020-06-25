////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_goal.h"

////////////////////////////////////////////
//      GOAL
bool
PES_goal::INIT(std::istringstream& data_stream)
{
 //default
 player = 0;
 score = 0;

 //get pos;
 set_pos(data_stream);
 //get player
 data_stream >> player;
 if(data_stream.eof()) return false;
 //get score
 data_stream >> score;
 if(data_stream.eof()) return false;

 return true;
}


////////////////////////////////////////////
//     RECT GOAL

//contructor from string
PES_rect_goal::PES_rect_goal(const std::string& data_string)
{
 std::istringstream data_stream(data_string);

 //default
 width = height = image_name = 0;

 INIT(data_stream); //pos, player, score
 
 //get width and height
 if(data_stream.eof()) return;
 data_stream >> width;
 if(data_stream.eof()) return;
 data_stream >> height;
 if(data_stream.eof()) return;
 data_stream >> respawn.x;
 if(data_stream.eof()) return;
 data_stream >> respawn.y;
 if(data_stream.eof()) return;
 data_stream >> image_name;

}


///////////////////////////////////////////
//      BUMPER
PES_bumper::PES_bumper(const std::string& data_string)
{
 std::istringstream data_stream(data_string);

 //default
 radius = image_name = 0;
 boost = 1;

 INIT(data_stream); //pos, player, score
 
 //read other parameters
 if(data_stream.eof()) return;
 data_stream >> radius;
 if(data_stream.eof()) return;
 data_stream >> boost;
 if(data_stream.eof()) return;
 data_stream >> image_name;

}

