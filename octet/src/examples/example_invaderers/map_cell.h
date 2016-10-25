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

    int cell_column_;
    int cell_row_;

    const CellType passable_[3] = { PATH, START, GOAL };

  public:

    MapCell() {
      cell_type_ = PATH;
    }

    void Init(int _texture,
              MapCell::CellType CellType,
              int column, int row,
              float x, float y,
              float w, float h) {
      cell_type_ = CellType;
      cell_column_ = column;
      cell_row_ = row;
      sprite_.init(_texture, x, y, w, h);
    }

    sprite& Sprite() {
      return sprite_;
    }

    vec2 Coords() {
      vec2 coords;
      coords.x() = (float)cell_column_;
      coords.y() = (float)cell_row_;
      return coords;
    }

    bool IsPassable() {
      if (cell_type_ == PATH  ||
          cell_type_ == START ||
          cell_type_ == GOAL) {
        return true;
      }
      return false;
    }

    bool IsInLineWithMe(MapCell map_cell, sprite::Direction direction, int distance) {
      if (direction % 2 == 0) {  // If direction is North or South
        if (map_cell.Coords().x() == cell_column_) {  // Is cell in the same column as this
          if (direction == sprite::NORTH &&
              cell_row_ - map_cell.Coords().y() - distance <= 0) { // Is within the distance cells
            return true;
          } 
          else if (map_cell.Coords().y() - cell_row_ - distance <= 0) {  // Assume direction == South
            return true;
          }
          return false;  // Else
        }
      }
      else {  // Assume direction == East or West
        if (map_cell.Coords().y() == cell_row_) {  // Is cell in same row as this
          if (direction == sprite::EAST &&
              map_cell.Coords().x() - cell_column_ - distance <= 0) {
            return true;
          }
          else if (cell_column_ - map_cell.Coords().x() - distance <= 0) {  // Assume direction == West
            return true;
          }
          return false;  // Else
        }
      }
    }

    bool IsAdjacentToMe(MapCell map_cell) {
      if (IsInLineWithMe(map_cell, sprite::NORTH, 1) ||
          IsInLineWithMe(map_cell, sprite::EAST,  1) ||
          IsInLineWithMe(map_cell, sprite::SOUTH, 1) ||
          IsInLineWithMe(map_cell, sprite::WEST,  1)) {
        return true;
      }
      return false;
    }

    bool IsAdjacentToMe(MapCell map_cell, sprite::Direction direction) {
      return IsInLineWithMe(map_cell, direction, 1);
    }

    MapCell& Above() {
      if (cell_row_ != 0) {
        int index = ((cell_row_ - 1) * Level::CurrentLevel().Width()) + cell_column_;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return;
    }

    MapCell& Below() {
      if (cell_row_ != Level::CurrentLevel().Height() - 1) {
        int index = ((cell_row_ + 1) * Level::CurrentLevel().Width()) + cell_column_;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return;
    }

    MapCell& Left() {
      if (cell_column_ != 0) {
        int index = (cell_row_ * Level::CurrentLevel().Width()) + cell_column_ - 1;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return;
    }

    MapCell& Right() {
      if (cell_column_ != Level::CurrentLevel().Width() - 1) {
        int index = (cell_row_ * Level::Level().Width()) + cell_column_ + 1;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return;
    }

  };

}

#endif // !map_cell_h
