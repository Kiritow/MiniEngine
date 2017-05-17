#include "MiniEngine_Xml.h"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <fstream>

namespace MiniEngine
{

namespace XML
{

Document::Document()
{
    _is_ready=false;
}

Document::Document(const std::string& filename)
{
    if(loadFrom(filename,false)!=0)
    {
        _is_ready=false;
    }
    else
    {
        _is_ready=true;
    }
}

int Document::loadFrom(const std::string& filename, bool clearCurrent)
{
    std::ifstream ifs(filename);
    if(!ifs)
    {
        /// File Read Error.
        return -1;
    }
    rapidxml::file<> infilereader(ifs);
    if(clearCurrent)
    {
        _doc.clear();
    }
    _doc.parse<0>(infilereader.data());
    return 0;
}

int Document::saveTo(const std::string& filename)
{
    std::string ans;
    rapidxml::print(std::back_inserter(ans),_doc,0);

    std::ofstream ofs(filename);

    if(!ofs) return -1;

    ofs<<ans;

    return 0;
}

bool Document::ready()
{
    return _is_ready;
}

Node Document::newNode(const std::string& name,const std::string& value)
{
    Node node;
    node._pnode=_doc.allocate_node(rapidxml::node_type::node_element,
                       _allocate_string(name),_allocate_string(value));
    node._pdoc=this;
    return node;
}

Attribute Document::newAttr(const std::string& name,const std::string& value)
{
    Attribute attr;
    attr._pattr=_doc.allocate_attribute(_allocate_string(name),_allocate_string(value));
    attr._pdoc=this;
    return attr;
}

//protected
char* Document::_allocate_string(const std::string& str)
{
    return _doc.allocate_string(str.c_str(),str.size());
}

//protected
char* Document::_allocate_string(const char* source,int sz)
{
    return _doc.allocate_string(source,sz);
}


}/// End of namespace MiniEngine::XML

}/// End of namespace MiniEngine
