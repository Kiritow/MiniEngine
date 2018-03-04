#include "SDLWrapper/Music.h"
#include "_caster.h"
namespace MiniEngine {
//private
void Music::_set(Mix_Music* p)
{
    _music.reset(p,Mix_FreeMusic);
}
//private
void Music::_clear()
{
    _music.reset();
}
//private
Mix_Music* Music::_get() const
{
    return _music.get();
}

Music::Music(const std::string& Filename)
{
    _set(Mix_LoadMUS(Filename.c_str()));
}

Music::Music(const RWOP& rwop,MusicType musicType)
{
    _set(Mix_LoadMUSType_RW(rwop._get(),_internal::getMixMusicTypeFromMusicType(musicType),0));
}

bool Music::isReady() const
{
    return (_get()!=nullptr);
}

void Music::release()
{
    _clear();
}

MusicType Music::getType() const
{
    return _internal::getMusicTypeFromMixMusicType(Mix_GetMusicType(_get()));
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

int MusicPlayer::fadeIn(Music music, int loops, int ms)
{
    m=music;
    return fadeIn(loops,ms);
}

int MusicPlayer::fadeIn(int loops, int ms)
{
    return Mix_FadeInMusic(m._get(), loops, ms);
}

int MusicPlayer::fadeOut(int ms)
{
    return Mix_FadeOutMusic(ms);
}

bool MusicPlayer::isPlaying() const
{
    return (Mix_PlayingMusic() == 1);
}

bool MusicPlayer::isPaused() const
{
    return (Mix_PausedMusic() == 1);
}

int MusicPlayer::isFading() const
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

int MusicPlayer::setPosition(double second)
{
    return Mix_SetMusicPosition(second);
}

} /// End of namespace MiniEngine
