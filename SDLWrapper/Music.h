#pragma once
#include "include.h"
#include "begin_code.h"
class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();
private:
    class _Audio
    {
    public:
        _Audio();
        ~_Audio();
    };

    static _Audio* _sysAudio;
    static int _sysAudioCounter;
};

/// Forward Declaration
class Music
{
public:

protected:
    Music() = default;
private:
    std::shared_ptr<Mix_Music> _music;
    void _set(Mix_Music*);
    void _clear();
    Mix_Music* _get();
    friend class MusicPlayer;
};

class MusicPlayer : public AudioPlayer
{
public:
    static int GetDecoderNum();
    static std::string GetDecoderName(int index);

    Music loadMusic(std::string Filename) throw (ErrorViewer);

    int play(Music music, int loops);
    void pause();
    void resume();
    void rewind();
    int stop();
    int fadeIn(int loops, int ms);
    int fadeOut(int ms);

    bool isPlaying();
    bool isPaused();
    int isFading();

    /// Experimental
    static int SetMusicPosition(double position);

private:
    Music m;
};

class Sound
{
public:
protected:
    Sound() = default;
private:
    std::shared_ptr<Mix_Chunk> _sound;
    void _set(Mix_Chunk*);
    void _clear();
    Mix_Chunk* _get();
    friend class SoundPlayer;
};

typedef int ChannelID;

class SoundPlayer : public AudioPlayer
{
public:
    static int GetDecoderNum();
    static std::string GetDecoderName(int index);

    SoundPlayer(int Channels = 16);
    Sound loadSound(std::string Filename) throw (ErrorViewer);
    ChannelID playSound(Sound sound, int loops) throw (ErrorViewer);
    ChannelID fadein(Sound sound, int loops, int ms) throw (ErrorViewer);
    int fadeout(ChannelID id, int ms);
    void pause(ChannelID id);
    void resume(ChannelID id);
    int stop(ChannelID id);

    /// Experimental
    int setPanning(ChannelID id,uint8_t left,uint8_t right);
    int setPosition(ChannelID id,int16_t angle,uint8_t distance);
    int setDistance(ChannelID id,uint8_t distance);
    int setReverseStereo(ChannelID id,int flip);

    /// Experimental: Direct Add/Remove Effect
    int addEffect(ChannelID id,Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);
    int removeEffect(ChannelID id,Mix_EffectFunc_t f);
    int removeAllEffect(ChannelID id);
};
#include "end_code.h"
