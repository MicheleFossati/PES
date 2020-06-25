//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_arena.h"

//////////////////////////////////////////////////
//             CONSTRUCTORS


//contructor from STRING
PES_arena::PES_arena(std::string game_folder, int screen_width, int screen_height)
{
 read_from_file(game_folder);
 INIT(screen_width, screen_height);
 if(!read_tex(game_folder)) std::cerr << "PES_arena::PES_arena error while loading texs\n" << std::endl;
}

//constructor from c_str
PES_arena::PES_arena(char* game_folder_str, int screen_width, int screen_height)
{
 std::string game_folder(game_folder_str);
 INIT(screen_width, screen_height);
 read_from_file(game_folder);
}





//////////////////////////////////////////////////
//             GET - electric and magnetic field
PES_EL_field
PES_arena::get_tot_EL_field(PES_vector2d pos)
{
 PES_EL_field EL_field; //initialized by default as 0
 int n_point_charges = get_n_obj(POINT_CHARGE);
 int n_wires = get_n_obj(WIRE);

 for (int i = 0; i < n_point_charges; i++)
 {
  EL_field = EL_field + point_charges[i].get_electric_field(pos, table_height);
 }
 
 for (int i = 0; i < n_wires; i++)
 {
  EL_field = EL_field + wires[i].get_electric_field(pos);
 }
 return EL_field;
}



PES_MG_field
PES_arena::get_tot_MG_field(PES_vector2d pos)
{
 PES_MG_field MG_field;
 int n_solenoids = solenoids.size();
 int n_coils = coils.size();
 MG_field = 0;

 for(int i = 0; i< n_solenoids; i++)
 {
  MG_field = MG_field + solenoids[i].get_magnetic_field(pos);
 }

 for(int i=0; i < n_coils; i++)
 {
  MG_field = MG_field + coils[i].get_magnetic_field(pos, table_height);
 }
 return MG_field;
}


//////////////////////////////////////////////////
//             ADD objects

void
PES_arena::add_obj(const PES_disk& new_disk)
{ disks.push_back(new_disk);}

void
PES_arena::add_obj(const PES_player& new_player)
{ players.push_back(new_player);}

//EM stuff
void
PES_arena::add_obj(const PES_point_charge& new_obj)
{ point_charges.push_back(new_obj);}

void
PES_arena::add_obj(const PES_coil& new_obj)
{ coils.push_back(new_obj);}

void
PES_arena::add_obj(const PES_solenoid& new_obj)
{ solenoids.push_back(new_obj);}


void
PES_arena::add_obj(const PES_wire& new_obj)
{ wires.push_back(new_obj);}

//goals
void
PES_arena::add_obj(const PES_rect_goal& new_obj)
{ rect_goals.push_back(new_obj);}

void
PES_arena::add_obj(const PES_bumper& new_obj)
{ bumpers.push_back(new_obj);}

