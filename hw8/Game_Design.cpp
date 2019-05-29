#include "Game_Design.h"
#include <iostream>	
using namespace std;	

Game::Game(){
	to_init = false;
	have_cpu = false;
}


Game::~Game()
{
	delete bag;
	delete board;

	delete dictionary;
	for(size_t i = 0; i < players.size(); i++)
		delete players[i];

	for(size_t i = 0; i < cpu.size(); i++)
		delete cpu[i];
}



void Game::begin_scrabble_game()
{
	//giving the tiles to the AI
	for(size_t i = 0; i < cpu.size(); i++){
		vector<Tile*> drawn_tiles_from_bag = bag->drawTiles(max_hand_size - cpu[i]->num_hand_tiles());
		cpu[i]->addTiles(drawn_tiles_from_bag);
	}
	for(size_t i = 0; i < players.size(); i++){			//updating the handtiles of all the players.
		vector<Tile*> drawn_tiles_from_bag = bag->drawTiles(max_hand_size - players[i]->num_hand_tiles());
		players[i]->addTiles(drawn_tiles_from_bag);
	}
	
	bool to_end_pass = false;
	size_t num_pass = 0;
	// bool game_end_AI = false;
	// bool game_end_player = false;
	while(!game_end() && !to_end_pass){		//as long as the game did not end and not all players choose to pass
		//AI get moved first
		if(have_cpu){
			for(size_t i = 0; i < cpu.size(); i++){
				bool to_pass = false;
				//there will not be an error for AI, so we dont need to catch the error or ask the AI to try again.
				//we only need to print the board once
				console.printBoard(*board);
				console.printHand(*(cpu[i]));
				vector<Tile*> letters_picked_up;	//to store the letters picked up from the bag
				vector<string> new_word_formed;		//to store the latest formed word
				int original_score = cpu[i]->show_player_score();
				cout << "Current Score: " << original_score << endl;

				// cout << "1" << endl;
				bool to_place = cpu[i]->to_move(letters_picked_up);

				// cout << "2" << endl;
				//if the AI chooses to place
				if(to_place){
					// cout << "3" << endl;
					cout << cpu[i]->get_player_name() << " chooses to PLACE tiles." << endl;
					num_pass = 0;
					new_word_formed = board->new_words;
				}
				else{
					// cout << "4" << endl;
					cout << cpu[i]->get_player_name() << " chooses to PASS" << endl;
					num_pass++;
					to_pass = true;
					if(num_pass == num_of_players){
						to_end_pass = true;			
						cout << "\nAll players, including AI (if there is any), choose to PASS.\nThe game ends!" << endl;
						break;
					}					
				}
				console.printBoard(*board);			//print out the results of the move
				console.printHand(*(cpu[i]));
				cout << cpu[i]->get_player_name() << "'s current score is " << cpu[i]->show_player_score() << endl;	
				cout << "You earned " << cpu[i]->show_player_score() - original_score << " in this turn." << endl;
				
				if(!to_pass){
					cout << "The word you formed: ";
					for(size_t k = 0; k < new_word_formed.size(); k++)
						cout << new_word_formed[k] << " ";
					cout << endl;
				}
				else
					cout << "You did not form any word in this turn." << endl;
				
				cout << "New letters picked up in this turn: ";
				for(size_t k = 0; k < letters_picked_up.size(); k++)
					cout << letters_picked_up[k]->getLetter() << " ";
				cout << endl;
				cin.ignore(256, '\n');
				if(game_end())
					break;
			}
			
		}
		if(to_end_pass || game_end())
			break;
		//players get moved after the AI moved
		for(size_t i = 0; i < players.size(); i++){
			vector<Tile*> letters_picked_up;	//to store the letters picked up from the bag
			vector<string> new_word_formed;		//to store the latest formed word
			int original_score = players[i]->show_player_score();	//to store the player's score before he chooses his action

			if(to_end_pass)	//the game ends if all players choose to pass
				break;
			bool succeed = false;
			bool to_print_board = true;
			bool formed_new_words = false;
			while(!succeed){
				if(to_print_board){
					console.printBoard(*board);
					console.printHand(*(players[i]));
				}
				cout << "Current Score: " << original_score << endl;
				cout << players[i]->get_player_name() << ", please choose to PASS, EXCHANGE, or PLACE tiles." << endl;
				string choice;
				getline(cin, choice);
				Move* move = Move::parseMove(choice, *(players[i]));		//used DYNAMIC MEMORY here. Remember to DELETE later	
				
				if(move->isPass()){											//parse the action that the player wants to execute
					num_pass++;
					cout << players[i]->get_player_name() << " chooses to PASS" << endl;
				}
				else if(move->isExchange()){
					cout << players[i]->get_player_name() << " chooses to EXCHANGE" << endl;
					num_pass = 0;
				}
				else if(move->isWord()){
					cout << players[i]->get_player_name() << " chooses to PLACE" << endl;
					num_pass = 0;
				}

				if(num_pass == num_of_players){
					to_end_pass = true;			
					cout << "\nAll players, including AI (if there is any), choose to PASS.\nThe game ends!" << endl;
					delete move;
					break;
				}

				try{
					move->execute(*board, *bag, *dictionary);	//to execute the move according to its type, whether pass/exchange/place
					succeed = true;			//the player does not need to reenter his choice only if there is no error during his move
					if(move->isWord())
						formed_new_words = true;
				} catch(logic_error& e){
					if(move->isWord())
						players[i]->addTiles(move->tileVector());	//if there is an error, players should not withdraw his tiles from his hand
					cout << "\nA logic error occured!" << endl;
					cout << e.what();
					cout << " Please try again!\n" << endl;
					to_print_board = false;					//do not show the board again if an error occurs
					
				} catch(exception &e){
					if(move->isWord())
						players[i]->addTiles(move->tileVector());
					cout << "\nGeneral exception error." << endl;
					cout << "Please try again!\n" << endl;
					to_print_board = false;

				}

				letters_picked_up = move->get_tiles_drawn_from_bag();
				if(formed_new_words)
					new_word_formed = board->new_words;
				
				delete move;
			}
			console.printBoard(*board);			//print out the results of the move
			console.printHand(*(players[i]));
			cout << players[i]->get_player_name() << "'s current score is " << players[i]->show_player_score() << endl;	
			cout << "You earned " << players[i]->show_player_score() - original_score << " in this turn." << endl;
			if(formed_new_words){
				cout << "The word you formed: ";
				for(size_t k = 0; k < new_word_formed.size(); k++)
					cout << new_word_formed[k] << " ";
				cout << endl;
			}
			else
				cout << "You did not form any word in this turn." << endl;
			
			cout << "New letters picked up in this turn: ";
			for(size_t k = 0; k < letters_picked_up.size(); k++)
				cout << letters_picked_up[k]->getLetter() << " ";
			cout << endl;
			cin.ignore(256, '\n');
			if(game_end())
				break;
		}
	}
	
	size_t num_of_players = cpu.size() + players.size();
	vector<Player*> Players;
	for(size_t i = 0; i < cpu.size(); i++)
		Players.push_back(cpu[i]);
	for(size_t i = 0; i < players.size(); i++)
		Players.push_back(players[i]);
	
	for(size_t i = 0; i < Players.size(); i++)	Players[i]->update_player_score(-1 * Players[i]->score_of_hand_tiles());

	if(game_end()){
		size_t pos_player_zero_tiles;
		int left_tiles_score = 0;;
		for(size_t i = 0; i < num_of_players; i++){
			if(Players[i]->num_hand_tiles() == 0){
				pos_player_zero_tiles = i;
			}
			else{
				left_tiles_score += Players[i]->score_of_hand_tiles();
			}
		}
		Players[pos_player_zero_tiles]->update_player_score(left_tiles_score);
	}

	int highest_score = Players[0]->show_player_score();		//get the position of the player who has the highest score
	size_t highest_score_pos = 0;
	for(size_t i = 0; i < num_of_players; i++){
		if(Players[i]->show_player_score() >= highest_score){
				highest_score_pos = i;
				highest_score = Players[i]->show_player_score();
		}
	}

	bool multiple_winners = false;
	for(size_t i = 0; i < num_of_players; i++){
		//if several people has the same highest score
		if(i != highest_score_pos && Players[i]->show_player_score() == Players[highest_score_pos]->show_player_score()){
			multiple_winners = true;
			break;
		}
	}	

	for(size_t i = 0; i < num_of_players; i++){
		if(Players[i]->show_player_score() == Players[highest_score_pos]->show_player_score() && multiple_winners)
			cout << "Winners: " << Players[i]->get_player_name() << " ";
		if(!multiple_winners){
			cout << "Winner: " << Players[highest_score_pos]->get_player_name() ;
			break;
		}
	}
	cout << endl << endl;
	for(size_t i = 0; i < num_of_players; i++)
		cout <<  Players[i]->get_player_name() << "'s score: " << Players[i]->show_player_score() << endl;
}




