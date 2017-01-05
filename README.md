# MiniEngine  

A C++ Mini Engine. Based on SDL2.  

C++编写的SDL2引擎.  

### 编译说明
Windows: 请使用Codeblocks 16.01(推荐)载入所有.cpp文件.接下来Codeblocks会完成其余的工作.  
> 依赖库
> SDL2 (SDL2.lib, SDL2main.lib, SDL2test.lib)
> SDL2 Image (SDL2_image.lib)
> SDL2 Mixer (SDL2_mixer.lib)
> SDL2 TTF (SDL2_ttf.lib)

C4droid: 长按编译键选择编译模式为Makefile. 选择编译目标为SDL2 Application. 修改程序名称为program_name(此处与makefile对应即可)
> 依赖库
> C4droid本体
> GCC Plugin For C4droid
> SDL2 Plugin For C4droid

### 类说明
class Window 窗口类  
class Renderer 渲染器类  
class Texture 纹理类  
class Surface 图层类  
class Font 字体渲染类  
class RGBA 颜色包  
class Rect 矩形类  

