#ifndef data_file_handler_h
#define data_file_handler_h

#include <fstream>
#include <string>
#include <vector>
#include <memory>

class data_file_handler {

  void readDataFile(std::string file_location,
                    std::vector<std::string> data_string_vectors[], // TODO Could this be more generic by having a pointer array to a set of different vector types based on passed argument type - the problem I'm facing is that we have to declair the pointer types explicitly as C++ doesn't have generics? For now I'm storing everything as strings and the calling code can handle translating them into the appropriate expected format.
                    char data_seperator
                   ) {
    // Open the CSV file
    std::ifstream myFile(file_location);

    // Read line by line
    // Check each char sequentially to locate the data seperator


  }

};

#endif // !data_file_handler_h
