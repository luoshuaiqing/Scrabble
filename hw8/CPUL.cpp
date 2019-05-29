#include "CPUL.h"
#include "Dictionary.h"
using namespace std;

CPUL::CPUL(std::string const & name, size_t maxTiles, Dictionary* dict, Board* board, Bag* bag):
CPU(name, maxTiles, dict, board, bag),
_dict(dict),
_board(board),
_bag(bag)
{

}

CPUL::~CPUL(){

}

void CPUL::permute_horizontal(vector<Tile*>& current_tiles, int col, string formed_word, int first_tile_row, int first_tile_col){
	while(col < (int)_board->getColumns()){
		if(_board->get_square(first_tile_row, col)->isOccupied()){
			formed_word += _board->get_square(first_tile_row, col)->getLetter();
			if(_dict->is_prefix(formed_word) == false)
				return;
			col++;
		}
		else{
			break;
		}
	}
	//if it is already the last column, then we have checked all the columns
	if(col >= (int)_board->getColumns())
		return;
	if(current_tiles.size() == _hand_tiles.size())
		return;
	for(size_t i = 0; i < _hand_tiles.size(); i++){
		Tile* tile = _hand_tiles[i];
		vector<Tile*>::iterator found = find(current_tiles.begin(), current_tiles.end(), tile);
		//if we did not find the tiles, meaning that we have not yet added the tile to the current tiles
		if(found == current_tiles.end()){
			if(tile->getLetter() != '?'){
				//vertical word is the word above and below the current letter
				string vertical_word = formed_vertical_word(first_tile_row, col, tile->getUse());
				if(vertical_word != ""){
					//change a tile to place here if the vertical word is not legal 
					if(_dict->isLegalWord(vertical_word) == false)
						continue;
				}			
				string new_word = formed_word + tile->getUse();

				//change a tile to place here if the newly formed word is not a prefix of anything
				if(_dict->is_prefix(new_word) == false)
					continue;
				
				else{
					//if the current tile does not lead to any problem, then we add it to the current_tiles
					current_tiles.push_back(tile);
					int _col = col;
					string _new_word = new_word;
					permute_horizontal(current_tiles, col+1, new_word, first_tile_row, first_tile_col);
					//if returned back, we need to check if the current word is legal. If it is legal, then we compare it with the longest word.
					//also, returning back to here means that we tried all the possibilities, and we want to change a tile to put here
					while(_col+1 < (int)_board->getColumns()){
						if(_board->get_square(first_tile_row,_col+1)->isOccupied()){
							_new_word += _board->get_square(first_tile_row,_col+1)->getLetter();
							_col++;
						}
						else
							break;
					}

					if(_dict->isLegalWord(_new_word) && check_have_adjacent(true, current_tiles, first_tile_row, first_tile_col) && 
						check_contain_starting_square(first_tile_row, first_tile_col, current_tiles, true)){
						if(current_tiles.size() > longest_word_ever.size()){
							longest_word_ever = current_tiles;
							string str_row = to_string(first_tile_row + 1);
							string str_col = to_string(first_tile_col + 1);
							string input = turn_tiles_to_string(current_tiles, true, str_row, str_col);
							longest_words.push_back(make_pair(input, (int)current_tiles.size()));
						}
					}
					current_tiles.pop_back(); 
				}
			}
			//if the tile we want to add is "?"
			else{
				current_tiles.push_back(tile);
				for(int j = 0; j < 26; j++){
					char ch = (char) ((int)'a' + j);
					string vertical_word = formed_vertical_word(first_tile_row, col, ch);
					//change a tile to place here if the vertical word is not legal 
					if(vertical_word != ""){
						//change a tile to place here if the vertical word is not legal 
						if(_dict->isLegalWord(vertical_word) == false)
							continue;
					}
					string new_word = formed_word + ch;
					//change a tile to place here if the newly formed word is not a prefix of anything
					if(_dict->is_prefix(new_word) == false)
						continue;
					else{
						
						tile->useAs(ch);	//if everything until now is all good, then this letter is a valid choice for "?", thus make it use as ch
						int _col = col;
						string _new_word = new_word;
						permute_horizontal(current_tiles, col+1, new_word, first_tile_row, first_tile_col);
						while(_col+1 < (int)_board->getColumns()){
							if(_board->get_square(first_tile_row,_col+1)->isOccupied()){
								_new_word += _board->get_square(first_tile_row,_col+1)->getLetter();
								_col++;
							}
							else
								break;
						}
						//if returned back, we need to check if the current word is legal. If it is legal, then we compare it with the longest word.
						//also, returning back to here means that we tried all the possibilities, and we want to change a tile to put here
						if(_dict->isLegalWord(_new_word) && check_have_adjacent(true, current_tiles, first_tile_row, first_tile_col) 
							&& check_contain_starting_square(first_tile_row, first_tile_col, current_tiles, true)){
							if(current_tiles.size() > longest_word_ever.size()){
								longest_word_ever = current_tiles;
								string str_row = to_string(first_tile_row + 1);
								string str_col = to_string(first_tile_col + 1);
								string input = turn_tiles_to_string(current_tiles, true, str_row, str_col);
								longest_words.push_back(make_pair(input, (int)current_tiles.size()));
							}
						}
						else
							tile->useAs('?');
					}
				}
				current_tiles.pop_back(); 
			}
		}
	}
}


