#pragma once
#include "include.h"
#include <memory>
#include <string>
#include "Audio.h"
#include "ErrorViewer.h"
#include "begin_code.h"
class Sound
{
public:
    Sound() = default;
    Sound(const std::string& WAVFilename);
    bool isReady() const;
    void release();
private:
    std::shared_ptr<Mix_Chunk> _sound;
    void _set(Mix_Chunk*);
    void _clear();
    Mix_Chunk* _get() const;

    friend class Channel;
};

class Channel
{
public:
    Channel& playSound(Sound sound,int loops) throw (ErrorViewer);
    Channel& fadeIn(Sound sound,int loops,int ms) throw (ErrorViewer);

    int fadeOut(int ms);
    void pause();
    void resume();
    int stop();

    /// Experimental
    int setPanning(uint8_t left,uint8_t right);
    int setPosition(int16_t angle,uint8_t distance);
    int setDistance(uint8_t distance);
    int setReverseStereo(int flip);

    /// Experimental: Direct Add/Remove Effect
    int addEffect(Mix_EffectFunc_t f, Mix_EffectDone_t d, void *arg);
    int removeEffect(Mix_EffectFunc_t f);
    int removeAllEffect();
protected:
    Channel();
private:
    void _set(int ChannelID);
    int _get() const;
    void _clear();

    int _id;
    friend class SoundPlayer;
};

class SoundPlayer : public AudioPlayer
{
public:
    static int GetDecoderNum();
    static std::string GetDecoderName(int index);

    SoundPlayer(int NumChannels = 16);
    Channel playSound(Sound sound, int loops) throw (ErrorViewer);
    Channel fadeIn(Sound sound, int loops, int ms) throw (ErrorViewer);
};
#include "end_code.h"
