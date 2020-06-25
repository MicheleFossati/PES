////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_event_handler.h"


//initialization
void
PES_handler::INIT(PES_arena_ptr a)
{
 arena = a;

 //default
 quit = false;
 wasd_code = NO_KEY;
 //init SDL mouse stuff
 SDL_SetRelativeMouseMode(SDL_TRUE);

}

PES_handler::PES_handler (PES_arena_ptr a, PES_FLOAT sens, PES_GAME_MODE mode): 
  mouse_sensitivity(sens), game_mode(mode)
{
 INIT(a);
}

//initialize the PES_mixer from the game_folder
bool
PES_handler::init_mixer(const std::string& game_folder)
{
 mixer.reset(new PES_mixer(game_folder));
 if(mixer == NULL)
  return false;

 return true;
}


bool
PES_handler::process(Uint32 dt)
{
 //get the player number, to be used to check if there is at least one player
 int n_players = arena->get_n_obj(PLAYER_TYPE);
 //mouse motion 
 PES_vector2d rel_motion;

 SDL_Event event;
 while(SDL_PollEvent(&event))
 {
  //quit
  if (event.type == SDL_QUIT)
  { 
   quit = true;
  }
  //key press
  else if (event.type == SDL_KEYDOWN)
  {
   switch(event.key.keysym.sym)
   {
    case SDLK_ESCAPE:
     quit = true;
     break;
    default:
     break;
   }
  }


  // - WASD control - //////////////////////////////////////////////////////////////
  if((event.type == SDL_KEYDOWN||event.type == SDL_KEYUP) && event.key.repeat == 0)
  {
   //WASD control is implemented via an 8bit bitmask, representing the keys pushed or not
   switch(event.key.keysym.sym)
   { 
    //WASD
    case SDLK_w:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^W_BMSK); break;
    case SDLK_s:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^S_BMSK); break;
    case SDLK_a:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^A_BMSK); break;
    case SDLK_d:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^D_BMSK); break;
    //ARROWS
    case SDLK_UP:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^UP_BMSK); break;
    case SDLK_DOWN:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^DOWN_BMSK); break;
    case SDLK_LEFT:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^LEFT_BMSK); break;
    case SDLK_RIGHT:
     wasd_code = static_cast<WASD_BITMASK>(wasd_code^RIGHT_BMSK); break;
    default: break;
   }
  }

 
  //user events - SOUND EFFECTS /////////////////////////////////////////////////
  //now we read the USER_EVENTS pushed by the game engine. 
  //music chunks are played accordingly 
  if (event.type == SDL_USEREVENT)
  { 
   int type = event.user.code;
   mixer->play_chunk(type, 0);
  }
  
  // - mouse motion - ////////////////////////////////////////////////////////////
  else if (event.type == SDL_MOUSEMOTION && (game_mode & PES_MOUSE_MODE))
  {
   //process mouse motion only if there is at least one player
   if (n_players >=1)
   {
    rel_motion.x = event.motion.xrel;
    rel_motion.y = event.motion.yrel;
   }
  }  

 }
 // - apply WASD changes - /////////////////////////////////////////////////////
 if(game_mode & PES_WASD_MODE)
  this->WASD_motion(dt);
 // - apply MOUSE changes - /////////////////////////////////////////////////////
 if (game_mode & PES_MOUSE_MODE)
  this->mouse_motion(rel_motion, dt);
 
return true; //default return
}



/////////////////////////////////////////////////////////////////////
// PRIVATE METHODS -- MOUSE
void PES_handler::mouse_motion(PES_vector2d rel_motion, Uint32 dt)
{
 if (dt != 0)
 {
  //use the relative mouse motion to move the player
  PES_vector2d vel = rel_motion*(mouse_sensitivity/(dt/1000.));
  PES_vector2d pos = arena->get_obj_pos(PLAYER_TYPE,0) 
                   + rel_motion*mouse_sensitivity;
 
  //clip the player to the arena
  PES_clip_obj(&pos, &vel, arena->get_player_min_h(0), arena->get_player_max_h(0), 0, arena->get_width(), arena->get_obj_radius(PLAYER_TYPE,0));

  //apply changes
  arena->set_obj_stat(PLAYER_TYPE, 0, pos, vel);
 }
}

// WASD
//move the players (one and sometimes two) 
void 
PES_handler::WASD_motion(Uint32 dt)
{
 if (dt <= 0)
  return;

 int n_players = arena->get_n_obj(PLAYER_TYPE);

 if(n_players > 0)  //first player
  move_player_wasd(0, wasd_code, dt);
 
 if(n_players > 1) //second player
  move_player_wasd(1, wasd_code >> 4, dt);
}

//move player wasd - moves one player using a 4bit bitmask
void
PES_handler::move_player_wasd(int player, Uint8 code, Uint32 dt)
{
 //only the first 4 bits of code are read
 PES_vector2d pos = arena->get_obj_pos(PLAYER_TYPE, player);
 //deconding code
 PES_vector2d motion( ((code & D_BMSK) != 0) - ((code & A_BMSK) != 0),
                      ((code & S_BMSK) != 0) - ((code & W_BMSK) != 0) );
 motion = motion*mouse_sensitivity; //scale
 PES_vector2d vel = motion/(dt/1000.);
 pos = pos + motion;

 //clip the player to the arena
 PES_clip_obj(&pos, &vel, arena->get_player_min_h(player), arena->get_player_max_h(player), 0, arena->get_width(), arena->get_obj_radius(PLAYER_TYPE,player));

 //apply changes
 arena->set_obj_stat(PLAYER_TYPE, player, pos, vel);

}


