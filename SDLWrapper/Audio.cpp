#include "Audio.h"
namespace MiniEngine
{
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

AudioPlayer::_Audio* AudioPlayer::_sysAudio = nullptr;
int AudioPlayer::_sysAudioCounter = 0;
} /// End of namespace MiniEngine 

