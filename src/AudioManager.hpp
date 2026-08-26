//
//  AudioManager.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 8/24/26.
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class AudioManager
{
public:
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
    static AudioManager& getInstance()
    {
        static AudioManager instance;
        return instance;
    }
    
   

    bool init(int sfxTrackCount = 16, const SDL_AudioSpec* requestedSpec = nullptr);
    void shutdown();
    bool loadSound(const std::string& id, const std::string& path, bool predecode = true);
    bool loadMusic(const std::string& id, const std::string& path, bool predecode = false);
    void unloadSound(const std::string& id);
    void unloadMusic(const std::string& id);

    // Returns the SFX track index used, or -1 on failure.
    int playSound(const std::string& id, float gain = 1.0f, int loops = 0, int fadeInMs = 0, bool allowSteal = true);

    // Convenient one-shot playback.
    // You cannot stop or modify this individual playback afterward.
    bool playSoundFireAndForget(const std::string& id);

    bool playMusic(const std::string& id, bool loop = true, int fadeInMs = 0);

    bool stopMusic(int fadeOutMs = 0);
    bool stopSoundTrack(int trackIndex, int fadeOutMs = 0);
    bool stopAllSounds(int fadeOutMs = 0);
    bool stopAll(int fadeOutMs = 0);
    bool pauseMusic();
    bool resumeMusic();
    bool pauseAllSounds();
    bool resumeAllSounds();
    bool pauseAll();
    bool resumeAll();
    bool setMasterVolume(float gain);
    bool setMusicVolume(float gain);
    bool setSoundVolume(float gain);
    bool isMusicPlaying() const;
    bool isMusicPaused() const;

private:
    
    AudioManager();
    ~AudioManager();
    
    using AudioMap = std::unordered_map<std::string, MIX_Audio*>;
    static float cleanGain(float gain);
    bool loadAudioInto(AudioMap& table, const std::string& id, const std::string& path, bool predecode);
    void unloadAudioFrom(AudioMap& table, const std::string& id);
    MIX_Audio* findAudio(AudioMap& table, const std::string& id, const char* kind);
    MIX_Track* chooseSfxTrack(bool allowSteal, int& outIndex);
    bool playTrackWithOptions(MIX_Track* track, int loops, int fadeInMs);
    bool stopTrackWithMilliseconds(MIX_Track* track, int fadeOutMs);
    bool INIT = false;
    bool STARTED = false;
    
    
    MIX_Mixer* MIXER = nullptr;
    MIX_Track* MUSIC_TRACK = nullptr;
    std::vector<MIX_Track*> sfxTracks_;
    std::vector<float> sfxTrackLocalGains_;
    AudioMap SOUNDS;
    AudioMap MUSIC;
    std::size_t nextSfxTrack_ = 0;
    float MASTER_VOLUME = 1.0f;
    float MUSIC_VOLUME = 1.0f;
    float SFX_VOLUME = 1.0f;
};


#endif /* AudioManager_hpp */
