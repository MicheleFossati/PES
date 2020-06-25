////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////


#include "PES_types.h"



void
log_SDL_err(std::ostream &os, const std::string &msg)
{
 os << msg << " ERROR: " << SDL_GetError() << std::endl;
}

void
PES_push_user_event(Uint32 type)
{
 SDL_Event new_event;

 SDL_zero(new_event); //clean the memory
 new_event.type = SDL_USEREVENT;
 new_event.user.code = type;

 SDL_PushEvent(&new_event); //push the event
}

