////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

/*
      ARENA
Here we define the actual arena where the game takes place

It contains general info about the arena:
 -width and height (both in float and pixel scale)
 -table height (to avoid divergencies)
Object vectors (with relative access via set and get functions)
Graphic stuff from SDL:
 -renderer
 -window
 -texture vectors (relative to the objects)
And some functions :)

Usually included from PES_game_engine.h
*/



#ifndef ARENA
#define ARENA

#include "PES_field_obj.h"
#include "PES_player.h"
#include "PES_goal.h"
#include "PES_global.h"

class PES_arena {
 
 public:
  
 PES_arena();
 ~PES_arena(); 
 PES_arena(std::string, int, int);//load arena from file (suggested constructor)
 PES_arena(char*,int, int);   //so... you still use char*, eh? ok...

 //adding objects. violent polimorfism.
 void add_obj(const PES_disk&);
 void add_obj(const PES_player&);
 void add_obj(const PES_point_charge&);
 void add_obj(const PES_coil&);
 void add_obj(const PES_solenoid&);
 void add_obj(const PES_wire&);
 void add_obj(const PES_rect_goal&);
 void add_obj(const PES_bumper&);

 template <class T> bool rm_obj (int); //actually, never used it

 //get EM fields
 //returns the total field in a particular position
 PES_EL_field get_tot_EL_field(PES_vector2d pos);
 PES_MG_field get_tot_MG_field(PES_vector2d pos);


 /////////////////// get and set functions ///////////////////////////

 PES_vector2d get_obj_pos(PES_OBJ_NAMES, int);
 PES_vector2d get_obj_vel(PES_OBJ_NAMES, int);

 bool set_obj_pos (PES_OBJ_NAMES, int, PES_vector2d);
 bool set_obj_vel (PES_OBJ_NAMES, int, PES_vector2d);
 bool set_obj_stat(PES_OBJ_NAMES, int, PES_vector2d, PES_vector2d);

 bool        check_n_obj    (PES_OBJ_NAMES, int); //returns false if that object doesn't exist
 int         get_n_obj      (PES_OBJ_NAMES);
 PES_FLOAT   get_obj_radius (PES_OBJ_NAMES, int);
 PES_EM_TYPE get_obj_charge (PES_OBJ_NAMES, int);
 SDL_Rect    get_obj_rect   (PES_OBJ_NAMES, int); //returns the SDL_rect with right dimensions and positions, to render the texture inside 

 PES_FLOAT get_obj_width (PES_OBJ_NAMES, int); 
 PES_FLOAT get_obj_height(PES_OBJ_NAMES, int); 

 PES_FLOAT get_obj_boost(PES_OBJ_NAMES, int); //returns bumper's boost
 PES_vector2d get_goal_respawn(int);

 ///////////////////////////////////////////////

 //scores
 //call it when a score event has occurred. arguments: goal name, goal number, disk number
 bool scored(PES_OBJ_NAMES, int, int); 
 int get_player_score(int n);
 

 //partial access to players
 PES_FLOAT get_player_max_h(int player) {return players[player].get_max_h();};  //  <- player's play box 
 PES_FLOAT get_player_min_h(int player){return players[player].get_min_h();};  //
 
 //access to the disks
 PES_MASS_TYPE get_disk_mass(int disk)  {return disks[disk].get_mass();};

 ///////////// graphic functions /////////////
 SDL_Renderer* get_renderer(){return renderer;}; 

 //float
 PES_FLOAT get_width()  {return width;};
 PES_FLOAT get_height() {return height;};
 
 //pixels
 PES_INT get_p_width()  {return p_width;};
 PES_INT get_p_height() {return p_height;};

 //renders all the objects in the arena
 bool render();



 private:

  //graphic stats (p stands for pixel)
  PES_INT p_width, p_height;

  //SDL objects 
  SDL_Window *window;
  SDL_Renderer *renderer;

  /////// PLAYING FIELD
  //field stats
  PES_FLOAT width, height;
  PES_FLOAT table_height;
  
