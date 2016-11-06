////////////////////////////////////////////////////////////////////////////////
//
//  Octet Framework (c) Andy Thomason 2012-2014.
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
// 
//  Extracted sound related elements from example invaiderers_app.
//
//  Additional code and modifications (c) Matthew Duddington 2016.
//

#ifndef sound_manager_h
#define sound_manager_h

namespace octet {

  class SoundManager {

  public: enum Sounds {
      BGM,
      SFX_WRONG,
      SFX_CAUGHT,
      SFX_SUCCESS,
      SFX_WIN,

      NUM_SOUND_SOURCES
    };


  private:
    static SoundManager*& sound_manager_(SoundManager* sound_manager = NULL) {
      static SoundManager* sound_manager_;
      if (sound_manager != NULL) {
        sound_manager_ = sound_manager;
      }
      return sound_manager_;
    }

    ALuint sound_ALuints[NUM_SOUND_SOURCES];
    ALuint sources[NUM_SOUND_SOURCES];
    unsigned current_source;
    
    ALuint get_sound_source() {
      // Get the next available source but % will wrap around value if run out of array.
      ALuint next_source = sources[current_source++ % NUM_SOUND_SOURCES];
      if (next_source == 0) { next_source = get_sound_source(); }
      return next_source;
    }


  public:
    SoundManager() {
      sound_manager_(this);
      GameSound().Init();
    }

    static SoundManager& GameSound() {
      return *sound_manager_();
    }

    void Init() {
      // Add sound files to resource dictionary.
      sound_ALuints[BGM] = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/Rising.wav");
      sound_ALuints[SFX_WRONG] = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/188013__isaac200000__error.wav");
      sound_ALuints[SFX_CAUGHT] = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/60521__robinhood76__00363-voice-shouting-stop.wav");
      //sound_ALuints[SFX_SUCCESS] = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "");
      //sound_ALuints[SFX_WIN] = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "");

      current_source = 0;
      alGenSources(NUM_SOUND_SOURCES, sources);


      // Play Background music. TODO Doesn't loop right now.
      ALuint source = sources[NUM_SOUND_SOURCES];
      alSourcei(source, AL_BUFFER, sound_ALuints[BGM]);
      alSourcePlay(source);
    }

    void PlaySoundfx(Sounds sound) {  // Can't use PlaySound because of namespace clash.
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, sound_ALuints[sound]);
      alSourcePlay(source);
    }
  };

}

#endif // !sound_manager_h
