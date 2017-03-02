#pragma once
#include "config.h"
#include <memory>

#define _MUSIC_MANAGER_IMPL \
    struct impl; \
    std::shared_ptr<impl> pimpl;

/// Fwd Decl
class MusicPlayer;

class Music
{
public:
    Music();
    Music(const char* MusicFileName);
    int load(const char* MusicFileName);
    int unload();
    bool ready();
    ~Music();
private:
    _MUSIC_MANAGER_IMPL

    friend class MusicPlayer;
};

class SoundEffect
{
public:
    SoundEffect();
    SoundEffect(const char* SoundEffectFileName);
    int load(const char* SoundEffectFileName);
    ~SoundEffect();
private:
    _MUSIC_MANAGER_IMPL
};

class MusicPlayer
{
public:
    MusicPlayer(int freq=MIX_DEFAULT_FREQUENCY,Uint16 format=MIX_DEFAULT_FORMAT,int soundChannel=2,int chunkSize=1024);
    ~MusicPlayer();
    int play();
    int stop();
    int add(Music& music,int times);
private:
    static MusicPlayer* pInstance;
    _MUSIC_MANAGER_IMPL
};

class SoundEffectPlayer
{
public:
    SoundEffectPlayer();
    ~SoundEffectPlayer();
    int play(SoundEffect& soundeffect,int times);
private:
    _MUSIC_MANAGER_IMPL;
};

