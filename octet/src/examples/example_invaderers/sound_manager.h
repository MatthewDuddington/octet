/*
Octet Framework (c) Andy Thomason 2012-2014.

Extracted sound related elements from invaiderers_app.

Additional and modified code (c) Matthew Duddington 2016.
*/

#ifndef sound_manager_h
#define sound_manager_h

namespace octet {

  class SoundManager {

  public: enum Sounds {
      SFX_WRONG,

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

    ALuint wrong;
    unsigned current_source;
    ALuint sources[NUM_SOUND_SOURCES];
    ALuint get_sound_source() { return sources[current_source++ % NUM_SOUND_SOURCES]; }


  public:
    SoundManager() {
      sound_manager_ (this);
      GameSound().Init();
    }

    static SoundManager& GameSound() {
      return *sound_manager_();
    }

    void Init() {
      // Add sound files to resource dictionary.
      wrong = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/invaderers/bang.wav");

      current_source = 0;
      alGenSources(NUM_SOUND_SOURCES, sources);
    }

    void PlaySoundx(Sounds sound) {
      ALuint source = get_sound_source();
      alSourcei(source, AL_BUFFER, sound);
      alSourcePlay(source);
    }
  };

}

#endif // !sound_manager_h
