/**
 * @file mymfs.cpp
 * 
 * @brief Arquivo principal do software mymfs
 */


#define USAGE "USAGE\n\
mymfs.exe X config D E F\n\
mymfs.exe X import file.txt\n\
mymfs.exe X listall\n\
mymfs.exe X export file.txt C:/file.txt\n"


#include <string>
#include <iostream>
#include <cstdlib>
#include "mymfsKernel.h"

using namespace std;


/* Interpreta comandos mymfs via linha de comandos e chama ação apropriada */
int main(int argc, char** argv)
{
    bool correct = true;
    /* Todos as chamadas tem pelo menos 3 argumentos */
    if (argc >= 3)
    {   
        std::string cmd = argv[2];
        std::string unityX = argv[1];
        if (cmd.compare("config") == 0)
        {
            std::string devices = "";
            for (int d=3; d<argc; d++)
            {
                devices.append(argv[d]);
                devices.append(" ");
            }
            std::cout << "Running config command." << std::endl;
            config(unityX, devices);
        }
        else if  (cmd.compare("import") == 0)
        {
            std::string file2import = argv[3];
            std::cout << "Running import command." << std::endl;
            import(unityX, file2import);
        }
        else if  (cmd.compare("listall") == 0)
        {
            std::cout << "Running listall command." << std::endl;
            listall(unityX);
        }
        else if  (cmd.compare("export") == 0)
        {
            std::string filename2export = argv[3];
            std::string externalFile = argv[4];
            std::cout << "Running export command." << std::endl;
            _export(unityX, filename2export, externalFile);
        }
        else
        {
            correct = false;
        }
    }
    else
    {
        correct = false;
    }
    
    if (!correct)
        std::cout << "Command not found\n\n"  << USAGE << std::endl;
        
    return 0;
}
