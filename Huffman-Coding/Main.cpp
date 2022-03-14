#include "Huffman.h"

int main()
{
    std::cout << "Enter the string you want to encode: " << std::endl;
    std::string text;
    std::getline(std::cin, text);

    Huffman process1 = Huffman(text);
    std::cout << "\n----Encoding started----\n\n";
    process1.encode();
    process1.decode();

    std::cout << "\n----Decoding other text----\n\n";
    std::map<std::string, std::string> codes;
    codes["e"] = "00";
    codes["h"] = "01";
    codes["o"] = "10";
    codes["l"] = "11";
    Huffman process2 = Huffman(std::vector<int>{1, 1, 2, 1}, codes, "0100111110"); // input: frequencies, symbols table, encoded message
    process2.decode();
}
