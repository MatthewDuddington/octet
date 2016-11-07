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

    void ExtractFileContent( const char file_location[]
      ,                      int &level_width, int &level_height )
    {
      std::ifstream input_file(file_location);  // Open file.

      if (input_file.bad()) {  // Check if file loaded.
        printf("ERROR: File was not loaded successfully. \n");
      }
      else {
        std::string line_buffer;  // Store current line here
        level_height = -1;        // Reset level height count (first row is 0 so start at -1)

        while (!input_file.eof()) {                // Stop at end of file
          std::getline( input_file, line_buffer ); // Extract the current line to the buffer
         
          // Add each char from the temporary buffer to the permanent vector
          for ( int i = 0; i < line_buffer.length(); i++ ) {
            level_design_.push_back(line_buffer.at(i));
          }
          level_height++;  // Determine the level height by counting each loop
        }
        level_width = (int)level_design_.size() / level_height; // Determine the level width from total chars compared to height (assumes rectangular maps)
      }
    }


    // Uses Flood Fill type algorithm
    // Influenced by example from https://www.youtube.com/watch?v=xYOG8kH2tF8
    void CreateFloodFillDesign( int &level_width, int &level_height ) {
      // Controls how many obstacles appear in the level.
      float obstical_density = 0.4f;
      
      // Decide how big the map should be.
      vec2 random_xy = RandomXY( 15, 15, 7, 7 );
      level_width = random_xy.x();
      level_height = random_xy.y();

      // Place start in top left corner somewhere
      vec2 start_position = RandomXY((int)level_width / 4, (int)level_height / 4); 
      // Place goal somewhere near the bottom
      vec2 goal_position = RandomXY((int)level_width, (int)level_height / 4);
      goal_position.y() += (int)(level_height * 3) / 4;

      // 2 dimensional array of bools to represent possible obstacle placement.
      std::vector< std::vector<bool> > proxy_map( level_height, std::vector<bool>(level_width) );
      //std::vector< std::vector<bool> > check_map( level_height, std::vector<bool>(level_width) );

      // Decide the total number of obstacls we should have.
      int number_of_obsticals = (int)(level_width * level_height * obstical_density);

      int current_placed_obsticals = 0;
      for ( int i = 0; i < number_of_obsticals; i++ )
      {
        random_xy = RandomXY(level_width - 2, level_height - 2);
        proxy_map[(int)random_xy.x()][(int)random_xy.y()] = true;
        current_placed_obsticals++;

        if ( random_xy.get() == start_position.get()
          || random_xy.get() == goal_position.get()
          || !map_can_be_walked( proxy_map
                               , current_placed_obsticals
                               , level_width - 2, level_height - 2
                               , start_position) )
        {
          proxy_map[(int)random_xy.x()][(int)random_xy.y()] = false;
          current_placed_obsticals--;
        }
      }

      // Create the map
      for ( int row = 0; row < level_height; row++ ) 
        // Wall off top and bottom rows.
        if ( row == 0 
          || row == level_height - 1 )
        { for ( int i = 0; i < level_width; i++ ) { level_design_.push_back('x'); } }
        else 
        { 
          for ( int colum = 0; colum < level_width; colum++ )
          {
            // Wall off sides.
            if ( colum == 0
              || colum == level_width - 1 )
            { level_design_.push_back('x'); }
            else 
            {
              if ( start_position.x() == row
                && start_position.y() == colum)
              { level_design_.push_back('S'); }
              else if ( goal_position.x() == row
                     && goal_position.y() == colum)
              { level_design_.push_back('G'); }
              else if (proxy_map[row][colum])
              { level_design_.push_back('b'); }
              else
              { level_design_.push_back(' '); }
            }
          }
        }
    }

    // Algorithm adapted from https://www.youtube.com/watch?v=q7BL-lboRXo
    bool map_can_be_walked( std::vector< std::vector<bool> > proxy_map
      ,                     int total_acceptable_obsticals
      ,                     int width, int height
      ,                     vec2 start_position ) 
    {
      std::vector< std::vector<bool> > checked_coords(height, std::vector<bool>(width));
      std::vector<vec2> queue;
      queue.push_back(start_position);
      checked_coords[start_position.x()][start_position.y()] = true;

      int walkable_cell_count = 1;

      while (queue.size() > 0) {
        vec2 coord = queue.at(queue.size()-1); // Get last item...
        queue.pop_back();  // ...and remove it from the queue.

        for (int x = -1; x <= 1; x++)    // For each of three x positions...
        {    
          for (int y = -1; y <= 1; y++)  // ...in each of the three y positions.
          {  
            int neighbour_x = coord.x() + x;
            int neighbour_y = coord.y() + y;
            if (x == 0 || y == 0)  // But ignoring diagonals.
            { 
              // Make sure the coords are not in the outer wall.
              if ( neighbour_x >= 0
                && neighbour_x < width
                && neighbour_y >= 0
                && neighbour_y < height )
              {
                if ( !checked_coords[neighbour_x][neighbour_y]  // Ignore if already checked.
                  && !proxy_map[neighbour_x][neighbour_y] )     // Block progress if obstacle.
                {
                  checked_coords[neighbour_x][neighbour_y] = true;  // Mark that coords have been checked.
                  queue.push_back(vec2(neighbour_x, neighbour_y));
                  walkable_cell_count++;
                }
              }
            }
          }
        }

        int target_walkable_cells = (int)(width * height - total_acceptable_obsticals);
        return target_walkable_cells == walkable_cell_count; // Answers whether we accessed all tiles that didn't have obsticles on them.
      }
    }


      //level_design_.push_back(generatedchar)

  vec2 RandomXY( int max_x, int max_y, int min_x = 1, int min_y = 1 )
  {
    random randomiser;
    float x = randomiser.get(min_x, max_x);
    float y = randomiser.get(min_y, max_y);
    printf("%f %f\n", x, y);
    return vec2( x, y );
    //return vec2(rand() % (max_x)+1, rand() % (max_y)+1);
  }

  public:
    LevelFileHandler() {}

    void Init( const std::string file_location
      ,        int &level_width
      ,        int &level_height )
    {
      if (file_location != "PROCEDURAL") {
        ExtractFileContent( file_location.c_str(), level_width, level_height );
      }
      else {
        CreateFloodFillDesign( level_width, level_height );
      }
    }

    // Loads the level file only once and returns the char.
    char GetDesignSymbol(int symbol_index)
    {
      if (level_design_.size() < symbol_index) 
      {
        printf( "ERROR: Index requested is out of range for the loaded level design. \n" );
        return NULL;
      }
      else {
        if (symbol_index == level_design_.size() - 1)
        {
          printf( "End of file. Level design file read complete. Clearing file content cache. \n" );
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
