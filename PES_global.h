////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
  GLOBAL
Contains global functions. they usually do not depend on othe PES lybraries

Usually included from PES_handler.h, PES_arena.h, PES_game_engine.h
*/

#include "PES_field_obj.h"
#include "PES_player.h"
#include "PES_goal.h"
#include "SDL_FontCache.h"

#ifndef GLOBAL
#define GLOBAL

//corrects a folder type string
inline void set_folder_slash(std::string &folder)
{
 int size = folder.size();
 if (size == 0)
 {
  folder = "./";
  return;
 }
 if(folder.c_str()[size-1] != '/')
  folder = folder + "/";
}

//get the first uncommented line in a stream and put it into line
void PES_get_uncomm_line(std::istream&, std::string&);

//checks if a string is a PES_OBJ_NAMES. If is it, sets the PES_OBJ_NAMES argument
bool is_string_obj_name (PES_OBJ_NAMES&, std::string&);

//clip an obj with radius r (pos and vel) to a rectangle boundary
//Arguments: pos, vel, min_y, max_y, min_x, max_x, object radius
bool PES_clip_obj(PES_vector2d*, PES_vector2d*, PES_FLOAT, PES_FLOAT, PES_FLOAT, PES_FLOAT, PES_FLOAT);

void delete_tex_vector (std::vector<SDL_Texture*>& );

//reads a Font from a filepath and links it to FC_Font*
bool PES_read_font(std::string&, FC_Font*, SDL_Renderer*, std::string&);

//given an object type, returns the relative PES_OBJ_NAME
template <class T>
PES_OBJ_NAMES PES_type_to_name()
{
 if(std::is_same<T,PES_disk>::value) 
  return DISK;
 if(std::is_same<T,PES_player>::value) 
  return PLAYER_TYPE;

//EM objects
 if(std::is_same<T,PES_point_charge>::value) 
  return POINT_CHARGE; 
 if(std::is_same<T,PES_solenoid>::value) 
  return SOLENOID;
 if(std::is_same<T,PES_coil>::value) 
  return COIL;
 if(std::is_same<T,PES_wire>::value) 
  return WIRE;
//goals
 if(std::is_same<T,PES_rect_goal>::value) 
  return RECT_GOAL;
 if(std::is_same<T,PES_bumper>::value) 
  return BUMPER;
//default
return DISK;
}


#endif

