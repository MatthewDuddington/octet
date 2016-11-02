/*
Octet Framework (c) Andy Thomason 2012-2014.

Extracted 'sprite' class from invaiderers_app.

Additional and modified code (c) Matthew Duddington 2016.
*/

#ifndef sprite_h
#define sprite_h

namespace octet {

    enum Direction { NORTH, EAST, SOUTH, WEST };  // Most appropriate place for this seems to be here as the sprite class holds the 'transform' functionality.

  class sprite {
    mat4t modelToWorld;  // where is our sprite (overkill for a 2D game!)
    float halfWidth;     // half the width of the sprite
    float halfHeight;    // half the height of the sprite
    int texture;         // what texture is on our sprite
    bool enabled;        // true if this sprite is enabled.

    // this is an array of the positions of the corners of the texture in 2D
    float uvs[8] = {
      0,  0,
      1,  0,
      1,  1,
      0,  1,
    };

    int animation_frame_counter = 0;  // Added: Keeps track of which frame of the currently playing animation the sequence is on.

    // Added: Moves the uv rectangle to a new position on the texture.
    void ChangeUVPosition(float lower_left_x, float lower_left_y) {
      float offset_x = lower_left_x - uvs[0];
      float offset_y = lower_left_y - uvs[1];
      for (int i = 0; i < 8; i += 2) {
        uvs[i] = uvs[i] + offset_x;
        uvs[i + 1] = uvs[i + 1] + offset_y;
      }
    }


  public:
    sprite() {
      texture = 0;
      enabled = true;
    }

    // Testing out a summary.
    /// <summary>
    /// Initialises into the scene the texture represented by a GLuint from get_texture_handle.
    /// x and y floats set the starting coords and w and h floats specify the size on screen.
    /// </summary>
    void init(int _texture, float x, float y, float w, float h) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, 0);
      halfWidth = w * 0.5f;
      halfHeight = h * 0.5f;
      texture = _texture;
      enabled = true;
    }

    void render(texture_shader &shader,
                mat4t &cameraToWorld,
                vec4 tint_colour = { 0.2f, 0.6f, 0.7f, 1 },
                texture_shader::BlendMode blend_mode = texture_shader::NORMAL)
    {
      // invisible sprite... used for gameplay.
      if (!texture) return;

      // build a projection matrix: model -> world -> camera -> projection
      // the projection space is the cube -1 <= x/w, y/w, z/w <= 1
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);

      // set up opengl to draw textured triangles using sampler 0 (GL_TEXTURE0)
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);

      // use "old skool" rendering
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      shader.render(modelToProjection, 0, tint_colour, blend_mode);

      // this is an array of the positions of the corners of the sprite in 3D
      // a straight "float" here means this array is being generated here at runtime.
      float vertices[] = {
        -halfWidth, -halfHeight, 0,
        halfWidth, -halfHeight, 0,
        halfWidth,  halfHeight, 0,
        -halfWidth,  halfHeight, 0,
      };

      // attribute_pos (=0) is position of each corner
      // each corner has 3 floats (x, y, z)
      // there is no gap between the 3 floats and hence the stride is 3*sizeof(float)
      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vertices);
      glEnableVertexAttribArray(attribute_pos);

      // attribute_uv is position in the texture of each corner
      // each corner (vertex) has 2 floats (x, y)
      // there is no gap between the 2 floats and hence the stride is 2*sizeof(float)
      glVertexAttribPointer(attribute_uv, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)uvs);
      glEnableVertexAttribArray(attribute_uv);

      // finally, draw the sprite (4 vertices)
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // Added: Get position of the object in world space
    const vec3 Position() {
      vec3 position;
      position.x() = modelToWorld[3][0];
      position.y() = modelToWorld[3][1];
      position.z() = modelToWorld[3][2];
      return position;
    }

    // move the object
    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    // Added: Rotate the object
    void rotate(float z) {
      modelToWorld.rotateZ(z);
    }

    // position the object relative to another.
    void set_relative(sprite &rhs, float x, float y) {
      modelToWorld = rhs.modelToWorld;
      modelToWorld.translate(x, y, 0);
    }

    // return true if this sprite collides with another.
    // note the "const"s which say we do not modify either sprite
    bool collides_with(const sprite &rhs) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
      float dy = rhs.modelToWorld[3][1] - modelToWorld[3][1];

      // both distances have to be under the sum of the halfwidths
      // for a collision
      return
        (fabsf(dx) < halfWidth + rhs.halfWidth) &&
        (fabsf(dy) < halfHeight + rhs.halfHeight)
        ;
    }

    bool is_above(const sprite &rhs, float margin) const {
      float dx = rhs.modelToWorld[3][0] - modelToWorld[3][0];
      return (fabsf(dx) < halfWidth + margin);
    }

    bool &is_enabled() {
      return enabled;
    }

    // Added:
    enum AnimationName {
      walk
    };

    // Added: 
    int Animate(AnimationName animation_name) {  // TODO Replace with an 'Animation' class object which contains a series of steps to carry out enabling compound behaviours.
      int number_of_frames;
      vec2 sprite_size;
      switch (animation_name)
      {
      case walk:
        number_of_frames = 8;
        sprite_size = (0.2f, 0.2f);
        break;
      default:
        break;
      }
      ChangeUVPosition(sprite_size.x() * (1 + animation_frame_counter), sprite_size.y() * (1 + animation_frame_counter));
    }
  };

}

#endif // !sprite_h
