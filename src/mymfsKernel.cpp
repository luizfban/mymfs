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
          mirrorConfig(unityX);
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
                std::vector<std::string> devices;
                bool fileNotExist = true;
                
                /* List of devices */
                getline (configReader, line);
                devices = split(line, ' ');
                
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
                        
                        /* initialize random seed: */
                        srand (time(NULL));
                        /* Choose a random device to read and 
                         * the next to write the backup.
                         */
                        int deviceToWrite = rand() % devices.size();
                        int deviceToWriteBackup = 
                            (deviceToWrite + 1) % devices.size();
                        
                        while(counter < fileLength)
                        {
                            filePart = devices[deviceToWrite];
                            filePart.append(":/");
                            filePart.append(baseName);
                            filePart.append(std::to_string(part));
                            ofstream newfile(filePart, std::ofstream::binary);
                            configWriter << "FILE-PART " 
                                    << filePart << std::endl;
                            if(counter+MAX_SIZE >= fileLength)
                            {
                                for (f=counter; f<fileLength; f++)
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
                        std::string fileZip = devices[deviceToWriteBackup];
                        fileZip.append(":/");
                        fileZip.append(filename);
                        fileZip.append(".zip");
                        zipFile((char*)filename.c_str(), 
                            (char*) fileZip.c_str());
                        configWriter.close();
                        mirrorConfig(unityX);
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


void listall(std::string unityX)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        fstream configReader(configFile, std::fstream::in);
            if (configReader.is_open())
            {
                std::string line;
                std::vector<std::string> lineSplit;
                
                std::cout << "Listing all files in RAID " 
                    << unityX << std::endl;
                while (!configReader.eof())
                {
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                    if (lineSplit[0] == "FILE")
                    {
                        std::cout << "FILE " << lineSplit[1] << " in device ";
                        getline (configReader, line);
                        lineSplit = split(line, ' ');
                        std::cout << lineSplit[1][0] << std::endl;
                    }
                }
                configReader.close();
            }
            else
            {
                std::cout << "Error opening the config file." << std::endl;
            }
    }
    else
    {
        std::cout << "Does not exist a raid X filesystem in " 
            << unityX << std::endl;
    }
}


bool _export(std::string unityX, std::string filename, std::string extFile)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        if (!fileExist(extFile))
        {
            std::fstream configReader(configFile, std::fstream::in);
            if (configReader.is_open())
            {
                std::string line;
                std::vector<std::string> lineSplit;
                std::vector<std::string> devices;
                bool fileExist = false;
                
                /* List of devices */
                getline (configReader, line);
                devices = split(line, ' ');
                
                while (!configReader.eof())
                {
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (lineSplit.size() == 2 
                        && lineSplit[0] == "FILE" 
                        && lineSplit[1] == filename)
                    {
                        fileExist = true;
                        break;
                    }
                }
                
                if(fileExist)
                {
                    std::cout << "Exporting the file " << filename << std::endl;
                    
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    char * buffer = NULL;
                    std::ifstream * fileReader = NULL;
                    int fileLength = 0;
                    std::ofstream fileWriter(extFile, std::ofstream::binary);
                    while (lineSplit[0] == "FILE-PART")
                    {
                        fileReader  = new std::ifstream(lineSplit[1]
                            , std::ifstream::binary);

                        fileReader->seekg (0, fileReader->end);
                        fileLength = fileReader->tellg();
                        fileReader->seekg (0, fileReader->beg);
                        
                        buffer = new char [fileLength];
                        fileReader->read(buffer, fileLength);

                        if (fileReader)
                            for (int i=0; i<fileLength; i++)
                                fileWriter << buffer[i];
                        
                        delete(buffer);
                        fileReader->close();
                        delete(fileReader);
                        
                        getline (configReader, line);
                        lineSplit = split(line, ' ');
                        if (configReader.eof())
                            break;
                    } 
                    fileWriter.close();
                }
                else
                {
                    std::cout << "The file " << filename 
                            << " does not exist." << std::endl;
                }
                configReader.close();
            }
            else
            {
                std::cout << "Error opening the configuration file." << std::endl;
                return false;
            }
        }
        else
        {
            std::cout << "The external file already exist" << std::endl;
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


void zipFile(char * filename, char * filezip)
{
    struct zip_t *zip = zip_open(filezip, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
    {
        zip_entry_open(zip, filename);
        {
            zip_entry_fwrite(zip, filename);
        }
        zip_entry_close(zip);
    }
    zip_close(zip);
}


void mirrorConfig(std::string unityX)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    
    fstream devReader(configFile, std::fstream::in);
    std::string line;
    std::vector<std::string> devices;
                
    /* List of devices */
    getline (devReader, line);
    devices = split(line, ' ');
    devReader.close();
    
    /* Read all data in config */
    std::ifstream fileReader (configFile, std::ifstream::binary);
    fileReader.seekg (0, fileReader.end);
    int fileLength = fileReader.tellg();
    fileReader.seekg (0, fileReader.beg);
    char * buffer = new char [fileLength];
    fileReader.read(buffer, fileLength);
    fileReader.close();
    
    /* Update config file in all devices */
    std::string dev;
    std::ofstream * configWrite;
    for(int i=0; i<devices.size(); i++)
    {
        if (devices[i] != unityX)
        {
            dev = devices[i];
            dev.append(":/");
            dev.append(CONFIG_FILE);
            configWrite =  new ofstream(dev, std::ofstream::out);
            *configWrite << buffer;
            configWrite->close();
        }
    }   
}
