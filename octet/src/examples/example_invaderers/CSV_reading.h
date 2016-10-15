#ifndef CSVhandler_H
#define CSVhandler_H

#include <iostream> // Will I need this one too?
#include <fstream>
#include <string>

class CSVhandler {

public:
  static void readFile() {
    // Open the CSV file
    std::ifstream myFile("Resources/someCSV.txt");

    /*
    if (!myFile.is_open()) {
      std::cout << "Failed to open file :(\n";
    }
    else {
      std::cout << "Opened CSV file :)\n";

      std::string wholeFile;
      std::string currentLine;
      //while (!myFile.eof()) {
        // Read a line from the file
      while (std::getline(myFile, currentLine));
        wholeFile + currentLine;
      }
      std::cout << currentLine << "\n";
    }
    */
  }

  enum entities {
    start = 0,
    end = 1,
    ground = 2,
    wall = 3,
  };

};

#endif // !CSVhandler_H
