#include "Sound.h"
namespace MiniEngine
{
//private
void Sound::_set(Mix_Chunk* p)
{
    _sound.reset(p,Mix_FreeChunk);
}
//private
void Sound::_clear()
{
    _sound.reset();
}
//private
Mix_Chunk* Sound::_get() const
{
    return _sound.get();
}

Sound::Sound(const std::string& WAVFilename)
{
    _set(Mix_LoadWAV(WAVFilename.c_str()));
}

Sound::Sound(const RWOP& rwop)
{
    _set(Mix_LoadWAV_RW(rwop._get(),0));
}

bool Sound::isReady() const
{
    return (_get()!=nullptr);
}

void Sound::release()
{
    _clear();
}

//private
void Channel::_set(int ChannelID)
{
    _id=ChannelID;
}
//private
int Channel::_get() const
{
    return _id;
}
//private
void Channel::_clear()
{
    _id=-1;
}
//protected
Channel::Channel()
{
    _id=-1;
}

Channel& Channel::playSound(Sound sound, int loops)
{
    int cret=Mix_PlayChannel(_get(),sound._get(),loops);
    if(cret==-1)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    _set(cret);
    return *this;
}

Channel& Channel::fadeIn(Sound sound, int loops, int ms)
{
    int cret=Mix_FadeInChannel(_get(),sound._get(),loops,ms);
    if(cret==-1)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    _set(cret);
    return *this;
}


int Channel::fadeOut(int ms)
{
    return Mix_FadeOutChannel(_get(), ms);
}

void Channel::pause()
{
    Mix_Pause(_get());
}

void Channel::resume()
{
    Mix_Resume(_get());
}

int Channel::stop()
{
    return Mix_HaltChannel(_get());
}

int Channel::setPanning(uint8_t left, uint8_t right)
{
    return Mix_SetPanning(_get(),left,right);
}

int Channel::setPosition(int16_t angle, uint8_t distance)
{
    return Mix_SetPosition(_get(),angle,distance);
}

int Channel::setDistance(uint8_t distance)
{
    return Mix_SetDistance(_get(),distance);
}

int Channel::setReverseStereo(int flip)
{
    return Mix_SetReverseStereo(_get(),flip);
}

int Channel::addEffect(Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg)
{
    return Mix_RegisterEffect(_get(),f,d,arg);
}

int Channel::removeEffect(Mix_EffectFunc_t f)
{
    return Mix_UnregisterEffect(_get(),f);
}

int Channel::removeAllEffect()
{
    return Mix_UnregisterAllEffects(_get());
}


//static
int SoundPlayer::GetDecoderNum()
{
    return Mix_GetNumChunkDecoders();
}

//static
std::string SoundPlayer::GetDecoderName(int index)
{
    return std::string(Mix_GetChunkDecoder(index));
}

SoundPlayer::SoundPlayer(int Channels)
{
    Mix_AllocateChannels(Channels);
}

Channel SoundPlayer::playSound(Sound sound, int loops)
{
    Channel c;
    c.playSound(sound,loops);
    return c;
}

Channel SoundPlayer::fadeIn(Sound sound, int loops, int ms)
{
    Channel c;
    c.fadeIn(sound,loops,ms);
    return c;
}
} /// End of namespace MiniEngine 

