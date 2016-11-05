////////////////////////////////////////////////////////////////////////////////
//
//  (C) Andy Thomason 2012-2014
//
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
//
//  invaderer example: simple game with sprites and sounds
//
//  Additional code and modifications (c) Matthew Duddington 2016.
//

namespace octet {

  class invaderers_app : public octet::app {

    enum GameState {
      MENU,
      GAME,
      GAME_OVER
    };

    GameState game_state_ = MENU;

    enum CameraMode {
      ZOOM_TO_MAP,
      JUMP_TO_PLAYER,
      SLIDE_TO_PLAYER
    };

    CameraMode camera_mode_ = ZOOM_TO_MAP;

    enum LevelGenMode {
      TEXT_FILES,
      FLOOD_FILL
    };

    LevelGenMode level_gen_mode_ = TEXT_FILES;

    int fps_ = 30;  // Assuming 30fps?
    unsigned long frames_since_start_ = 0;  // Used for timing

    SoundManager sound_manager_;
    Level level_;
    int number_of_levels_ = 4;  // TODO Count number of files in a Level directory (this functionality is apparently OS specific however).
    int current_level_num_ = 0;

    bool game_over = false;
    bool waiting_for_input_ = false;
    int input_wait = (int)(0.25f * fps_); // Sec * FPS to nearest frame
    int input_wait_counter = input_wait;

    // Matrix to transform points in our camera space to the world.
    // This lets us move our camera
    mat4t cameraToWorld;

    // shader to draw a textured triangle
    texture_shader texture_shader_;

    float current_camera_z_ = 1;

