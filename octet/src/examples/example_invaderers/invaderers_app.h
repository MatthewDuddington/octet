////////////////////////////////////////////////////////////////////////////////
//
//  (C) Andy Thomason 2012-2014
//
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
//  invaderer example: simple game with sprites and sounds
//
//  'Facility' game code and modifications (c) Matthew Duddington 2016.
//

namespace octet {

  class invaderers_app : public octet::app {

    enum GameState {
      MENU,
      GAME,
      FAILED_LEVEL,
      GAME_OVER
    };
    GameState game_state_ = MENU;

    enum CameraMode {
      ZOOM_TO_MAP,
      JUMP_TO_PLAYER,
      SLIDE_TO_PLAYER,
      
      // Used for consistent reset
      DEFAULT_CAMERA_MODE = SLIDE_TO_PLAYER
    };
    CameraMode camera_mode_ = DEFAULT_CAMERA_MODE;

    enum LevelGenMode {
      TEXT_FILES,
      FLOOD_FILL,

      // Used for consistent reset
      DEFAULT_LEVEL_GEN_MODE = TEXT_FILES
    };
    LevelGenMode level_gen_mode_ = DEFAULT_LEVEL_GEN_MODE;

    enum GrassMode {
      FROM_FILE,
      NOISE,

      // Used for consistent reset
      DEFAULT_GRASS_MODE = NOISE
    };
    GrassMode grass_mode_ = DEFAULT_GRASS_MODE;

    enum {
      FPS = 30,  // Assuming 30fps?
      NUM_LEVELS = 4,  // TODO Count number of files in a Level directory (this functionality is apparently OS specific however).
      STARTING_LEVEL = 1,  // Change to jump during testing
      GUI_CAMERA_Z = 1,

      // Timer defaults (Sec * FPS to nearest frame)
      INPUT_WAIT_TIME = (int)(0.25f * FPS),
      FAIL_WAIT_TIME = (int)(5 * FPS)
    };

    unsigned long frames_since_start_ = 0;  // Used for timing

    SoundManager sound_manager_;
    Level level_;
    sprite gui_screen;
    
    int current_level_num_ = STARTING_LEVEL;  // Overwritten with menu during ResetGame() but that's fine as the menu start option will trigger the correct STARTING_LEVEL value.

    bool should_blank_screen_ = false;  // Used to give a quick black out.
    bool waiting_for_input_ = false;
    int input_wait_counter = INPUT_WAIT_TIME;
    int timer = 0;

    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

    GLuint menu_texture;  // Bit of a cheat to quickly have a menu screen.
    GLuint win_texture;

    float current_camera_z_ = GUI_CAMERA_Z;

    // random number generator
    class random randomizer;


  public:
    // this is called when we construct the class
    invaderers_app( int argc, char **argv ) : app( argc, argv ) {}

    // this is called once OpenGL is initialized
    void app_init()
    {
      // set up the shader
      texture_shader_.init();

      // set up the matrices
      cameraToWorld.loadIdentity();

      // Load menu image.
      menu_texture =
        resource_dict::get_texture_handle( GL_RGBA, "assets/invaderers/menu.gif" );

      // Preload win image.
      win_texture =
        resource_dict::get_texture_handle( GL_RGBA, "assets/invaderers/win.gif" );

      // Setup the game as new.
      ResetGame();
    }

    // TODO Do I need this anymore?
    const unsigned long& FrameTime() { return frames_since_start_; }



  // // CAMERA STUFF // //

    void TransformCamera( float x, float y, float z ) { cameraToWorld.translate( x, y, z ); }

    void ResetCamera() { TransformCamera(0, 0, -current_camera_z_); } // TODO Maybe better if this was just a straight matrix reset?

    // Used for repositioning for each level size
    void ZoomCameraToMap()
    {
      ResetCamera();
      current_camera_z_ = Level::CurrentLevel().LongestSideSize() / 4.0f;  //  Fits the camera to the edges of the longest level edge.
      TransformCamera( 0, 0, current_camera_z_ );
    }

    // Moves the camera's matrix transform to match the player's, ignoring rotation and with a vertical offset for visual comfort.
    void CentreCameraOnPlayer()
    {
      cameraToWorld = Actor::Player().GetSprite().ModelToWorld();
      cameraToWorld.rotateZ( (float)-Actor::Player().GetSprite().LocalRotation() );  // Reset the rotation after copying the player transform matrix.
      TransformCamera( 0, -Level::CurrentLevel().CellSize(), current_camera_z_ );  // Feels more comfortable offset by one cell higher than centre.
    }

