#ifndef level_h
#define level_h

#include <string>

class level {

  int level_width = 0;
  int level_height = 0;
  std::string level_name = "No Level Loaded";

  std::vector<sprite> level_grid;

  void build_level() {

    for (int j = 0; j != level_height; ++j) {
      for (int i = 0; i != level_width; ++i) {
        int current_cell = i + (j * level_width);
        switch (readFile(current_cell)) {
        case '.':
          // Path
          level_grid[current_cell].is_enabled() = false;
          break;
        case 'x':
        case 'X':
          // Spawn invaiderer
          sprites[first_invaderer_sprite + i + j*num_cols].init(
            invaderer, ((float)i - num_cols * 0.5f) * 0.5f, 2.50f - ((float)j * 0.5f), 0.25f, 0.25f);
          break;
        case NULL:
          std::cout << "null";
          break;
        default:
          std::cout << "Unknown char";
          // Happens at eof too.
          break;
        }

      }
    }
  }

public:
  void load_level(int level_num) {
    level_file_handler level_handler;
    level_handler.get_design_symbol  file_reader.
  }

};

#endif // !level_h
