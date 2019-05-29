#ifndef CPUS_H_
#define CPUS_H_
#include "Player.h"
#include "Move.h"
#include "Board.h"
#include <string>
#include <utility>
#include "CPU.h"

class CPUS : public CPU{
public:
  CPUS(std::string const & name, std::size_t maxTiles, Dictionary* dict, Board* board, Bag* bag);

  ~CPUS();

  void permute_horizontal(std::vector<Tile*>& current_tiles,
    int col, std::string formed_word, const int first_tile_row, const int first_tile_col);  

  void permute_vertical(std::vector<Tile*>& current_tiles, 
    int row,  std::string formed_word, const int first_tile_row, const int first_tile_col);


  bool to_move(std::vector<Tile*>& letters_picked_up);

  unsigned int calculate_score(std::vector<Tile*> tiles_to_place, std::size_t row, std::size_t col, bool horizontal);
private:
  Dictionary* _dict;
  Board* _board;
  std::vector<Tile*> _hand_tiles;
  Bag* _bag;
  std::pair<std::string, int> highest_score;
};

#endif 