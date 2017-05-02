CXXFLAGS = -std=c++14 -Wall -O2 -D__C4DROID__ -Iinclude
LDFLAGS =
LDLIBS = -lSDL2_image -lSDL2_net -ltiff -ljpeg -lpng -lz -lSDL2_ttf -lfreetype -lSDL2_mixer -lSDL2_test -lsmpeg2 -lvorbisfile -lvorbis -logg -lstdc++ -lSDL2 -lEGL -lGLESv1_CM -lGLESv2 -landroid -Wl,--no-undefined -shared

PROG = program_name
OBJS = MiniEngine.o MiniEngine_Android.o MiniEngine_Event.o MiniEngine_Widget.o sqlite/sqlite3.o MiniEngine_SQLite.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS) `sdl2-config --cflags --libs`

clean:
	rm -f $(PROG) $(OBJS)