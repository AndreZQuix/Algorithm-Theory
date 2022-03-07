#include <iostream>
#include "ArithemticEncoding.h"

struct segment
{
    char symb;  // символ
    double left;    // левый интервал
    double right;   // правый интервал
};

vector<segment> defineSegments(const string& str, const vector<double> probabilities)
{
    double l = 0;
    const int m = str.size();
    vector<segment> segments;
    segments.resize(m);
    for (int i = 0; i < m; i++) // проводится расчет сегментов
    {
        segments[i].symb = str[i];
        segments[i].left = l;
        segments[i].right = l + probabilities[i];
        l = segments[i].right;

        cout << "Symbol: " << segments[i].symb << " Left: " << segments[i].left << " Right: " << segments[i].right << '\n';
    }
    return segments;
}

const segment& getSegBySymb(const vector<segment>& segments, char symb) // найти интервалы сегмента по символу, которому принадлежит интервал
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
    for (int i = 0; i < text.size(); i++)   // процесс кодировки
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

string arithmeticDecoding(const string& alphabet, const vector<double>& probabilities, double code, int textSize)   // декодирование строки
{
    vector<segment> segments = defineSegments(alphabet, probabilities); // расчет сегментов
    string text;
    for (int i = 0; i < textSize; i++) // декодируем символы, граница - номер последнего символа закодированного текста
    {
        for (int j = 0; j < alphabet.size(); j++)   // ищем сегменты, в интервалы которых входит код
            if (code >= segments[j].left && code < segments[j].right && text.size() < textSize) // если код попал в интервал и при этом не перешел границу...
            {
                text += segments[j].symb;   // ...добавить символ в строку...
                code = (code - segments[j].left) / (segments[j].right - segments[j].left);  // ...расчитать новое значение кода для поиска в сегментах
            }
    }
    return text;
}

void calculateAlphabet(const string& text, string& alphabet)    // рассчитать алфавит для закодирования
{
    alphabet = text;
    sort(alphabet.begin(), alphabet.end());
    alphabet.erase(unique(alphabet.begin(), alphabet.end()), alphabet.end());   // получаем уникальный набор символов в введенной строке (т.е. алфавит)
}

void calculateProbability(const string& text, const string& alphabet, vector<double>& probabilities)    // рассчитать вероятности появления символов алфавита в тексте
{
    double symbCount = 0;
    double size = text.size();
    for (const auto& alphabetSymb : alphabet)
    {
        for (const auto& textSymb : text)   // увеличиваем счетчик каждый раз, когда символ алфавита встречается в тексте
        {
            if (alphabetSymb == textSymb)
                symbCount++;
        }
        probabilities.push_back(symbCount / size);  // в вектор вероятностей добавляем посчитанную вероятность, обнуляем счетчик, переходим к следующему символу алфавита
        symbCount = 0;
    }
}

int main()
{
    cout << "Enter the string you want to encode: " << endl; // введите строку для закодирования
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