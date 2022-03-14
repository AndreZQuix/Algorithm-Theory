#pragma once
#include "Node.h"
#include <vector>
#include <queue>
#include <map>

class LowestPriority
{
public:
	bool operator()(const Node::pointer& left, const Node::pointer& right) const
	{
		return left->getFrequency() > right->getFrequency();
	}
};

class Huffman
{
	using queueType = std::priority_queue<Node::pointer, std::vector<Node::pointer>, LowestPriority>;

	std::string text{ "" };
	std::string alphabet{ "" };
	std::vector<int> frequency;
	std::map<std::string, std::string> codes;
	std::string encodedMessage{ "" };

	void calculateAlphabet();	// calculate alphabet for encoding
	void calculateAlphabet(const std::map<std::string, std::string>& codes);	// calculate alphabet for decoding
	void calculateFrequency();	// calculate symbols frequencies in the text
	void fillQueue(queueType& queue);	// fill priority queue with binary tree nodes
	void makeCodeTable(Node::pointer& node, std::string str); // create code table ( symbol and it's binary code)
	void makeTree(queueType& queue);	// create binary tree of symbols
	void printEncodedMessage();	// print encoded message
	void print(queueType& queue); // for debugging: print queue values
	void makeChar(const Node::pointer& root);	// decode symbol in encoded message

public:
	Huffman() = default;
	Huffman(std::string text) : text(text) {}	// constructor for encoding process
	Huffman(const std::vector<int>& frequency, const std::map<std::string, std::string>& codes, const std::string message)
		: frequency(frequency), codes(codes), encodedMessage(message) {}	// constructor for decoding process

	std::string getOriginalText() const { return text; }
	std::string getEncodedText() const { return encodedMessage; }
	std::map<std::string, std::string> getCodes() const { return codes; }	// get table of codes
	std::string getAlphabet() const { return alphabet; }
	std::vector<int> getFrequency() const { return frequency; }

	void setOriginalText(const std::string& text) { this->text = text; } // set original text for encoding
	void setEncodedText(std::string& text) { this->encodedMessage = text; }	// set encoded message for decoding
	void setOriginalCodes(const std::map<std::string, std::string>& codes) { this->codes = codes; }	// set table of codes

	void encode();	// encoding process
	void decode();  // decoding process
};

