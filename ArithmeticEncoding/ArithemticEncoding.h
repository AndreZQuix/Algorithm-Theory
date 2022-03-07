#pragma once

#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void calculateAlphabet(const string& text, string& alphabet); // ���������� ������� ������

void calculateProbability(const string& text, const string& alphabet, vector<double>& probabilities);	// ���������� ����������� ��������� ������� ������� �������� � ������

vector<segment> defineSegments(const string& str, const vector<double> probabilities);	// ���������� ��������� ���������, ���������� �� ������� ��������

const segment& getSegBySymb(const vector<segment>& segments, char symb); // �������� ��������� �������� �� ��� �������

double arithmeticEncoding(const string& text, const string& alphabet, vector<double>& probabilities);	// ������������ �����

string arithmeticDecoding(const string& alphabet, const vector<double>& probabilities, double code, int textSize);	// ������������ �����