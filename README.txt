////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

WELCOME to PES - Pinball Edi & Simo

GENERAL OVERVIEW:
Create and play your level with... PES! Man's best friend.



HOW TO USE:
 - install the graphic library SDL2
 - modify the paths in the `Makefile` to match your installation
 - run the file PES_singleplayer or PES_twoplayers 

HOW TO CREATE A GAME:
create (or copy and modify) a game folder which must contain:

  match.txt   general match settings 

  images/
    images.txt       contains the paths of the image files that are directly loaded
                     in the texture variables and vectors 
    many image files
  
  audio/
    audio.txt        contains the paths of the background music and audio chunks
    .wav or .mp3 music files
    
  fonts/
    fonts.txt        contains the paths of the fonts used in the game 
    .ttf fonts

In order to create the game, other files can be added:
(NOTE: if one or more of these files don't exist, the game will give a warning via terminal
       but the game will still boot and be playable, although really boring)
    
  playing object files:
    disks.txt
    players.txt

  electromagnetic object files:
    point_charges.txt
    solenoids.txt
    wires.txt
    coils.txt
    rect_goals.txt

  goal object files:
    rect_goals.txt
    bumpers.txt

HOW TO EXECUTE A GAME:
We provide two game launchers:
 - `twoplayer` with WASD and arrows control
 - `singleplayer` with mouse control
Just launch wichever mode you wish to play with and enjoy. Other mode combination can be
compiled.



