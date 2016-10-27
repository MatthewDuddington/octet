#pragma once

#ifndef level_h
#define level_h

namespace octet {

  class Level {

    static Level* current_level_;
    //std::unique_ptr<Level> current_level_;  // TODO Would it be better to use these?

    int level_width_ = 0;
    int level_height_ = 0;
    // std::string level_name = "Level name goes here";  // TODO extract level name from file and display onscreen.

    std::vector<MapCell> level_grid_;  // Stores grid of map sprites.

    // Load map textures
    const GLuint start_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/start.gif");
    const GLuint goal_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/goal.gif");
    const GLuint path_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/path.gif");
    const GLuint wall_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/wall.gif");
    const GLuint bush_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/bush.gif");
    const GLuint fence_verti_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/fence_vertical.gif");
    const GLuint fence_horiz_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/fence_horizontal.gif");
    
    // Construct the level map.
    void BuildLevel() {
      
      // Load level design and set level specific information.
      LevelFileHandler level_file_handler;  // Assistant module to read the level design file. 
      // TODO add way to load level from int rather than hard coded address.
      level_file_handler.Init("Resources/level.txt", level_width_, level_height_);
      // Prepare level grid vector with the sizes calculated during level file buffering.
      level_grid_.resize(Size());
      MapCell::ProvideClassWithLevelSizes(level_width_, level_height_);  // Part of fix for circular dependency. TODO Avoid having to do this!

      // Iterate through the rows and colls of a grid and instantiate the correct sprite for that cell
      for (int row = 0; row != level_height_; ++row) {            // For each row...
        printf("%s %d \n", "J loop ", row);  // DEBUG
        for (int column = 0; column != level_width_; ++column) {  // ...and each column in that row
          printf("%s %d \n", "I loop ", column);  // DEBUG

          int texture = path_texture;  // Switch will store the texture to be applied here
          MapCell::CellType cell_type = MapCell::PATH;  // Switch will store enum type here
          float x_pos = 0.25f + ((float)column - level_width_ * 0.5f) * 0.5f;
          float y_pos = -0.25f + ((float)row - level_height_* 0.5f) * -0.5f;
         
          // Check the level design file for the symbol that matches this cell's index and determine the texture and type. 
          int current_cell = column + (row * level_width_);  // Calculate index of current cell
          switch (level_file_handler.GetDesignSymbol(current_cell)) {
          case '.':  // Path (already pointed to)
            cell_type = MapCell::PATH;
            break;
          case 'x':  // fall through to
          case 'X':  // Wall
            texture = wall_texture;
            cell_type = MapCell::WALL;
            break;
          case 'I':  // Fence vertical
            texture = fence_verti_texture;
            cell_type = MapCell::FENCE;
            break;
          case 'H':  // Fence horizontal
            texture = fence_horiz_texture;
            cell_type = MapCell::FENCE;
            break;
          case 'b':  // Bush
            texture = bush_texture;
            cell_type = MapCell::BUSH;
            break;
          case 'S':  // Start
            texture = start_texture;
            cell_type = MapCell::START;
            SetupPlayer(x_pos, y_pos);
            break;
          case 'G':  // Goal
            texture = goal_texture;
            cell_type = MapCell::GOAL;
            break;
          case NULL:
            printf("Map gen reading in: Null");
            break;
          default:
            printf("Map gen reading in: Unknown char");
            break;
          }

          // Use the texture and type refrences to instantiate the cell.
          level_grid_[current_cell].Init(
            texture,    // Texture image
            cell_type,  // Cell type identified above
            column,  // Level grid x coordinate
            row,     // Level grid y coordinate
            x_pos,   // x Pos
            y_pos,   // y Pos
            0.5f,    // Width
            0.5f);   // Height

          // Loop until map filled.
        }
      }
    }

   void SetupPlayer(float x_pos, float y_pos) {
     Actor::Player().GetSprite().translate(x_pos, y_pos);
   }

  public:
    Level() {
      current_level_ = this;
    }

    // Way to access functions of the current level
    static Level& CurrentLevel() {
      return *current_level_;
    }

    const int Size() {
      return level_width_ * level_height_;
    }

    const int Width() {
      return level_width_;
    }

    const int Height() {
      return level_height_;
    }

    std::vector<MapCell>& LevelGrid() {
      return level_grid_;
    }
    
    void LoadLevel(int level_num) {  // TODO make argument load specific level from file.
      BuildLevel();
    }

  };

}

#endif  // level_h
