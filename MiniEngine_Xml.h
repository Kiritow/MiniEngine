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

    std::string getName() const;
    std::string getValue() const;

    char* getNameRaw() const;
    char* getValueRaw() const;
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

    bool operator == (const Node& node);

    bool hasPrevNode() const;
    bool hasNextNode() const;
    bool hasParentNode() const;
    Node getPrevNode() const;
    Node getNextNode() const;
    Node getParentNode() const;

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

