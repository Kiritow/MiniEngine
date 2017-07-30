#pragma once
#include "include.h"
namespace MiniEngine
{
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
} /// End of namespace MiniEngine 

