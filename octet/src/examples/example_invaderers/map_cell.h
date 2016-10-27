#pragma once

#ifndef map_cell_h
#define map_cell_h

/*
class Level {  // Circular dependency solver???
  virtual Level& CurrentLevel();
};
*/

namespace octet {

  class MapCell {

  public: enum CellType {
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

    // Vars for circular dependency fix
    int my_index;
    static int& current_level_width;
    static int& current_level_height;
    // End of fix

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

    bool IsInLineWithMe(MapCell map_cell, Direction direction, int distance) {
      if (direction % 2 == 0) {  // If direction is North or South
        if (map_cell.Coords().x() == cell_column_) {  // Is cell in the same column as this
          if (direction == NORTH &&
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
          if (direction == EAST &&
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
      if (IsInLineWithMe(map_cell, NORTH, 1) ||
          IsInLineWithMe(map_cell, EAST,  1) ||
          IsInLineWithMe(map_cell, SOUTH, 1) ||
          IsInLineWithMe(map_cell, WEST,  1)) {
        return true;
      }
      return false;
    }

    bool IsAdjacentToMe(MapCell map_cell, Direction direction) {
      return IsInLineWithMe(map_cell, direction, 1);
    }

    /* Creates circular dependency because Level.h hasn't been read yet, but also contains references to MapCell in itself.
    MapCell& Above() {
      if (cell_row_ != 0) {
        int index = ((cell_row_ - 1) * Level::CurrentLevel().Width()) + cell_column_;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return *(this);
    }

    MapCell& Below() {
      if (cell_row_ != Level::CurrentLevel().Height() - 1) {
        int index = ((cell_row_ + 1) * Level::CurrentLevel().Width()) + cell_column_;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return *(this);
    }

    MapCell& Left() {
      if (cell_column_ != 0) {
        int index = (cell_row_ * Level::CurrentLevel().Width()) + cell_column_ - 1;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return *(this);
    }

    MapCell& Right() {
      if (cell_column_ != Level::CurrentLevel().Width() - 1) {
        int index = (cell_row_ * Level::Level().Width()) + cell_column_ + 1;
        return Level::CurrentLevel().LevelGrid().at(index);
      }
      printf("Out of grid error.");
      return *(this);
    }
    */

    // Alternate functions as fix for circular dependency

    static void ProvideClassWithLevelSizes(int width, int height) {  // This is a horrible way of having to do this. TODO Avoid having to do this!
      current_level_width = width;
      current_level_height = height;
    }

    // Calculates index of neighbour
    int AdjacentCellIndex(Direction direction) {
      int index = -1;
      if (direction == NORTH) {
        if (cell_row_ != 0) {
          index = ((cell_row_ - 1) * current_level_width) + cell_column_;
        }
      }
      else if (direction == SOUTH) {
        if (cell_row_ < current_level_height - 1) {
          index = ((cell_row_ + 1) * current_level_width) + cell_column_;
        }
      }
      else if (direction == WEST) {
        if (cell_column_ != 0) {
          index = (cell_row_ * current_level_width) + cell_column_ - 1;
        }
      }
      else if (cell_column_ < current_level_width - 1) {  // Can assume EAST
        index = (cell_row_ * current_level_width) + cell_column_ + 1;
      }
      if (index == -1) {
        printf("Cell requested is outside of grid. \n");
        return my_index;
      }
      return index;
    }
    // End of alternate functions

  };

}

#endif // !map_cell_h
