#include "Player.h"
#include <iostream>
using namespace std;

Player::Player (string const & name, size_t maxTiles)
{
	player_name = name;
	max_tiles = maxTiles;
	player_score = 0;
}

Player::~Player ()
{
	for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); it++)
		delete *it;
}

int Player::score_of_hand_tiles() const   	//calculate the player's score of the hand tiles
{
	unsigned int score = 0;
	for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); it++)
		score += (*it)->getPoints();

	return score;
}



size_t Player::num_hand_tiles() const
{
	return getHandTiles().size();
}
size_t const Player::getMaxTiles() const{
	return max_tiles;
}

string Player::get_player_name() const
{
	return player_name;
}

int Player::show_player_score() const
{
	return player_score;
}

void Player::update_player_score(int score)		//to add the score to the player's total score
{
	player_score += score;
}

set<Tile*> Player::getHandTiles() const  //to return the handtiles of the player
{
	return hand_tiles;
}

size_t Player::max_hand_tiles() const
{
	return max_tiles;
}

void Player::addTiles (vector<Tile*> const & tilesToAdd)//make sure tilesToAdd contain only lower-case letters, so that hand_tiles will contain only lower-case leetters
{
	for(size_t i = 0; i < tilesToAdd.size(); i++)
		hand_tiles.insert(tilesToAdd[i]);
}

vector<Tile*> Player::takeTiles (string const & move, bool resolveBlanks)
{
	vector<Tile*> result;
	if(hasTiles(move, resolveBlanks)){
		string move_lower_case = move;
		transform(move_lower_case.begin(), move_lower_case.end(), move_lower_case.begin(), ::tolower); 	//make sure the "move" string contains only lower-case letters
		for(size_t i = 0; i < move_lower_case.size(); i++){
			for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); ++it){
			 	if((*it)->getLetter() == move_lower_case[i]){
					if(move_lower_case[i] == '?' && resolveBlanks)			//compare the chars in move string with the handtiles. if they are the same, we take it out from the player's hand 
						(*it)->useAs(move_lower_case[++i]);				//if it is a "?" and I want to place it, I use the tile as the letter after it
					result.push_back(*it);
					hand_tiles.erase(*it);
					break;
				}
			}
		}
	}
	return result;	//if does not have the move, then return empty result without modifying any of the members in hand_tiles
}


bool Player::hasTiles(string const & move, bool resolveBlanks) const	
{
	string move_lower_case = move;
	transform(move_lower_case.begin(), move_lower_case.end(), move_lower_case.begin(), ::tolower);

	if(resolveBlanks){
		map<char, int> m;
		for(size_t i = 0; i < move_lower_case.size(); i++){
			m[move_lower_case[i]]++;
			if(move_lower_case[i] == '?')	//skip the letter after "?"
				i++;
		}
		for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); ++it){
			for(map<char, int>::iterator mit = m.begin(); mit != m.end(); ++mit){
				if((*it)->getLetter() == mit->first)
					mit->second--;			//comparing hand tiles with the move string
			}
		}
		for(map<char, int>::iterator mit = m.begin(); mit != m.end(); ++mit){
			if(mit->second > 0)		//the difference should be <= 0 if we have those tiles
				return false;
		}
	}

	else{					//if the player wants to exchange
		map<char, int> m2;
		for(size_t i = 0; i < move_lower_case.size(); i++)
			m2[move_lower_case[i]]++;		//record the chars
	
		for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); ++it){
			for(map<char, int>::iterator mit = m2.begin(); mit != m2.end(); ++mit){
				if((*it)->getLetter() == mit->first){
					m2[mit->first]--;		//match each chars in the move string with the hand tiles
				}
			}
		}
		for(map<char, int>::iterator mit = m2.begin(); mit != m2.end(); ++mit){
			if(mit->second > 0)
				return false;		//the difference should be <= 0 if we have all the tiles
		}
	}
	return true;
}

bool Player::to_move(std::vector<Tile*>& letters_picked_up){
	//the real operation will be defined in CPUL.cpp and CPUS.cpp
	return false;
}


vector<Tile*> Player::getHandTiles_vector(){
	vector<Tile*> result;
	for(set<Tile*>::iterator it = hand_tiles.begin(); it != hand_tiles.end(); ++it)
		result.push_back(*it);
	return result;
}

string Player::turn_tiles_to_string(vector<Tile*> tiles, bool horizontal, string row, string col){
	string word = "";
	if(horizontal) 
		word += "PLACE - ";
	else word += "PLACE | ";
	word += row;
	word += " ";
	word += col;
	word += " ";
	for(size_t i = 0; i < tiles.size(); i++){
		if(tiles[i]->getLetter() != '?')
			word += tiles[i]->getLetter();
		else{
			word += tiles[i]->getLetter();
			word += tiles[i]->getUse();
		}
	}
	return word;
}


