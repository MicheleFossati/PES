CXX = g++
CC = gcc #compiler for C files

# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install

# Simo paths
#SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf \
          -lSDL2_mixer  -Wl,-rpath=/usr/local/lib
#SDL_INCLUDE = -I/usr/include/SDL2

# Edi paths
SDL_LIB = -L/usr/local/Cellar/ -lsdl2 -lsdl2_image -lsdl2_ttf \
          -lSDL2_mixer  -Wl,-rpath,/usr/local/Cellar
SDL_INCLUDE = -I/usr/local/include/SDL2

 
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CXXFLAGS =  -ggdb -c  -std=c++11 $(SDL_INCLUDE)
CFLAGS = -ggdb -c -std=c11 $(SDL_INCLUDE)      # C flags
LDFLAGS = $(SDL_LIB)


objects = PES_data_templates.o \
          PES_EM_structs.o  \
          PES_field_obj.o \
          PES_player.o \
          PES_arena.o \
          PES_game_engine.o  \
          PES_event_handler.o \
          PES_match.o  \
          PES_goal.o \
          PES_global.o \
          PES_sound.o \
          SDL_FontCache.o
          
headers = PES_EM_structs.h \
          PES_types.h \
          PES_data_templates.h \
          PES_field_obj.h\
          PES_arena.h \
          PES_game_engine.h \
          PES_player.h \
          PES_event_handler.h \
          PES_match.h \
          PES_goal.h \
          PES_global.h \
          PES_sound.h \
          SDL_FontCache.h 

exe = PES_singleplayer PES_twoplayers

all: $(exe)

PES_singleplayer :  $(objects) PES_singleplayer.o
	$(CXX) PES_singleplayer.o $(objects) $(LDFLAGS) -o $(@F)

PES_twoplayers :  $(objects) PES_twoplayers.o
	$(CXX) PES_twoplayers.o $(objects) $(LDFLAGS) -o $(@F)

$(objects) : $(headers)

PES_singleplayer.o : PES_singleplayer.cpp $(headers)
	$(CXX) $(CXXFLAGS) PES_singleplayer.cpp -o PES_singleplayer.o

PES_twoplayers.o : PES_twoplayers.cpp $(headers)
	$(CXX) $(CXXFLAGS) PES_twoplayers.cpp -o PES_twoplayers.o

# Compile SDL_FontCache.c using gcc instead of g++
SDL_FontCache.o : SDL_FontCache.c $(headers)
	$(CC) $(CFLAGS) SDL_FontCache.c -o SDL_FontCache.o

clean:
	rm -f $(objects) PES_singleplayer.o PES_twoplayers.o
