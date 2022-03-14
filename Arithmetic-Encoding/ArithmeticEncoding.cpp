#include <iostream>
#include "ArithemticEncoding.h"

vector<segment> defineSegments(const string& str, const vector<double> probabilities)
{
    double l = 0;
    const int m = str.size();
    vector<segment> segments;
    segments.resize(m);
    for (int i = 0; i < m; i++) // calculate segments intervals
    {
        segments[i].symb = str[i];
        segments[i].left = l;
        segments[i].right = l + probabilities[i];
        l = segments[i].right;

        cout << "Symbol: " << segments[i].symb << " Left: " << segments[i].left << " Right: " << segments[i].right << '\n';
    }
    return segments;
}

const segment& getSegBySymb(const vector<segment>& segments, char symb) // find segment intervals by symbol
{
    for (const auto& seg : segments)
    {
        if (seg.symb == symb)
            return seg;
    }
}

double arithmeticEncoding(const string& text, const string& alphabet, vector<double>& probabilities)
{
    vector<segment> segments = defineSegments(alphabet, probabilities);
    double left = 0;
    double right = 1;
    char symb;
    double newRight, newLeft;
    cout << "--------------------" << endl;
    for (int i = 0; i < text.size(); i++)   // encoding process
    {
        symb = text[i];
        cout << "Symbol: " << symb << " Segment.left: " << getSegBySymb(segments, symb).left << " Segments.right: " << getSegBySymb(segments, symb).right << '\n';

        newRight = left + (right - left) * getSegBySymb(segments, symb).right;
        newLeft = left + (right - left) * getSegBySymb(segments, symb).left;
        cout << "Left: " << left << " Right: " << right << '\n';
        cout << "New left: " << newLeft << " New right: " << newRight << '\n';
        cout << "--------------------" << endl;
        left = newLeft;
        right = newRight;
    }
    return (left + right) / 2;
}

string arithmeticDecoding(const string& alphabet, const vector<double>& probabilities, double code, int textSize)   // decoding process
{
    vector<segment> segments = defineSegments(alphabet, probabilities);
    string text;
    for (int i = 0; i < textSize; i++) // string decoding
    {
        for (int j = 0; j < alphabet.size(); j++)   // searching segments, whose intervals include code
            if (code >= segments[j].left && code < segments[j].right && text.size() < textSize) // if code is in interval and didn't cross the textSize border...
            {
                text += segments[j].symb;   // ...add symbol to string...
                code = (code - segments[j].left) / (segments[j].right - segments[j].left);  // ...calculate new code value for next searching iteration
    }
    return text;
}

void calculateAlphabet(const string& text, string& alphabet)    // calculate encoding alphabet
{
    alphabet = text;
    sort(alphabet.begin(), alphabet.end());
    alphabet.erase(unique(alphabet.begin(), alphabet.end()), alphabet.end());   // getting unique symbols set in the string (i.e alphabet)
}

void calculateProbability(const string& text, const string& alphabet, vector<double>& probabilities)    // caluclate probabilities of symbol occurrence in the text
{
    double symbCount = 0;
    double size = text.size();
    for (const auto& alphabetSymb : alphabet)
    {
        for (const auto& textSymb : text)   // if alphabet character is in the text, increase counter value
        {
            if (alphabetSymb == textSymb)
                symbCount++;
        }
        probabilities.push_back(symbCount / size);  // add probabillity value
        symbCount = 0;
    }
}

int main()
{
    cout << "Enter the string you want to encode: " << endl;
    string text;
    getline(cin, text);
    cout << "Text size: " << text.size();
    string alphabet;
    vector<double> probabilities;

    calculateAlphabet(text, alphabet);
    calculateProbability(text, alphabet, probabilities);

    double code;
    code = arithmeticEncoding(text, alphabet, probabilities);
    cout << "Encoded string: " << code << '\n';

    cout << "Decoded string: " << arithmeticDecoding(alphabet, probabilities, code, text.size()) << endl;
}
