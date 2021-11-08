#include "blockParser.h"


BlockParser::BlockParser()
{

}
BlockParser::~BlockParser()
{

}

bool BlockParser::readFile(const string &filename)
{
    fstream  file;
    file.open(filename.c_str(),std::ios::in);
    if(!file.is_open())
    {
        qDebug() << "Can't open file: "<<filename.c_str();
        return false;
    }
    string line;
    while(getline(file, line)){ //read data from file object and put it into string.
       qDebug() << line.c_str(); //print the data of the string
    }
    file.close(); //close the file object.

    return true;
}

void BlockParser::parseVerilog()
{

}
