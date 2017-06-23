#include "Music.h"
#include "begin_code.h"
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

Music MusicPlayer::loadMusic(const std::string& Filename) throw(ErrorViewer)
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

#include "end_code.h"