    /*
    enum {
      NUM_SOUND_SOURCES = 8,
      num_rows = 5,
      num_cols = 10,
      num_missiles = 2,
      num_bombs = 2,
      num_borders = 4,
      num_invaderers = num_rows * num_cols,

      // sprite definitions
      ship_sprite = 0,
      game_over_sprite,

      first_invaderer_sprite,
      last_invaderer_sprite = first_invaderer_sprite + num_invaderers - 1,

      first_missile_sprite,
      last_missile_sprite = first_missile_sprite + num_missiles - 1,

      first_bomb_sprite,
      last_bomb_sprite = first_bomb_sprite + num_bombs - 1,

      first_border_sprite,
      last_border_sprite = first_border_sprite + num_borders - 1,

      dog_sprite,

      explosion_sprite,

      num_sprites, // This needs to stay at the end of the enum, with any new sprites added before it, otherwise the sprit array 'sprites' gets asked to add somthing out of range, which crashes everything.

    };

    // timers for missiles and bombs
    int missiles_disabled;
    int bombs_disabled;

    // accounting for bad guys
    int live_invaderers;
    int num_lives;

    // game state
    bool game_over;
    int score;

    // speed of enemy
    float invader_velocity;

    // sounds
    ALuint whoosh;
    ALuint bang;
    unsigned cur_source;
    ALuint sources[NUM_SOUND_SOURCES];

    // big array of sprites
    sprite sprites[num_sprites]; // Cool so because the count starts at 0 the num_sprites adds 1 to the total by being present itself, so it actually represents the right total intrinsically!

    // random number generator
    class random randomizer;

    // a texture for our text
    GLuint font_texture;

    // information for our text
    bitmap_font font;

    ALuint get_sound_source() { return sources[cur_source++ % NUM_SOUND_SOURCES]; }

    // called when we hit an enemy
    void on_hit_invaderer() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

      live_invaderers--;
      score++;
      if (live_invaderers == 4) {
        invader_velocity *= 4;
      }
      else if (live_invaderers == 0) {
        game_over = true;
        sprites[game_over_sprite].translate(-20, 0);
      }
    }

    // called when we are hit
    void on_hit_ship() {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, bang);
      alSourcePlay(source);

      if (--num_lives == 0) {
        game_over = true;
        sprites[game_over_sprite].translate(-20, 0);
      }
    }

    // use the keyboard to move the ship
    void move_ship() {
      const float ship_speed = 0.05f;
      // left and right arrows
      if (is_key_down(key_left)) {
        sprites[ship_sprite].translate(-ship_speed, 0);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 2])) {
          sprites[ship_sprite].translate(+ship_speed, 0);
        }
      }
      else if (is_key_down(key_right)) {
        sprites[ship_sprite].translate(+ship_speed, 0);
        if (sprites[ship_sprite].collides_with(sprites[first_border_sprite + 3])) {
          sprites[ship_sprite].translate(-ship_speed, 0);
        }
      }
    }

    // Use WASD to move Dog
    // Adaptation of Andy's move_ship code.
    void move_dog()
    {
      const float dog_Xspeed = 0.07f;
      const float dog_Yspeed = 0.1f;
      // A D for horizontal
      if (is_key_down(0x41)) { // A
        sprites[dog_sprite].translate(-dog_Xspeed, 0);
        if (sprites[dog_sprite].collides_with(sprites[first_border_sprite + 2])) {
          sprites[dog_sprite].translate(+dog_Xspeed, 0);
        }
      }
      else if (is_key_down(0x44)) { // D
        sprites[dog_sprite].translate(+dog_Xspeed, 0);
        if (sprites[dog_sprite].collides_with(sprites[first_border_sprite + 3])) {
          sprites[dog_sprite].translate(-dog_Xspeed, 0);
        }
      }
      // W S for vertical
      if (is_key_down(0x57)) { // W
        sprites[dog_sprite].translate(0, +dog_Yspeed);
        if (sprites[dog_sprite].collides_with(sprites[first_border_sprite + 2])) {
          sprites[dog_sprite].translate(0, -dog_Yspeed);
        }
      }
      else if (is_key_down(0x53)) { // S
        sprites[dog_sprite].translate(0, -dog_Yspeed);
        if (sprites[dog_sprite].collides_with(sprites[first_border_sprite + 3])) {
          sprites[dog_sprite].translate(0, +dog_Yspeed);
        }
      }
    }

    // Rotate dog with Q E
    void rotate_dog()
    {
      float rotation_speed = 3.0f;

      if (is_key_down(0x51)) // Q
      {
        sprites[dog_sprite].rotate(-rotation_speed);
      }
      if (is_key_down(0x45)) // E
      {
        sprites[dog_sprite].rotate(+rotation_speed);
      }
    }

    // fire button (space)
    void fire_missiles() {
      if (missiles_disabled) {
        --missiles_disabled;
      }
      else if (is_key_going_down(' ')) {
        // find a missile
        for (int i = 0; i != num_missiles; ++i) {
          if (!sprites[first_missile_sprite + i].is_enabled()) {
            sprites[first_missile_sprite + i].set_relative(sprites[dog_sprite], 0, 0.5f);
            sprites[first_missile_sprite + i].is_enabled() = true;
            missiles_disabled = 5;
            ALuint source = get_sound_source();
            alSourcei(source, AL_BUFFER, whoosh);
            alSourcePlay(source);
            break;
          }
        }
      }
    }

    // pick and invader and fire a bomb
    void fire_bombs() {
      if (bombs_disabled) {
        --bombs_disabled;
      }
      else {
        // find an invaderer
        sprite &ship = sprites[ship_sprite];
        for (int j = randomizer.get(0, num_invaderers); j < num_invaderers; ++j) {
          sprite &invaderer = sprites[first_invaderer_sprite + j];
          if (invaderer.is_enabled() && invaderer.is_above(ship, 0.3f)) {
            // find a bomb
            for (int i = 0; i != num_bombs; ++i) {
              if (!sprites[first_bomb_sprite + i].is_enabled()) {
                sprites[first_bomb_sprite + i].set_relative(invaderer, 0, -0.25f);
                sprites[first_bomb_sprite + i].is_enabled() = true;
                bombs_disabled = 30;
                ALuint source = get_sound_source();
                alSourcei(source, AL_BUFFER, whoosh);
                alSourcePlay(source);
                return;
              }
            }
            return;
          }
        }
      }
    }

    // animate the missiles
    void move_missiles() {
      const float missile_speed = 0.3f;
      for (int i = 0; i != num_missiles; ++i) {
        sprite &missile = sprites[first_missile_sprite + i];
        if (missile.is_enabled()) {
          missile.translate(0, missile_speed);
          for (int j = 0; j != num_invaderers; ++j) {
            sprite &invaderer = sprites[first_invaderer_sprite + j];
            if (invaderer.is_enabled() && missile.collides_with(invaderer)) {
              // Make the enemy expload?
              sprites[explosion_sprite].set_relative(invaderer, 0, 0);

              invaderer.is_enabled() = false;
              invaderer.translate(20, 0);
              missile.is_enabled() = false;
              missile.translate(20, 0);
              on_hit_invaderer();

              goto next_missile;
            }
          }
          if (missile.collides_with(sprites[first_border_sprite])
            || missile.collides_with(sprites[first_border_sprite + 1])
            || missile.collides_with(sprites[last_border_sprite - 1])
            || missile.collides_with(sprites[last_border_sprite])) {
            missile.is_enabled() = false;
            missile.translate(20, 0);
          }
        }
      next_missile:;
      }
    }

    // animate the bombs
    void move_bombs() {
      const float bomb_speed = 0.2f;
      for (int i = 0; i != num_bombs; ++i) {
        sprite &bomb = sprites[first_bomb_sprite + i];
        if (bomb.is_enabled()) {
          bomb.translate(0, -bomb_speed);
          if (bomb.collides_with(sprites[ship_sprite])) {
            bomb.is_enabled() = false;
            bomb.translate(20, 0);
            bombs_disabled = 50;
            on_hit_ship();
            goto next_bomb;
          }
          if (bomb.collides_with(sprites[first_border_sprite + 0])) {
            bomb.is_enabled() = false;
            bomb.translate(20, 0);
          }
        }
      next_bomb:;
      }
    }

    // move the array of enemies
    void move_invaders(float dx, float dy) {
      for (int j = 0; j != num_invaderers; ++j) {
        sprite &invaderer = sprites[first_invaderer_sprite + j];
        if (invaderer.is_enabled()) {
          invaderer.translate(dx, dy);
        }
      }
    }

    // check if any invaders hit the sides.
    bool invaders_collide(sprite &border) {
      for (int j = 0; j != num_invaderers; ++j) {
        sprite &invaderer = sprites[first_invaderer_sprite + j];
        if (invaderer.is_enabled() && invaderer.collides_with(border)) {
          return true;
        }
      }
      return false;
    }
    
    
    void draw_text(texture_shader &shader, float x, float y, float scale, const char *text) {
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      modelToWorld.scale(scale, scale, 1);
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      //mat4t tmp;
      //glLoadIdentity();
      //glTranslatef(x, y, 0);
      //glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);
      //glScalef(scale, scale, 1);
      //glGetFloatv(GL_MODELVIEW_MATRIX, (float*)&tmp);

      enum { max_quads = 32 };
      bitmap_font::vertex vertices[max_quads * 4];
      uint32_t indices[max_quads * 6];
      aabb bb(vec3(0, 0, 0), vec3(256, 256, 0));

      unsigned num_quads = font.build_mesh(bb, vertices, indices, max_quads, text, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font_texture);

      shader.render(modelToProjection, 0);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].x);
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_uv, 3, GL_FLOAT, GL_FALSE, sizeof(bitmap_font::vertex), (void*)&vertices[0].u);
      glEnableVertexAttribArray(attribute_uv);

      glDrawElements(GL_TRIANGLES, num_quads * 6, GL_UNSIGNED_INT, indices);
    }
    */


