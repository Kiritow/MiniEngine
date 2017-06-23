#pragma once
#include "include.h"
#include <memory>
#include <string>
#include "ErrorViewer.h"
#include "Audio.h"
#include "begin_code.h"
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

    Music loadMusic(const std::string& Filename) throw (ErrorViewer);

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


#include "end_code.h"
