#include "markdown.h"

MarkDown::MarkDown(string _path) :path(_path) {}

bool MarkDown::read() {
    fstream file;
    file.open(path.c_str(), ios::in);
    if (!file.is_open())
        return false;
    string line;
    while (getline(file, line)) {
        vs.push_back(line);
    }
    this->total = getNumber(vs[TOTAL_LINE]);
    this->easy = getNumber(vs[EASY_LINE]);
    this->medium = getNumber(vs[MEDIUM_LINE]);
    this->hard = getNumber(vs[HARD_LINE]);
    file.close();
    return true;
}

bool MarkDown::save() {
    fstream file;
    file.open(path.c_str(), ios::out);
    if (!file.is_open())
        return false;
    for (auto& s : vs) {
        file << s << endl;
    }
    return true;
}

void MarkDown::tidy() {
    int sz = vs.size();
    int cntEasy = 0, cntMedium = 0, cntHard = 0;
    for (int n = START_LINE; n < sz; ++n) {
        string& s = vs[n];
        int len = s.size();
        int i = len - 2;
        while (s[i] != '|') --i;
        int j = i - 1;
        while (s[j] != '|') --j;
        string diff = s.substr(j + 1, i - j - 1);
        if (diff == "💚")
            ++cntEasy;
        else if (diff == "🧡")
            ++cntMedium;
        else
            ++cntHard;
        for (int i = 1; i < len; ++i) {
            if (s[i] == '|') {
                s.replace(1, i - 1, to_string(sz - n));
                break;
            }
        }
    }
    this->easy = cntEasy;
    this->medium = cntMedium;
    this->hard = cntHard;
    this->total = cntEasy + cntMedium + cntHard;
}

size_t MarkDown::size() {
    assert(!vs.empty());
    return vs.size() - START_LINE;
}

string &MarkDown::operator[](size_t n) {
    return this->at(n);
}

string &MarkDown::at(size_t n) {
    assert(n < this->size());
    return vs[n + START_LINE];
}

bool MarkDown::empty() {
    return vs.empty();
}

void MarkDown::add(string s) {
    vs.insert(vs.begin() + START_LINE, s);
}

void MarkDown::setDiffNum() {
    setNumber(vs[EASY_LINE], easy);
    setNumber(vs[MEDIUM_LINE], medium);
    setNumber(vs[HARD_LINE], hard);
    setNumber(vs[TOTAL_LINE], total);
}

int MarkDown::getNumber(string &s) {
    for (size_t i = 0, j; i < s.size(); ++i) {
        if (isdigit(s[i])) {
            for (j = i + 1; j < s.size(); ++j) {
                if (!isdigit(s[j])) {
                    break;
                }
            }
            return stoi(s.substr(i, j - i));
        }
    }
    return -1;
}

void MarkDown::setNumber(string &s, int newValue) {
    for (size_t i = 0, j; i < s.size(); ++i) {
        if (isdigit(s[i])) {
            for (j = i + 1; j < s.size(); ++j) {
                if (!isdigit(s[j])) {
                    break;
                }
            }
            s.replace(i, j - i, to_string(newValue));
            return;
        }
    }
}
