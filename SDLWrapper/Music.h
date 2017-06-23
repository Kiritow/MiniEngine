#pragma once
#include "include.h"
#include <memory>
#include <string>
#include "ErrorViewer.h"
#include "Audio.h"
#include "__Noncopyable.h"
#include "begin_code.h"
/// Forward Declaration
class Music
{
public:
    Music()=default;
    Music(const std::string& Filename);
    bool isReady() const;
    void release();
private:
    std::shared_ptr<Mix_Music> _music;
    void _set(Mix_Music*);
    void _clear();
    Mix_Music* _get() const;
    friend class MusicPlayer;
};

class MusicPlayer : public AudioPlayer, public NonCopyable
{
public:
    static int GetDecoderNum();
    static std::string GetDecoderName(int index);

    /// Play Music. Loop: -1:Infinite, >0:Exact that time.
    int play(Music music, int loops);
    void pause();
    void resume();
    void rewind();
    int setPosition(double second);
    int stop();
    int fadeIn(int loops, int ms);
    int fadeOut(int ms);

    bool isPlaying() const;
    bool isPaused() const;
    int isFading() const;
private:
    Music m;
};


#include "end_code.h"
