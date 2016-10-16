#ifndef level_builder_h
#define level_builder_h

class level_builder {

  enum {
    level_width = 15,
    level_height = 9,
    num_level_cells = level_width * level_height,

    // Cell indicies
    first_level_cell = 0,
    last_level_cell = num_level_cells;
  };

  void build_level() {
    for (int j = 0; j != level_height; ++j) {
      for (int i = 0; i != level_width; ++i) {

        switch (readFile(i + (j * num_cols))) {
        case '.':
          // Path
          sprites[first_level_cell + i + (j * level_num_cols)].is_enabled() = false;
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

};

#endif // !level_builder_h