void CPUL::permute_vertical(std::vector<Tile*>& current_tiles,
    int row, std::string formed_word, const int first_tile_row, const int first_tile_col){
	//we are not adding anything to the current tiles in the while loop
	while(row < (int)_board->getRows()){
		if(_board->get_square(row, first_tile_col)->isOccupied()){
			formed_word += _board->get_square(row, first_tile_col)->getLetter();
			if(_dict->is_prefix(formed_word) == false)
				return;
			row++;
		}
		else
			break;
	}
	//if it is already the last column, then we have checked all the columns
	if(row >= (int)_board->getRows())
		return;
	if(current_tiles.size() == _hand_tiles.size())
		return;
	
	for(size_t i = 0; i < _hand_tiles.size(); i++){
		Tile* tile = _hand_tiles[i];
		vector<Tile*>::iterator found = find(current_tiles.begin(), current_tiles.end(), tile);
		//if we did not find the tiles, meaning that we have not yet added the tile to the current tiles
		if(found == current_tiles.end()){
			if(tile->getLetter() != '?'){
				//vertical word is the word above and below the current letter
				string horizontal_word = formed_horizontal_word(row, first_tile_col, tile->getUse());
				if(horizontal_word != ""){
					//change a tile to place here if the vertical word is not legal 
					if(_dict->isLegalWord(horizontal_word) == false)
						continue;
				}			
				string new_word = formed_word + tile->getUse();

				//change a tile to place here if the newly formed word is not a prefix of anything
				if(_dict->is_prefix(new_word) == false)
					continue;
				
				else{
					//if the current tile does not lead to any problem, then we add it to the current_tiles
					current_tiles.push_back(tile);

					permute_vertical(current_tiles, row+1, new_word, first_tile_row, first_tile_col);

					//if returned back, we need to check if the current word is legal. If it is legal, then we compare it with the longest word.
					//also, returning back to here means that we tried all the possibilities, and we want to change a tile to put here
					int _row = row;
					string _new_word = new_word;
					while(_row+1 < (int)_board->getRows()){
						if(_board->get_square(_row+1,first_tile_col)->isOccupied()){
							_new_word += _board->get_square(_row+1,first_tile_col)->getLetter();
							_row++;
						}
						else
							break;
					}
					if(_dict->isLegalWord(_new_word) && check_have_adjacent(false, current_tiles, first_tile_row, first_tile_col) && 
						check_contain_starting_square(first_tile_row, first_tile_col, current_tiles, false)){
						if(current_tiles.size() > longest_word_ever.size()){
							longest_word_ever = current_tiles;
							string str_row = to_string(first_tile_row + 1);
							string str_col = to_string(first_tile_col + 1);
							string input = turn_tiles_to_string(current_tiles, false, str_row, str_col);
							longest_words.push_back(make_pair(input, (int)current_tiles.size()));
						}
					}
					current_tiles.pop_back(); 
				}
			}
			//if the tile we want to add is "?"
			else{
				current_tiles.push_back(tile);
				for(int j = 0; j < 26; j++){
					char ch = (char) ((int)'a' + j);
					string horizontal_word = formed_horizontal_word(row, first_tile_col, ch);
					//change a tile to place here if the vertical word is not legal 
					if(horizontal_word != ""){
						//change a tile to place here if the vertical word is not legal 
						if(_dict->isLegalWord(horizontal_word) == false)
							continue;
					}
					string new_word = formed_word + ch;
					//change a tile to place here if the newly formed word is not a prefix of anything
					if(_dict->is_prefix(new_word) == false)
						continue;
					else{
						tile->useAs(ch);	//if everything until now is all good, then this letter is a valid choice for "?", thus make it use as ch
						permute_vertical(current_tiles, row+1, new_word, first_tile_row, first_tile_col);
						int _row = row;
						string _new_word = new_word;
						while(_row+1 < (int)_board->getRows()){
							if(_board->get_square(_row+1,first_tile_col)->isOccupied()){
								_new_word += _board->get_square(_row+1,first_tile_col)->getLetter();
								_row++;
							}
							else
								break;
						}
						//if returned back, we need to check if the current word is legal. If it is legal, then we compare it with the longest word.
						//also, returning back to here means that we tried all the possibilities, and we want to change a tile to put here
						if(_dict->isLegalWord(_new_word) && check_have_adjacent(false, current_tiles, first_tile_row, first_tile_col) 
							&& check_contain_starting_square(first_tile_row, first_tile_col, current_tiles, false)){
							if(current_tiles.size() > longest_word_ever.size()){
								longest_word_ever = current_tiles;
								string str_row = to_string(first_tile_row + 1);
								string str_col = to_string(first_tile_col + 1);
								string input = turn_tiles_to_string(current_tiles, false, str_row, str_col);
								longest_words.push_back(make_pair(input, (int)current_tiles.size()));
							}
						}
						else
							tile->useAs('?');
					}
				}
				current_tiles.pop_back(); 
			}
		}
	}
}


