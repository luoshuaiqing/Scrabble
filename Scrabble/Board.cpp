#include "Board.h"
#include <fstream>
using namespace std;

Board::Board(string board_file_namey, bool to_init)	//remember to delete the dynamic memory of arr
{
	init = to_init;
	ifstream board_file(board_file_namey);
	board_file >> num_cols >> num_rows >> starting_cols >> starting_rows;		//if starting_rows = 8, it means it is on the 8th row, not arr[8] but arr[7]
	starting_cols--;
	starting_rows--;						//these two variables represent the real INDEX in the array.
	arr = new Square**[num_rows];
	for(size_t i = 0; i < num_rows; i++)
		arr[i] = new Square*[num_cols];

	for(size_t i = 0; i < num_rows; i++){		//create squares
		for(size_t j = 0; j < num_cols; j++)
		{
			unsigned int LMult = 1, WMult = 1;
			bool starting_pos = false;
			char ch;
			board_file >> ch;
			if(i == starting_rows && j == starting_cols)	//check if it is the starting pos
				starting_pos = true;

			if(ch == 't')				//setting up the parameters of the squares
				WMult = 3;
			else if(ch == '2')
				LMult = 2;
			else if(ch == '3')
				LMult = 3;
			else if(ch == 'd')
				WMult = 2;

			arr[i][j] = new Square(LMult, WMult, starting_pos);
		}
	}
}


Board::~Board()
{
	for(size_t i = 0; i < num_rows; i++)
	{
		for(size_t j = 0; j < num_cols; j++)
			delete arr[i][j];
		delete [] arr[i];
	}				
	delete [] arr;				//delete all the squares

}

bool Board::is_empty_board() const
{
	for(size_t i = 0; i < num_rows; i++){
		for(size_t j = 0; j < num_cols; j++){
			if(arr[i][j]->isOccupied())
				return false;
		}
	}
	return true;
}

void Board::to_init(string init_file){
	//need to be completed
	ifstream myfile(init_file);
	string myline;
	size_t current_row = 0;
	while(getline(myfile, myline)){
		for(size_t i = 0; i < myline.size(); i++){
			if(myline[i] != '.'){
				//it does not matter whether the letter is capital or not, because we will always transform the letters into capital/lower_case
				char letter = myline[i];
				int num[2] = {myline[i+1]-'0', myline[i+2]-'0'};
				int points = num[0]*10 + num[1];
				Tile* init_tile = new Tile(letter, points);			//remember to free the memory in the destructor for Board


				//putting the init_tile on the square
				get_square(current_row, i/3)->placeTile(init_tile);
				i+=2;
			}
		}

		current_row++;	//go to next row
	}
}


