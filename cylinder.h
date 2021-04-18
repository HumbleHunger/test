#pragma once

class cylinder
{
public:
    cylinder():radius(0),height(0) {}
    ~cylinder() {}
    void set();
    double V();
    void print();
private:
    double radius;
    double height;
    static double a;
};

