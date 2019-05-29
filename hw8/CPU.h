#ifndef CPU_H_
#define CPU_H_
#include "Player.h"
#include "Move.h"
#include "Board.h"
#include "Dictionary.h"
#include <string>
#include <utility>


class CPU : public Player{
public:
  CPU(std::string const & name, std::size_t maxTiles, Dictionary* dict, Board* board, Bag* bag);

  virtual ~CPU();

  std::string formed_vertical_word(int row, int col, char ch);

  std::string formed_horizontal_word(int row, int col, char ch);

  virtual void permute_horizontal(std::vector<Tile*>& current_tiles, 
    int col, std::string formed_word, const int first_tile_row, const int first_tile_col) = 0;  

  virtual void permute_vertical(std::vector<Tile*>& current_tiles, 
    int row,  std::string formed_word, const int first_tile_row, const int first_tile_col) = 0;

  virtual bool check_contain_starting_square(int row, int col, std::vector<Tile*>current_tiles, bool horizontal);

  virtual bool check_have_adjacent(bool horizontal, std::vector<Tile*>current_tiles, int row, int col);

  virtual bool to_move(std::vector<Tile*>& letters_picked_up) = 0;
private:
  Dictionary* _dict;
  Board* _board;
  std::vector<Tile*> _hand_tiles;
  Bag* _bag;

};

#endif 