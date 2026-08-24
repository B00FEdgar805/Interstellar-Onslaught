//
//  AudioManager.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 8/24/26.
//

#include "AudioManager.hpp"

AudioManager::AudioManager() = default;

AudioManager::~AudioManager()
{
    shutdown();
}

bool AudioManager::init(int sfxTrackCount, const SDL_AudioSpec* requestedSpec)
{
    if (INIT)
    {
        return true;
    }

    if (sfxTrackCount <= 0)
    {
        sfxTrackCount = 1;
    }

    if (!MIX_Init())
    {
        SDL_Log("MIX_Init failed: %s", SDL_GetError());
        return false;
    }

    STARTED = true;

    MIXER = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, requestedSpec);

    if (!MIXER)
    {
        SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
        shutdown();
        return false;
    }

    MUSIC_TRACK = MIX_CreateTrack(MIXER);

    if (!MUSIC_TRACK)
    {
        SDL_Log("Failed to create music track: %s", SDL_GetError());
        shutdown();
        return false;
    }

    if (!MIX_TagTrack(MUSIC_TRACK, "music"))
    {
        SDL_Log("Failed to tag music track: %s", SDL_GetError());
        shutdown();
        return false;
    }

    sfxTracks_.reserve(static_cast<std::size_t>(sfxTrackCount));
    sfxTrackLocalGains_.reserve(static_cast<std::size_t>(sfxTrackCount));

    for (int i = 0; i < sfxTrackCount; ++i)
    {
        MIX_Track* track = MIX_CreateTrack(MIXER);

        if (!track)
        {
            SDL_Log("Failed to create SFX track: %s", SDL_GetError());
            shutdown();
            return false;
        }

        if (!MIX_TagTrack(track, "sfx"))
        {
            SDL_Log("Failed to tag SFX track: %s", SDL_GetError());
            MIX_DestroyTrack(track);
            shutdown();
            return false;
        }

        sfxTracks_.push_back(track);
        sfxTrackLocalGains_.push_back(1.0f);
    }

    INIT = true;
    return true;
}

void AudioManager::shutdown()
{
    if (MIXER)
    {
        MIX_StopAllTracks(MIXER, 0);
    }

    for (MIX_Track* track : sfxTracks_)
    {
        MIX_DestroyTrack(track);
    }

    sfxTracks_.clear();
    sfxTrackLocalGains_.clear();

    if (MUSIC_TRACK)
    {
        MIX_DestroyTrack(MUSIC_TRACK);
        MUSIC_TRACK = nullptr;
    }

    for (auto& pair : SOUNDS)
    {
        MIX_DestroyAudio(pair.second);
    }

    SOUNDS.clear();

    for (auto& pair : MUSIC)
    {
        MIX_DestroyAudio(pair.second);
    }

    MUSIC.clear();

    if (MIXER)
    {
        MIX_DestroyMixer(MIXER);
        MIXER = nullptr;
    }

    if (STARTED)
    {
        MIX_Quit();
        STARTED = false;
    }

    INIT = false;
    nextSfxTrack_ = 0;

    MASTER_VOLUME = 1.0f;
    MUSIC_VOLUME = 1.0f;
    SFX_VOLUME = 1.0f;
}

bool AudioManager::loadSound(const std::string& id, const std::string& path, bool predecode)
{
    return loadAudioInto(SOUNDS, id, path, predecode);
}

bool AudioManager::loadMusic(const std::string& id, const std::string& path, bool predecode)
{
    return loadAudioInto(MUSIC, id, path, predecode);
}

void AudioManager::unloadSound(const std::string& id)
{
    unloadAudioFrom(SOUNDS, id);
}

void AudioManager::unloadMusic(const std::string& id)
{
    unloadAudioFrom(MUSIC, id);
}

int AudioManager::playSound(const std::string& id, float gain, int loops, int fadeInMs, bool allowSteal)
{
    MIX_Audio* audio = findAudio(SOUNDS, id, "sound");

    if (!audio)
    {
        return -1;
    }

    int trackIndex = -1;
    MIX_Track* track = chooseSfxTrack(allowSteal, trackIndex);

    if (!track)
    {
        SDL_Log("No available SFX track for sound: %s", id.c_str());
        return -1;
    }

    if (!MIX_SetTrackAudio(track, audio)) {
        SDL_Log("MIX_SetTrackAudio failed: %s", SDL_GetError());
        return -1;
    }

    const std::size_t index = static_cast<std::size_t>(trackIndex);
    sfxTrackLocalGains_[index] = cleanGain(gain);

    if (!MIX_SetTrackGain(track, SFX_VOLUME * sfxTrackLocalGains_[index]))
    {
        SDL_Log("MIX_SetTrackGain failed: %s", SDL_GetError());
        return -1;
    }

    if (!playTrackWithOptions(track, loops, fadeInMs))
    {
        return -1;
    }

    return trackIndex;
}

