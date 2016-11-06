////////////////////////////////////////////////////////////////////////////////
//
//  Octet Framework (c) Andy Thomason 2012-2014.
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
//
//  (c) Matthew Duddington 2016.
//

#ifndef level_h
#define level_h

namespace octet {

  class Level {

    // static Level* current_level_ = NULL;
    // Can't define a static member var in a .h file, but without initialising the static pointer current_level_ it creates a compilation linking error (LNK2001).
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
    float cell_size_ = 0.5f;

    std::vector<MapCell> level_grid_;  // Stores grid of map sprites.
    sprite map_background_;

    int num_of_guards_ = 0;

    bool use_noise_grass_ = true;

    // Construct the level map.
    void BuildLevel(std::string level_file_location = "PROCEDURAL") {

      // Reset guards
      while (num_of_guards_ > 0) {
        sprite& guard = Actor::Actors().at(num_of_guards_).GetSprite();
        guard.translate(-100, 0);
        guard.is_enabled() = false;
        num_of_guards_--;
      }

      // Load map textures
      GLuint start_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/start.gif");
      GLuint goal_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/goal.gif");
      GLuint path_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/path.gif");
      GLuint wall_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/wall.gif");
      GLuint bush_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bush.gif");
      GLuint fence_verti_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_vertical.gif");
      GLuint fence_horiz_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_horizontal.gif");
      GLuint bush_texture_t = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bush_t.gif");
      GLuint fence_verti_texture_t = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_vertical_t.gif");
      GLuint fence_horiz_texture_t = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/fence_horizontal_t.gif");

      // Load level design and set level specific information.

      LevelFileHandler level_file_handler;  // Assistant module to read the level design file.
      level_file_handler.Init(level_file_location, level_width_, level_height_);  // TODO add way to load level from int rather than hard coded address.
      level_grid_.resize(Size());  // Prepare level grid vector with the sizes calculated during level file buffering.

      // Create a big background tile for the grass.
      if (use_noise_grass_) { map_background_.init(path_texture, 0, 0, level_width_ / 2.0f, level_height_ / 2.0f); }
      else { map_background_.init(0, -100, 0, 0, 0); }

      // Iterate through the rows and colls of a grid and instantiate the correct sprite for each cell
      for (int row = 0; row < level_height_; row++) {            // For each row...
        for (int column = 0; column < level_width_; column++) {  // ...and each column in that row

          int texture = 0;  // Switch will store the texture to be applied here
          MapCell::CellType cell_type = MapCell::PATH;  // Switch will store enum type here
          float x_pos = 0.25f + ((float)column - level_width_ * cell_size_) * 0.5f;
          float y_pos = -0.25f + ((float)row - level_height_* cell_size_) * -0.5f;

          // Check the level design file for the symbol that matches this cell's index and determine the texture and type. 
          int current_cell = column + (row * level_width_);  // Calculate index of current cell
          char current_design_symbol = level_file_handler.GetDesignSymbol(current_cell);
          switch (current_design_symbol) {
          case ' ':  // Path, fall through.
          case '.':  // Planned route char to help with txt file map making only.
          case '!':  // Line of sight danger to help with txt file map making only.
            if (!use_noise_grass_) { texture = path_texture; }
            cell_type = MapCell::PATH;
            break;
          case 'X':  // Wall, fall through
          case 'x':
            texture = wall_texture;
            cell_type = MapCell::WALL;
            break;
          case 'I':  // Fence vertical
            if (!use_noise_grass_) { texture = fence_verti_texture; }
            else { texture = fence_verti_texture_t; }
            cell_type = MapCell::FENCE;
            break;
          case 'H':  // Fence horizontal
            if (!use_noise_grass_) { texture = fence_horiz_texture; }
            else { texture = fence_horiz_texture_t; }
            cell_type = MapCell::FENCE;
            break;
          case 'b':  // Bush
            if (!use_noise_grass_) { texture = bush_texture; }
            else { texture = bush_texture_t; }
            cell_type = MapCell::BUSH;
            break;
          case 'S':  // Start
            texture = start_texture;
            cell_type = MapCell::START;
            SetupActor(Actor::Player(), Actor::PLAYER, x_pos, y_pos, current_cell, R_SOUTH);
            printf("%d", Actor::Actors().size());
            break;
          case 'G':  // Goal
            texture = goal_texture;
            cell_type = MapCell::GOAL;
            break;
          case '>':  // Guards, fall through
          case '^':
          case '<': 
          case 'V':
            cell_type = MapCell::PATH;
            Direction rotation;
            if (current_design_symbol == '>') { rotation = R_EAST; }   // Guard facing right
            if (current_design_symbol == '^') { rotation = R_NORTH; }  // Guard facing upwards
            if (current_design_symbol == '<') { rotation = R_WEST; }   // Guard facing left
            if (current_design_symbol == 'V') { rotation = R_SOUTH; }  // Guard facing down
            num_of_guards_++;
            SetupActor(Actor::Actors().at(num_of_guards_), Actor::GUARD, x_pos, y_pos, current_cell, rotation);
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
            cell_size_);   // Width and height of sprite

          // Loop until map filled.
        }
      }
    }

    // TODO This should really be in the Actor class.
    // Sets up the actor.
    // Rotation must be set using one of the 'R_' directions.
    void SetupActor(Actor& actor, Actor::ActorType type, float x_pos, float y_pos, int current_cell, Direction rotation) {
      actor.Init(type, x_pos, y_pos, cell_size_);
      actor.OccupiedCell(&level_grid_.at(current_cell));
      actor.GetSprite().set_relative(level_grid_.at(current_cell-1).GetSprite(), cell_size_, 0); // Because current cell hasn't been initialised with its position yet, need to offset from previous cell. 'Start' type cell is never in column 0 so it is safe to assume previous index cell is always to the left.
      actor.GetSprite().LocalRotation(rotation);
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

    const int LongestSideSize() {
      return level_width_ > level_height_ ? level_width_ : level_height_;
    }

    std::vector<MapCell>& LevelGrid() {
      return level_grid_;
    }

    const float& CellSize() {
      return cell_size_;
    }

    const int& NumberOfGuards() {
      return num_of_guards_;
    }

    void DontUseNoiseGrass() {
      use_noise_grass_ = false;
    }

    void LoadLevel(int level_num = 0) {
      if (level_num != 0) {  // Builds level from .txt file design.
        std::string level_file_location = "Resources/level" + std::to_string(level_num) + ".txt";  // Construct file path as string.
        BuildLevel(level_file_location);
        return;
      }
      BuildLevel("PROCEDURAL");
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
