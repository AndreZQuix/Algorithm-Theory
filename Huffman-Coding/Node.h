#pragma once

#include <memory>
#include <string>
#include <iostream>

class Node
{
    friend class Huffman;
    char symb{ 0 };
    int frequency{ 0 };
    std::string code{ "" };
    std::string name{ "" };

    std::string getCode() const { return code; }

    void setCode(const std::string& code) { this->code = code; }
    bool isLeaf() const { return (left == nullptr && right == nullptr); }   // the node is leaf, if there are no left and right nodes

public:
    Node() = default;
    Node(char symb, int frequency) : symb(symb), frequency(frequency) {}
    Node(const std::string & name, int frequency) : name(name), frequency(frequency) {}

    using pointer = std::shared_ptr<Node>;
    pointer left{ nullptr };    // left node
    pointer right{ nullptr };   // right node
    pointer parent{ nullptr };  // parent node

    char getSymb() const { return symb; }
    int getFrequency() const { return frequency; }
    std::string getName() const
    {
        if (symb == 0)
            return name;
        else
            return std::string(1, symb);
    }
};

inline bool operator<(const Node::pointer& left, const Node::pointer& right)
{
    return left->getFrequency() < right->getFrequency();
}

inline std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out << '[' << node.getName() << "] = " << node.getFrequency() << ' ';
    return out;
}