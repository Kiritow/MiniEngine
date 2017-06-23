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
#include "end_code.h"
