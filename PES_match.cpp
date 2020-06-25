////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_match.h"


///////////////////////////////////////////////////////
//             MAIN LOOP RUN()
void
PES_match::run()
{

 //pre-checks and stuff
 if (arena == nullptr || engine == nullptr || handler == nullptr)
  quit = true;

 //exit loop if some error have occured
 if(quit)
 {
  CLEAN(); return;
 }

 //time
 last_time = current_time = starting_time = game_time = SDL_GetTicks();
 dt = 0;

 //winning player (-1 if nobody has won yet)
 int winning_player = -1;

 //start backgroung music
 handler->play_music();

 ///////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////
 //                       MAIN LOOP
 while(!quit)    
 {
  
  //check the events
  if(!handler->process(dt))
  {
   #ifdef DEBUG 
   std::cerr << "DEBUG: Handler exited with value false.\nExiting the main loop\n";
   #endif
   break;  //violently exiting the game
  }
  //check if handler received a "quit" event
  if (handler->squitting()) break;

  //call engine's process
  if(!engine->process(dt))
  {
   #ifdef DEBUG 
   std::cerr << "DEBUG: Game Engine exited with value false.\nExiting the main loop\n";
   #endif
   break;  //violently exiting the game
  }
  
  ///////////////////////////////////////////////////////////////////
  //rendering on screen
  SDL_RenderClear(renderer);
  if(!arena->render())
  {
   #ifdef DEBUG 
   std::cerr << "DEBUG: Arena renderer exited with value false.\nExiting the main loop\n";
   #endif
   break;  //violently exiting the game
  }

  if(!this->render(renderer))
  {
   #ifdef DEBUG 
   std::cerr << "DEBUG: Match renderer exited with value false.\nExiting the main loop\n";
   #endif
   break;  //violently exiting the game
  }

  //check for a winning player or a timeout
  winning_player = is_over_yet();
  if(winning_player >= 0)
  {
   winning(winning_player);
  }

  SDL_RenderPresent(renderer);

  //update dt and game time
  update_time();

 }//                 END OF MAIN LOOP
 ////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////

 //cleanup everything
 CLEAN();
}


////////////////////////////////////////////////////////////
//         CONSTRUCTORS
PES_match::PES_match(std::string game_folder, PES_GAME_MODE g_mode) : mode(g_mode)
{
 if (!INIT(game_folder))
 { //something wrong hapened in the initialization process. 
   //set quit to false so the loop won't start
  quit = true; 
 }
}


PES_match::PES_match(char* game_folder_c, PES_GAME_MODE g_mode) : mode(g_mode)
{
 std::string game_folder(game_folder_c);

 if (!INIT(game_folder))
 { //something wrong hapened in the initialization process. 
   //set quit to false so the loop won't start
  quit = true; 
 }
}





///////////////////////////////////////////////////////////
//        INIT and similar
bool
PES_match::INIT(std::string game_folder)
{
 //utilities
 int width, height;
 PES_FLOAT mouse_sens, dump;

 //STANDARD
 quit = false;  

 arena = nullptr; 
 engine = nullptr;
 handler = nullptr;
 renderer = nullptr;

 //correct folder format (add / if necessary)
 set_folder_slash(game_folder);

 //allocate the FC_font
 font_time  = FC_CreateFont();
 font_score = FC_CreateFont();
 font_title = FC_CreateFont();


 /////////////////////////////////////////////////
 //              READ
 //read general info from game_folder/match.txt
 if(!read_from_file(game_folder, width, height, mouse_sens, dump))
 {
  std::cerr << "ERROR: could not find/read file " << game_folder << "match.txt\nAborting initialization\n";
  return false;
 }

 //time stuff
 time_delta = 1000/FPS;


 ///////////////////////////////////////////////
 //            CHECKS
 //check width and heigth
 if(width <= 0 || height <= 0) 
 {
  std::cerr << "Invalid width and/or height.\nAborting initialization\n";
  return false;
 }

 //check mouse sensibility
 if(mouse_sens <= 0) 
 {
  std::cerr << "Invalid mouse sensibility.\nAborting initialization\n";
  return false;
 }


 /////////////////////////////////////////////////////////
 //                 ALLOCATE
 //instantiating the ARENA. The object will be instantiated even if
 //some or all the filepath cannot be read. 
 arena.reset(new PES_arena(game_folder, width, height)); // <- ARENA

 if (arena == nullptr) //check
 {
  std::cerr << "Arena couldn't be initialized. PES_create_arena exited with value NULL\nAborting initialization\n";
  return false;
 }
 renderer = arena->get_renderer();


 //instantiating the GAME ENGINE
 engine.reset(new PES_game_engine(arena, dump));          // <- ENGINE ALLOCATION

 if (engine == nullptr) //check
 {
  std::cerr << "Game engine couldn't be initialized. PES_create_engine exited with value NULL\nAborting initialization\n";
  return false;
 }

 //instantiating the EVENT HANDLER
 handler.reset(new PES_handler(arena, mouse_sens, mode)); // <- HANDLER ALLOCATION

 if (handler == nullptr) //check
 {
  std::cerr << "Event handler couldn't be initialized. PES_create_handler exited with value NULL\nAborting initialization\n";
  return false;
 }

 //initialize audio
 if(!handler->init_mixer(game_folder))
 {
  std::cerr << "Mixer couldn't be initialized. init_mixer exited with value NULL\nAborting initialization\n";
  return false;
 }

 //reading and loading FONTS
 if(!read_fonts(game_folder))
 {
  std::cerr << "Couldn't read fonts.\nAborting initialization\n";
  return false;
 }

 return true;    // default return
}//end of INIT()



