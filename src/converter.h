#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

using namespace std;
class Converter
{
public:
    Converter();
    bool Video2Imgs(const string&,const string&,const string&,const int,const int,const int,bool);
    bool Video2Png(const string&,const string&,const int,const int,const int,bool);
    bool Video2Jpg(const string&,const string&,const int,const int,const int,bool);
    const string CurrentDateTime();
private:
    bool Mkdir(const string&);
};

#endif // CONVERTER_H