    // Gradually moves the camera towards the players position minus a offset.
    // TODO This seems needlessly complicated, review the maths.
    void SlideCameraToPlayer()
    {
      float new_x_pos = 0;
      float new_y_pos = 0;
      bool should_move = false;

      for ( int i = 0; i < 2; i++ )
      {
        float camera_pos = cameraToWorld[3][i];
        float player_pos = Actor::Player().GetSprite().ModelToWorld()[3][i];
        float difference_in_pos = camera_pos - player_pos;

        if ( difference_in_pos > 0.5f
          || difference_in_pos < -0.5f
          || should_move == true)
        {
          switch (i)
          {
          case 0:
            should_move = true;
            new_x_pos = difference_in_pos / (FPS * 2);
            break;
          case 1:
            should_move = true;
            new_y_pos = Lerp(0, difference_in_pos, 0.5f / FPS);
            break;
          }
        }
      }

      if (should_move) { TransformCamera(-new_x_pos, -new_y_pos, 0); }
    }

    // Returns the y position on the line between start and end.
    // TODO This isn't the right way of doing Lerp, but it gives an ok result for now.
    float Lerp(float y_pos_start, float y_pos_end, float t)
    {
      // General format for Linear Interpolation from:
      // http://answers.unity3d.com/questions/533465/explanation-of-lerp.html
      return (1 - t) * y_pos_start + t * y_pos_end;
    }

    void LoadLevel()
    {
      if (level_gen_mode_ == FLOOD_FILL) { Level::CurrentLevel().LoadLevel(); }
      else { Level::CurrentLevel().LoadLevel(current_level_num_); }
      
      if (camera_mode_ == ZOOM_TO_MAP) { ZoomCameraToMap(); }
      else { CentreCameraOnPlayer(); }

      game_state_ = GAME;
      should_blank_screen_ = false;  // Reset for when we arrive here after fail loop.
      waiting_for_input_ = true;
    }

    void ResetGame()
    {
      // Reset variables
      game_state_ = MENU;
      camera_mode_ = DEFAULT_CAMERA_MODE;
      level_gen_mode_ = DEFAULT_LEVEL_GEN_MODE;
      grass_mode_ = DEFAULT_GRASS_MODE;
      current_level_num_ = 0;  // Not STARTING_LEVEL so it's always the menu.
      current_camera_z_ = GUI_CAMERA_Z;
      
      TransformCamera(0, 0, current_camera_z_);   // Setup camera for menu.
      gui_screen.init(menu_texture, 0, 0, 2, 2);  // Display menu image.

      SoundManager::GameSound().PlayBGM(SoundManager::BGM);   // Start background music.
    }

  // // END OF CAMERA STUFF // //