//////////////////////////////////////////////////
//             Initialization
bool 
PES_arena::INIT(int w, int h, PES_FLOAT scale, PES_FLOAT t_height) 
{
 p_width = w;
 p_height = h;
 width = p_width/scale;
 height = p_height/scale;
 table_height = t_height;

 //SDL init
 if (SDL_Init(SDL_INIT_VIDEO) != 0)
 {
  log_SDL_err(std::cout, "SDL_Init_video");
 }

 //Setup our window 
 window = SDL_CreateWindow("PES", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
 if (window == nullptr)
 {
  log_SDL_err(std::cout, "CreateWindow");
  SDL_Quit();
 }
 

 //Setup our renderer 
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 
 if (renderer == nullptr)
 {
  log_SDL_err(std::cout, "CreateRenderer");
  SDL_DestroyWindow(window);
  window = nullptr;
  SDL_Quit();
 }
 
}


//reads from files and loads objects
void PES_arena::read_from_file(std::string game_folder)
{
 
 read_objs_ff<PES_disk>         (game_folder + "disks.txt");
 read_objs_ff<PES_player>       (game_folder + "players.txt");
 
 read_objs_ff<PES_point_charge> (game_folder + "point_charges.txt");
 read_objs_ff<PES_solenoid>     (game_folder + "solenoids.txt");
 read_objs_ff<PES_coil>         (game_folder + "coils.txt");
 read_objs_ff<PES_wire>         (game_folder + "wires.txt");
 read_objs_ff<PES_rect_goal>    (game_folder + "rect_goals.txt");
 read_objs_ff<PES_bumper>       (game_folder + "bumpers.txt");

}

bool
PES_arena::read_tex( std::string folder)
{
 PES_OBJ_NAMES obj_name;
 std::ifstream input;
 std::string line;

 input.open((folder+"images/images.txt").c_str(), std::ifstream::in); 
 if (input.fail())
 {
  input.close();
  std::cerr << "PES_arena::read_tex couldn't open " << folder << std::endl;
  return false;
 }

 PES_get_uncomm_line(input, line);
 b_ground = IMG_LoadTexture (renderer, (folder+line).c_str());
 if (b_ground == nullptr)
 {
  log_SDL_err(std::cout, "PES_arena::read_tex IMG_LoadTexture");
  return false;
 }

 PES_get_uncomm_line( input, line);
 f_ground = IMG_LoadTexture (renderer, (folder+line).c_str());
 if (f_ground == nullptr)
 {
  log_SDL_err(std::cout, "PES_arena::read_tex IMG_LoadTexture");
  return false;
 }
  
 while (!input.eof())
 {
  PES_get_uncomm_line(input, line);
  if (line.empty()) continue;
  if(!is_string_obj_name(obj_name, line))
  {
   if(!add_obj_tex( obj_name, folder+line)) return false;
  }
 }

 return true;
}

bool
PES_arena::add_obj_tex( PES_OBJ_NAMES obj_name, std::string path)
{
 SDL_Texture* t_tex = IMG_LoadTexture (renderer, path.c_str());
 if (t_tex == nullptr)
 {
  log_SDL_err(std::cout, "PES_arena::add_obj_tex");
  return false;
 }
 switch (obj_name)
 {
  case DISK:             disk_texs.push_back(t_tex);           break;
  case PLAYER_TYPE:      player_texs.push_back(t_tex);         break;
  case POINT_CHARGE:     point_charge_texs.push_back(t_tex);   break;
  case SOLENOID:         solenoid_texs.push_back(t_tex);       break;
  case COIL:             coil_texs.push_back(t_tex);           break;
  case WIRE:             wire_texs.push_back(t_tex);           break;
  case RECT_GOAL:        rect_goal_texs.push_back(t_tex);      break;
  case BUMPER:           bumper_texs.push_back(t_tex);         break;

  default: std::cerr << "PES_arena::add_obj_tex  unknown object name"; return false;
 }
 return true;
}


//////////////////////////////////////////////////
//             DRAWING
bool
PES_arena::render()
{

 //render background image
 SDL_RenderCopy(renderer, b_ground, NULL, NULL);

 //idem di read_from_file
 if(!render_objs<PES_solenoid>(solenoids, solenoid_texs))
  return false;
 if(!render_objs<PES_point_charge>(point_charges, point_charge_texs))
  return false; 
 if(!render_objs<PES_coil>(coils, coil_texs))
  return false;
 if(!render_objs<PES_wire>(wires, wire_texs))
  return false;
 if(!render_objs<PES_rect_goal>(rect_goals, rect_goal_texs))
  return false;
 if(!render_objs<PES_bumper>(bumpers, bumper_texs))
  return false;
 
 if(!render_objs<PES_disk>(disks, disk_texs))
  return false;
 if(!render_objs<PES_player>(players, player_texs))
  return false;

//render overground image
 SDL_RenderCopy(renderer, f_ground, NULL, NULL);

 return true;
}


int
PES_arena::get_n_obj(PES_OBJ_NAMES name)  //returns the size of an obj vector
{
 switch (name)
 {
  case DISK:         return disks.size();             break;
  case PLAYER_TYPE:  return players.size();           break; 
  case POINT_CHARGE: return point_charges.size();     break;
  case SOLENOID:     return solenoids.size();         break;
  case COIL:         return coils.size();             break;
  case WIRE:         return wires.size();             break;
  case RECT_GOAL:    return rect_goals.size();        break;
  case BUMPER:       return bumpers.size();           break;
  default:
   return 0;
   break;
 }
}

bool  //check the object number and sends a ERROR message to cerr
PES_arena::check_n_obj(PES_OBJ_NAMES obj_name, int n)
{
 if (n >= get_n_obj(obj_name) || n < 0)
 {
  std::cerr << "ERROR: no object #" << n << " of type " << obj_name
            << " was found.\nExiting with value NULL\n";
  return false;
 }

 return true;
}


//////////////////////////////////////////////////
//             GET - pos and vel
PES_vector2d
PES_arena::get_obj_pos(PES_OBJ_NAMES obj_name, int n)
{
 PES_vector2d empty(0,0);
 //checks
 if (!check_n_obj(obj_name, n)) return empty;

 switch(obj_name)
 {
  case DISK:         return disks[n].get_pos();            break; 
  case PLAYER_TYPE:  return players[n].get_pos();          break;
  case POINT_CHARGE: return point_charges[n].get_pos();    break;
  case SOLENOID:     return solenoids[n].get_pos();        break;
  case COIL:         return coils[n].get_pos();            break;
  case WIRE:         return wires[n].get_pos();            break;
  case RECT_GOAL:    return rect_goals[n].get_pos();       break;
  case BUMPER:       return bumpers[n].get_pos();          break;
  default:
   
   return empty;
   break;
 }
}

PES_vector2d
PES_arena::get_obj_vel(PES_OBJ_NAMES obj_name, int n)
{
 PES_vector2d empty(0,0);
 //checks
 if (!check_n_obj(obj_name, n)) return empty;

 switch(obj_name)
 {
  case DISK : return disks[n].get_vel();                 break;
  case PLAYER_TYPE: return players[n].get_vel();         break;

  default: 
   PES_vector2d empty(0,0);
   return empty;
   break;
 }
}


///////////////////////////////////////////////////////////
//                    SET - pos and vel
bool
PES_arena::set_obj_stat(PES_OBJ_NAMES obj_name, int n, PES_vector2d pos, PES_vector2d vel)
{
 //set the stats
 if (!set_obj_pos(obj_name, n, pos)) return false;
 if (!set_obj_vel(obj_name, n, vel)) return false;

 return true;
}

bool 
PES_arena::set_obj_pos(PES_OBJ_NAMES obj_name, int n, PES_vector2d pos)
{
 //checks
 if (!check_n_obj(obj_name, n)) return false;
 
 switch(obj_name)
 {
  case DISK:         disks[n].set_pos(pos);         return true;  break;
  case PLAYER_TYPE:  players[n].set_pos(pos);       return true;  break;
  case POINT_CHARGE: point_charges[n].set_pos(pos); return true;  break;
 
  default: 
   return true;
   break;
 }
}

bool 
PES_arena::set_obj_vel(PES_OBJ_NAMES obj_name, int n, PES_vector2d vel)
{
 //checks
 if (!check_n_obj(obj_name, n)) return false;
 
 switch(obj_name)
 {
  case DISK:         disks[n].set_vel(vel);    return true;  break;
  case PLAYER_TYPE:  players[n].set_vel(vel);  return true;  break; 

  default: 
   return true;
   break;
 }
}



///////////////////////////////////////////////////
//                    GET - other parameters
PES_FLOAT
PES_arena::get_obj_radius(PES_OBJ_NAMES obj_name, int n)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;

 switch(obj_name)
 {
  case DISK:         return disks[n].get_radius();      break;
  case PLAYER_TYPE:  return players[n].get_radius();    break; 
  case SOLENOID:     return solenoids[n].get_radius();  break;
  case BUMPER:       return bumpers[n].get_radius();    break;

  default:  //no other obj has the parameter radius
   std::cerr << "ERROR: object of type " << obj_name << " has no parameter radius \nGet_obj_radius exited with value 0 \n";
   return 0;
   break;
 }
}


