#include "Move.h"
#include "Board.h"
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

Move::~Move()
{
	
}

vector<Tile*> Move::get_tiles_drawn_from_bag() const
{
	return tiles_drawn_from_bag;
}

Move * Move::parseMove(string moveString, Player &p)
{
	transform(moveString.begin(), moveString.end(), moveString.begin(), ::tolower);
	stringstream ss(moveString);
	string word;
	ss >> word;
	
	if(word == "pass")	//if the command is to pass
		return new PassMove(&p);
	else if(word == "exchange")			//examining what is the user's command and create the corresponding move for it
	{
		ss >> word;
		return new ExchangeMove(word, &p);
	}
	else if(word == "place")
	{
		bool horizontal_ = true;
		ss >> word;
		if(word == "|")
			horizontal_ = false;
		size_t pos_row, pos_col;
		ss >> pos_row >> pos_col;
		ss >> word;

		return new PlaceMove(pos_col-1, pos_row-1, horizontal_, word, &p);		//The rows and columes stored in the PLACE MOVE are the real index in the board!!!

	}
	return nullptr;
}

Move::Move(Player * player){
	_player = player;
}


PassMove::PassMove(Player * player) :Move(player)
{
	
}

vector<Tile*> const & PassMove::tileVector () const
{
	return nothing_in_here;			//pass did not change anything
}
void PassMove::execute(Board & board, Bag & bag, Dictionary & dictionary)
{
	//do nothing if it is to pass
}

ExchangeMove::ExchangeMove(string tileString, Player * p): Move(p)	
{
	_tile_string = tileString;
}

vector<Tile*> const & ExchangeMove::tileVector () const{
	return nothing_in_here;	//exchange did not change the tiles on the board
}

void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary)	
{
	if(_player->hasTiles(_tile_string, false) == false){
		throw logic_error("You have an error! You do not have correct tiles to exchange!");			//remember to try to catch the error when calling execute
	}
	else{	//if the player really have the tiles
		//to put the tiles from the player into the bag
		tiles_to_exchange = _player->takeTiles(_tile_string, false);
		bag.addTiles(tiles_to_exchange);

		//to put the new tiles from the bag into the hand
		int num_of_tiles_drawn = tiles_to_exchange.size();
		tiles_drawn_from_bag = bag.drawTiles(num_of_tiles_drawn);
		_player->addTiles(tiles_drawn_from_bag);			//add the tills to the player's hand
	}
	
}

PlaceMove::PlaceMove (size_t x, size_t y, bool horizontal, string tileString, Player * p): Move(p)	
{
	row = y;
	col = x;
	_horizontal = horizontal;
	_tile_string = tileString;
}

vector<Tile*> const & PlaceMove::tileVector () const	
{
	return tiles_to_place;	//return the tiles that the player wants to place 
}

void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary)
{
	p_dictionary = &dictionary;
	// cout << "_tile_string: " << _tile_string << endl;
	if(_player->hasTiles(_tile_string, true) == false)
		throw logic_error("You have an error! You do not have correct tiles to place!");					//remember to try to catch the error when calling execute
	
	// cout << "here error" << endl;
	tiles_to_place = _player->takeTiles(_tile_string, true);
	
	//most stuff are done in the board.cpp
	board.executePlaceMove(*this);			//we make sure the player has enough tiles to make the "place"
	//check if the player can have bonus points
	if(_player->num_hand_tiles() == 0 && tiles_to_place.size() == _player->max_hand_tiles())	//if the user used up all his letters in one turn, give him bonus scores!
		_player->update_player_score(50);
	//draw tiles
	tiles_drawn_from_bag = bag.drawTiles(_player->max_hand_tiles() - _player->num_hand_tiles());
	//add tiles to the player's hand
	_player->addTiles(tiles_drawn_from_bag);
}

Player* Move::get_player() const
{
	return _player;
}


Dictionary* PlaceMove::get_dictionary() const
{
	return p_dictionary;
}

size_t PlaceMove::get_row() const
{
	return row;
}
size_t PlaceMove::get_col() const
{
	return col;
}
bool const PlaceMove::is_horizontal() const
{
	return _horizontal;
}

