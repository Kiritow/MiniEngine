# MiniEngine  

A C++ Mini Engine. Based on SDL2.  

C++编写的SDL2引擎.  

**可能存在的错误**: 由于Event和Widget体系尚未构建完全，使用时可能出现问题(包括未解决的编译错误)。若只使用MiniEngine主体则不会出现问题。  
**重要提示**: master分支为稳定分支,dev分支为开发分支.  

### 编译说明

Windows: 请使用Codeblocks 16.01(推荐)载入所有.cpp文件.接下来Codeblocks会完成其余的工作.  
> 依赖库  
> SDL2 (SDL2.lib, SDL2main.lib, SDL2test.lib)  
> SDL2 Image (SDL2_image.lib)  
> SDL2 Mixer (SDL2_mixer.lib)  
> SDL2 TTF (SDL2_ttf.lib)  

Windows-Visual Studio: 使用VS编译本项目可能会出现某些错误,目前还没有很好的解决办法.  

C4droid: 长按编译键选择编译模式为Makefile. 选择编译目标为SDL2 Application. 修改程序名称为program_name(此处与makefile对应即可)
> 依赖库  
> C4droid本体  
> GCC Plugin For C4droid  
> SDL2 Plugin For C4droid  

[前往SDL2官网下载最新版本](http://www.libsdl.org/download-2.0.php)  
[C4droid on GooglePlay](https://play.google.com/store/apps/details?id=com.n0n3m4.droidc&hl=en)  

##### 依赖库下载地址
[SDL2_image下载地址](https://www.libsdl.org/projects/SDL_image/)  
[SDL2_mixer下载地址](https://www.libsdl.org/projects/SDL_mixer/)  
[SDL2_ttf下载地址](https://www.libsdl.org/projects/SDL_ttf/)  