void Game::create()	// store the info needed to start the game 
{
	board = new Board(board_file, to_init);
	try{
		bag = new Bag(bag_file, seed);
	} catch(exception &e){
		cout << "Bag Openning Error occured!" << endl;
		cout << "Please check your configuratation file and run the game again!" << endl;
	}
	dictionary = new Dictionary(dictionary_file);
	if(to_init){
		board->to_init(init_file);
	}
	get_player_info();


}

void Game::get_player_info()
{
	cout << "Please enter the number of players in total."<<endl;			//get players' information
	cin >> num_of_players;
	cin.ignore();
	if(!(num_of_players <= 8 && num_of_players > 0)){
		cout << "The number of players should be between 1 - 8. Please try again!" << endl;
		cin >> num_of_players;
		cin.ignore();
	}
	for(size_t i = 0; i < num_of_players; i++){
		cout << "Please enter player_" << i+1 << "'s name: " << endl;
		string name;
		getline(cin, name);
		bool is_CPUS = false;
		bool is_CPUL = false;
		// cout << name.size() << endl;
		//checking whether the player will be AI or not
		if(name.size() >= 4){
			if((name[0] == 'c' || name[0] == 'C') && (name[1] == 'p' || name[1] == 'P') && (name[2] == 'u' || name[2] == 'U') && (name[3] == 's' || name[3] == 'S'))
				is_CPUS = true;
			if((name[0] == 'c' || name[0] == 'C') && (name[1] == 'p' || name[1] == 'P') && (name[2] == 'u' || name[2] == 'U') && (name[3] == 'l' || name[3] == 'L'))
				is_CPUL = true;
		}
		if(is_CPUS){
			cpu.push_back(new CPUS(name, max_hand_size, dictionary, board, bag));	//need to be added extra parameters here
			have_cpu = true;
		}
		else if(is_CPUL){
			// cout << "here" << endl;	//debugging
			cpu.push_back(new CPUL(name, max_hand_size, dictionary, board, bag));	//need to be added extra parameters here
			have_cpu = true;
			// cout << "hi" << endl;
		}
		else
			players.push_back(new Player(name, max_hand_size));
	}
	
}