//if the move is not legal, then throw an error.
//else if the move is legal, return the words that are formed
vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove &m) const
{
	vector<pair<string, unsigned int>> words_formed;	//We know that the player has all the letters required by the intended move, or this function will not be called
	bool horizontal = m.is_horizontal();
	size_t row = m.get_row(), col = m.get_col();
	vector<Tile*> tiles_to_place = m.tileVector();
	
	if(row >= num_rows || row < 0 || col < 0 || col >= num_cols)		throw logic_error("One or more of the tiles placed would be out of bounds of the board");// if the player entered an out-of-bound row and col
	if(arr[row][col]->isOccupied())		throw logic_error("You are trying to place a word starting at a position that is already occupied by a tile.");//if the player is trying to place a word starting at a position that is already occupied by a tile.
	if(is_empty_board() && !init){//If the player is trying to make a first move that does not use the start square.
		bool contain_starting_square = false;
		if(horizontal){
			if(row == starting_rows && col <= starting_cols && col + tiles_to_place.size() - 1 >= starting_cols)
				contain_starting_square = true;
		}
		else if(!horizontal){
			if(col == starting_cols && row <= starting_rows && row + tiles_to_place.size() - 1 >= starting_rows)
				contain_starting_square = true;
		}
		if(!contain_starting_square)	throw logic_error("You are trying to make a first move that does not use the starting square.");
	}
	if(!is_empty_board()){// If the player is trying to make a move (other than the first move) that does not have at least one tile adjacent to a previously placed tile.
		bool encountered_an_occupied_tile = false;
		if(horizontal){
			for(int i = -1; i < (int)tiles_to_place.size()+1; i++){
				if((int)col+i < (int)num_cols && (int)col+i >= 0){
					if(arr[row][col+i]->isOccupied()) 
						encountered_an_occupied_tile = true;		//if it encountes an occupied tile
				}
			}
			for(int i = 0; i < (int)tiles_to_place.size(); i++){
				if((int)row-1 >= 0 && (int)col+i < (int)num_cols){
					if(arr[row-1][col+i]->isOccupied())
						encountered_an_occupied_tile = true;
				}
				if(row+1 < num_rows && col+i < num_cols){
					if(arr[row+1][col+i]->isOccupied())
						encountered_an_occupied_tile = true;
				}
			}				
		}
		else{			// similar to the code above
			for(int i = -1; i < (int)tiles_to_place.size()+1; i++){
				if((int)row+i < (int)num_rows && (int)row+i >= 0){
					if(arr[row+i][col]->isOccupied())
						encountered_an_occupied_tile = true;
				}
			}
			for(int i = 0; i < (int)tiles_to_place.size(); i++){
				if((int)col-1 >= 0 && (int)row+i < (int)num_rows){
					if(arr[row+i][col-1]->isOccupied())
						encountered_an_occupied_tile = true;
				}
				if(col+1 < num_cols && row+i < num_rows){
					if(arr[row+i][col+1]->isOccupied())
						encountered_an_occupied_tile = true;
				}		
			}	
		}
		if(!encountered_an_occupied_tile)	//it is an error if not encountered any occupied tile
			throw logic_error("You are trying to make a move that does not have at least one tile adjacent to a previously placed tile. Error!");
	}

	if(!horizontal){	//to check if there is anything on the left or right of the tile
		size_t tile_row = row;
		for(size_t i = 0; i < tiles_to_place.size(); i++){
			if(i != 0)	
				tile_row++;
			if(tile_row >= num_rows)	//if it is already out of bound but we are still trying to add a tile
				throw logic_error("One or more of the tiles placed would be out of bounds of the board. Error!");	
			while(arr[tile_row][col]->isOccupied()){		//read until arr[tile_row][col] is not occupied. We want to find a place where we can put our tile on
				if(++tile_row >= num_rows)			
					throw logic_error("One or more of the tiles placed would be out of bounds of the board. Error!");
			}
			string word;
			unsigned int score = 0, multiply_word = arr[tile_row][col]->getWMult();
			size_t left = col, right = col, temp_col = col;
			while(temp_col - 1 >= 0 && temp_col != 0){
				if(arr[tile_row][temp_col-1]->isOccupied())
					left = --temp_col;		//read until left is the exact index of the left-most occupied square 
				else	break;
			}
			for(size_t j = left; j < col; j++){				//get the word and the score on the left
				word += arr[tile_row][j]->getLetter();
				score += arr[tile_row][j]->getScore();
			}
			word += tiles_to_place[i]->getUse();
			score += tiles_to_place[i]->getPoints() * arr[tile_row][col]->getLMult();	//Letter multiply works if it is the tile that we are really adding
			
			temp_col = col;
			while(temp_col + 1 < num_cols){
				if(arr[tile_row][temp_col+1]->isOccupied())
					right = ++temp_col;			//read until right is the exact index of the right-most occupied square 
				else	break;
			}
						
			for(size_t j = col+1; j <= right; j++){
				word += arr[tile_row][j]->getLetter();		//updating the word and the score
				score += arr[tile_row][j]->getScore();
			}
			score *= multiply_word;
			if(word_size(word) != 1){	//if the word formed is not size 1, then we can push it to the words_formed
				transform(word.begin(), word.end(), word.begin(), ::toupper);
				words_formed.push_back(pair<string, unsigned int>(word, score));	
			}	
		}
		string main_word;	//trying to examine the main word formed
		size_t temp_row = row, index_tiles_to_place = 0, main_word_score = 0, main_word_multiply = 1;
		while(temp_row-1 >= 0 && temp_row != 0){
			if(arr[temp_row-1][col]->isOccupied())
				temp_row--;			//read until temp_row is the exact index of the top-most occupied square 
			else	break;
		}
		while(index_tiles_to_place < tiles_to_place.size() || arr[temp_row][col]->isOccupied()){
			if(arr[temp_row][col]->isOccupied()){
				main_word += arr[temp_row][col]->getLetter();
				main_word_score += arr[temp_row++][col]->getScore();
				if(temp_row >= num_rows)
					break;
			}
			else{				//if there is no tile on the square, which means we are going to add player's tile on this square
				main_word += tiles_to_place[index_tiles_to_place]->getUse();
				main_word_score += tiles_to_place[index_tiles_to_place++]->getPoints() *arr[temp_row][col]->getLMult();
				main_word_multiply *= arr[temp_row++][col]->getWMult();
				if(temp_row >= num_rows)
					break;
			}
		}
		main_word_score *= main_word_multiply;
		transform(main_word.begin(), main_word.end(), main_word.begin(), ::toupper);
		if(word_size(main_word) != 1)		words_formed.push_back(pair<string, unsigned int>(main_word, main_word_score));
	}
	else if(horizontal)	//to check if there is anything on the top or bottom of the tile
	{	
		size_t tile_col = col;
		for(size_t i = 0; i < tiles_to_place.size(); i++){
			if(i != 0)	tile_col++;
			if(tile_col >= num_cols)	//if it is already out of bound but we are still trying to add a tile
				throw logic_error("One or more of the tiles placed would be out of bounds of the board. Error!");	
			while(arr[row][tile_col]->isOccupied()){		//read until arr[row][tile_col] is not occupied. We want to find a place where we can put our tile on
				if(++tile_col >= num_cols)			throw logic_error("One or more of the tiles placed would be out of bounds of the board. Error!");
			}
			string word;
			unsigned int score = 0, multiply_word = arr[row][tile_col]->getWMult();
			size_t top = row, bot = row, temp_row = row;
			while(temp_row - 1 >= 0 && temp_row != 0){
				if(arr[temp_row-1][tile_col]->isOccupied())
					top = --temp_row;		//read until top is the exact index of the top-most occupied square 
				else 	break;
			}
			for(size_t j = top; j < row; j++){				//get the word and the score on the top
				word += arr[j][tile_col]->getLetter();
				score += arr[j][tile_col]->getScore();
			}
			word += tiles_to_place[i]->getUse();
			score += tiles_to_place[i]->getPoints() * arr[row][tile_col]->getLMult();	//Letter multiply works if it is the tile that we are really adding
			temp_row = row;
			while(temp_row + 1 < num_rows){
				if(arr[temp_row+1][tile_col]->isOccupied())
					bot = ++temp_row;			//read until bot is the exact index of the bottom-most occupied square 
				else	break;
			}	
			for(size_t j = row+1; j <= bot; j++){
				word += arr[j][tile_col]->getLetter();
				score += arr[j][tile_col]->getScore();
			}
			score *= multiply_word;
			if(word_size(word) != 1){	//if there are other letters on the top or bottom of the tile
				transform(word.begin(), word.end(), word.begin(), ::toupper);
				words_formed.push_back(pair<string, unsigned int>(word, score));	
			}	
		}
		string main_word;
		size_t temp_col = col, index_tiles_to_place = 0, main_word_score = 0, main_word_multiply = 1;
		while(temp_col-1 >= 0 && temp_col != 0){
			if(arr[row][temp_col-1]->isOccupied())	
				temp_col--;			//read until temp_col is the exact index of the left-most occupied square
			else	break; 
		}
		while(index_tiles_to_place < tiles_to_place.size() || arr[row][temp_col]->isOccupied()){
			if(arr[row][temp_col]->isOccupied()){
				main_word += arr[row][temp_col]->getLetter();
				main_word_score += arr[row][temp_col++]->getScore();
				if(temp_col >= num_cols)	break;
			}
			else{				//if there is no tile on the square, which means we are going to add player's tile on this square
				main_word += tiles_to_place[index_tiles_to_place]->getUse();
				main_word_score += tiles_to_place[index_tiles_to_place++]->getPoints() * arr[row][temp_col]->getLMult();
				main_word_multiply *= arr[row][temp_col++]->getWMult();
				if(temp_col >= num_cols)	break;
			}
		}
		main_word_score *= main_word_multiply;
		transform(main_word.begin(), main_word.end(), main_word.begin(), ::toupper);
		if(word_size(main_word) != 1)		
			words_formed.push_back(pair<string, unsigned int>(main_word, main_word_score));
	}
	return words_formed;
}

