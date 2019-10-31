/**
 * @file mymfsKernel.cpp
 * 
 * @brief Funcionalidades do software mymfs
 */

#include <vector>

#include "mymfsKernel.h"


bool config(std::string unityX, std::string devices)
{
    std:string fullPathname = unityX;
    fullPathname.append(CONFIG_FILE);
    if(fileExist(fullPathname))
    {
        std::cout << "A configuration file already exist!" << std::endl;
        return false;
    }
    else
    {
        ofstream configFile (fullPathname, std::ifstream::out);
        if (configFile.is_open())
        {
          configFile << devices << std::endl;
          configFile.close();
          std::cout << "The configuration file was created." << std::endl;
        }
        else
        {
          std::cout << "Error creating config file. Maybe the directory " 
            << unityX << " does not exist."<< std::endl;
        }
        
        return true;
    }
}


bool import(std::string unityX, std::string filename)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        if (fileExist(filename))
        {
            fstream configReader(configFile, std::fstream::in);
            if (configReader.is_open())
            {
                std::string line;
                std::vector<std::string> lineSplit;
                bool fileNotExist = true;
                while (!configReader.eof())
                {
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (lineSplit.size() == 2 
                        && lineSplit[0] == "FILE" 
                        && lineSplit[1] == filename)
                        fileNotExist = false;
                }
                configReader.close();
                
                if(fileNotExist)
                {
                    std::cout << "Importing the file." << std::endl;
                    
                    std::ifstream fileReader (filename, std::ifstream::binary);
                    
                    fileReader.seekg (0, fileReader.end);
                    int fileLength = fileReader.tellg();
                    fileReader.seekg (0, fileReader.beg);
                    
                    char * buffer = new char [fileLength];
                    fileReader.read(buffer, fileLength);

                    if (fileReader)
                    {
                        int counter = 0;
                        int part = 1;
                        std::string baseName = filename;
                        baseName.append(".");
                        std::string filePart = "";
                        ofstream configWriter(configFile
                            , std::ofstream::out | std::ofstream::app);
                        configWriter << "FILE " << filename << std::endl;
                        int f;
                        while(counter < fileLength)
                        {
                            filePart = "D/";
                            filePart.append(baseName);
                            filePart.append(std::to_string(part));
                            ofstream newfile(filePart, std::ofstream::binary);
                            configWriter << "FILE-PART " 
                                    << filePart << " D" << std::endl;
                            if(counter+MAX_SIZE >= fileLength)
                            {
                                while(!newfile.eof())
                                    for (f=counter; f<(counter+MAX_SIZE); f++)
                                    {
                                        newfile << buffer[f]; 
                                        counter++;
                                    }
                            }
                            else
                            {
                                for (f=counter; f<(counter+MAX_SIZE); f++)
                                    newfile << buffer[f]; 
                                counter += MAX_SIZE;
                            }
                            newfile.close();
                            part++;
                        }
                    }
                    else
                        std::cout << "Error while reading." << std::endl;
                    
                    fileReader.close();
                    delete[] buffer;
                }
                else
                {
                    std::cout << "File already in mymfs " 
                        << unityX << std::endl; 
                    return false;
                }
            }
            else
            {
                std::cout << "Error opening the file." << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "The file " << filename 
                << " does not exist." << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Does not exist a raid X filesystem in " 
            << unityX << std::endl;
        return false;
    }
}


bool fileExist(std::string filename)
{
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
        return false;
}


std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
} 