#ifndef MARKDOWN_H
#define MARKDOWN_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

enum TextLine {
    TOTAL_LINE = 4,
    EASY_LINE = 6,
    MEDIUM_LINE = 7,
    HARD_LINE = 8,
    START_LINE = 12
};

enum Difficulty {
    Easy,
    Medium,
    Hard
};

class MarkDown {
public:
    MarkDown(string _path);
    bool read();
    bool save();
    void tidy();
    size_t size();
    string& operator[](size_t n);
    string& at(size_t n);
    bool empty();
    void add(string s);
    void setDiffNum();
private:
    int getNumber(string& s);
    void setNumber(string& s, int newValue);
private:
    string path;
    vector<string> vs;
public:
    int total = 0;
    int easy = 0;
    int medium = 0;
    int hard = 0;
};

#endif // MARKDOWN_H
