#pragma once

#include <string>
#include <algorithm>
#include <vector>

using namespace std;

struct segment
{
    char symb;  // symbol
    double left;    // left interval
    double right;   // right interval
};

void calculateAlphabet(const string& text, string& alphabet); // calculate text alphabet

void calculateProbability(const string& text, const string& alphabet, vector<double>& probabilities);	// calculate the probabillity of symbol occurrence in the text

vector<segment> defineSegments(const string& str, const vector<double> probabilities);	// calculate the segments intervals

const segment& getSegBySymb(const vector<segment>& segments, char symb); // get segment intervals by the symbol it owned

double arithmeticEncoding(const string& text, const string& alphabet, vector<double>& probabilities);	// encode text

string arithmeticDecoding(const string& alphabet, const vector<double>& probabilities, double code, int textSize);	// decode text
