////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Invaderers - will space be free of 'em?
//

#include "../../octet.h"

#include "invaderers_app.h"

#include "CSV_reading.h"

/// Create a box with octet
int main(int argc, char **argv) {

  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::invaderers_app app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}

