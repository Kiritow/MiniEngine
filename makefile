CXXFLAGS = -std=c++14 -Wall -D__C4DROID__
LDFLAGS =
LDLIBS = -lSDL2_image -lSDL2_net -ltiff -ljpeg -lpng -lz -lSDL2_ttf -lfreetype -lSDL2_mixer -lSDL2_test -lsmpeg2 -lvorbisfile -lvorbis -logg -lstdc++ -lSDL2 -lEGL -lGLESv1_CM -lGLESv2 -landroid -Wl,--no-undefined -shared

PROG = program_name
OBJS = basic_config.o sdl_engine.o config.o InitManager.o mini_engine.o App.o main.o MiniEngine/Widget.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS) `sdl2-config --cflags --libs`

clean:
	rm -f $(PROG) $(OBJS)