/////////////////////////////////////////////////////////////////
//read from file
bool
PES_match::read_from_file(std::string game_folder, int& width, int& height, PES_FLOAT& mouse_sens, PES_FLOAT& dump)
{
 //arguments width, height and mouse_sens are passed by reference
 //in order to modify them 

 //STANDARD
 width = 640;
 height = 480;
 mouse_sens = 0.05;
 dump = 0;
 max_time = 60000;
 winning_score = ~0;


 std::string current_line; //string containing lines   |
                           //       if valid goes into V
 std::istringstream line_stream; //iss to be used to decode valid lines
 std::string match_path = game_folder + "match.txt";
 std::ifstream input;

 //open file and check 
 input.open(match_path.c_str(), std::ifstream::in);
 if (input.fail()) {input.close(); return false;}

 //get valid line from input
 PES_get_uncomm_line(input, current_line);
 //check
 if (current_line.size() == 0)//something wrong with the stream or the file
  {input.close(); return false;}

 //read values
 line_stream.str(current_line);

 line_stream >> width;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> height;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> title;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> mouse_sens;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> FPS;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> dump;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> max_time;
 if (line_stream.eof()) {input.close(); return false;}
 line_stream >> winning_score;


 input.close();   //close match.txt
 
 return true;
}//end of read_from_file




//  read fonts
bool
PES_match::read_fonts(std::string game_folder)
{
 std::ifstream input;
 std::string font_info_path = game_folder + "fonts/fonts.txt";
 std::string current_line;

 //open font.txt (contains font infos)
 input.open(font_info_path.c_str(), std::ifstream::in);
 if (input.fail()) {input.close(); return false;} //check

 //time font
 //read valid line
 PES_get_uncomm_line(input, current_line);
 if (current_line.size() == 0)//something wrong with the stream or the file
  {input.close(); return false;}
 //reads .ttf file found in fonts.txt 
 if(!PES_read_font(current_line, font_time, renderer, game_folder))
  {input.close(); return false;}
 
 //player score font
 PES_get_uncomm_line(input, current_line);
 if (current_line.size() == 0)//something wrong with the stream or the file
  {input.close(); return false;}
 if(!PES_read_font(current_line, font_score, renderer, game_folder))
  {input.close(); return false;}

 //title font
 PES_get_uncomm_line(input, current_line);
 if (current_line.size() == 0)//something wrong with the stream or the file
  {input.close(); return false;}
 if(!PES_read_font(current_line, font_title, renderer, game_folder))
  {input.close(); return false;}


 input.close(); //close fonts.txt
 return true;
}



//Update time
void
PES_match::update_time()
{
 // timer. if we are too fast, we'll wait
  current_time = SDL_GetTicks();
  if (current_time < last_time + time_delta) 
  {
   SDL_Delay( last_time + time_delta - current_time ); //wait for the next frame
   dt = time_delta;
  }
  else dt = current_time - last_time; //game is running slow, go immediately to the next frame
  last_time = current_time;
  game_time = current_time - starting_time;
} 


