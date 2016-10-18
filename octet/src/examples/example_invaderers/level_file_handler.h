/*
Handles the parsing of custom level design files.
Assumes that the file has been prepared to the same size as the array.

Influenced by: https://github.com/andy-thomason/read_a_csv_file
*/

#ifndef level_file_handler_h
#define level_file_handler_h

#include <fstream>
#include <string>
#include <vector>

class level_file_handler {
  
  std::vector<char> file_contents;

  void extract_file_content(const std::string &file_location,
                            int &level_width,
                            int &level_height) {
    
    std::ifstream input_file(file_location); // Open file
    if (input_file.bad()) { // Check if file loaded
      printf("%s", "ERROR: File was not loaded successfully.");
    }
    else {
      std::string line_buffer; // Store current line here
      level_height = 0; // Reset level height count

      while (!input_file.eof()) { // Stop at end of file
        std::getline(input_file, line_buffer); // Extract a line
        // Add each char from the current line to the vector
        for (int i = 0; i < line_buffer.length(); i++) {
          file_contents.push_back(line_buffer.at(i));
        }
        // Update the level size parameters
        // level_width = line_buffer.length(); // Is this less efficient than calculating after the loop as below?
        level_height++;
        printf("level height: %d", level_height);
      }
      level_width = file_contents.size() / level_height; // TODO Can this divide be avoided? 
      
      /* // DEBUG
      for (int i = 0; i < file_contents.size(); i++) {
        printf("%c", file_contents.at(i));
      }
      */
      
    }

    // TODO add the ability to recognise difference between level map design and other level data:
    // Could use specific symbol to delineate level variables.
    // Object links such as which door is opened by which switch.
    // Name of level
    // Size of level - though could we infer this from the map design somehow from length of line and number of rows?
  }

public:

  level_file_handler() {

  }

  void init(const std::string &file_location,
            int &level_width,
            int &level_height) {

  }
 
  // Loads the level file only once and returns the char.
  // Use in a level loading loop like the space invaiderers array.
  char get_design_symbol(int symbol_index,
                         const std::string &file_location) {
    if (file_contents.empty()) {
      extract_file_content(file_location);
    }
    //printf("%c\n", get_design_symbol(symbol_index)); // DEBUG
    return get_design_symbol(symbol_index);
  }

  char get_design_symbol(int symbol_index) {
    if (file_contents.empty()) {
      printf("ERROR: No level design file has been loaded.");
      return NULL;
    }
    if (file_contents.size() < symbol_index) {
      printf("ERROR: Index requested is out of range for the loaded level design.");
      return NULL;
    }
    else {
      if (symbol_index == file_contents.size() - 1) {
        printf("End of file. Level design file read complete. Clearing file content cache.");
        char temp_char = file_contents.at(symbol_index); // Temporarily store the return value so it doesn't get cleared.
        file_contents.clear();
        return temp_char;
      }
      return file_contents.at(symbol_index);
    }
  }

};

#endif // !level_file_handler_h
