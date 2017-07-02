/// makefile_c4gen.cpp
/// Under MIT License. Part of MiniEngine Project.
/// You can run this code to generate a makefile for c4droid.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

char buff[1024];
int main()
{
    printf("#Detecting source files...#\n");
	// Set up environment
	sprintf(buff,"PATH=%s:/data/data/com.n0n3m4.droidc/bzbox/",getenv("PATH"));
	putenv(buff);
	fflush(stdout);
	system("find . -name \"*.cpp\" > cpplist.txt ");
	system("find . -name \"*.c\" > clist.txt ");
	vector<string> dvec;
	FILE* fp=fopen("cpplist.txt","r");
	memset(buff,0,1024);
	while(fgets(buff,1024,fp)!=nullptr)
	{
		int len=strlen(buff);
		if(buff[len-1]=='\n') buff[len-1]=0;
		char* p=strstr(buff,".cpp");
		sprintf(p,".o");
		string d(buff);
		dvec.push_back(d);
		memset(buff,0,1024);
	}
	fclose(fp);
	vector<string> ccd;
	fp=fopen("clist.txt","r");
	memset(buff,0,1024);
	while(fgets(buff,1024,fp)!=nullptr)
	{
		int len=strlen(buff);
		if(buff[len-1]=='\n') buff[len-1]=0;
		char* p=strstr(buff,".c");
		sprintf(p,".o");
		string d(buff);
		dvec.push_back(d);
		memset(buff,0,1024);
	}
	fclose(fp);
	
	printf("#Excluding files...#\n");
	auto iter=find_if(dvec.begin(),dvec.end(),[](const string& str){ return (str.find("makefile_c4gen")!=string::npos);});
	if(iter!=dvec.end()) dvec.erase(iter);

	printf("#Generating makefile...#\n");
	fp=fopen("makefile","w");
	fprintf(fp,
"CXXFLAGS = -std=c++14 -Wall -O2 -D__C4DROID__ -Iinclude\n"
"LDFLAGS =\n"
"LDLIBS = -lSDL2_image -lSDL2_net -ltiff -ljpeg -lpng -lz -lSDL2_ttf -lfreetype -lSDL2_mixer "
"-lSDL2_test -lsmpeg2 -lvorbisfile -lvorbis -logg -lstdc++ -lSDL2 -lEGL -lGLESv1_CM -lGLESv2 "
"-landroid -Wl,--no-undefined -shared\n"
"PROG = program_name\n"
"OBJS = ");
    for(auto& d:dvec)
    {
        fprintf(fp,"%s ",d.c_str());
    }
    fprintf(fp,"\n");
    fprintf(fp,
"all: $(PROG)\n"
"\n"
"$(PROG): $(OBJS)\n"
"\t$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS) `sdl2-config --cflags --libs`\n"
"\n"
"clean:\n"
"\trm -f $(PROG) $(OBJS))\n");
    fclose(fp);

	printf("#End of Generation#\n");
	return 0;
}
