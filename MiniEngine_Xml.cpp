#include "MiniEngine_Xml.h"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace MiniEngine
{

XMLDB::XMLDB()
{
    _opened=false;
}

XMLDB::~XMLDB()
{
    close();
}

int XMLDB::open(const std::string& XMLFileName,bool saveOnClose)
{
    if(_opened) return -2;

    try
    {
        rapidxml::file<> temp(XMLFileName.c_str());
        _doc.parse<0>(temp.data());
        _rootnode=_doc.first_node("Database");
        _dbinfo=_rootnode->first_node("DBInfo");
        _filename=XMLFileName;
        _opened=true;
        _changed=false;
        _autosave_onclose=saveOnClose;
    }
    catch(...)
    {
        return -1;
    }

    return 0;
}

int XMLDB::create(const std::string& XMLFileName)
{
    if(_opened) return -2;
    _rootnode=_doc.allocate_node(rapidxml::node_element,"Database");
    _dbinfo=_doc.allocate_node(rapidxml::node_element,"DBInfo");
    _tbinfo=_doc.allocate_node(rapidxml::node_element,"TableInfo");
    _tbroot=_doc.allocate_node(rapidxml::node_element,"Tables");

    _dbinfo->append_node(_tbinfo);

    _rootnode->append_node(_dbinfo);
    _rootnode->append_node(_tbroot);

    _doc.append_node(_rootnode);

    _opened=true;
    _changed=true;
    _autosave_onclose=true;
    _filename=XMLFileName;
    return 0;
}

int XMLDB::save()
{
    if(!_opened) return -2;
    if(!_changed) return 0;
    std::ofstream ofs(_filename);
    ofs<<_doc;
    _changed=false;
    return 0;
}

int XMLDB::close()
{
    if(!_opened) return 0;

    if(_autosave_onclose) save();

    _doc.clear();
    _filename.clear();
    _opened=false;
    return 0;
}

bool XMLDB::isopen() const
{
    return _opened;
}

void XMLDB::setChanged()
{
    _changed=true;
}

// protected
bool XMLDB::isNameUnique(const std::string& tbname)
{
    return _tbroot->first_node(tbname.c_str()) == nullptr;
}

int XMLDB::newTB(const std::string& tbname,const std::vector<std::string>& colname)
{
    if(tbname.empty()) return -1;
    if(!isNameUnique(tbname)) return -2;

    char* stra=_doc.allocate_string(tbname.c_str());
    node* newtb=_doc.allocate_node(rapidxml::node_element,stra);

    node* newinfo=_doc.allocate_node(rapidxml::node_element,stra);
    for(size_t i=0;i<colname.size();i++)
    {
        newinfo->append_node(_doc.allocate_node(rapidxml::node_element,"col",_doc.allocate_string(colname.at(i).c_str())));
    }

    _tbinfo->append_node(newinfo);
    _tbroot->append_node(newtb);

    return 0;
}

int XMLDB::dropTB(const std::string& tbname)
{
    node* tbnode=getTBroot(tbname);
    _tbroot->remove_node(tbnode);
    _tbinfo->remove_node(_tbinfo->first_node(tbname.c_str()));
    setChanged();
    return 0;
}

int XMLDB::dropAllTB()
{
    _tbroot->remove_all_nodes();
    _tbinfo->remove_all_nodes();
    setChanged();
    return 0;
}

// protected
XMLDB::node* XMLDB::getTBroot(const std::string& tbname)
{
    return _tbroot->first_node(tbname.c_str());
}

//experimental
rapidxml::xml_document<>* XMLDB::getdoc()
{
    return &_doc;
}


}///End of namespace MiniEngine
