/*
Handles the parsing of custom level design files.
Assumes that the file has been prepared to the same size as the array.

Influenced by: https://github.com/andy-thomason/read_a_csv_file
*/

#ifndef level_file_handler_h
#define level_file_handler_h

#include <fstream>
//#include <string>
//#include <vector>

namespace octet {

  class LevelFileHandler {

    std::vector<char> file_contents_;

    void ExtractFileContent(const std::string &file_location,
      int &level_width,
      int &level_height) {

      std::ifstream input_file(file_location);  // Open file
      if (input_file.bad()) {  // Check if file loaded
        printf("ERROR: File was not loaded successfully.");
      }
      else {
        std::string line_buffer;  // Store current line here
        level_height = -1;        // Reset level height count

        while (!input_file.eof()) {  // Stop at end of file
          std::getline(input_file, line_buffer); // Extract a line
          // Add each char from the current line to the vector
          for (int i = 0; i < line_buffer.length(); i++) {
            file_contents_.push_back(line_buffer.at(i));
          }
          // Update the level size parameters
          // level_width = line_buffer.length();  // TODO Is this less efficient than calculating after the loop as below?
          level_height++;
          printf("Level height: %d \n", level_height);
        }
        level_width = (int)file_contents_.size() / level_height;  // TODO Can this divide be avoided? 

        /* // DEBUG
        for (int i = 0; i < file_contents_.size(); i++) {
          printf("%c", file_contents_.at(i));
        }
        */
      }

      // TODO add the ability to recognise difference between level map design and other level data:
      // Could use specific symbol to delineate level variables.
      // Object links such as which door is opened by which switch.
      // Name of level
    }

  public:

    LevelFileHandler() {

    }

    void Init(const std::string &file_location,
      int &level_width,
      int &level_height) {
      ExtractFileContent(file_location, level_width, level_height);
    }

    // Loads the level file only once and returns the char.
    char GetDesignSymbol(int symbol_index) {
      if (file_contents_.size() < symbol_index) {
        printf("ERROR: Index requested is out of range for the loaded level design.");
        return NULL;
      }
      else {
        if (symbol_index == file_contents_.size() - 1) {
          printf("End of file. Level design file read complete. Clearing file content cache.");
          char temp_char = file_contents_.at(symbol_index); // Temporarily store the return value so it doesn't get cleared.
          file_contents_.clear();  // Cleanup char array
          return temp_char;
        }
        return file_contents_.at(symbol_index);
      }
    }
  };

}

#endif // !level_file_handler_h