  public:
    // this is called when we construct the class
    invaderers_app(int argc, char **argv) : app(argc, argv) {//, font(512, 256, "assets/big.fnt") {
    }

    // this is called once OpenGL is initialized
    void app_init() {
      // set up the shader
      texture_shader_.init();

      // set up the matrices
      cameraToWorld.loadIdentity();

      // Setup camera for menu.
      TransformCamera(0, 0, current_camera_z_);

      /*
      font_texture = resource_dict::get_texture_handle(GL_RGBA, "assets/big_0.gif");

      GLuint ship = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/ship.gif");
      sprites[ship_sprite].init(ship, 0, -2.75f, 0.25f, 0.25f);

      GLuint GameOver = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/GameOver.gif");
      sprites[game_over_sprite].init(GameOver, 20, 0, 3, 1.5f);

      GLuint tree = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/tree.jpg");

      GLuint explosion = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/explosion.jpg");
      sprites[explosion_sprite].init(explosion, -5, -5, 0.25f, 0.25f);

      GLuint invaderer = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/invaderer.gif");

      GLuint dog = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/dog.jpg");
      sprites[dog_sprite].init(dog, 2, 2, 0.5f, 0.5f);

      // set the border to white for clarity
      GLuint white = resource_dict::get_texture_handle(GL_RGB, "#ffffff");
      sprites[first_border_sprite + 0].init(white, 0, -3, 6, 0.2f);
      sprites[first_border_sprite + 1].init(white, 0, 3, 6, 0.2f);
      sprites[first_border_sprite + 2].init(white, -3, 0, 0.2f, 6);
      sprites[first_border_sprite + 3].init(white, 3, 0, 0.2f, 6);

      // use the missile texture
      GLuint missile = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/missile.gif");
      for (int i = 0; i != num_missiles; ++i) {
        // create missiles off-screen
        sprites[first_missile_sprite + i].init(missile, 20, 0, 0.0625f, 0.25f);
        sprites[first_missile_sprite + i].is_enabled() = false;
      }

      // use the bomb texture
      GLuint bomb = resource_dict::get_texture_handle(GL_RGBA, "assets/invaderers/bomb.gif");
      for (int i = 0; i != num_bombs; ++i) {
        // create bombs off-screen
        sprites[first_bomb_sprite + i].init(bomb, 20, 0, 0.0625f, 0.25f);
        sprites[first_bomb_sprite + i].is_enabled() = false;
      }

      // sounds
      whoosh = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/whoosh.wav");
      bang = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/bang.wav");
      cur_source = 0;
      alGenSources(NUM_SOUND_SOURCES, sources);

      // sundry counters and game state.
      missiles_disabled = 0;
      bombs_disabled = 50;
      invader_velocity = 0.01f;
      live_invaderers = num_invaderers;
      num_lives = 3;
      game_over = false;
      score = 0;
      */

    }