PES_EM_TYPE 
PES_arena::get_obj_charge(PES_OBJ_NAMES obj_name, int n)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;

 switch(obj_name)
 {
  case DISK:          return disks[n].get_charge();        break;
  //case POINT_CHARGE:  return point_charges[n].get_charge();    break; 

  default:  //no other obj has the parameter charge
   std::cerr << "ERROR: object of type " << obj_name << " has no parameter charge \nGet_obj_charge exited with value 0 \n";
   return 0;
   break;
 }
}



SDL_Rect
PES_arena::get_obj_rect(PES_OBJ_NAMES obj_name, int n)
{
 SDL_Rect rect;
 rect.x = 0; rect.y = 0; rect.w = 0; rect.h = 0;
 //checks
 if (!check_n_obj(obj_name, n)) return rect;

 PES_vector2d pos;
 switch(obj_name)
 {
  case DISK:
  {
   PES_FLOAT radius = get_obj_radius(DISK, n);
   pos = get_obj_pos(DISK, n);
   rect.x = map_fti(pos.x - radius);
   rect.y = map_fti(pos.y - radius);
   rect.h = map_fti(radius*2);
   rect.w = map_fti(radius*2);
   break;
  }

  case POINT_CHARGE:
  {
   pos = get_obj_pos(POINT_CHARGE, n);
   rect.x = map_fti(pos.x) - 1;
   rect.y = map_fti(pos.y) - 1;
   rect.w = 3;
   rect.h = 3;
   break;
  }

  case SOLENOID:
  {
   PES_FLOAT radius = get_obj_radius(SOLENOID, n);
   pos = get_obj_pos(SOLENOID, n);
   rect.x = map_fti(pos.x - radius);
   rect.y = map_fti(pos.y - radius);
   rect.h = map_fti(radius*2);
   rect.w = map_fti(radius*2);
   break;
  }

  case COIL:
  {
   pos = get_obj_pos(COIL, n);
   rect.x = map_fti(pos.x) - 1;
   rect.y = map_fti(pos.y) - 1;
   rect.w = 3;
   rect.h = 3;
   break;
  }
  
  case WIRE:
  {
   pos = get_obj_pos(WIRE, n);
   rect.x = map_fti(pos.x) - 1;
   rect.y = map_fti(pos.y) - 1;
   rect.w = 3;
   rect.h = 3;
   break;
  }
  
  case PLAYER_TYPE:
  {
   PES_FLOAT radius = get_obj_radius(PLAYER_TYPE, n);
   pos = get_obj_pos(PLAYER_TYPE, n);
   rect.x = map_fti(pos.x - radius);
   rect.y = map_fti(pos.y - radius);
   rect.h = map_fti(radius*2);
   rect.w = map_fti(radius*2);
   break;
  }
 
  case RECT_GOAL:
  {
   pos = get_obj_pos(RECT_GOAL, n);
   rect.x = map_fti(pos.x - get_obj_width(RECT_GOAL, n)/2);
   rect.y = map_fti(pos.y - get_obj_height(RECT_GOAL, n)/2);
   rect.w = map_fti(get_obj_width(RECT_GOAL, n));
   rect.h = map_fti(get_obj_height(RECT_GOAL, n));
   break;
  }

  case BUMPER:
  {
   PES_FLOAT radius = get_obj_radius(BUMPER, n);
   pos = get_obj_pos(BUMPER, n);
   rect.x = map_fti(pos.x - radius);
   rect.y = map_fti(pos.y - radius);
   rect.h = map_fti(radius*2);
   rect.w = map_fti(radius*2);
   break;
  }

 }
 return rect;

}


