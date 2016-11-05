#pragma once

#ifndef level_h
#define level_h

namespace octet {

  class Level {

    // static Level* current_level_ = NULL;
    // Can't define a static member var in a .h file, but without initialising the static pointer player_ it creates a compilation linking error (LNK2001).
    // In order to have static class variables, they need to be initialised, which can only be done in the .cpp file as you can't double declare in a header. As we're working with 'header only' I needed a workaround.
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf
    // http://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header
    // Suggested answer is to use a static function which returns it's own static variable of the type desired. I have modified the example to enable Set and Get behaviour within the same function. With setting only possible within the class as public getter only returns.
    static Level*& current_level_(Level* level_object = NULL) {
      static Level* current_level_;
      if (level_object != NULL) {
        current_level_ = level_object;
      }
      return current_level_;
    };

    int level_width_ = 0;
    int level_height_ = 0;
    // std::string level_name = "Level name goes here";  // TODO extract level name from file and display onscreen.
    float cell_size_ = 0.5f;

    std::vector<MapCell> level_grid_;  // Stores grid of map sprites.
    sprite map_background_;


    // Construct the level map.
    void BuildLevel(std::string level_file_location = "Auto") {

      // Load map textures
      GLuint start_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/start.gif");
      GLuint goal_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/goal.gif");
      //GLuint path_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/path.gif");
      GLuint wall_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/wall.gif");
      GLuint bush_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bush_t.gif");
      GLuint fence_verti_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_vertical_t.gif");
      GLuint fence_horiz_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_horizontal_t.gif");

      // Load level design and set level specific information.

      LevelFileHandler level_file_handler;  // Assistant module to read the level design file.
      level_file_handler.Init(level_file_location, level_width_, level_height_);  // TODO add way to load level from int rather than hard coded address.
      level_grid_.resize(Size());  // Prepare level grid vector with the sizes calculated during level file buffering.

      // Create a big background tile for the grass.
      map_background_.init(wall_texture, 0, 0, (level_width_ / 2) + cell_size_, (level_height_ / 2) + cell_size_);

      // Iterate through the rows and colls of a grid and instantiate the correct sprite for each cell
      for (int row = 0; row != level_height_; ++row) {            // For each row...
        //printf("%s %d \n", "J loop ", row);  // DEBUG
        for (int column = 0; column != level_width_; ++column) {  // ...and each column in that row
          //printf("%s %d \n", "I loop ", column);  // DEBUG

          int texture = 0;  // Switch will store the texture to be applied here
          MapCell::CellType cell_type = MapCell::PATH;  // Switch will store enum type here
          float x_pos = 0.25f + ((float)column - level_width_ * cell_size_) * 0.5f;
          float y_pos = -0.25f + ((float)row - level_height_* cell_size_) * -0.5f;

          // Check the level design file for the symbol that matches this cell's index and determine the texture and type. 
          int current_cell = column + (row * level_width_);  // Calculate index of current cell
          switch (level_file_handler.GetDesignSymbol(current_cell)) {
          case '.':  // Path
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
            SetupPlayer(x_pos, y_pos, current_cell);
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
            cell_type,     // Cell type identified above
            level_grid_,
            current_cell,  // Index of cell in grid
            level_width_, level_height_,
            column, row,   // Level grid x and y coordinates
            texture,       // Texture image
            x_pos, y_pos,  // x and y positions of sprite
            cell_size_, cell_size_);   // Width and height of sprite

          // Loop until map filled.
        }
      }
    }

    // Sets up the player in the given position.
    void SetupPlayer(float x_pos, float y_pos, int current_cell) {
      //Actor::Player().GetSprite().translate(x_pos, y_pos);
      Actor::Player().OccupiedCell(&level_grid_.at(current_cell));
      Actor::Player().GetSprite().set_relative(level_grid_.at(current_cell-1).GetSprite(), cell_size_, 0); // Because current cell hasn't been initialised with its position yet, need to offset from previous cell. 'Start' type cell is never in column 0 so it is safe to assume previous index cell is always to the left.
      Actor::Player().GetSprite().SetLocalRotation(270);
    }


  public:
    Level() {
      current_level_(this);
    }

    // Way to access functions of the current level
    static Level& CurrentLevel() {
      return *current_level_();
    }

    const int Size() {
      return level_width_ * level_height_;
    }

    const int& Width() {
      return level_width_;
    }

    const int& Height() {
      return level_height_;
    }

    const int LargestSideSize() {
      return level_width_ > level_height_ ? level_width_ : level_height_;
    }

    std::vector<MapCell>& LevelGrid() {
      return level_grid_;
    }

    const float& CellSize() {
      return cell_size_;
    }

    void LoadLevel() {  // Builds procedural level.
      BuildLevel();
    }

    void LoadLevel(int level_num) {  // Builds level from .txt file design.
      std::string level_file_location = "Resources/level" + std::to_string(level_num) + ".txt";  // Construct file path as string.
      BuildLevel(level_file_location);
    }

    void RenderMap(texture_shader& texture_shader, mat4t cameraToWorld) {
      map_background_.render(texture_shader, cameraToWorld, vec4{ 0.6f, 0.9f, 0.4f, 1 }, texture_shader::GRASS);
      for (int i = 0; i < Size(); ++i) {
        LevelGrid().at(i).GetSprite().render(texture_shader, cameraToWorld);
      }
    }

  };

}

#endif  // level_h
