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

    if (!myFile.is_open()) {
      std::cout << "Failed to open file :(\n";
    }
    else {
      std::cout << "Opened CSV file :)\n";

      while (!myFile.eof()) {
        // Read a line from the file
        std::string currentLine;
        std::getline(myFile, currentLine);

        std::cout << currentLine << "\n";
      }
    }
  }

}

#endif // !CSVhandler_H
