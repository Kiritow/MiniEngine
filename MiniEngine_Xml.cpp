#include "MiniEngine_Xml.h"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include <fstream>

namespace MiniEngine
{

namespace XML
{

void Attribute::_set(XAttr* pattr)
{
    _pattr=pattr;
}

XAttr* Attribute::_get() const
{
    return _pattr;
}

void Attribute::_clear()
{
    _pattr=nullptr;
}

void Attribute::_setdoc(Document* pDoc)
{
    _pdoc=pDoc;
}

std::string Attribute::getName() const
{
    return std::string(getNameRaw());
}

std::string Attribute::getValue() const
{
    return std::string(getValueRaw());
}

char* Attribute::getNameRaw() const
{
    return _pattr->name();
}

char* Attribute::getValueRaw() const
{
    return _pattr->value();
}


Node::Node()
{
    _pnode=nullptr;
    _pdoc=nullptr;
}

Node::Node(XNode* expNode)
{
    _pnode=expNode;
    _pdoc=nullptr;
}



void Node::_set(XNode* node)
{
    _pnode=node;
}

XNode* Node::_get() const
{
    return _pnode;
}

void Node::_clear()
{
    _pnode=nullptr;
}

void Node::_setdoc(Document* pDoc)
{
    _pdoc=pDoc;
}

std::string Node::getName() const
{
    return std::string(getNameRaw());
}

std::string Node::getValue() const
{
    return std::string(getValueRaw());
}

char* Node::getNameRaw() const
{
    return _pnode->name();
}

char* Node::getValueRaw() const
{
    return _pnode->value();
}


void Node::push_front(const Node& node)
{
    _pnode->prepend_node(node._pnode);
}

void Node::push_back(const Node& node)
{
    _pnode->append_node(node._pnode);
}

void Node::insert(const Node& where, const Node& val)
{
    _pnode->insert_node(where._pnode,val._pnode);
}

void Node::remove_first_node()
{
    _pnode->remove_first_node();
}

void Node::remove_last_node()
{
    _pnode->remove_last_node();
}

void Node::remove_node(const Node& todelete)
{
    _pnode->remove_node(todelete._pnode);
}

void Node::remove_all_node()
{
    _pnode->remove_all_nodes();
}

void Node::push_front(const Attribute& attr)
{
    _pnode->prepend_attribute(attr._get());
}

void Node::push_back(const Attribute& attr)
{
    _pnode->append_attribute(attr._get());
}

void Node::insert(const Attribute& where, const Attribute& val)
{
    _pnode->insert_attribute(where._get(),val._get());
}

void Node::remove_first_attr()
{
    _pnode->remove_first_attribute();
}

void Node::remove_last_attr()
{
    _pnode->remove_last_attribute();
}

void Node::remove_attr(const Attribute& todelete)
{
    _pnode->remove_attribute(todelete._get());
}

void Node::remove_all_attr()
{
    _pnode->remove_all_attributes();
}

bool Node::operator==(const Node& node)
{
    return _pnode==node._pnode && _pdoc==node._pdoc;
}

bool Node::hasPrevNode() const
{
    return _pnode->previous_sibling()!=nullptr;
}

bool Node::hasNextNode() const
{
    return _pnode->next_sibling()!=nullptr;
}

bool Node::hasParentNode() const
{
    return _pnode->parent()!=nullptr;
}

Node Node::getPrevNode() const
{
    return Node(_pnode->previous_sibling());
}

Node Node::getNextNode() const
{
    return Node(_pnode->next_sibling());
}

Node Node::getParentNode() const
{
    return Node(_pnode->parent());
}

bool Node::valid()
{
    return _pnode!=nullptr && _pdoc!=nullptr;
}


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
    node._set(_doc.allocate_node(rapidxml::node_type::node_element,
                       _allocate_string(name),_allocate_string(value)));
    node._setdoc(this);
    return node;
}

Attribute Document::newAttr(const std::string& name,const std::string& value)
{
    Attribute attr;
    attr._set(_doc.allocate_attribute(_allocate_string(name),_allocate_string(value)));
    attr._setdoc(this);
    return attr;
}

Node Document::cloneNode(const Node& node)
{
    return Node(_doc.clone_node(node._get()));
}

void Document::clear()
{
    return _doc.clear();
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