  // // UPDATE FUNCTION // //
    // Main gameplay loop. Called by draw_world() (see below) every frame.
    void Update() {

      // INPUT CHECKS :: UPDATE FUNCTION //
      // At any time press ESC to restart.
      if (is_key_down(key_esc)) { ResetGame(); }

      // MENU STUFF :: UPDATE FUNCTION //
      if (game_state_ == MENU) {
        bool start_game = false;

        // Check for camera option choice.
        if      (is_key_down(key::key_f1)) { camera_mode_ = ZOOM_TO_MAP; }
        else if (is_key_down(key::key_f2)) { camera_mode_ = JUMP_TO_PLAYER; }
        else if (is_key_down(key::key_f3)) { camera_mode_ = SLIDE_TO_PLAYER; }

        // Check for fixed worlds or procedural.
        if      (is_key_down(key::key_f5)) { level_gen_mode_ = TEXT_FILES; }
        else if (is_key_down(key::key_f6)) { level_gen_mode_ = FLOOD_FILL; }

        // Check for noise generated grass or texture from file.
        if      (is_key_down(key::key_f8)) { grass_mode_ = NOISE; }
        else if (is_key_down(key::key_f9)) { grass_mode_ = FROM_FILE; }

        // Check for starting the game.
        if (is_key_down(key::key_space))
        {
          start_game = true;
          gui_screen.init(0, -100, 0, 0, 0);  // Clear menu sprite.

          if (grass_mode_ == FROM_FILE) { Level::CurrentLevel().DontUseNoiseGrass(); }
        }

        if (start_game)
        {
          // Create player and get pointer.
          Actor::Actors().resize(15);
          float actor_size = Level::CurrentLevel().CellSize();
          Actor::Player().Init(Actor::PLAYER, 0, 0, actor_size);

          // Initialise actors.
          for ( int i = 1; i < 15; i++ )
          {
            Actor::Actors().at(i).Init(Actor::GUARD, -100, 0, actor_size);
          }

          // Load the opening level.
          current_level_num_ = STARTING_LEVEL;
          LoadLevel();

          // Setup the default zoom level based on first level.
          ZoomCameraToMap();

          // Set camera position if applicable.
          if (camera_mode_ != ZOOM_TO_MAP) { CentreCameraOnPlayer(); }
        }
        return;
      }

      // FAILING LEVEL :: UPDATE FUNCTION //
      // When guard has caught the player we display the outcome for a few frames then restart.
      else if (game_state_ == FAILED_LEVEL)
      {
        if (timer > 0) { timer--; }

        if ( timer < (FAIL_WAIT_TIME / 2)
          && timer > 0 )
        {
          should_blank_screen_ = true;
          return;
        }
        else { LoadLevel(); } // Reset level. TODO Not very efficient but it'll do for now.
      }

      // WINNING GAME :: UPDATE FUNCTION //
      else if (game_state_ == GAME_OVER)
      {
        if (is_key_down(key::key_space)) { ResetGame(); }
        return;
      }



      // PLAYER CHECK  :: UPDATE FUNCTION //
      {  // IDE organisation braces
        if (waiting_for_input_) {
          int player_update_return = Actor::Player().Update();
          if (player_update_return < Actor::ACTOR_IDLE) {  // A button has been pressed.

            waiting_for_input_ = false;       // Stop listening for input.
            input_wait_counter = INPUT_WAIT_TIME;  // Reset wait counter.

            // Update camera if applicable.
            if (camera_mode_ == JUMP_TO_PLAYER) { CentreCameraOnPlayer(); }
          }
          else if ( player_update_return == Actor::GOAL_REACHED
                 || is_key_down(key::key_backspace) )  // Cheat to end level.
          {  // Goal reached
            if (current_level_num_++ < NUM_LEVELS)  // Add 1 after comparison
            {
              SoundManager::GameSound().PlaySoundfx(SoundManager::SFX_SUCCESS);
              LoadLevel();
              waiting_for_input_ = false;
              input_wait_counter = INPUT_WAIT_TIME;
            }
            else
            {  // All levels completed.
              SoundManager::GameSound().PlayBGM(SoundManager::SFX_WIN);  // Stops BGM and overwrights with Win sound.
              gui_screen.init( win_texture, 100, 0, 2, 2 );  // Load win screen
              ResetCamera();
              TransformCamera( 0, 0, GUI_CAMERA_Z );
              game_state_ = GAME_OVER;
            }
          }
        }
        else {  // 'waiting_for_input_' == false
          // Ignore input while counting down wait time.
          if (input_wait_counter > 0) { input_wait_counter--; }
          // Once time is over, start listing for input again.
          else { waiting_for_input_ = true; }
        }
      }

      // GUARD CHECK :: UPDATE FUNCTION //
      {  // IDE organisation braces
        for ( int i = Level::CurrentLevel().NumberOfGuards(); i > 0; i-- ) {  // Run update on each guard one by one.
          int guard_update_return = Actor::GetActor(i).Update();
          if ( guard_update_return == Actor::CAUGHT_BY_GUARD
            && game_state_ == GAME)
          {
            SoundManager::GameSound().PlaySoundfx(SoundManager::SFX_CAUGHT);
            waiting_for_input_ = false;
            timer = FAIL_WAIT_TIME;
            game_state_ = FAILED_LEVEL;
          }
        }
      }

      // Update camera if applicable.
      if (camera_mode_ == SLIDE_TO_PLAYER) { SlideCameraToPlayer(); }
    } // // END OF UPDATE FUNCTION // //



  // // RENDER STUFF // //
    // this is called to draw the world
    void draw_world( int x, int y, int w, int h ) {
      Update();

      // set a viewport - includes whole window area
      glViewport( x, y, w, h );

      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      if (game_state_ == GAME ?  // If in normal game loop then display stuff.
            true
          : game_state_ == FAILED_LEVEL ?  // After failure triggered only display stuff until fail loop says we should blank stuff.
              !should_blank_screen_  // True if we should display stuff during fail loop.
            : false)
      {
        // Draw the map
        Level::CurrentLevel()
          .RenderMap( texture_shader_, cameraToWorld );
      
        // Draw actors
        for ( int i = 0; i < Actor::Actors().size(); i++ )
        {
          Actor::Actors().at(i).GetSprite()
            .render( texture_shader_, cameraToWorld );  // Render guards
        }
        Actor::Player().GetSprite()
          .render( texture_shader_, cameraToWorld );    // Render player
      }
      else if (game_state_ == MENU) {
        gui_screen.render( texture_shader_, cameraToWorld );
      }

      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f( AL_POSITION, cpos.x(), cpos.y(), cpos.z() );

      frames_since_start_++; // Increment frame counter;
    }  // // END OF RENDER STUFF // //

  };  // End class

}  // End namespace Octet
