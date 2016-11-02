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

    BlendMode blend_mode_ = NORMAL;


  public:
    void init() {
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
        uniform vec4 blend_result;

        void main() {
          if (NORMAL == NORMAL) {}  // Using elements from outside the shader seems to break it.
          gl_FragColor = texture2D(sampler, uv_);
        }

        /*
        vec4 BlendTextureWithTint() {
          switch (1 == 1)
          {
            case MULTIPLY:
              blend_result = texture2D(sampler, uv_) * colour_uniform;
              break;
            case SCREEN:
              blend_result = 1 - (1 - texture2D(sampler, uv_)) * (1 - colour_uniform);
              break;
            case OVERLAY:
              break;
            case NORMAL:  // Fall through
            default:
              blend_result = texture2D(sampler, uv_) * colour_uniform;
            break;
          }
        }
        */
      );

      // use the common shader code to compile and link the shaders
      // the result is a shader program
      shader::init(vertex_shader, fragment_shader);

      // extract the indices of the uniforms to use later
      modelToProjectionIndex_ = glGetUniformLocation(program(), "modelToProjection");
      samplerIndex_ = glGetUniformLocation(program(), "sampler");
      colour_index_ = glGetUniformLocation(program(), "colour_uniform");  // Added: (3) Set GLuint to relevant fragment shader uniform index.
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
      
      blend_mode_ = blend_mode;
      float tint_colour_[4] = { tint_colour.x(), tint_colour.y(), tint_colour.z(), tint_colour.w() };
      glUniform4fv(colour_index_, 1, tint_colour_);  // Added: (4) Passes input arguments into the glUniform type at 2. 4fv specifies its a vector4 type, the count of 1 says it's not an array of vectors, i.e. there is only 1.

      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());
    }
  };

}}
