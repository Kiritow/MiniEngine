# MiniEngine  

A C++ Mini Engine. Based on SDL2.  

[English Version](README_en_US.md)

C++编写的SDL2引擎.  
 
### 编译说明

Windows: 推荐使用VS2017. 将项目克隆后加入解决方案, 注意删除`makefile_c4gen.cpp`以及`test`文件夹.

Linux: 请使用Codeblocks 17.12(推荐)载入所有文件.接下来Codeblocks会完成其余的工作.需要手动删除`makefile_c4gen.cpp`并排除`test`文件夹.

C4droid: 编译并运行`makefile_c4gen.cpp`,将生成一份makefile. 修改编译目标为SDL2 Application. 修改程序名称为program_name(也可以修改makefile为其他名称)

Windows,Linux需要以下依赖库:

> SDL2 (SDL2.lib, SDL2main.lib, SDL2test.lib)  
> SDL2 Image (SDL2_image.lib)  
> SDL2 Mixer (SDL2_mixer.lib)  
> SDL2 TTF (SDL2_ttf.lib)  

C4droid需要保证已经安装以下应用:
 
> C4droid本体  
> GCC Plugin For C4droid  
> SDL2 Plugin For C4droid  

Linux Codeblocks PPA 参见: [Code::Blocks Release Builds](https://launchpad.net/~damien-moore/+archive/ubuntu/codeblocks-stable)

### 下载链接

[SDL2官网下载](http://www.libsdl.org/download-2.0.php)

[SDL2_image下载地址](https://www.libsdl.org/projects/SDL_image/)

[SDL2_mixer下载地址](https://www.libsdl.org/projects/SDL_mixer/)

[SDL2_ttf下载地址](https://www.libsdl.org/projects/SDL_ttf/)

[在GooglePlay上下载C4droid](https://play.google.com/store/apps/details?id=com.n0n3m4.droidc&hl=en "付费+需要科学上网")

[C4droid百度贴吧](http://tieba.baidu.com/f?kw=c4droid "虽然自从吧主换届之后大不如前...")
