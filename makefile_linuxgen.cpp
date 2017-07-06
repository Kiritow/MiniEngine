/// makefile_linuxgen.cpp
/// Under MIT License. Part of MiniEngine Project.
/// You can run this code to generate a makefile for linux.

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
    /// Find files
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

	printf("#Generating makefile...#\n");
	fp=fopen("makefile","w");
	fprintf(fp,
"CXXFLAGS = -std=c++14 -Wall -O2 -D__LINUX__ -Iinclude\n"
"LDFLAGS = \n"
"LDLIBS = -lstdc++ -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer -lSDL2_test -lSDL2 -shared\n"
"\n"
"PROG = your_project\n"
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
