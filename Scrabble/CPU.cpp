#include "CPU.h"
using namespace std;

CPU::CPU(std::string const & name, size_t maxTiles, Dictionary* dict, Board* board, Bag* bag):
Player(name,maxTiles),
_dict(dict),
_board(board),
_bag(bag)
{

}

CPU::~CPU(){

}

string CPU::formed_vertical_word(int row, int col, char ch){
	string word = "";
	int temp_row = row;
	while(temp_row-1 >= 0 && temp_row-1 < (int)_board->getRows()){
		if(_board->get_square(temp_row-1,col)->isOccupied())
			temp_row--;
		else
			break;
	}
	//adding the characters before the word to word
	while(_board->get_square(temp_row,col)->isOccupied() || temp_row == row){
		if(temp_row == row){
			word += ch;
		}
		else{
			word += _board->get_square(temp_row,col)->getLetter();			
		}

		temp_row++;
		if(temp_row >= (int)_board->getRows())
			break;
	}
	if(word.size() == 1)
		return "";
	return word;
}

string CPU::formed_horizontal_word(int row, int col, char ch){
	string word = "";
	int temp_col = col;

	while(temp_col-1 >= 0 && temp_col-1 < (int)_board->getColumns()){
		if(_board->get_square(row,temp_col-1)->isOccupied())
			temp_col--;
		else
			break;
	}
	//adding the characters before the word to word
	while(_board->get_square(row,temp_col)->isOccupied() || temp_col == col){
		if(temp_col == col){
			word += ch;
		}
		else{
			word += _board->get_square(row,temp_col)->getLetter();			
		}
		
		temp_col++;
		if(temp_col >= (int)_board->getColumns())
			break;
	}
	if(word.size() == 1)
		return "";

	return word;
}

bool CPU::check_contain_starting_square(int row, int col, vector<Tile*>current_tiles, bool horizontal){
	if(_board->get_square(_board->get_starting_row(),_board->get_starting_col())->isOccupied())
		return true;
	//the following conditoin requires us to check if the intended move contain the starting square
	if(_board->is_empty_board() && !_board->check_init()){
		if(horizontal){
			if(row != _board->get_starting_row())
				return false;

			int num = current_tiles.size();
			while(col >= 0 && col  < (int)_board->getColumns() && num >= 1){
				if(col == _board->get_starting_col())
					return true;
				if(!_board->get_square(row, col)->isOccupied())
					num--;
				col++;
			}
		}
		else{
			if(col != _board->get_starting_col())
				return false;

			int num = (int)current_tiles.size();
			while(row >= 0 && row  < (int)_board->getRows() && num >= 1){
				if(row == _board->get_starting_row())
					return true;
				if(!_board->get_square(row, col)->isOccupied())
					num--;
				row++;
			}
		}
		return false;
	}
	return true;
}

bool CPU::check_have_adjacent(bool horizontal, vector<Tile*>current_tiles, int row, int col){
	if(!_board->is_empty_board()){// If the player is trying to make a move (other than the first move) that does not have at least one tile adjacent to a previously placed tile.
		if(horizontal){
			for(int i = -1; i < (int)current_tiles.size()+1; i++){
				if(col+i < (int)_board->getColumns() && col+i >= 0){
					if(_board->get_square(row,col+i)->isOccupied()) 
						return true;
				}
			}
			for(int i = 0; i < (int)current_tiles.size(); i++){
				if(row-1 >= 0 && col+i < (int)_board->getColumns()){
					if(_board->get_square(row-1,col+i)->isOccupied())
						return true;
				}
				if(row+1 < (int)_board->getRows() && col+i < (int)_board->getColumns()){
					if(_board->get_square(row+1,col+i)->isOccupied())
						return true;
				}
			}		
		}
		else{			// similar to the code above
			for(int i = -1; i < (int)current_tiles.size()+1; i++){
				if((int)row+i < (int)_board->getRows() && (int)row+i >= 0){
					if(_board->get_square(row+i,col)->isOccupied())
						return true;
				}
			}
			for(int i = 0; i < (int)current_tiles.size(); i++){
				if((int)col-1 >= 0 && (int)row+i < (int)_board->getRows()){
					if(_board->get_square(row+i,col-1)->isOccupied())
						return true;
				}
				if(col+1 < (int)_board->getColumns() && row+i < (int)_board->getRows()){
					if(_board->get_square(row+i,col+1)->isOccupied())
						return true;
				}		
			}	
		}
		return false;
	}
	return true;
}