bool Board::check_init() const{
	return init;
}


size_t Board::word_size(string word) const{		//calculating the number of characters in the word that are of alphabet letters
	size_t count = 0;
	for(size_t i = 0; i < word.size(); i++){
		if(isalpha(word[i]))
			count++;
	}
	return count;
}

size_t Board::getRows() const  	//get the number of rows
{
	return num_rows;
}
	
size_t Board::getColumns() const   //get the number of cols
{
	return num_cols;
}

Square * Board::getSquare (size_t x, size_t y) const
{
	if(x == 0 || y == 0)
		return nullptr;
	return arr[y-1][x-1];
}

void Board::executePlaceMove (const PlaceMove & m)
{
	new_words.clear();
	//We know that the player has all the letters required by the intended move, otherwise this function will not be called
	vector<pair<string, unsigned int>> words_formed = getPlaceMoveResults(m);

	for(size_t i = 0; i < words_formed.size(); i++){
		if((m.get_dictionary())->isLegalWord(words_formed[i].first) == false)	//if the word is NOT LEGAL in the dictionary
			throw logic_error("The word does not appear in the dictionary! Error!");
	}
	//if all the words intended to add are legal in the dictionary
	bool horizontal = m.is_horizontal();
	size_t row = m.get_row();
	size_t col = m.get_col();
	vector<Tile*> tiles_to_place = m.tileVector();

	for(size_t i = 0; i < tiles_to_place.size(); i++)
	{
		while(arr[row][col]->isOccupied()){	//read until the place is not occupied so that we can place our tile on it
			if(!horizontal)			
				row++;
			else if(horizontal)		
				col++;
		}
		arr[row][col]->placeTile(tiles_to_place[i]);			
	}
			
	for(size_t i = 0; i < words_formed.size(); i++){
		(m.get_player())->update_player_score(words_formed[i].second);	//updating the players' score if nothing goes wrong
		new_words.push_back(words_formed[i].first);
	}

}


	

Square* Board::get_square(int row, int col) const{
	return arr[row][col];
}


int Board::get_starting_row() const{
	return (int)starting_rows;
}

int Board::get_starting_col() const{
	return (int)starting_cols;
}