#pragma once
#include "MiniEngine.h"
#include <vector>
#include <string>

#include "rapidxml/rapidxml.hpp"

/** MiniEngine XML-Engine
*   MiniEngine XML-Engine is based on rapidXML.
*/

namespace MiniEngine
{

class XMLDB : public MiniEngine::NonCopyable
{
public:
    XMLDB();
    ~XMLDB();
    int open(const std::string& XMLFileName);
    int save();
    int close();
    bool isopen() const;

    rapidxml::xml_node<>* getDBroot(const std::string& dbname);

    /// Directly Get Document
    rapidxml::xml_document<>* getdoc();
private:
    rapidxml::xml_document<> _doc;
    bool _changed;
    bool _opened;
    std::string _filename;
};

class XMLController
{
public:
    XMLController();
    ~XMLController();
    int connect(const XMLDB& xmldb);
    int usedb(const std::string& dbname);
    int createdb(const std::string& dbname,std::vector<std::string>& colname);
    int dropdb(const std::string& dbname);
    int close();

    int execute(const std::string& command);
    int execute(const std::string& command,std::vector<std::string>& vec);
private:
    XMLDB* _pdb;
    std::string _dbname;
    rapidxml::xml_node<>* _dbroot;
};

}/// End of namespace MiniEngine
