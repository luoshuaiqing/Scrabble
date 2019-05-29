#ifndef CPUL_H_
#define CPUL_H_
#include "Player.h"
#include "Move.h"
#include "Board.h"
#include <string>
#include <utility>
#include "CPU.h"


class CPUL : public CPU{
public:
  CPUL(std::string const & name, std::size_t maxTiles, Dictionary* dict, Board* board, Bag* bag);

  ~CPUL();

  void permute_horizontal(std::vector<Tile*>& current_tiles, 
    int col, std::string formed_word, const int first_tile_row, const int first_tile_col);  

  void permute_vertical(std::vector<Tile*>& current_tiles, 
    int row, std::string formed_word, const int first_tile_row, const int first_tile_col);

  bool to_move(std::vector<Tile*>& letters_picked_up);

private:
  Dictionary* _dict;
  Board* _board;
  std::vector<Tile*> _hand_tiles;
  Bag* _bag;
  std::vector<Tile*> longest_word_ever;
  std::vector<std::pair<std::string, int>> longest_words;

};

#endif 