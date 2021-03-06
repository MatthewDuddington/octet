////////////////////////////////////////////////////////////////////////////////
//
//  Octet Framework (c) Andy Thomason 2012-2014.
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
//
//  (c) Matthew Duddington 2016.
//

#ifndef map_cell_h
#define map_cell_h

namespace octet {

  class MapCell {

    sprite sprite_;
    
    enum CellType;
    CellType cell_type_;
    const CellType passable_[3] = { PATH, START, GOAL };

    // Stores pointers to the adjacent North, South, East and West cells.
    // Thanks to Jack Evans for the suggestion of storing these rather than recalculating each time.
    MapCell* adjacent_cells_[4] = { NULL, NULL, NULL, NULL };  
    
    int my_index;

    void SetupAdjacentCells(std::vector<MapCell>& level_grid
      ,                     int level_grid_index
      ,                     int level_width, int level_height
      ,                     int cell_column, int cell_row)
    {
      my_index = level_grid_index;
      if (cell_column < level_width - 1)
      {  // Cell to the right
        adjacent_cells_[EAST] = &level_grid[level_grid_index + 1];
      } 
      if (cell_row > 0)
      {  // Cell in row above
        adjacent_cells_[NORTH] = &level_grid[level_grid_index - level_width];
      }
      if (cell_column > 0)
      {  // Cell to the left
        adjacent_cells_[WEST] = &level_grid[level_grid_index - 1];
      }
      if (cell_row < level_height - 1)
      {  // Cell in row below
        adjacent_cells_[SOUTH] = &level_grid[level_grid_index + level_width];
      }
    }


  public:
    enum CellType {
      START,
      GOAL,
      PATH,
      WALL,
      BUSH,
      FENCE
    };

    MapCell() {
      cell_type_ = PATH;
    }

    // TODO The init is rather overloaded on function, consider splitting into separate inits or rather just get the MapCell init caller to reach in to the sprite's init function directly.
    void Init( MapCell::CellType CellType
      ,        std::vector<MapCell>& level_grid
      ,        int level_grid_index
      ,        int level_width, int level_height
      ,        int column, int row
      ,        int _texture
      ,        float x, float y
      ,        float s )
    {
      cell_type_ = CellType;
      SetupAdjacentCells(level_grid, level_grid_index, level_width, level_height, column, row);
      sprite_.init(_texture, x, y, s, s);
    }

    sprite& GetSprite() { return sprite_; }
    const int GetIndex() { return my_index; }
    CellType GetType() { return cell_type_; }

    // Returns true if this MapCell can be occupied by an actor.
    bool IsWalkable()
    {
      if ( cell_type_ == PATH
        || cell_type_ == START
        || cell_type_ == GOAL )
      {
        return true;
      }
      return false;
    }

    // Returns True if the passed MapCell is located within the passed distance (number of cells to check each side) and direction.
    bool IsInLineWithMe( MapCell& map_cell
      ,                  Direction direction, int distance )
    {
      MapCell* test_cell = this;
      for (int i = 0; i < distance; i++)
      {
        test_cell = &test_cell->GetAdjacentCell(direction);
        if (test_cell == &map_cell) { return true; }
      }

      return false;
    }

    bool IsAdjacentToMe( MapCell map_cell )
    {
      if ( IsInLineWithMe(map_cell, EAST,  1)
        || IsInLineWithMe(map_cell, NORTH, 1)
        || IsInLineWithMe(map_cell, WEST,  1)
        || IsInLineWithMe(map_cell, SOUTH, 1) )
      { return true; }
      
      return false;
    }

    bool IsAdjacentToMe( MapCell map_cell
      ,                  Direction direction)
    { return IsInLineWithMe( map_cell, direction, 1 ); }

    MapCell& GetAdjacentCell( Direction direction )
    {
      int converted_direction = ( direction / 90 );  // In case of passing in a R_Direction.
      if ( direction < 4 ) { converted_direction = direction; }
      if ( adjacent_cells_[converted_direction] == NULL ) { printf("Asking for NULL cell pointer. \n"); }
      return *adjacent_cells_[converted_direction];
    }

  };

}

#endif // !map_cell_h
