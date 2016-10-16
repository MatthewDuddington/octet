/*
Handles the parsing of custom level design files.
Assumes that the file has been prepared to the same size as the array.
*/

#ifndef level_file_handler_h
#define level_file_handler_h

#include <fstream>
#include <string>
#include <vector>

class level_file_handler {
  
  std::vector<char> file_contents;

  void extract_file_content(const std::string &file_location) {
    std::ifstream inputFile(file_location);

    if (inputFile.bad) {
      printf("%s", "ERROR: File was not loaded succesfully.");
    }
    else {
      // Load the chars into file_contents
    }
  }

public:

  // Loads the level file only once and returns the char.
  // Use in a level loading loop like the space invaiderers array.
  char get_design_symbol(int symbol_index,
                         const std::string &file_location) {
    if (file_contents.empty) {
      extract_file_content(file_location);
    }
    return get_design_symbol(symbol_index);
  }

  char get_design_symbol(int symbol_index) {
    if (file_contents.empty) {
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

  ~level_file_handler() {
    file_contents.clear();
  }

  // Function: Private: Extract file content
  // Read in the level design file
  // Store it in a char vector
  // Read in each line and add it to the end of the array
  // At end of file stop

  // Function: Public: get design symbol
  // Pass in vector nth term int
  // Return the symbol at that locaiton

  // Function: Private: Clean up extracted file vector

};

#endif // !level_file_handler_h