bool Game::is_empty_bag(){			//check if the bag is empty
	if(bag->tilesRemaining() == 0)
		return true;
	return false;
}

bool Game::game_end()				//check if the game ends
{
	if(is_empty_bag()){
		for(size_t i = 0; i < players.size(); i++){
			if(players[i]->num_hand_tiles() == 0)
				return true;
		}
		for(size_t j = 0; j < cpu.size(); j++){
			if(cpu[j]->num_hand_tiles() == 0)
				return true;
		}
	}
	return false;
}




void Game::get_config(const char* file_name)	//get configuration 
{
	ifstream config_file(file_name);
	string myline;

	while(getline(config_file, myline)){	
		if(!myline.empty()){
			if(myline[0] == 'H'){
				size_t index = myline.find_first_of(":");
				string str = myline.substr(index + 1);	//get the string after ":"
				max_hand_size = stoi(str);
			}
			
			else if(myline[0] == 'B' || myline[0] == 'T' || myline[0] == 'D' || myline[0] == 'I'){
				size_t index = myline.find_first_of(":");
				string str = myline.substr(index + 1);		//get the string after ":"
				size_t index_2 = str.find_first_not_of(" \t");	//get the string that is not whitespace
				string str2 = str.substr(index_2);

				size_t index_final = str2.find_first_of(" \t");

				if(myline[0] == 'B'){
					if(index_final != str2.npos)	// if there is whitespace at the end of str2
						board_file = str2.substr(0, index_final);
					else
						board_file = str2;
				}
				else if(myline[0] == 'T'){
					if(index_final != str2.npos)	// if there is whitespace at the end of str2
						bag_file = str2.substr(0, index_final);
					else
						bag_file = str2;
				}
				else if(myline[0] == 'D'){
					if(index_final != str2.npos)	// if there is whitespace at the end of str2
						dictionary_file = str2.substr(0, index_final);
					else
						dictionary_file = str2;	
				}
				else if(myline[0] == 'I'){
					to_init = true;
					if(index_final != str2.npos)	// if there is whitespace at the end of str2
						init_file = str2.substr(0, index_final);
					else
						init_file = str2;
				}	
			}

			else if(myline[0] == 'S'){
				size_t index = myline.find_first_of(":");
				string str = myline.substr(index + 1);
				seed = stoi(str);
			}
		}
	}
	create();	//use the configuration file to call the create function
}