//will return true if it places the tiles. Otherwise it will return false
bool CPUL::to_move(vector<Tile*>& letters_picked_up){
	longest_words.clear();
	longest_word_ever.clear();
	_hand_tiles = getHandTiles_vector();
	size_t start_to_check = 0;
	for(int i = 0; i < (int)_board->getRows(); i++){
		for(int j = 0; j < (int)_board->getColumns(); j++){
			//go to the next square if the current one is already occupied
			if(_board->get_square(i,j)->isOccupied())
				continue;
			//if we are possible to form a word that is adjacent with other tiles on the board, then we want to try that
			if(check_have_adjacent(true, _hand_tiles, i, j) || check_have_adjacent(false, _hand_tiles, i, j)){
				//if it is possible to place the word horizontally, then we want to try that
				if(check_have_adjacent(true, _hand_tiles, i, j)){
					vector<Tile*> current_tiles;
					string word = "";
					int temp_col = j;
					//adding the characters before the word to word
					while(temp_col-1 >= 0 && temp_col-1 < (int)_board->getColumns()){
						if(_board->get_square(i,temp_col-1)->isOccupied())
							temp_col--;
						else
							break;
					}
					while(_board->get_square(i,temp_col)->isOccupied()){
						word += _board->get_square(i,temp_col)->getLetter();
						temp_col++;
					}
					//find the longest possible word HORIZONTALLY in this position
					permute_horizontal(current_tiles, j, word, i, j);
					for(size_t x = 0; x < longest_words.size(); x++){
						//if we used as many hand tiles as we can, then we succeed and do not need to check vertically
						if(longest_words[x].second == (int)_hand_tiles.size()){
							Move* move = Move::parseMove(longest_words[x].first, *this);
							move->execute(*_board, *_bag, *_dict);
							letters_picked_up = move->get_tiles_drawn_from_bag();
							delete move;
							return true;
						}
					}
					start_to_check = longest_words.size();	
				}

				//if we are possible to form a word VERTICALLY that is adjacent with other tiles on the board, then we want to try that
				if(check_have_adjacent(false, _hand_tiles, i, j)){
					vector<Tile*> current_tiles;
					string word = "";
					int temp_row = i;
					//adding the characters above the word to word
					while(temp_row-1 >= 0 && temp_row-1 < (int)_board->getRows()){
						if(_board->get_square(temp_row-1,j)->isOccupied())
							temp_row--;
						else
							break;
					}
					while(_board->get_square(temp_row,j)->isOccupied()){
						word += _board->get_square(temp_row,j)->getLetter();
						temp_row++;
					}
					//find the longest possible word VERTICALLY in this position
					permute_vertical(current_tiles, i, word, i, j);
					for(size_t x = start_to_check; x < longest_words.size(); x++){
						//if we used as many hand tiles as we can, then we succeed and do not need to check vertically
						if(longest_words[x].second == (int)_hand_tiles.size()){
							Move* move = Move::parseMove(longest_words[x].first, *this);
							move->execute(*_board, *_bag, *_dict);
							letters_picked_up = move->get_tiles_drawn_from_bag();
							delete move;
							return true;
						}
					}
				}
			}
		}
	}
	if(longest_word_ever.size() != 0){	//if longest_word_ever is size 0, then we did not form a word, so we want to pass
		int max = -1, max_pos = -1;
		for(size_t i = 0; i < longest_words.size(); i++){
			//if we used as many hand tiles as we can, then we succeed and do not need to check vertically
			if(longest_words[i].second == (int)_hand_tiles.size()){
				Move* move = Move::parseMove(longest_words[i].first, *this);
				move->execute(*_board, *_bag, *_dict);
				letters_picked_up = move->get_tiles_drawn_from_bag();
				delete move;
				return true;
			}
			if(longest_words[i].second > max){
				max = longest_words[i].second;
				max_pos = i;
			}
		}
		Move* move = Move::parseMove(longest_words[max_pos].first, *this);
		move->execute(*_board, *_bag, *_dict);
		letters_picked_up = move->get_tiles_drawn_from_bag();
		delete move;
		return true;	//return true, because we really made a PLACE move
	}
	return false;
}