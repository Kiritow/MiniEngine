#include "Music.h"
#include "begin_code.h"
AudioPlayer::AudioPlayer()
{
    if (!_sysAudioCounter)
    {
        _sysAudio = new _Audio;
    }
    ++_sysAudioCounter;
}

AudioPlayer::~AudioPlayer()
{
    --_sysAudioCounter;
    if (!_sysAudioCounter)
    {
        delete _sysAudio;
    }
}

AudioPlayer::_Audio::_Audio()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}

AudioPlayer::_Audio::~_Audio()
{
    Mix_CloseAudio();
}

void Music::_set(Mix_Music* p)//private
{
    _music.reset(p,Mix_FreeMusic);
}

void Music::_clear()//private
{
    _music.reset();
}

Mix_Music* Music::_get()//private
{
    return _music.get();
}

//static
int MusicPlayer::GetDecoderNum()
{
    return Mix_GetNumMusicDecoders();
}

//static
std::string MusicPlayer::GetDecoderName(int index)
{
    return std::string(Mix_GetMusicDecoder(index));
}

Music MusicPlayer::loadMusic(std::string Filename) throw(ErrorViewer)
{
    Mix_Music* temp = Mix_LoadMUS(Filename.c_str());
    if (temp == nullptr)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    Music m;
    m._set(temp);
    return m;
}

int MusicPlayer::play(Music music, int loops)
{
    m = music;
    return Mix_PlayMusic(m._get(), loops);
}

void MusicPlayer::pause()
{
    Mix_PauseMusic();
}

void MusicPlayer::resume()
{
    Mix_ResumeMusic();
}

void MusicPlayer::rewind()
{
    Mix_RewindMusic();
}

int MusicPlayer::stop()
{
    return Mix_HaltMusic();
}

int MusicPlayer::fadeIn(int loops, int ms)
{
    return Mix_FadeInMusic(m._get(), loops, ms);
}

int MusicPlayer::fadeOut(int ms)
{
    return Mix_FadeOutMusic(ms);
}

bool MusicPlayer::isPlaying()
{
    return (Mix_PlayingMusic() == 1);
}

bool MusicPlayer::isPaused()
{
    return (Mix_PausedMusic() == 1);
}

int MusicPlayer::isFading()
{
    switch (Mix_FadingMusic())
    {
    case MIX_NO_FADING:
        return 0;
    case MIX_FADING_IN:
        return 1;
    case MIX_FADING_OUT:
        return 2;
    default:
        return -1;
    }
}

//static
int MusicPlayer::SetMusicPosition(double position)
{
    return Mix_SetMusicPosition(position);
}

void Sound::_set(Mix_Chunk* p)
{
    _sound.reset(p,Mix_FreeChunk);
}

void Sound::_clear()//private
{
    _sound.reset();
}

Mix_Chunk* Sound::_get()
{
    return _sound.get();
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

Sound SoundPlayer::loadSound(std::string Filename) throw(ErrorViewer)
{
    Mix_Chunk* temp = Mix_LoadWAV(Filename.c_str());
    if (temp == NULL)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    Sound s;
    s._set(temp);
    return s;
}

ChannelID SoundPlayer::playSound(Sound sound, int loops) throw(ErrorViewer)
{
    ChannelID id;
    if (-1 == (id = Mix_PlayChannel(-1, sound._get(), loops)))
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    return id;
}

ChannelID SoundPlayer::fadein(Sound sound, int loops, int ms) throw(ErrorViewer)
{
    ChannelID id;
    if (-1 == (id = Mix_FadeInChannel(-1, sound._get(), loops, ms)))
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    return id;
}

int SoundPlayer::fadeout(ChannelID id, int ms)
{
    return Mix_FadeOutChannel(id, ms);
}

void SoundPlayer::pause(ChannelID id)
{
    Mix_Pause(id);
}

void SoundPlayer::resume(ChannelID id)
{
    Mix_Resume(id);
}

int SoundPlayer::stop(ChannelID id)
{
    return Mix_HaltChannel(id);
}

int SoundPlayer::setPanning(ChannelID id, uint8_t left, uint8_t right)
{
    return Mix_SetPanning(id,left,right);
}

int SoundPlayer::setPosition(ChannelID id, int16_t angle, uint8_t distance)
{
    return Mix_SetPosition(id,angle,distance);
}

int SoundPlayer::setDistance(ChannelID id, uint8_t distance)
{
    return Mix_SetDistance(id,distance);
}

int SoundPlayer::setReverseStereo(ChannelID id, int flip)
{
    return Mix_SetReverseStereo(id,flip);
}

int SoundPlayer::addEffect(ChannelID id,Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg)
{
    return Mix_RegisterEffect(id,f,d,arg);
}

int SoundPlayer::removeEffect(ChannelID id,Mix_EffectFunc_t f)
{
    return Mix_UnregisterEffect(id,f);
}

int SoundPlayer::removeAllEffect(ChannelID id)
{
    return Mix_UnregisterAllEffects(id);
}


AudioPlayer::_Audio* AudioPlayer::_sysAudio = nullptr;
int AudioPlayer::_sysAudioCounter = 0;
#include "end_code.h"
