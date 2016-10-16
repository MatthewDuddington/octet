////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Invaderers - will space be free of 'em?
//

#include "../../octet.h"

//#include "invaderers_app.h"

#include "level_file_handler.h"

/// Create a box with octet
int main(int argc, char **argv) {

  //*

  level_file_handler level_file;
  level_file.get_design_symbol(0, "Resources/level.txt");
  printf("%s", "main.cpp is being rerouted");

  std::getchar();

  /*/

  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::invaderers_app app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();

  //*/
}