PES_FLOAT
PES_arena::get_obj_width(PES_OBJ_NAMES obj_name, int n)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;

 switch(obj_name)
 {
  case RECT_GOAL:  return rect_goals[n].get_width();        break;
  //case POINT_CHARGE:  return point_charges[n].get_charge();    break; 

  default:  //no other obj has the parameter charge
   std::cerr << "ERROR: object of type " << obj_name << " has no parameter width \nGet_obj_width exited with value 0 \n";
   return 0;
   break;
 }
}


PES_FLOAT
PES_arena::get_obj_height(PES_OBJ_NAMES obj_name, int n)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;

 switch(obj_name)
 {
  case RECT_GOAL:  return rect_goals[n].get_height();        break;
  
  default:  //no other obj has the parameter charge
   std::cerr << "ERROR: object of type " << obj_name << " has no parameter height \nGet_obj_height exited with value 0 \n";
   return 0;
   break;
 }
}


PES_FLOAT
PES_arena::get_obj_boost(PES_OBJ_NAMES obj_name, int n)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;

 switch(obj_name)
 {
  case BUMPER:  return bumpers[n].get_boost();        break;
  
  default:  //no other obj has the parameter charge
   std::cerr << "ERROR: object of type " << obj_name << " has no parameter boost \nGet_obj_boost exited with value 0 \n";
   return 0;
   break;
 }
}

////////////////////////////////////////////////////////////////////
//                  SCORES

bool //changes the score of a player
PES_arena::scored(PES_OBJ_NAMES obj_name, int n, int disk)
{
 //checks
 if (!check_n_obj(obj_name, n)) return 0;
 if (!check_n_obj(DISK, disk)) return 0;

 int player; //the player who has to receive the points.
             //it is contained in the goal obj
 int disk_mult = disks[disk].get_goal_score();
 
 switch(obj_name)
 {
  case RECT_GOAL:  
   player = rect_goals[n].get_player();
   players[player].add_points(rect_goals[n].get_score()*disk_mult);
        break;
  case BUMPER:  
   player = bumpers[n].get_player();
   players[player].add_points(bumpers[n].get_score()*disk_mult);
        break;

  default:  //no other obj has the parameter charge
   std::cerr << "ERROR: object of type " << obj_name << " is not a goal \nScored exited with value 0 \n";
   return 0;
   break;
 }
}


int
PES_arena::get_player_score(int player)
{
 if(!check_n_obj(PLAYER_TYPE, player)) return 0;
 return players[player].get_score();
}

PES_vector2d 
PES_arena::get_goal_respawn(int goal)
{
 //checks
 if (!check_n_obj(RECT_GOAL, goal)) return 0;

 return rect_goals[goal].get_respawn();

}

/////////////////////// DESTROY EVERYTHING MUHAHAHAHA //////////////////////
PES_arena::~PES_arena()
{
 delete_tex_vector(disk_texs);
 delete_tex_vector(point_charge_texs);
 delete_tex_vector(solenoid_texs);
 delete_tex_vector(wire_texs);
 delete_tex_vector(coil_texs);
 delete_tex_vector(player_texs);
 delete_tex_vector(rect_goal_texs);
 delete_tex_vector(bumper_texs);
 SDL_DestroyTexture(b_ground);
 SDL_DestroyTexture(f_ground);
 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
}











































