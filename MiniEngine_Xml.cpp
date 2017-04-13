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

int XMLDB::open(const std::string& XMLFileName)
{
    if(_opened) return -2;

    try
    {
        rapidxml::file<> temp(XMLFileName.c_str());
        _doc.parse<0>(temp.data());
        _filename=XMLFileName;
        _opened=true;
        _changed=false;
    }
    catch(...)
    {
        return -1;
    }

    return 0;
}

int XMLDB::save()
{
    if(!_opened) return -2;
    if(!_changed) return 0;
    std::ofstream ofs(_filename);
    ofs<<_doc;
    return 0;
}

int XMLDB::close()
{
    if(!_opened) return 0;

    _doc.clear();
    _filename.clear();
    _opened=false;
    return 0;
}

bool XMLDB::isopen() const
{
    return _opened;
}

rapidxml::xml_node<>* XMLDB::getDBroot(const std::string& dbname)
{
    for(rapidxml::xml_node<>* root=_doc.first_node("Database");root!=nullptr;root=root->next_sibling("Database"))
    {
        rapidxml::xml_attribute<>* attr=root->first_attribute("name");
        if(attr==nullptr||dbname!=attr->value()) continue;
        else return root;
    }

    return nullptr;
}

rapidxml::xml_document<>* XMLDB::getdoc()
{
    return &_doc;
}

XMLController::XMLController()
{
    _pdb=nullptr;
}

XMLController::~XMLController()
{

}

int XMLController::connect(const XMLDB& xmldb)
{
    if(!xmldb.isopen())
    {
        return -1;
    }
    _pdb=const_cast<XMLDB*>(&xmldb);
    return 0;
}

int XMLController::usedb(const std::string& dbname)
{
    rapidxml::xml_node<>* node=_pdb->getDBroot(dbname);
    if(node==nullptr)
    {
        return -1;
    }
    else
    {
        _dbroot=node;
        return 0;
    }
    return 0;
}

int XMLController::createdb(const std::string& dbname,std::vector<std::string>& colname)
{
    if(dbname.empty()) return -1;

    if(_pdb->getDBroot(dbname)!=nullptr)
    {
        /// Invalid Name (Same as another database).
        return -2;
    }
    rapidxml::xml_node<>* newnode=_pdb->getdoc()->allocate_node(rapidxml::node_element,"Database");
    if(newnode==nullptr) return -3;
    rapidxml::xml_attribute<>* newattr=_pdb->getdoc()->allocate_attribute("name",dbname.c_str());
    if(newattr==nullptr)
    {
        /// FIXME: What will happen to newnode?
        return -4;
    }
    rapidxml::xml_node<>* dbinfonode=_pdb->getdoc()->allocate_node(rapidxml::node_element,"DBInfo");
    if(dbinfonode==nullptr) return -5;
    for(size_t i=0;i<colname.size();i++)
    {
        rapidxml::xml_node<>* ntnode=_pdb->getdoc()->allocate_node(rapidxml::node_element,"colname",colname.at(i).c_str());
        if(ntnode==nullptr) return -6;
        dbinfonode->append_node(ntnode);
    }
    newnode->append_node(dbinfonode);
    newnode->append_attribute(newattr);

    return 0;
}

}///End of namespace MiniEngine