//Winning 
void
PES_match::winning(int player)
{
 //called whan a player has won
 quit = true; //this will stop the game as soon as the function returns

 //get window dimensions (in pixel)
 int p_width  = arena->get_p_width();
 int p_height = arena->get_p_height();


 std::string win_mess  = "Player " + std::to_string(player+1) + " wins!";
 std::string win_score = "Score: " + std::to_string(arena->get_player_score(player));

 //render text in the middle of the screen
 int mess_w = FC_GetWidth (font_title, win_mess.c_str());
 int mess_h = FC_GetHeight(font_title, win_mess.c_str());
 FC_Draw(font_title, renderer, p_width/2-mess_w/2, p_height/2-mess_h/2,
          win_mess.c_str(), "WIN_MESS");
 //render score beneath it
 FC_Draw(font_score, renderer, p_width/2- FC_GetWidth(font_score, win_score.c_str())/2, 
         p_height/2+mess_h/2, win_score.c_str(), "SCORE");

 //render
 SDL_RenderPresent(renderer);

 //wait for 3 seconds
 SDL_Delay(3000);
}

//is over yet
//check is someone has won
int
PES_match::is_over_yet()
{
 int winning_player = 0;
 int n_players = arena->get_n_obj(PLAYER_TYPE);
 if (n_players <= 0) return -1;
 int max_score = arena->get_player_score(0);

 //run through every player's score, chose the highest score
 for (int i = 0; i < n_players; i++)
 {
  if (arena->get_player_score(i) > max_score)
  {
   max_score = arena->get_player_score(i);
   winning_player = i;
  }
 }

 //return winning player is the game is over
 if(max_score >= winning_score || game_time > max_time)
  return winning_player;

 return -1;  //defoult: nobody has won yet
}





//////////////////////////////////////////////////////////////////
//                   RENDER
bool
PES_match::render(SDL_Renderer* renderer)
{
 //here we use FontCache library
 //utilities

 std::string game_time_str = 
            std::to_string(static_cast<int>(game_time/1000));
 //get window dimensions (in pixel)
 int p_width = arena->get_p_width();
 int p_height = arena->get_p_height();


 //     - draw the game time -      ////////////////////////////////
 // get the width to center the writing in the screen
 int time_w = FC_GetWidth(font_time, game_time_str.c_str());
 FC_Draw(font_time, renderer, p_width/2 - time_w/2, 0, 
         game_time_str.c_str(), "game time");


 //     - player scores -     /////////////////////////////////////
 int n_players = arena->get_n_obj(PLAYER_TYPE);
 int player_score;
 std::string player_score_str;
 int player_h;

 for(int i = 0; i<n_players; i++)
 {
  player_score = arena->get_player_score(i);
  player_score_str = std::to_string(player_score);
  player_h =  FC_GetHeight(font_score, player_score_str.c_str());
  FC_Draw(font_score, renderer, 0+p_width/100, p_height - (2-i)*player_h,
          ("player " + std::to_string(i+1) + ": " + player_score_str).c_str(), "player score" + i);
 }


 // - title - /////////////////////////////////////////////////////
 if(game_time < 3000)
 {
  int title_w =  FC_GetWidth(font_title, title.c_str());
  int title_h =  FC_GetHeight(font_title, title.c_str());
  FC_Draw(font_title, renderer, p_width/2-title_w/2, p_height/2-title_h/2,    
          title.c_str(), "TITLE");
 
 
 }
 return true;
}



////////////////////////////////////////////////////////////////////
//CLEAN and DESTRUCTOR
void
PES_match::CLEAN()
{
 //remove SDL stuff 
 SDL_Quit();

 //remove fonts
 if (font_time != nullptr) 
  {FC_FreeFont(font_time); font_time = nullptr;}
 if (font_score != nullptr) 
  {FC_FreeFont(font_score); font_score = nullptr;}
 if (font_title != nullptr) 
  {FC_FreeFont(font_title); font_title = nullptr;}
}

//also overload destructor, just in case
PES_match::~PES_match()
{ CLEAN();}







