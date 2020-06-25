////////////////////////////////////////////////////////
//                        PES                         //
//         written by: Wronsky & Edi Edison           //
////////////////////////////////////////////////////////

#include "PES_match.h"

int main(int argc, char* argv[])
{
 std::string game_folder;

 //title
 std::cout << "////////////////////////////////////////////////////////  \n";
 std::cout << "//                        PES                         //  \n";
 std::cout << "//         written by: Wronsky & Edi Edison           //  \n";
 std::cout << "////////////////////////////////////////////////////////  \n\n";


 //check if a match folder has been passed as a main argument
 if(argc > 1)
  game_folder = argv[1];
 else
 {
  std::cout << "PES - singleplayer laucher. Enter game folder:\n";
  std::cin >> game_folder;
 }

 //initialize the match with the game folder
 PES_match match(game_folder, SINGLEPLAYER_MOUSE);

 //run the game
 match.run();

 //goodbye
 std::cout << "I must away now, I can no longer terry. \n";

 return 0;
}


