#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <QDebug>

using std::string;
using std::vector;
using std::fstream;

class ProjectConfig
{
    public:
        ProjectConfig();
        ~ProjectConfig();

        void setProjectPath(const string &path);


    private:
        void readFile();
        void readFileContent();

        string m_projectPath;

        vector<string> m_fileContent;

};

#endif // PROJECTCONFIG_H
