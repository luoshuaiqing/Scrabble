#include "CPUS.h"
#include "Dictionary.h"
using namespace std;

CPUS::CPUS(string const & name, size_t maxTiles, Dictionary* dict, Board* board, Bag* bag):
CPU(name, maxTiles, dict, board, bag),
_dict(dict),
_board(board),
_bag(bag),
highest_score(make_pair("",-1))
{

}

CPUS::~CPUS(){

}


unsigned int CPUS::calculate_score(vector<Tile*> tiles_to_place, size_t row, size_t col, bool horizontal){
	size_t num_rows = _board->getRows();
	size_t num_cols = _board->getColumns();
	vector<pair<string, unsigned int>> words_formed;
	if(!horizontal){	//to check if there is anything on the left or right of the tile
		size_t tile_row = row;
		for(size_t i = 0; i < tiles_to_place.size(); i++){
			if(i != 0)	
				tile_row++;
			if(tile_row >= num_rows)	//if it is already out of bound but we are still trying to add a tile
				return -1;
			while(_board->get_square(tile_row,col)->isOccupied()){		//read until _board->get_square[tile_row][col] is not occupied. We want to find a place where we can put our tile on
				if(++tile_row >= num_rows)			
					return -1;
			}
			string word;
			unsigned int score = 0, multiply_word = _board->get_square(tile_row,col)->getWMult();
			size_t left = col, right = col, temp_col = col;
			while(temp_col - 1 >= 0 && temp_col != 0){
				if(_board->get_square(tile_row,temp_col-1)->isOccupied())
					left = --temp_col;		//read until left is the exact index of the left-most occupied square 
				else	break;
			}
			for(size_t j = left; j < col; j++){				//get the word and the score on the left
				word += _board->get_square(tile_row,j)->getLetter();
				score += _board->get_square(tile_row,j)->getScore();
			}
			word += tiles_to_place[i]->getUse();
			score += tiles_to_place[i]->getPoints() * _board->get_square(tile_row,col)->getLMult();	//Letter multiply works if it is the tile that we are really adding
			
			temp_col = col;
			while(temp_col + 1 < num_cols){
				if(_board->get_square(tile_row,temp_col+1)->isOccupied())
					right = ++temp_col;			//read until right is the exact index of the right-most occupied square 
				else	break;
			}
						
			for(size_t j = col+1; j <= right; j++){
				word += _board->get_square(tile_row,j)->getLetter();		//updating the word and the score
				score += _board->get_square(tile_row,j)->getScore();
			}
			score *= multiply_word;
			if(_board->word_size(word) != 1){	//if the word formed is not size 1, then we can push it to the words_formed
				transform(word.begin(), word.end(), word.begin(), ::toupper);
				words_formed.push_back(pair<string, unsigned int>(word, score));	
			}	
		}
		string main_word;	//trying to examine the main word formed
		size_t temp_row = row, index_tiles_to_place = 0, main_word_score = 0, main_word_multiply = 1;
		while(temp_row-1 >= 0 && temp_row != 0){
			if(_board->get_square(temp_row-1,col)->isOccupied())
				temp_row--;			//read until temp_row is the exact index of the top-most occupied square 
			else	break;
		}
		while(index_tiles_to_place < tiles_to_place.size() || _board->get_square(temp_row,col)->isOccupied()){
			if(_board->get_square(temp_row,col)->isOccupied()){
				main_word += _board->get_square(temp_row,col)->getLetter();
				main_word_score += _board->get_square(temp_row++,col)->getScore();
				if(temp_row >= num_rows)
					break;
			}
			else{				//if there is no tile on the square, which means we are going to add player's tile on this square
				main_word += tiles_to_place[index_tiles_to_place]->getUse();
				main_word_score += tiles_to_place[index_tiles_to_place++]->getPoints() *_board->get_square(temp_row,col)->getLMult();
				main_word_multiply *= _board->get_square(temp_row++,col)->getWMult();
				if(temp_row >= num_rows)
					break;
			}
		}
		main_word_score *= main_word_multiply;
		transform(main_word.begin(), main_word.end(), main_word.begin(), ::toupper);
		if(_board->word_size(main_word) != 1)		words_formed.push_back(pair<string, unsigned int>(main_word, main_word_score));
	}
	else if(horizontal)	//to check if there is anything on the top or bottom of the tile
	{	
		size_t tile_col = col;
		for(size_t i = 0; i < tiles_to_place.size(); i++){
			if(i != 0)	tile_col++;
			if(tile_col >= num_cols)	//if it is already out of bound but we are still trying to add a tile
				return -1;
			while(_board->get_square(row,tile_col)->isOccupied()){		//read until _board->get_square[row][tile_col] is not occupied. We want to find a place where we can put our tile on
				if(++tile_col >= num_cols)			return -1;
			}
			string word;
			unsigned int score = 0, multiply_word = _board->get_square(row,tile_col)->getWMult();
			size_t top = row, bot = row, temp_row = row;
			while(temp_row - 1 >= 0 && temp_row != 0){
				if(_board->get_square(temp_row-1,tile_col)->isOccupied())
					top = --temp_row;		//read until top is the exact index of the top-most occupied square 
				else 	break;
			}
			for(size_t j = top; j < row; j++){				//get the word and the score on the top
				word += _board->get_square(j,tile_col)->getLetter();
				score += _board->get_square(j,tile_col)->getScore();
			}
			word += tiles_to_place[i]->getUse();
			score += tiles_to_place[i]->getPoints() * _board->get_square(row,tile_col)->getLMult();	//Letter multiply works if it is the tile that we are really adding
			temp_row = row;
			while(temp_row + 1 < num_rows){
				if(_board->get_square(temp_row+1,tile_col)->isOccupied())
					bot = ++temp_row;			//read until bot is the exact index of the bottom-most occupied square 
				else	break;
			}	
			for(size_t j = row+1; j <= bot; j++){
				word += _board->get_square(j,tile_col)->getLetter();
				score += _board->get_square(j,tile_col)->getScore();
			}
			score *= multiply_word;
			if(_board->word_size(word) != 1){	//if there are other letters on the top or bottom of the tile
				transform(word.begin(), word.end(), word.begin(), ::toupper);
				words_formed.push_back(pair<string, unsigned int>(word, score));	
			}	
		}
		string main_word;
		size_t temp_col = col, index_tiles_to_place = 0, main_word_score = 0, main_word_multiply = 1;
		while(temp_col-1 >= 0 && temp_col != 0){
			if(_board->get_square(row,temp_col-1)->isOccupied())	
				temp_col--;			//read until temp_col is the exact index of the left-most occupied square
			else	break; 
		}
		while(index_tiles_to_place < tiles_to_place.size() || _board->get_square(row,temp_col)->isOccupied()){
			if(_board->get_square(row,temp_col)->isOccupied()){
				main_word += _board->get_square(row,temp_col)->getLetter();
				main_word_score += _board->get_square(row,temp_col++)->getScore();
				if(temp_col >= num_cols)	break;
			}
			else{				//if there is no tile on the square, which means we are going to add player's tile on this square
				main_word += tiles_to_place[index_tiles_to_place]->getUse();
				main_word_score += tiles_to_place[index_tiles_to_place++]->getPoints() * _board->get_square(row,temp_col)->getLMult();
				main_word_multiply *= _board->get_square(row,temp_col++)->getWMult();
				if(temp_col >= num_cols)	break;
			}
		}
		main_word_score *= main_word_multiply;
		transform(main_word.begin(), main_word.end(), main_word.begin(), ::toupper);
		if(_board->word_size(main_word) != 1)		
			words_formed.push_back(pair<string, unsigned int>(main_word, main_word_score));
	}
	unsigned total_score = 0;
	for(size_t i = 0; i < words_formed.size(); i++)
		total_score += words_formed[i].second;
	return total_score;
}

