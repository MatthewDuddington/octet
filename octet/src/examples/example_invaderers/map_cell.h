#pragma once

#ifndef map_cell_h
#define map_cell_h

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
    const CellType passable_[3] = { PATH, START, GOAL };

    // Stores pointers to the adjacent North, South, East and West cells.
    // Thanks to Jack Evans for the suggestion of storing these rather than recalcualting each time.
    MapCell* adjacent_cells_[4] = { NULL, NULL, NULL, NULL };  
    int my_index;

    void SetupAdjacentCells(std::vector<MapCell>& level_grid, int level_grid_index,
                            int level_width, int level_height,
                            int cell_column, int cell_row)
    {
      my_index = level_grid_index;
      if (cell_row > 0) {  // Cell in row above
        adjacent_cells_[NORTH] = &level_grid[level_grid_index - level_width];
      }
      if (cell_column < level_width - 1) {  // Cell to the right
        adjacent_cells_[EAST] = &level_grid[level_grid_index + 1];
      } 
      if (cell_row < level_height - 1) {  // Cell in row below
        adjacent_cells_[SOUTH] = &level_grid[level_grid_index + level_width];
      }
      if (cell_column > 0) {  // Cell to the left
        adjacent_cells_[WEST] = &level_grid[level_grid_index - 1];
      }
    }


  public:
    MapCell() {
      cell_type_ = PATH;
    }

    // TODO The init is rather overloaded on function, consider splitting into separate inits or rather just get the MapCell init caller to reach in to the sprite's init function directly.
    void Init(MapCell::CellType CellType,
              std::vector<MapCell>& level_grid,
              int level_grid_index,
              int level_width, int level_height,
              int column, int row,
              int _texture,
              float x, float y,
              float w, float h)
    {
      cell_type_ = CellType;
      SetupAdjacentCells(level_grid, level_grid_index, level_width, level_height, column, row);
      sprite_.init(_texture, x, y, w, h);
    }

    sprite& GetSprite() {
      return sprite_;
    }

    // Returns true if this MapCell can be occupied by an actor.
    bool IsWalkable() {
      if (cell_type_ == PATH  ||
          cell_type_ == START ||
          cell_type_ == GOAL)
      {
        return true;
      }
      return false;
    }

    CellType GetType() {
      return cell_type_;
    }

    // Returns True if the passed MapCell is located within the passed distance (number of cells to check each side) and direction.
    bool IsInLineWithMe(MapCell& map_cell, Direction direction, int distance) {
      MapCell* test_cell = this;
      for (int i = 0; i < distance; i++) {
        test_cell = &test_cell->GetAdjacentCell(direction);
      }
      if (test_cell == &map_cell) {
        printf("In line");
        return true;
      }
      printf("Not in line with me");
      return false;
    }

    bool IsAdjacentToMe(MapCell map_cell) {
      if (IsInLineWithMe(map_cell, NORTH, 1) ||
          IsInLineWithMe(map_cell, EAST,  1) ||
          IsInLineWithMe(map_cell, SOUTH, 1) ||
          IsInLineWithMe(map_cell, WEST,  1))
      {
        return true;
      }
      return false;
    }

    bool IsAdjacentToMe(MapCell map_cell, Direction direction) {
      return IsInLineWithMe(map_cell, direction, 1);
    }

    MapCell& GetAdjacentCell(Direction direction) {
      if (adjacent_cells_[direction] == NULL) {
        printf("asking for NULL cell pointer");
      }
      return *adjacent_cells_[direction];
    }

  };

}

#endif // !map_cell_h
