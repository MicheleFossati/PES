////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_player.h"


//costruttore
PES_player::PES_player(const std::string data_string)
{
 std::istringstream data_stream(data_string);
 //get pos;
 set_pos(data_stream);

 //default
 min_h = 0;
 max_h = 0;
 radius = score = image_name = 0;

 //get all the other parameters
 if(data_stream.eof()) return;
 data_stream >> min_h;
 if(data_stream.eof()) return;
 data_stream >> max_h;
 if(data_stream.eof()) return;
 data_stream >> radius;
 if(data_stream.eof()) return;
 data_stream >> score;
 if(data_stream.eof()) return;
 data_stream >> image_name;

}