bool AudioManager::playSoundFireAndForget(const std::string& id)
{
    MIX_Audio* audio = findAudio(SOUNDS, id, "sound");

    if (!audio)
    {
        return false;
    }

    if (!MIX_PlayAudio(MIXER, audio))
    {
        SDL_Log("MIX_PlayAudio failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::playMusic(const std::string& id, bool loop, int fadeInMs)
{
    MIX_Audio* audio = findAudio(MUSIC, id, "music");

    if (!audio) {
        return false;
    }

    if (MIX_TrackPlaying(MUSIC_TRACK) || MIX_TrackPaused(MUSIC_TRACK)) {
        MIX_StopTrack(MUSIC_TRACK, 0);
    }

    if (!MIX_SetTrackAudio(MUSIC_TRACK, audio))
    {
        SDL_Log("MIX_SetTrackAudio for music failed: %s", SDL_GetError());
        return false;
    }

    if (!MIX_SetTrackGain(MUSIC_TRACK, MUSIC_VOLUME))
    {
        SDL_Log("MIX_SetTrackGain for music failed: %s", SDL_GetError());
        return false;
    }

    const int loops = loop ? -1 : 0;
    return playTrackWithOptions(MUSIC_TRACK, loops, fadeInMs);
}

bool AudioManager::stopMusic(int fadeOutMs)
{
    return stopTrackWithMilliseconds(MUSIC_TRACK, fadeOutMs);
}

bool AudioManager::stopSoundTrack(int trackIndex, int fadeOutMs)
{
    if (trackIndex < 0 || static_cast<std::size_t>(trackIndex) >= sfxTracks_.size())
    {
        SDL_Log("Invalid SFX track index: %d", trackIndex);
        return false;
    }

    return stopTrackWithMilliseconds(
        sfxTracks_[static_cast<std::size_t>(trackIndex)],
        fadeOutMs
    );
}

bool AudioManager::stopAllSounds(int fadeOutMs)
{
    if (!MIXER)
    {
        return false;
    }

    if (fadeOutMs < 0)
    {
        fadeOutMs = 0;
    }

    if (!MIX_StopTag(MIXER, "sfx", static_cast<Sint64>(fadeOutMs)))
    {
        SDL_Log("MIX_StopTag for SFX failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::stopAll(int fadeOutMs)
{
    if (!MIXER)
    {
        return false;
    }

    if (fadeOutMs < 0)
    {
        fadeOutMs = 0;
    }

    if (!MIX_StopAllTracks(MIXER, static_cast<Sint64>(fadeOutMs)))
    {
        SDL_Log("MIX_StopAllTracks failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::pauseMusic()
{
    return MUSIC_TRACK && MIX_PauseTrack(MUSIC_TRACK);
}

bool AudioManager::resumeMusic()
{
    return MUSIC_TRACK && MIX_ResumeTrack(MUSIC_TRACK);
}

bool AudioManager::pauseAllSounds()
{
    return MIXER && MIX_PauseTag(MIXER, "sfx");
}

bool AudioManager::resumeAllSounds()
{
    return MIXER && MIX_ResumeTag(MIXER, "sfx");
}

bool AudioManager::pauseAll()
{
    return MIXER && MIX_PauseAllTracks(MIXER);
}

bool AudioManager::resumeAll()
{
    return MIXER && MIX_ResumeAllTracks(MIXER);
}

bool AudioManager::setMasterVolume(float gain)
{
    MASTER_VOLUME = cleanGain(gain);

    if (!MIXER)
    {
        return false;
    }

    if (!MIX_SetMixerGain(MIXER, MASTER_VOLUME))
    {
        SDL_Log("MIX_SetMixerGain failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::setMusicVolume(float gain)
{
    MUSIC_VOLUME = cleanGain(gain);

    if (!MUSIC_TRACK)
    {
        return false;
    }

    if (!MIX_SetTrackGain(MUSIC_TRACK, MUSIC_VOLUME))
    {
        SDL_Log("MIX_SetTrackGain for music failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::setSoundVolume(float gain)
{
    SFX_VOLUME = cleanGain(gain);

    bool ok = true;

    for (std::size_t i = 0; i < sfxTracks_.size(); ++i)
    {
        const float finalGain = SFX_VOLUME * sfxTrackLocalGains_[i];

        if (!MIX_SetTrackGain(sfxTracks_[i], finalGain))
        {
            SDL_Log("MIX_SetTrackGain for SFX failed: %s", SDL_GetError());
            ok = false;
        }
    }

    return ok;
}

bool AudioManager::isMusicPlaying() const
{
    return MUSIC_TRACK && MIX_TrackPlaying(MUSIC_TRACK);
}

bool AudioManager::isMusicPaused() const
{
    return MUSIC_TRACK && MIX_TrackPaused(MUSIC_TRACK);
}

float AudioManager::cleanGain(float gain)
{
    return gain < 0.0f ? 0.0f : gain;
}

bool AudioManager::loadAudioInto(AudioMap& table, const std::string& id, const std::string& path, bool predecode)
{
    if (!MIXER)
    {
        SDL_Log("AudioManager is not initialized");
        return false;
    }

    unloadAudioFrom(table, id);

    MIX_Audio* audio = MIX_LoadAudio(MIXER, path.c_str(), predecode);

    if (!audio)
    {
        SDL_Log("MIX_LoadAudio failed for '%s': %s", path.c_str(), SDL_GetError());
        return false;
    }

    table[id] = audio;
    return true;
}

void AudioManager::unloadAudioFrom(AudioMap& table, const std::string& id)
{
    auto it = table.find(id);

    if (it == table.end())
    {
        return;
    }

    MIX_DestroyAudio(it -> second);
    table.erase(it);
}

MIX_Audio* AudioManager::findAudio(AudioMap& table, const std::string& id, const char* kind)
{
    auto it = table.find(id);

    if (it == table.end())
    {
        SDL_Log("AudioManager could not find %s id: %s", kind, id.c_str());
        return nullptr;
    }

    return it->second;
}

MIX_Track* AudioManager::chooseSfxTrack(bool allowSteal, int& outIndex)
{
    outIndex = -1;

    if (sfxTracks_.empty())
    {
        return nullptr;
    }

    for (std::size_t checked = 0; checked < sfxTracks_.size(); ++checked)
    {
        const std::size_t index = (nextSfxTrack_ + checked) % sfxTracks_.size();
        MIX_Track* track = sfxTracks_[index];

        if (!MIX_TrackPlaying(track) && !MIX_TrackPaused(track))
        {
            nextSfxTrack_ = (index + 1) % sfxTracks_.size();
            outIndex = static_cast<int>(index);
            return track;
        }
    }

    if (!allowSteal)
    {
        return nullptr;
    }

    const std::size_t index = nextSfxTrack_;
    nextSfxTrack_ = (nextSfxTrack_ + 1) % sfxTracks_.size();

    MIX_Track* track = sfxTracks_[index];
    MIX_StopTrack(track, 0);

    outIndex = static_cast<int>(index);
    return track;
}

bool AudioManager::playTrackWithOptions(MIX_Track* track, int loops, int fadeInMs)
{
    if (!track)
    {
        return false;
    }

    SDL_PropertiesID options = 0;

    if (loops != 0 || fadeInMs > 0)
    {
        options = SDL_CreateProperties();

        if (!options)
        {
            SDL_Log("SDL_CreateProperties failed: %s", SDL_GetError());
            return false;
        }

        if (loops != 0)
        {
            if (!SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, static_cast<Sint64>(loops)))
            {
                SDL_Log("Failed to set loop property: %s", SDL_GetError());
                SDL_DestroyProperties(options);
                return false;
            }
        }

        if (fadeInMs > 0)
        {
            if (!SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, static_cast<Sint64>(fadeInMs)))
            {
                SDL_Log("Failed to set fade-in property: %s", SDL_GetError());
                SDL_DestroyProperties(options);
                return false;
            }
        }
    }

    const bool ok = MIX_PlayTrack(track, options);

    if (options)
    {
        SDL_DestroyProperties(options);
    }

    if (!ok)
    {
        SDL_Log("MIX_PlayTrack failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::stopTrackWithMilliseconds(MIX_Track* track, int fadeOutMs)
{
    if (!track)
    {
        return false;
    }

    Sint64 fadeFrames = 0;

    if (fadeOutMs > 0)
    {
        fadeFrames = MIX_TrackMSToFrames(track, static_cast<Sint64>(fadeOutMs));

        if (fadeFrames < 0)
        {
            fadeFrames = 0;
        }
    }

    if (!MIX_StopTrack(track, fadeFrames))
    {
        SDL_Log("MIX_StopTrack failed: %s", SDL_GetError());
        return false;
    }

    return true;
}
