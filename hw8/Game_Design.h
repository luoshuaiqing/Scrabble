#include "Player.h"
#include "Board.h"
#include "Move.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "ConsolePrinter.h"
#include "CPUL.h"
#include "CPUS.h"


class Game
{
public:
	Game();
	~Game();
	//read in the configuration file
	void get_config(const char* file_name);
	//check if the game ends
	bool game_end();
	//get the user's name, number, and create players here
	void get_player_info();
	//check if the bag is empty
	bool is_empty_bag();
	//to begin the game
	void begin_scrabble_game();

private:
	//to initialize bag, board, dictionary, initialization file
	void create();

	size_t max_hand_size;
	uint32_t seed;					
	std::string dictionary_file;
	std::string board_file;
	std::string bag_file;
	std::string init_file;
	std::vector<Player*> players;	//this vector only stores human players
	std::vector<Player*> cpu; //this vector only stores computer players
	size_t num_of_players;			//stores the total number of players, including both human and computers
	Bag* bag;
	Dictionary* dictionary;
	ConsolePrinter console;
	Board* board;
	bool to_init;
	bool have_cpu;

};