    const unsigned long& FrameTime() {
      return frames_since_start_;
    }

    void TransformCamera(float x = 0, float y = 0, float z = 0) {
      cameraToWorld.translate(x, y, z);
    }

    // Used for repositioning for each level size
    void ZoomCameraToMap() {
      TransformCamera(0, 0, -current_camera_z_);
      current_camera_z_ = Level::CurrentLevel().LongestSideSize() / 4.0f;  //  Fits the camera to the edges of the longest level edge.
      TransformCamera(0, 0, current_camera_z_);
    }

    void CentreCameraOnPlayer() {
      cameraToWorld = Actor::Player().GetSprite().ModelToWorld();
      cameraToWorld.rotateZ((float)-Actor::Player().GetSprite().LocalRotation());  // Reset the rotation after copying the player transform matrix.
      TransformCamera(0, -Level::CurrentLevel().CellSize(), current_camera_z_);  // Feels more comfortable offset by one cell higher than centre.
    }

    // Gradually moves the camera towards the players position minus a offset.
    // TODO This seems needlessly complicated, review the maths.
    void SlideCameraToPlayer() {
      float new_x_pos = 0;
      float new_y_pos = 0;
      bool should_move = false;
      for (int i = 0; i < 2; i++) {
        float camera_pos = cameraToWorld[3][i];
        float player_pos = Actor::Player().GetSprite().ModelToWorld()[3][i];
        float difference_in_pos = camera_pos - player_pos;
        if (difference_in_pos > 0.5f || difference_in_pos < -0.5f || should_move == true) {
          switch (i) {
          case 0:
            should_move = true;
            new_x_pos = difference_in_pos / (fps_ * 2);
            break;
          case 1:
            should_move = true;
            new_y_pos = Lerp(0, difference_in_pos, 0.5f / fps_);
            break;
          }
        }
      }
      if (should_move) {
        TransformCamera(-new_x_pos, -new_y_pos, 0);
      }
    }

    // Returns the y position on the line between start and end.
    float Lerp(float y_pos_start, float y_pos_end, float t) {
      // General format for Linear Interpolation from:
      // http://answers.unity3d.com/questions/533465/explanation-of-lerp.html
      return (1 - t) * y_pos_start + t * y_pos_end;
    }
    
