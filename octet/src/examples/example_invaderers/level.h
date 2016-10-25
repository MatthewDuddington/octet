#pragma once

#ifndef level_h
#define level_h

namespace octet {

  class Level {

    static Level& current_level_;
    //std::unique_ptr<Level> current_level_;  // TODO Would it be better to use these?

    int level_width_ = 0;
    int level_height_ = 0;
    // std::string level_name = "Level name goes here";  // TODO extract level name from file and display onscreen.

    std::vector<MapCell> level_grid_;  // Stores grid of map sprites.
    std::vector<Actor> actors;  // Stores 

    // Construct the level map.
    void BuildLevel() {
      
      // Load map textures
      // TODO Should I be looping over a text file to import these, or would this create a problem with needing to declare new GLuints?
      static GLuint start_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/start.gif");
      static GLuint goal_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/goal.gif");
      static GLuint path_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/path.gif");
      static GLuint wall_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/wall.gif");
      static GLuint bush_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/bush.gif");
      static GLuint fence_verti_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/fence_vertical.gif");
      static GLuint fence_horiz_texture = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/fence_horizontal.gif");

      // Load level design and set level specific information.
      LevelFileHandler level_file_handler;  // Assistant module to read the level design file. 
      // TODO add way to load level from int rather than hard coded address.
      level_file_handler.Init("Resources/level.txt", level_width_, level_height_);
      // Prepare level grid vector with the sizes calculated during level file buffering.
      level_grid_.resize(Size());

      // Iterate through the rows and colls of a grid and instantiate the correct sprite for that cell
      for (int row = 0; row != level_height_; ++row) {            // For each row...
        printf("%s %d \n", "J loop ", row);  // DEBUG
        for (int column = 0; column != level_width_; ++column) {  // ...and each column in that row
        printf("%s %d \n", "I loop ", column);  // DEBUG

          // Check the level design file for the symbol that matches this cell's index and store the texture and type in temp variables. 
          int current_cell = column + (row * level_width_);  // Calculate index of current cell
          GLuint texture = path_texture;  // Switch will store the texture to be applied here
          MapCell::CellType cell_type = MapCell::PATH;  // Switch will store enum type here
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
            break;
          case 'G':  // Goal
            texture = goal_texture;
            cell_type = MapCell::GOAL;
            break;
          case NULL:
            std::cout << "Map gen reading in: Null";
            break;
          default:
            std::cout << "Map gen reading in: Unknown char";
            break;
          }

          // Use the stored texture and type to instantiate the cell
          level_grid_[current_cell].Init(
            texture,    // Texture image
            cell_type,  // Cell type identified above
            column,  // Level grid x coordinate
            row,     // Level grid y coordinate
            0.25f + ((float)column - level_width_ * 0.5f) * 0.5f,  // x Pos
            -0.25f + ((float)row - level_height_* 0.5f) * -0.5f,   // y Pos
            0.5f,   // Width
            0.5f);  // Height

          // Loop until map filled.
        }
      }
    }

  public:
    Level() {
      current_level_ = *(this);
    }

    // Way to access functions of the current level
    static Level& CurrentLevel() {
      return current_level_;
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

    void AddActor(Actor actor) {
      actors.push_back(actor);
    }

    Actor& Actors(int actor_index) {
      return actors.at(actor_index);
    }
    
    void LoadLevel(int level_num) {  // TODO make argument load specific level from file.
      BuildLevel();
    }

  };

}

#endif  // level_h