  // electromagnetic objects
  std::vector <PES_point_charge> point_charges;
  std::vector <PES_coil>         coils;
  std::vector <PES_solenoid>     solenoids;
  std::vector <PES_wire>         wires;

  //scoring objects
  std::vector <PES_rect_goal>    rect_goals;
  std::vector <PES_bumper>       bumpers;

  //disks and players
  std::vector <PES_disk> disks;
  std::vector <PES_player> players; 

  /////// TEXTURES
  std::vector <SDL_Texture*> point_charge_texs;
  std::vector <SDL_Texture*> solenoid_texs;
  std::vector <SDL_Texture*> coil_texs;
  std::vector <SDL_Texture*> wire_texs;  

  std::vector <SDL_Texture*> rect_goal_texs;
  std::vector <SDL_Texture*> bumper_texs;

  std::vector <SDL_Texture*> player_texs;
  std::vector <SDL_Texture*> disk_texs;

  SDL_Texture* b_ground;
  SDL_Texture* f_ground; 

  /////// PRIVATE FUNCTIONS

  bool INIT(int w, int h, PES_FLOAT scale=100, PES_FLOAT t_height = 0.4);  //initialize SDL objects (window and renderer)
  PES_INT map_fti(PES_FLOAT x) {return x*p_width/width;};  //maps physic float to graphic int
  PES_FLOAT map_itf(int x) {return x*width/p_width;}; //inverse transformation

  void read_from_file(std::string); //reads from files and loads objects
  bool add_obj_tex( PES_OBJ_NAMES, std::string);  //creates a texture from a path and adds it in the relative textures vector

  template <class T> bool read_objs_ff(std::string); //reads a file and loads the specified objects by calling their constructor

  //graphic funcions
  bool read_tex (std::string);   //reads and loads textures from /images/images.txt
 
  template <class T> 
   bool render_objs(std::vector<T>&, std::vector<SDL_Texture*>&); //renders all the objects contained in a vector
 
};


typedef std::shared_ptr<PES_arena> PES_arena_ptr;

//      templates
//          |
//          |
//          V












//////////////////////////////////////////////////////////////////////

//remove objects
template <class T>
bool
PES_arena::rm_obj(int n)
{

 //checks
 if (n < 0) return false;

 if(std::is_same<T,PES_disk>::value) 
 {
  if(n>disks.size()) return false;
  disks.erase(disks.begin() + n);
  return true;
 }

 if(std::is_same<T,PES_player>::value) 
 {
  if(n>players.size()) return false;
  players.erase(players.begin() + n);
  return true;
 }

 if(std::is_same<T,PES_point_charge>::value) 
 {
  if(n>point_charges.size()) return false;
  point_charges.erase(point_charges.begin() + n);
  return true;
 }

 //TODO: other objects

 return false;
}


//read obj vector from filename ////////////////////////////////////////
template <class T>
bool
PES_arena::read_objs_ff(std::string file_path)
{
 std::ifstream input;
 std::string current_line;

 //try to open file
 input.open(file_path.c_str(), std::ifstream::in);

 if (input.fail())
 {
  #ifdef DEBUG
  std::cerr << "Couldn't open " << file_path << std::endl;
  #endif
  return false;
 }
 do
 {
  PES_get_uncomm_line(input, current_line);
  if (current_line.size() == 0)//end of file     |
   break; //exit the cicle                   <---'

   T new_obj(current_line);
   this->add_obj(new_obj);
 } while (!input.eof());
 
return true;
}



//render objs tex ///////////////////////////////////////////////////////
template <class T>
bool
PES_arena::render_objs(std::vector<T>& obj_vec, std::vector<SDL_Texture*>& tex_vec)
{
 int tex_size = tex_vec.size();
 int obj_size = obj_vec.size();
 PES_OBJ_NAMES obj_name = PES_type_to_name<T>();

 for(int n = 0; n < obj_size; n++)
 { 
  int im = (obj_vec)[n].get_image(); 
  if ( im >= tex_size ) 
  {
   return false;
  }
  SDL_Rect destination = get_obj_rect(obj_name, n);
  SDL_RenderCopy(renderer, (tex_vec)[im], NULL, &destination);
 }

return true;
}




#endif



