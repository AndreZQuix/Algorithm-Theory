#pragma once

#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void calculateAlphabet(const string& text, string& alphabet); // рассчитать алфавит текста

void calculateProbability(const string& text, const string& alphabet, vector<double>& probabilities);	// рассчитать вероятность появления каждого символа алфавита в тексте

vector<segment> defineSegments(const string& str, const vector<double> probabilities);	// рассчитать интервалы сегментов, отвечающих за символы алфавита

const segment& getSegBySymb(const vector<segment>& segments, char symb); // получить интервалы сегмента по его символу

double arithmeticEncoding(const string& text, const string& alphabet, vector<double>& probabilities);	// закодировать текст

string arithmeticDecoding(const string& alphabet, const vector<double>& probabilities, double code, int textSize);	// декодировать текст