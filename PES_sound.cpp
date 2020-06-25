////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_sound.h"

//////////////////////////////////////////////////////////////////
//                          INIT                                //

PES_mixer::PES_mixer(const std::string& game_folder)
{
 if(!INIT(game_folder))
 {
  #ifdef DEBUG
  std::cerr << "PES_mixer: Couldn't initialize sounds\n";
  #endif
 }
}


bool
PES_mixer::INIT(const std::string& game_folder)
{
 //init SDL audio
 if (SDL_Init(SDL_INIT_AUDIO) != 0)
 {
  #ifdef DEBUG
  log_SDL_err(std::cout, "SDL_Init audio");
  #endif 
  return false;
 }

 //Initialize SDL_mixer 
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
  { return false; }

 //read sounds
 if(!read_from_file(game_folder))
 { 
  return false;
 }

 return true;
}


bool
PES_mixer::read_from_file(const std::string& game_folder)
{
 std::ifstream input;
 std::string current_line;
 std::string audio_path = game_folder + "audio/audio.txt"; 

 //open audio.txt (contains audio infos)
 input.open(audio_path.c_str(), std::ifstream::in);
 if (input.fail()) //check
 {
  input.close(); 
  #ifdef DEBUG
  std::cerr << "PES_mixer::read_from_file: Couldn't find/read " << audio_path << std::endl;
  #endif
  return false;
 } 

 //   - load background music - ///////////////////////////
 PES_get_uncomm_line(input, current_line);
 if (current_line.size() == 0)//something wrong with the stream or the file
  {input.close(); return false;}
 background_music = Mix_LoadMUS((game_folder + current_line).c_str());
 if (background_music == NULL)
 {
  #ifdef DEBUG
  std::cerr << "PES_mixer::read_from_file: Couldn't load background music from " 
            << current_line.c_str() << std::endl;
  #endif
 }

 //  - load music chunks - //////////////////////
 Mix_Chunk* new_chunk;
 PES_get_uncomm_line(input, current_line);
 while(current_line.size() > 0)
 {
  new_chunk = Mix_LoadWAV((game_folder + current_line).c_str());
  if (new_chunk == NULL)
  {
   #ifdef DEBUG
   std::cerr << "PES_mixer::read_from_file: Couldn't read sound from " 
             << game_folder + current_line << std::endl;
   #endif
  }
  else
   chunks.push_back(new_chunk); //add new chunk to chunk vector
 
 //read new line
 PES_get_uncomm_line(input, current_line);
 }

 input.close();

 return true;
}



///////////////////////////////////////////////////////////////////
//                            PLAY                               //
void
PES_mixer::play_music()
{
 //check
 if(background_music == NULL || Mix_PlayingMusic() != 0)
  return;

 Mix_PlayMusic(background_music, -1);
}

void 
PES_mixer::play_chunk(int chunk, int rep) //rep=repetitions. Pass 0 for single sound.
{
 //check
 if(chunk >= chunks.size() || chunk < 0)
  return;

 Mix_PlayChannel(-1, chunks[chunk], rep);
}


//////////////////////////////////////////////////////////////////
//                      DESTRUCTOR                              //
PES_mixer::~PES_mixer()
{
 CLEAN();
}

void
PES_mixer::CLEAN()
{
 int n_chunks = chunks.size();

 //clear the music
 Mix_FreeMusic(background_music);
 background_music = NULL;

 //clear the sound effects
 for(int i = 0; i < n_chunks; i++)
 {
  Mix_FreeChunk(chunks[i]);
  chunks[i] = NULL;
 }

 //quit SDL_Mixer
 Mix_Quit();
}




