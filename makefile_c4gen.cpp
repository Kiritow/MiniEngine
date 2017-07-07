/// makefile_c4gen.cpp
/// Under MIT License. Part of MiniEngine Project.
/// You can run this code to generate a makefile for C4droid.

#include <string>
#include <functional>
/// Declaration
void FindFileRev(const std::string& dirname,const std::function<void(const std::string&)>& func);

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

char buff[1024];
int main()
{
    printf("Generator: Detecting source files...\n");
    /// Find files
	vector<string> objvec;
	const string toFindCpp=".cpp";
	const string toFindC=".c";
	const string toAppendObj=".o";
	auto replaceEnd=[](const string& source,const string& replaceFrom,const string& replaceTo)->string
	{
        return source.substr(0,source.size()-replaceFrom.size()).append(replaceTo);
	};
	auto endWith=[](const string& text,const string& testEndWith)->bool
	{
        return (text.substr(text.size()-testEndWith.size())==testEndWith);
	};
    FindFileRev(".",[&](const std::string& name)
                {
                    if(endWith(name,toFindCpp))
                    {
                        objvec.push_back(replaceEnd(name,toFindCpp,toAppendObj));
                    }
                    else if(endWith(name,toFindC))
                    {
                        objvec.push_back(replaceEnd(name,toFindC,toAppendObj));
                    }
                });
    printf("Generator: Excluding files...\n");
    objvec.erase(remove_if(objvec.begin(),objvec.end(),[](const string& objname)->bool
                      {
                          return ( /// Beginning of excluding list
                                  (objname.find("makefile_")!=string::npos && objname.find("gen.")!=string::npos) ||
                                  (objname.find("_Windows.")!=string::npos)
                                  );
                      }),objvec.end());
	printf("Generator: Generating makefile...\n");
	FILE* fp=fopen("makefile","w");
	fprintf(fp,
"CFLAGS = -Wall -s -O2 -D__LINUX__ -Iinclude -fPIC\n"
"CXXFLAGS = -std=c++14 -Wall -s -O2 -D__C4DROID__ -Iinclude\n"
"LDFLAGS =\n"
"LDLIBS = -lSDL2_image -lSDL2_net -ltiff -ljpeg -lpng -lz -lSDL2_ttf -lfreetype -lSDL2_mixer "
"-lSDL2_test -lsmpeg2 -lvorbisfile -lvorbis -logg -lstdc++ -lSDL2 -lEGL -lGLESv1_CM -lGLESv2 "
"-landroid -Wl,--no-undefined -shared\n"
"PROG = program_name\n"
"OBJS = ");
    for(auto& obj:objvec)
    {
        fprintf(fp,"%s ",obj.c_str());
    }
    fprintf(fp,"\n");
    fprintf(fp,
"all: $(PROG)\n"
"\n"
"$(PROG): $(OBJS)\n"
"\t$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS) `sdl2-config --cflags --libs`\n"
"\n"
"clean:\n"
"\trm -f $(PROG) $(OBJS)\n");
    fclose(fp);

	printf("Generator: Generation Finished.\n");
	return 0;
}

/// Implement
#if defined(_MSC_VER) || defined(_WIN32) /// VS or Windows
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
void FindFileRev(const std::string& dirname,const std::function<void(const std::string&)>& func)
{
    std::string patternString;
    if(dirname[dirname.size()-1]!='\\')
    {
        patternString=dirname+"\\*";
    }
    else
    {
        patternString=dirname+"*";
    }

    WIN32_FIND_DATA fnd;
    HANDLE hand=FindFirstFile(patternString.c_str(),&fnd);
    if(hand!=INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string fullname=dirname+fnd.cFileName;
            if(fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                fullname.append("\\");
                FindFileRev(fullname,func);
            }
            else
            {
                func(fullname);
            }
        }
        while(FindNextFile(hand,&fnd));
        FindClose(hand);
    }
}
#else /// Linux-like
#include <dirent.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void FindFileRev(const std::string& dirname,const std::function<void(const std::string&)>& func)
{
    DIR* Dir = NULL;
    struct dirent* file = NULL;
    std::string curDir;
    if (dirname[dirname.size()-1] != '/')
    {
        curDir=dirname+"/";
    }
    else
    {
        curDir=dirname;
    }

    if ((Dir = opendir(curDir.c_str())) == NULL)
    {
        return ;
    }
    while ((file = readdir(Dir)) != nullptr)
    {
        if (file->d_type == DT_REG)
        {
            func(curDir + file->d_name);
        }
        else if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0)
        {
            FindFileRev(curDir + file->d_name,func);
        }
    }
    closedir(Dir);
}
#endif
