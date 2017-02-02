#include "MusicManager.h"
#include <memory>
#include <vector>
using namespace std;

struct Music::impl
{
    friend class Music;
    shared_ptr<Mix_Music> sMusic;
};
Music::Music()
{
    pimpl.reset(new impl);
}
Music::Music(const char* MusicFileName) : Music()
{
    load(MusicFileName);
}
int Music::load(const char* MusicFileName)
{
    Mix_Music* ptemp=Mix_LoadMUS(MusicFileName);
    if(ptemp==nullptr) return -1;
    pimpl->sMusic.reset(ptemp,Mix_FreeMusic);
    return 0;
}
int Music::unload()
{
    if(pimpl->sMusic.get())
    {
        pimpl->sMusic.reset();
        return 0;
    }
    else return -2;
}
bool Music::ready()
{
    return (pimpl->sMusic.get()!=nullptr);
}
Music::~Music()
{
    unload();
}


struct MusicPlayer::impl
{
    vector<pair<Music,int>> vec;
};
MusicPlayer::MusicPlayer(int freq,Uint16 format,int soundChannel,int chunkSize)
{
    pimpl.reset(new impl);
    if(pInstance) return;
    Mix_OpenAudio(freq,format,soundChannel,chunkSize);
    pInstance=this;
}
MusicPlayer::~MusicPlayer()
{
    if(pInstance) Mix_CloseAudio();
    pInstance=nullptr;
}
int MusicPlayer::play()
{
    return Mix_PlayMusic(pimpl->vec.front().first.pimpl->sMusic.get(),pimpl->vec.front().second);
}
int MusicPlayer::add(Music& music,int times)
{
    if(!music.ready()) return -1;
    pimpl->vec.push_back(make_pair(music,times));
    return 0;
}
MusicPlayer* MusicPlayer::pInstance=nullptr;



struct SoundEffect::impl
{
    friend class SoundEffect;
    shared_ptr<Mix_Chunk> sChunk;
};

SoundEffect::SoundEffect()
{
    pimpl.reset(new impl);
}
