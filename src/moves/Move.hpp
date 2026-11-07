#pragma once
#include "board/Cell.hpp"

class Move {
  public:
    Cell* cell;
    MoveType type;
    Move() {}
    ~Move() {}
};
