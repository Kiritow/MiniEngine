# Rect 矩形类
## 成员  
| 类型 | 名称 | 描述 |
| --- | --- | --- |
| int | x | 矩形左上角的x坐标 |
| int | y | 矩形左上角的y坐标 |
| int | w | 矩形宽度 |
| int | h | 矩形高度 |
## 方法
Rect()  
>
默认构造.长宽为0且坐标为(0,0)   

Rect(int X,int Y,int W,int H)  
>
指定参数的构造  

SDL_Rect Rect::toSDLRect()  
>
将当前矩形转换为SDL_Rect表示
