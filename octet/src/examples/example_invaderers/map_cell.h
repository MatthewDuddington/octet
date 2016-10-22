#pragma once

#ifndef map_cell_h
#define map_cell_h

namespace octet {

  class MapCell {

  public: static const enum CellType {
    START,
    GOAL,
    PATH,
    WALL,
    BUSH,
    FENCE
  };

  private:
    sprite sprite_;
    CellType cell_type_;

    const static int myInt = 1;

    const CellType passable_[] = { PATH, START, GOAL };

  public:

    MapCell() {
      cell_type_ = PATH;
    }

    void Init(int _texture,
      MapCell::CellType CellType,
      float x, float y,
      float w, float h) {
      cell_type_ = CellType;
      sprite_.init(_texture, x, y, w, h);
    }

    sprite &Sprite() {
      return sprite_;
    }

    bool IsPassable() {
      if (passable_. == PATH || cell_type_ == GOAL || cell_type_ == START)
      {
        return true;
      }
      return false;
    }

    bool IsAdjacent(MapCell map_cell, sprite::Direction direction) {
      
    }

    bool IsAdjacent(MapCell map_cell) {
      if (IsAdjacent(map_cell, sprite::NORTH) ||
          IsAdjacent(map_cell, sprite::EAST)  ||
          IsAdjacent(map_cell, sprite::SOUTH) ||
          IsAdjacent(map_cell, sprite::WEST)) {
        return true;
      }
      return false;
    }

  };

}

#endif // !map_cell_h
