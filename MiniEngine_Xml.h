#pragma once
#include "rapidxml/rapidxml.hpp"
#include <string>

namespace MiniEngine
{

namespace XML
{

typedef rapidxml::xml_node<> XNode;
typedef rapidxml::xml_attribute<> XAttr;
typedef rapidxml::xml_document<> XDoc;

/// Fwd Decl
class Document;

class Attribute
{
private:
    XAttr* _pattr;
    Document* _pdoc;
    friend class Document;
};

class Node
{
public:
    void push_front(const Node&);
    void push_back(const Node&);
    void insert(const Node& where,const Node& val);

    void remove_first_node();
    void remove_last_node();
    void remove_node(const Node& todelete);
    void remove_all_node();

    void push_front(const Attribute&);
    void push_back(const Attribute&);
    void insert(const Attribute& where,const Attribute& val);

    void remove_first_attr();
    void remove_last_attr();
    void remove_attr(const Attribute& todelete);
    void remove_all_attr();

    bool operator == (const Node& node)
    {
        return _pnode==node._pnode && _pdoc==node._pdoc;
    }

    bool hasPrevNode() const;
    bool hasNextNode() const;
    bool hasParentNode() const;
    Node getPrevNode() const;
    Node getNextNode() const;
    Node getParentNode() const;

private:
    XNode* _pnode;
    Document* _pdoc;
    friend class Document;
};

class Document
{
public:
    Document();
    Document(const std::string& filename);
    int loadFrom(const std::string& filename,bool clearCurrent=true);
    int saveTo(const std::string& filename);
    bool ready();
    Node newNode(const std::string& name,const std::string& value);
    Attribute newAttr(const std::string& name,const std::string& value);
protected:
    char* _allocate_string(const std::string& str);
    char* _allocate_string(const char* pstr,int sz);
private:
    XDoc _doc;
    bool _is_ready;
};



}/// End of namespace MiniEngine::XML

}/// End of namespace MiniEngine

