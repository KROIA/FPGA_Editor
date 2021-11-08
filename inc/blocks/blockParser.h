#ifndef BLOCKPARSER_H
#define BLOCKPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "debug.h"

using std::string;
using std::vector;
using std::fstream;

class BlockParser
{
    public:
        BlockParser();
        ~BlockParser();

        bool readFile(const string &filename);
        void parseVerilog();

    protected:


    private:


        vector<string> m_fileContent;

        string m_blockname;
        unsigned int m_inputs;
        unsigned int m_outputs;

};
#endif
