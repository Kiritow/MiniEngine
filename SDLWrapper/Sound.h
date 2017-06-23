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