    // called every frame to move things
    void simulate() {
      if (game_state_ == MENU) {
        bool start_game = false;

        // Check for camera option choice.
        if (is_key_down(key::key_f1)) {
          camera_mode_ = ZOOM_TO_MAP;
        }
        else if (is_key_down(key::key_f2)) {
          camera_mode_ = JUMP_TO_PLAYER;
        }
        else if (is_key_down(key::key_f3)) {
          camera_mode_ = SLIDE_TO_PLAYER;
        }

        // Check for fixed worlds or procedural
        if (is_key_down(key::key_f5)) {
          level_gen_mode_ = TEXT_FILES;
        }
        else if (is_key_down(key::key_f5)) {
          level_gen_mode_ = FLOOD_FILL;
        }

        // Check for starting the game.
        if (is_key_down(key::key_space)) {
          start_game = true;
        }

        if (start_game) {
          // Create player and get pointer.
          Actor::Actors().resize(15);
          Actor::Player().Init(Actor::PLAYER, 0, 0, 0.5f, 0.5f);

          // Load the opening level.
          current_level_num_ = 1;
          Level::CurrentLevel().LoadLevel(current_level_num_);

          // Setup the default zoom level based on first level;
          ZoomCameraToMap();
          if (camera_mode_ != ZOOM_TO_MAP) {
            CentreCameraOnPlayer();
          }

          // Start to listen for player game key-presses.
          waiting_for_input_ = true;

          // Start the game on next loop.
          game_state_ = GAME;
        }
        return;
      }
      else if (game_state_ == GAME_OVER) {
        return;
      }

      /*
      move_ship();

      move_dog();

      rotate_dog();

      fire_missiles();

      fire_bombs();

      move_missiles();

      move_bombs();

      move_invaders(invader_velocity, 0);

      sprite &border = sprites[first_border_sprite + (invader_velocity < 0 ? 2 : 3)];
      if (invaders_collide(border)) {
        invader_velocity = -invader_velocity;
        move_invaders(invader_velocity, -0.1f);
      }
      */

      if (waiting_for_input_) {
        // Check player's decisions first.
        int player_update_return = Actor::Player().Update();
        if (player_update_return < 4) {     // A button has been pressed.
         
          waiting_for_input_ = false;       // Stop listening for input.
          input_wait_counter = input_wait;  // Reset wait counter.

          // Update camera if nececery.
          if (camera_mode_ == JUMP_TO_PLAYER) {
            CentreCameraOnPlayer();
          }
        }
        else if (player_update_return == 10) {
          if (current_level_num_ != number_of_levels_) {
            SoundManager::GameSound().PlaySoundfx(SoundManager::SFX_WIN);
            current_level_num_++;
            if (level_gen_mode_ == FLOOD_FILL) {
              Level::CurrentLevel().LoadLevel();
            }
            else {
              Level::CurrentLevel().LoadLevel(current_level_num_);
            }
            if (camera_mode_ == ZOOM_TO_MAP) {
              ZoomCameraToMap();
            }
            else {
              CentreCameraOnPlayer();
            }
          }
          else { 
            printf("You Win!");
            game_over = true; 
          }
        }
      }
      else {  // Ignore input while counting down wait time.
        if (input_wait_counter > 0) {
          input_wait_counter--;
        }
        else {  // Once time is over, start listing for input again.
          waiting_for_input_ = true;
        }
      }

      // Check Guards' decisions.
      //for (int i = 1; i < Actor::Actors().size(); i++) {
      //  Actor::GetActor(i).Update();
      //}

      // Update camera if nececery.
      if (camera_mode_ == SLIDE_TO_PLAYER) {
        SlideCameraToPlayer();
      }
    }

    // this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      simulate();

      // set a viewport - includes whole window area
      glViewport(x, y, w, h);

      // clear the background to black
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // don't allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glDisable(GL_DEPTH_TEST);

      // allow alpha blend (transparency when alpha channel is 0)
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      if (game_state_ == GAME) {
        // Draw the map
        Level::CurrentLevel().RenderMap(texture_shader_, cameraToWorld);
      
        // Draw actors
        for (int i = 0; i < Actor::Actors().size(); i++) {
          Actor::Actors().at(i).GetSprite().render(texture_shader_, cameraToWorld);
        }
        Actor::Player().GetSprite().render(texture_shader_, cameraToWorld);
      }

      /*
      // draw all the sprites
      for (int i = 0; i != num_sprites; ++i) {
        sprites[i].render(texture_shader_, cameraToWorld);
      }
      
      char score_text[32];
      sprintf(score_text, "score: %d   lives: %d\n", score, num_lives);
      draw_text(texture_shader_, -1.75f, 2, 1.0f / 256, score_text);
      */

      // move the listener with the camera
      vec4 &cpos = cameraToWorld.w();
      alListener3f(AL_POSITION, cpos.x(), cpos.y(), cpos.z());

      frames_since_start_++; // Increment frame counter;
    }
  };
}
