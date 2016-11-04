////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Invaderers - will space be free of 'em?
//

#include "../../octet.h"

#include <string>
#include <vector>
#include <memory>
#include <random>

#include "sprite.h"  // Andy's sprite class moved to its own header.

#include "sound_manager.h"
#include "map_cell.h"
#include "level_file_handler.h"
#include "actor.h"
#include "level.h"

#include "invaderers_app.h"

/// Create a box with octet
int main(int argc, char **argv) {

  /*

  // Testing code goes here

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

