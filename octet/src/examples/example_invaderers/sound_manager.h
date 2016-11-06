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
    
    public: enum Sounds {  // Has to be declared in additional public at top, as NUM_SOUND_SOURCES is used in a private array deceleration.
      BGM,
      SFX_WRONG,
      SFX_CAUGHT,
      SFX_SUCCESS,
      SFX_WIN,

      NUM_SOUND_SOURCES
    };


  private:
    static SoundManager*& sound_manager_( SoundManager* sound_manager = NULL )
    {
      static SoundManager* sound_manager_;
      if (sound_manager != NULL) { sound_manager_ = sound_manager; }
      return sound_manager_;
    }

    ALuint sound_ALuints_[NUM_SOUND_SOURCES];
    ALuint sources_[NUM_SOUND_SOURCES];
    unsigned current_source_;
    
    ALuint get_sound_source()
    {
      // Get the next available source but % will wrap around value if run out of array.
      ALuint next_source = sources_[current_source_++ % NUM_SOUND_SOURCES];
      if (next_source == 0) { next_source = get_sound_source(); } // Stops BGM slot from being overwritten.
      return next_source;
    }


  public:
    SoundManager()
    {
      sound_manager_(this);
      GameSound().Init();
    }

    static SoundManager& GameSound() { return *sound_manager_(); }

    void Init()
    {
      // Add sound files to resource dictionary.
      sound_ALuints_[BGM] =
        resource_dict::get_sound_handle( AL_FORMAT_MONO16, "assets/invaderers/rising.wav" );
      sound_ALuints_[SFX_WRONG] =
        resource_dict::get_sound_handle( AL_FORMAT_MONO16, "assets/invaderers/wrong.wav" );
      sound_ALuints_[SFX_CAUGHT] =
        resource_dict::get_sound_handle( AL_FORMAT_MONO16, "assets/invaderers/caught.wav" );
      sound_ALuints_[SFX_SUCCESS] =
        resource_dict::get_sound_handle( AL_FORMAT_MONO16, "assets/invaderers/success.wav" );
      sound_ALuints_[SFX_WIN] =
        resource_dict::get_sound_handle( AL_FORMAT_MONO16, "assets/invaderers/win.wav" );

      current_source_ = 0;
      alGenSources( NUM_SOUND_SOURCES, sources_ );
    }

    void PlaySoundfx( Sounds sound )
    {  // Can't use PlaySound because of namespace clash.
      ALuint source = get_sound_source();
      alSourcei( source, AL_BUFFER, sound_ALuints_[sound] );
      alSourcePlay(source);
    }

    // Use for accessing reserved end slot (prevent incidental sounds from overwriting bgm).
    void PlayBGM( Sounds bgm )
    { 
      sources_[NUM_SOUND_SOURCES] = NULL;  // Need this otherwise crashes when setting second sound if more than NUM_SOUND_SOURCES sounds have been played so far.
      ALuint current_bg_source_ = sources_[NUM_SOUND_SOURCES];
      alSourcei( current_bg_source_, AL_BUFFER, sound_ALuints_[bgm] );
      alSourcePlay(current_bg_source_);
    }
  };

}

#endif // !sound_manager_h
