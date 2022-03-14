#include "Huffman.h"
#include <algorithm>

void Huffman::calculateAlphabet()   // alphabet for encoding
{
    alphabet = text;
    sort(alphabet.begin(), alphabet.end());
    alphabet.erase(unique(alphabet.begin(), alphabet.end()), alphabet.end());   // calculate string of unique symbols from text (alphabet) 
}

void Huffman::calculateAlphabet(const std::map<std::string, std::string>& codes)    // alphabet for decoding
{ 
    alphabet.clear();
    for (auto const& symb : codes)  // add to the alphabet values from table of codes
        alphabet.push_back(symb.first[0]);
}

void Huffman::calculateFrequency()
{
    int symbCount = 0;
    for (const auto& alphabetSymb : alphabet)
    {
        symbCount = std::count_if(text.begin(), text.end(), [&alphabetSymb](const auto& textSymb) { return alphabetSymb == textSymb; });    // increase counter, if there is an alphabet symbol in text
        frequency.push_back(symbCount);  // add counter value to frequency vector
        symbCount = 0;
    }
}

void Huffman::fillQueue(queueType& queue)
{
    for (int i = 0; i < alphabet.size(); i++)
    {
        Node::pointer node = std::make_shared<Node>(alphabet[i], frequency[i]); // add to priority queue the nodes with alphabet symbol and it's frequency
        queue.push(node);
    }
}

void Huffman::makeCodeTable(Node::pointer& node, std::string str)
{
    if (node->left != nullptr)  // recursively go through binary tree
        makeCodeTable(node->left, str + '0');   // if left node exist, go left and add 0 to the code string

    if (node->right != nullptr)
        makeCodeTable(node->right, str + '1');  // if right node exist, go right and add 1 to the code string

    if (node->isLeaf()) // if there is a leaf..
    {
        node->setCode(str); // ..set code to the symbol
        codes[node->getName()] = node->getCode();   // add it to the table of codes (symbol and it's code)
    }
}

void Huffman::makeTree(queueType& queue)
{
    while(queue.size() > 1) // until one node remains (root)..
    {
        Node::pointer x = queue.top();  // ..merge lower nodes into the one
        queue.pop();

        Node::pointer y = queue.top();
        queue.pop();

        std::string name;
        name += x->getName();
        name += y->getName();
        Node::pointer z = std::make_shared<Node>(name, x->getFrequency() + y->getFrequency());

        z->left = x;
        z->right = y;

        x->parent = z;
        y->parent = z;

        queue.push(z);
    }
}

void Huffman::encode()
{
    calculateAlphabet();
    calculateFrequency();
    queueType queue;    // priority queue with nodes
    fillQueue(queue);

    makeTree(queue);

    Node::pointer root = queue.top();   // last queue element is a root of binary tree
    makeCodeTable(root, std::string(""));
    printEncodedMessage();
}

void Huffman::printEncodedMessage()
{
    for (const auto& ch : text) // encoding message according to the table of codes
    {
        auto it = codes.find(std::string(1, ch));
        if (it == codes.end()) {
            std::cout << "Error";
            return;
        }
        else
            encodedMessage += it->second;
    }

    std::cout << "Encoded text: \n" << encodedMessage << '\n' << "Encoded text size: " << encodedMessage.length() << '\n';
}

void Huffman::print(queueType& queue)
{
    while (!queue.empty())
    {
        std::cout << *(queue.top());
        queue.pop();
    }
}

void Huffman::makeChar(const Node::pointer& root)
{
    Node::pointer node = root;
    char ch;
    text.clear();
    for (size_t i = 0; i < encodedMessage.size(); i++)  // reversed "encoding algorithm"
    {
        ch = encodedMessage[i]; // get encoded message character
        if (ch == '0')  // if it's 0, go left... 
        {
            if (node->left != nullptr)
            {
                node = node->left;
                if (node->isLeaf()) // if the node is a leaf, add character to the string and return to the root
                {
                    text += node->getSymb();
                    node = root;
                }
            }
        }
        else if (ch == '1') // else if it's 1, go right
        {
            if (node->right != nullptr)
            {
                node = node->right;
                if (node->isLeaf())
                {
                    text += node->getSymb();
                    node = root;
                }
            }
        }
    }
}

void Huffman::decode()
{
    std::cout << "Decoding started..." << std::endl;
    queueType queue;
    calculateAlphabet(codes);   // alphabet calculating, filling queue, binary tree creating
    fillQueue(queue);
    makeTree(queue);

    Node::pointer root = queue.top();
    makeChar(root);

    std::cout << "Original text:\n" << text << std::endl;
}