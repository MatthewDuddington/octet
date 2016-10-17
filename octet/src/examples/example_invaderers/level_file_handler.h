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

  void extract_file_content(const std::string &file_location) {
    // Open file
    std::ifstream input_file(file_location);

    if (input_file.bad()) {
      printf("%s", "ERROR: File was not loaded succesfully.");
    }
    else {
      std::string line_buffer; // Store current line here

      while (!input_file.eof()) { // Stop at end of file
        std::getline(input_file, line_buffer); // Extract a line
        // Add each char from the current line to the vector
        for (int i = 0; i < line_buffer.length(); i++) {
          file_contents.push_back(line_buffer.at(i));
        }
      }
      
      /* // DEBUG
      for (int i = 0; i < file_contents.size(); i++) {
        printf("%c", file_contents.at(i));
      }
      */
      
    }

    // TODO add the ability to recognise end of the level symbols and the start of object links within the same file.
  }

public:

  level_file_handler() {

  }

  // Destructor to clean up 
  ~level_file_handler() {
    file_contents.clear();
  }

  // Loads the level file only once and returns the char.
  // Use in a level loading loop like the space invaiderers array.
  char get_design_symbol(int symbol_index,
                         const std::string &file_location) {
    if (file_contents.empty()) {
      extract_file_content(file_location);
    }
    //printf("%c%s", get_design_symbol(symbol_index), "\n"); // DEBUG
    return get_design_symbol(symbol_index);
  }

  char get_design_symbol(int symbol_index) {
    if (file_contents.empty()) {
      printf("%s", "ERROR: No level design file has been loaded.");
      return NULL;
    }
    if (file_contents.size() < symbol_index) {
      printf("%s", "ERROR: Index requested is out of range for the loaded level design.");
      return NULL;
    }
    else {
      if (file_contents.at(symbol_index) == NULL) {
        printf("%s", "End of file. Level design file read complete. Clearing file content cache and destructing file handler.");
        delete[] this;
      }
      return file_contents.at(symbol_index);
    }
  }

};

#endif // !level_file_handler_h
