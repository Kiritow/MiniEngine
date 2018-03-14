echo "MiniEngine-ToolCenter Integration Script"

echo "TC_ROOT is "
echo $TC_ROOT

if [ 0"$TC_ROOT" = "0" ]; then
    echo "ToolCenter not detected. Please set TC_ROOT"
    exit 
fi

mkdir include
cp -r $TC_ROOT/SDL2/include/* include/
cp -r $TC_ROOT/SDL2_image/include/* include/
cp -r $TC_ROOT/SDL2_mixer/include/* include/
cp -r $TC_ROOT/SDL2_ttf/include/* include/
mkdir lib
cp -r $TC_ROOT/SDL2/lib/* lib/
cp -r $TC_ROOT/SDL2_image/lib/* lib/
cp -r $TC_ROOT/SDL2_mixer/lib/* lib/
cp -r $TC_ROOT/SDL2_ttf/lib/* lib/

cp -r $TC_ROOT/SDL2/dll/x86/* .
cp -r $TC_ROOT/SDL2_image/dll/x86/* .
cp -r $TC_ROOT/SDL2_mixer/dll/x86/* .
cp -r $TC_ROOT/SDL2_ttf/dll/x86/* .

echo "Paste in Visual Studio:"
echo "======================"
echo "SDL2test.lib"
echo "SDL2main.lib"
echo "SDL2.lib"
echo "SDL2_image.lib"
echo "SDL2_mixer.lib"
echo "SDL2_ttf.lib"
echo "======================"
echo "Finish."