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
public:
    void _set(XAttr*);
    XAttr* _get() const;
    void _clear();
    void _setdoc(Document*);

    Attribute();
    Attribute(XAttr*);

    std::string getName() const;
    std::string getValue() const;

    char* getNameRaw() const;
    char* getValueRaw() const;

    bool hasPrevAttr() const;
    bool hasNextAttr() const;
    Attribute getPrevAttr() const;
    Attribute getNextAttr() const;
    Attribute getPrevAttr(const std::string& name) const;
    Attribute getNextAttr(const std::string& name) const;

private:
    XAttr* _pattr;
    Document* _pdoc;
};

class Node
{
public:
    void _set(XNode*);
    XNode* _get() const;
    void _clear();
    void _setdoc(Document*);

    Node();
    Node(XNode*);

    std::string getName() const;
    std::string getValue() const;

    char* getNameRaw() const;
    char* getValueRaw() const;

    Node& push_front(const Node&);
    Node& push_back(const Node&);
    Node& insert(const Node& where,const Node& val);

    Node& remove_first_node();
    Node& remove_last_node();
    Node& remove_node(const Node& todelete);
    Node& remove_all_node();

    Node& push_front(const Attribute&);
    Node& push_back(const Attribute&);
    Node& insert(const Attribute& where,const Attribute& val);

    Node& remove_first_attr();
    Node& remove_last_attr();
    Node& remove_attr(const Attribute& todelete);
    Node& remove_all_attr();

    bool operator == (const Node& node);

    bool hasPrevNode() const;
    bool hasNextNode() const;
    bool hasParentNode() const;
    Node getPrevNode() const;
    Node getNextNode() const;
    Node getParentNode() const;
    Node getPrevNode(const std::string& name) const;
    Node getNextNode(const std::string& name) const;

    Node getChild() const;
    Node getChild(const std::string& nodename) const;

    bool valid();

private:
    XNode* _pnode;
    Document* _pdoc;
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
    Node cloneNode(const Node&);
    void clear();

protected:
    char* _allocate_string(const std::string& str);
    char* _allocate_string(const char* pstr,int sz);
private:
    XDoc _doc;
    bool _is_ready;
};



}/// End of namespace MiniEngine::XML

}/// End of namespace MiniEngine

