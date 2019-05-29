/*
 * Player.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* Encodes the state of a player, mostly the name, tiles, points. 
   The main non-trivial ability of this class is to produce a vector
   of Tile* from a string describing tiles to place/exchange. */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include "Tile.h"

class Player
{

public:
	/* Constructor giving the player the given name, and setting their points to 0.
	   Does not give the player any tiles.
	*/
	Player (std::string const & name, size_t maxTiles);

	/* Destructor for a player. Deletes all the tiles the player still has. */
	virtual ~Player ();

	/* Returns the set of tiles the player currently holds. */
	std::set<Tile*> getHandTiles() const;

	/* Reads a move string and confirms that the player has the tiles necessary to
	   execute this move.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as.

	   By definition, if this function returns true, then takeTiles() would
	   succeed.
	 */

	virtual bool hasTiles(std::string const & move, bool resolveBlanks) const;

	virtual std::vector<Tile*> getHandTiles_vector();

	/* Reads a move string, finds the corresponding tiles in the player's hand, and
	   removes and returns those tiles in the order they appear in the move string.

	   The move string is case-insensitive.

	   The boolean resolveBlanks states whether this is a PLACE or EXCHANGE move.
	   If resolveBlanks is true, then when the string contains a '?',
	   the next letter is interpreted as the letter to use the blank as,
	   and the "use" field of the tile is set accordingly.

	   The move string is assumed to have correct syntax.
	*/
	virtual std::vector<Tile*> takeTiles (std::string const & move, bool resolveBlanks);

	// Adds all the tiles in the vector to the player's hand.
	virtual void addTiles (std::vector<Tile*> const & tilesToAdd);


	//Add more public/protected/private functions/variables here.
	virtual void update_player_score(int score);
	virtual int show_player_score() const;
	virtual std::size_t num_hand_tiles() const;
	virtual std::size_t max_hand_tiles() const;
	virtual int score_of_hand_tiles() const;
	virtual std::string get_player_name() const;
	virtual std::size_t const getMaxTiles() const;

	//THE FOLLOWING FUNCTIONS ARE TO HELP 
	virtual bool to_move(std::vector<Tile*>& letters_picked_up);
	virtual std::string turn_tiles_to_string(std::vector<Tile*> tiles, bool horizontal, std::string row, std::string col);

private:
	size_t max_tiles;				//max_hand_size
	std::string player_name;	
	std::set<Tile*> hand_tiles;		//the set of tiles the player currently holds.
	int player_score;
};


#endif /* PLAYER_H_ */
