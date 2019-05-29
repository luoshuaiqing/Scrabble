#include"Game_Design.h"

int main(int argc, char const *argv[])
{
	Game game;
	game.get_config(argv[1]);	//read in the configuration 
	game.begin_scrabble_game();	//start the game
	return 0;
}


