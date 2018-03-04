#pragma once
#include "include.h"
#include <memory>
#include <string>
#include "ErrorViewer.h"
#include "Audio.h"
#include "RWOP.h"
#include "MusicType.h"
#include "__Noncopyable.h"
#include "__Plugin.h"
namespace MiniEngine {
/// Forward Declaration
class Music
{
public:
    Music()=default;
    Music(const std::string& Filename);
    Music(const RWOP& rwop,MusicType musicType);
    bool isReady() const;
    void release();
    MusicType getType() const;
private:
    std::shared_ptr<Mix_Music> _music;
    void _set(Mix_Music*);
    void _clear();
    Mix_Music* _get() const;

    friend class MusicPlayer;
    friend class _internal::Plugin;
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
    int fadeIn(Music music,int loops,int ms);
    int fadeIn(int loops, int ms);
    int fadeOut(int ms);

    bool isPlaying() const;
    bool isPaused() const;
    int isFading() const;
private:
    Music m;
};


} /// End of namespace MiniEngine