//need to remove row later
void CPUS::permute_horizontal(vector<Tile*>& current_tiles, int col, string formed_word, int first_tile_row, int first_tile_col){
	while(col < (int)_board->getColumns()){
		if(_board->get_square(first_tile_row, col)->isOccupied()){
			formed_word += _board->get_square(first_tile_row, col)->getLetter();
			if(_dict->is_prefix(formed_word) == false)
				return;
			col++;
		}
		else
			break;
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
						int current_score = calculate_score(current_tiles, first_tile_row, first_tile_col, true);
						if(current_score > highest_score.second){
							highest_score.second = current_score;
							string str_row = to_string(first_tile_row + 1);
							string str_col = to_string(first_tile_col + 1);
							string input = turn_tiles_to_string(current_tiles, true, str_row, str_col);
							highest_score.first = input;
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
							int current_score = calculate_score(current_tiles, first_tile_row, first_tile_col, true);
							if(current_score > highest_score.second){
								highest_score.second = current_score;
								string str_row = to_string(first_tile_row + 1);
								string str_col = to_string(first_tile_col + 1);
								string input = turn_tiles_to_string(current_tiles, true, str_row, str_col);
								highest_score.first = input;
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

void CPUS::permute_vertical(std::vector<Tile*>& current_tiles,
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
						int current_score = calculate_score(current_tiles, first_tile_row, first_tile_col, false);
						if(current_score > highest_score.second){
							highest_score.second = current_score;
							string str_row = to_string(first_tile_row + 1);
							string str_col = to_string(first_tile_col + 1);
							string input = turn_tiles_to_string(current_tiles, false, str_row, str_col);
							highest_score.first = input;
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
							int current_score = calculate_score(current_tiles, first_tile_row, first_tile_col, false);
							if(current_score > highest_score.second){
								highest_score.second = current_score;
								string str_row = to_string(first_tile_row + 1);
								string str_col = to_string(first_tile_col + 1);
								string input = turn_tiles_to_string(current_tiles, false, str_row, str_col);
								highest_score.first = input;
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
bool CPUS::to_move(vector<Tile*>& letters_picked_up){
	_hand_tiles = getHandTiles_vector();
	highest_score.first = "";
	highest_score.second = -1; 
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
				}
			}
		}
	}
	//if the AI can make a PLACE move
	if(highest_score.second >= 0){
		Move* move = Move::parseMove(highest_score.first, *this);
		move->execute(*_board, *_bag, *_dict);
		letters_picked_up = move->get_tiles_drawn_from_bag();
		delete move;
		return true;
	}
	return false;
}