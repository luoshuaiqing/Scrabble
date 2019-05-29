/*
 * Move.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The Move class family encodes a move made by one player, including its type
   (PASS, EXCHANGE, PLACE), the tiles used (and use for blanks),
   start square, and direction (if needed).
   Call Move::parseMove() to turn an entered string into its subclass of
   Move, then call apply() on that returned move to have it execute.*/

#ifndef MOVE_H_
#define MOVE_H_

#include <vector>
#include <string>
#include "Tile.h"
#include "Player.h"
#include "Bag.h"
#include "Dictionary.h"
#include <exception>
#include <stdexcept>

// forward declaration to prevent circular includes
class Board;

class Move
{

public:


	/* Parses the given move m, and constructs the corresponding move subclass.
	   m is assumed to be in the format described on the assignment sheet for a move command.
	   The player passed in is used to verify that the proposed tiles are
	   in fact in the player's hand.
	   It can handle all three types of move (PASS, EXCHANGE, PLACE).
	   Coordinates start with 1.
	*/
	static Move * parseMove(std::string moveString, Player &p);	//only be defined once, cannot be overloaded


	/* Returns true iff the move is a PASS move */
	virtual bool isPass () const
	{
		return false;
	}

	/* Returns true iff the move is an EXCHANGE move */
	virtual bool isExchange () const
	{
		return false;
	}

	/* Returns true iff the move is a PLACE move */
	virtual bool isWord () const
	{
		return false;
	}


	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	virtual void execute(Board & board, Bag & bag, Dictionary & dictionary) = 0;
	
	virtual ~Move();

	//Add more public/protected/private functions/variables here.
	Player* get_player() const;	//return the player
	virtual std::vector<Tile*> const & tileVector () const = 0;
	virtual std::vector<Tile*> get_tiles_drawn_from_bag() const;

	
protected:


	// constructor -- only accessible to subclasses
	Move(Player * player);
	std::vector<Tile*> tiles_drawn_from_bag;
	
	std::vector<Tile*> nothing_in_here;
	Player * _player;
};

// Represents a pass move, where the player takes no action
class PassMove : public Move
{

public:

	/* Constructs a pass move. */
	PassMove(Player * player);

	/* Returns true iff the move is a PASS move */
	bool isPass () const
	{
		return true;
	}

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	void execute(Board & board, Bag & bag, Dictionary & dictionary);

	//Add more public/protected/private functions/variables here.
	std::vector<Tile*> const & tileVector () const;

};

// represents an exchange move, were a player replaces certain tiles
class ExchangeMove : public Move
{

public:
	/* Creates an EXCHANGE move, exchanging the tiles listed in the
	   string (formatted according to the EXCHANGE command description)
	   with new tiles from the bag.
	   */
	ExchangeMove(std::string tileString, Player * p);

	/* Returns true iff the move is an EXCHANGE move */
	bool isExchange () const
	{
		return true;
	}

	std::vector<Tile*> const & tileVector () const;
	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	void execute(Board & board, Bag & bag, Dictionary & dictionary);

	//Add more public/protected/private functions/variables here.
private:
	std::vector<Tile*> tiles_to_exchange;
	std::string _tile_string;
};

// represents a place move, where a player places one or more tiles onto the board.
class PlaceMove : public Move
{

public:
	/* Creates a PLACE move, starting at row y, column x, placing the tiles
	   described by the string tileString. If "horizontal" is true, then the tiles
	   are placed horizontally, otherwise vertically.
	   Coordinates start with 0!!!!!
	   The string m is in the format described in HW4; in particular, a '?'
	   must be followed by the letter it is to be used as.
	*/
	PlaceMove (size_t x, size_t y, bool horizontal, std::string tileString, Player * p);

	/* Returns true iff the move is a PLACE move */
	bool isWord () const
	{
		return true;
	}

	/* Returns the vector of tiles associated with a PLACE/EXCHANGE move.
	   Return value could be arbitrary for PASS moves. */
	std::vector<Tile*> const & tileVector () const;

	/* Executes this move, whichever type it is.
	   This may throw exceptions; students: it's up to you to
	   decide (and document) what exactly it throws*/
	void execute(Board & board, Bag & bag, Dictionary & dictionary);

	//Add more public/protected/private functions/variables here.
	size_t get_row() const;
	size_t get_col() const;
	bool const is_horizontal() const;
	Dictionary* get_dictionary() const;

private:
	size_t row;
	size_t col;
	bool _horizontal;
	std::vector<Tile*> tiles_to_place;
	std::string _tile_string;
	Dictionary* p_dictionary;
	
};



#endif /* MOVE_H_ */
