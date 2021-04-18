#pragma once

class String
{
public:
    String():content(nullptr) {}
    ~String() {
        delete []content;
    }
    void set();
    void print();
    int length();
    void add(const char*);
private:
    char *content;
};

