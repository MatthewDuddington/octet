////////////////////////////////////////////////////////////////////////////////
//
//  Octet Framework (c) Andy Thomason 2012-2014.
//
//  (c) Matthew Duddington 2016.
//
//  Handles the parsing of custom level design files.
//  Assumes that the file has been prepared to the same size as the array.
//
//  Influenced by: https://github.com/andy-thomason/read_a_csv_file
//

#ifndef level_file_handler_h
#define level_file_handler_h

#include <fstream>

namespace octet {

  class LevelFileHandler {

    std::vector<char> level_design_;  // Stores the current level's string of map design characters.

    void ExtractFileContent(const char file_location[],
                            int &level_width,
                            int &level_height)
    {
      std::ifstream input_file(file_location);  // Open file.

      if (input_file.bad()) {  // Check if file loaded.
        printf("ERROR: File was not loaded successfully. \n");
      }
      else {
        std::string line_buffer;  // Store current line here
        level_height = -1;        // Reset level height count (first row is 0 so start at -1)

        while (!input_file.eof()) {              // Stop at end of file
          std::getline(input_file, line_buffer); // Extract the current line to the buffer
         
          // Add each char from the temporary buffer to the permanent vector
          for (int i = 0; i < line_buffer.length(); i++) {
            level_design_.push_back(line_buffer.at(i));
          }
          level_height++;  // Determine the level height by counting each loop
        }
        level_width = (int)level_design_.size() / level_height; // Determine the level width from total chars compared to height (assumes rectangular maps)
      }
    }


  public:
    LevelFileHandler() {}

    void Init(const std::string file_location,
              int &level_width,
              int &level_height)
    {
      if (file_location != "PROCEDURAL") {
        ExtractFileContent(file_location.c_str(), level_width, level_height);
      }
      else {
        // TODO Procedurally generate level design.
        // Add level design to level_design_ variable.
      }
    }

    // Loads the level file only once and returns the char.
    char GetDesignSymbol(int symbol_index) {
      if (level_design_.size() < symbol_index) {
        printf("ERROR: Index requested is out of range for the loaded level design. \n");
        return NULL;
      }
      else {
        if (symbol_index == level_design_.size() - 1) {
          printf("End of file. Level design file read complete. Clearing file content cache. \n");
          char temp_char = level_design_.at(symbol_index); // Temporarily store the return value so it doesn't get cleared.
          level_design_.clear();  // Clean up char array (also important for correct level width to be determined if previous level was a larger map)
          return temp_char;
        }
        return level_design_.at(symbol_index);
      }
    }
  };

}

#endif // !level_file_handler_h
