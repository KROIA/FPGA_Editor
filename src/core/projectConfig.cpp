#include "projectConfig.h"

ProjectConfig::ProjectConfig()
{

}
ProjectConfig::~ProjectConfig()
{

}

void ProjectConfig::setProjectPath(const string &path)
{
    m_projectPath = path;
    readFile();
    readFileContent();
}

void ProjectConfig::readFile()
{
    fstream  file;
    file.open(m_projectPath.c_str(),std::ios::in);
    if(!file.is_open())
    {
        qDebug() << "can't open File: "<<m_projectPath.c_str();
        return;
    }
    string line;
    m_fileContent.clear();
    while(getline(file, line)){ //read data from file object and put it into string.
       m_fileContent.push_back(line);
    }
    file.close(); //close the file object.
}
void ProjectConfig::readFileContent()
{

}
