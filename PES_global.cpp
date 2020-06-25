////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_global.h"


//get valid line from stream
void PES_get_uncomm_line(std::istream& is, std::string& line)
{
 //check
 if(!is.good())
  {line = ""; return;}

 //throw all the empty lines or the ones starting with #
 do
 { 
  if (is.eof()) //all the lines where comments
  { line = ""; return; }
  std::getline(is, line); 
 } 
 while(line.c_str()[0] == '#' || line.find_first_not_of(" ") == std::string::npos);

}


bool
is_string_obj_name ( PES_OBJ_NAMES &obj_name, std::string &str)
{
  if (str == "DISK")           {obj_name = DISK;         return true;}
  if (str == "PLAYER_TYPE")    {obj_name = PLAYER_TYPE;  return true;}
  if (str == "POINT_CHARGE")   {obj_name = POINT_CHARGE; return true;}
  if (str == "SOLENOID")       {obj_name = SOLENOID;     return true;}
  if (str == "COIL")           {obj_name = COIL;         return true;}
  if (str == "WIRE")           {obj_name = WIRE;         return true;}
  if (str == "RECT_GOAL")      {obj_name = RECT_GOAL;    return true;}
  if (str == "BUMPER")         {obj_name = BUMPER;       return true;}
  
  return false;
}



bool PES_clip_obj(PES_vector2d* pos, PES_vector2d* vel, PES_FLOAT min_y,PES_FLOAT max_y,PES_FLOAT min_x,PES_FLOAT max_x,PES_FLOAT radius)
{
 //check for argument validity
 if( pos == nullptr || vel == nullptr) return false;

 //clip y
 if(pos->y < min_y + radius)
 { 
  pos->y = min_y + radius;
  vel->y = 0; 
 }

 if(pos->y > max_y - radius)
 {
  pos->y = max_y - radius;
  vel->y = 0;
 }

 
 //clip x
 if(pos->x < min_x + radius)
 { 
  pos->x = min_x + radius;
  vel->x = 0; 
 }
 
 if(pos->x > max_x - radius)
 {
  pos->x = max_x - radius;
  vel->x = 0;
 }
}

void
delete_tex_vector (std::vector<SDL_Texture*>& vec)
{
 int n = vec.size();
 for (int i=0; i<n; i++)
 {
  SDL_DestroyTexture(vec[i]);
  vec[i] = nullptr;
 }
 vec.clear();
}


//read font for the game
bool PES_read_font(std::string& current_line, FC_Font* font, SDL_Renderer* renderer,           std::string& game_folder)
{
 
 std::istringstream line_stream(current_line);
 std::string font_path;
 int font_size, r, g, b, a;

 line_stream >> font_path;
 if (line_stream.eof()) {return false;}
 line_stream >> font_size;
 if (line_stream.eof()) {return false;}
 line_stream >> r;
 if (line_stream.eof()) {return false;}
 line_stream >> g;
 if (line_stream.eof()) {return false;}
 line_stream >> b;
 if (line_stream.eof()) {return false;}
 line_stream >> a;

 font_path = game_folder + font_path;

 //if everything was read correctly, call the LoadFont function
 FC_LoadFont(font, renderer, font_path.c_str(), font_size, 
            FC_MakeColor(r,g,b,a), TTF_STYLE_NORMAL); 
 
 return true;
}






