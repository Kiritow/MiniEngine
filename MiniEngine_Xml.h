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
    typedef rapidxml::xml_node<> node;
    typedef rapidxml::xml_attribute<> attribute;
    typedef rapidxml::xml_document<> document;

    XMLDB();
    ~XMLDB();
    int open(const std::string& XMLFileName,bool saveOnClose);
    int create(const std::string& XMLFileName);
    int save();
    int close();
    bool isopen() const;

    /// Create a table with colname, returns table node
    int newTB(const std::string& tbname,const std::vector<std::string>& colname);
    /// Insert a col before a index (use -1 for push back)
    int newTBCol(const std::string& tbname,const std::string& newColname,int insertBefore);

    int newTBRow(const std::string& tbname,const std::vector<std::string>& rowval,int insertBefore);

    int setTBRow(const std::string& tbname,int rowid,const std::vector<std::string>& rowval);

    int getTBRow(const std::string& tbname,int rowid,std::vector<std::string>& out_rowval);

    int dropTBRow(const std::string& tbname,int rowid);

    /// Get Table Col Size
    int getTBColsize(const std::string& tbname);
    /// Drop a table
    int dropTB(const std::string& tbname);
    /// Drop all table
    int dropAllTB();

    /// Directly Get Document (Experimental)
    document* getdoc();
    /// Announce something changed
    void setChanged();
protected:
    bool isNameUnique(const std::string& tbname);
    /// Get root node of a table
    node* getTBroot(const std::string& tbname);
private:
    document _doc;

    node* _rootnode;

    node* _dbinfo;
    node* _tbinfo;

    node* _tbroot;

    bool _changed;
    bool _opened;
    bool _autosave_onclose;
    std::string _filename;
};


}/// End of namespace MiniEngine
