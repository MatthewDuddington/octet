////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Single texture shader with no lighting
// Added: An optional tint parameter

namespace octet { namespace shaders {
  
  class texture_shader : public shader {
  public: enum BlendMode {
      NORMAL,    // No blend, just pass through regular values.
      MULTIPLY,  // Results in a darker blend.
      SCREEN,    // Results in a brighter blend.
      OVERLAY    // Multiply on brighter parts and screen on darker parts.
    };
  

  private:
    // indices to use with glUniform*()
    GLuint modelToProjectionIndex_;  // index for model space to projection space matrix
    GLuint samplerIndex_;            // index for texture sampler
    GLuint colour_index_;            // Added: (1) Index to set a specific colour directly. Set at 3, used at 4.
    GLuint blend_mode_index_;


  public:
    void init() {
      GLuint mix_texture_ = resource_dict::get_texture_handle(GL_RGBA,
        "assets/invaderers/cloud.gif");
      
      // this is the vertex shader.
      // it is called for each corner of each triangle
      // it inputs pos and uv from each corner
      // it outputs gl_Position and uv_ to the rasterizer
      const char vertex_shader[] = SHADER_STR(
        varying vec2 uv_;

        attribute vec4 pos;
        attribute vec2 uv;

        uniform mat4 modelToProjection;

        void main() { gl_Position = modelToProjection * pos; uv_ = uv; }
      );

      // this is the fragment shader
      // after the rasterizer breaks the triangle into fragments
      // this is called for every fragment
      // it outputs gl_FragColor, the color of the pixel and inputs uv_
      const char fragment_shader[] = SHADER_STR(
        varying vec2 uv_;
        uniform sampler2D sampler;
        uniform vec4 colour_uniform;  // Added: (2) Uniform which holds tint colour.
        uniform int blend_mode;


        // Equations for calculating blend types referenced from: https://en.wikipedia.org/wiki/Blend_modes
        vec4 BlendTextureWithTint() {
          switch (blend_mode)
          {
          case 1:  // MULTIPLY
            return texture2D(sampler, uv_) * colour_uniform;
            break;
          case 2:  // SCREEN
            return 1 - ((1 - texture2D(sampler, uv_)) * (1 - colour_uniform));
            break;
          case 3:  // OVERLAY
            vec4 a = texture2D(sampler, uv_);
            if ((a.x + a.y + a.z) / 3 < 0.5) {
              return 2 * texture2D(sampler, uv_) * colour_uniform;
            }
            else {
              return 1 - (2 * (1 - texture2D(sampler, uv_)) * (1 - colour_uniform));
            }
            break;
          case 0:  // NORMAL (Fall through)
          default:
            return texture2D(sampler, uv_) * colour_uniform;
            break;
          }
        }

        void main() {
          gl_FragColor = BlendTextureWithTint();
        }
      );

      // use the common shader code to compile and link the shaders
      // the result is a shader program
      shader::init(vertex_shader, fragment_shader);

      // extract the indices of the uniforms to use later
      modelToProjectionIndex_ = glGetUniformLocation(program(), "modelToProjection");
      samplerIndex_ = glGetUniformLocation(program(), "sampler");
      colour_index_ = glGetUniformLocation(program(), "colour_uniform");  // Added: (3) Set GLuint to relevant fragment shader uniform index.
      blend_mode_index_ = glGetUniformLocation(program(), "blend_mode");
    }

    void render(const mat4t &modelToProjection,
                int sampler,
                vec4& tint_colour,
                BlendMode blend_mode)
    {
      // tell openGL to use the program
      shader::render();

      // customize the program with uniforms
      glUniform1i(samplerIndex_, sampler);
      
      float tint_colour_[4] = { tint_colour.x(), tint_colour.y(), tint_colour.z(), tint_colour.w() };
      glUniform4fv(colour_index_, 1, tint_colour_);  // Added: (4) Passes input arguments into the glUniform type at 2. 4fv specifies its a vector4 type, the count of 1 says it's not an array of vectors, i.e. there is only 1.
      glUniform1i(blend_mode_index_, blend_mode);

      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
    }
  };

}}
