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
                        configWriter << "\nFILE " << filename << std::endl;
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
                    
                    return true;
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
                    if (lineSplit.size() == 2 && lineSplit[0] == "FILE")
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
                bool filenameExist = false;
                
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
                        filenameExist = true;
                        break;
                    }
                }
                
                if(filenameExist)
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
                    return true;
                }
                else
                {
                    std::cout << "The file " << filename 
                            << " does not exist." << std::endl;
                    return false;
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


bool _remove(std::string unityX, std::string filename)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        std::fstream configReader(configFile, std::fstream::in);
        if (configReader.is_open())
        {
            std::string line;
            std::vector<std::string> lineSplit;
            std::vector<std::string> devices;
            bool filenameExist = false;
            
            std::string configTempName = unityX;
            configTempName.append("://config.temp");
            std::ofstream configTemp(configTempName, std::ofstream::out);

            /* List of devices */
            getline (configReader, line);
            configTemp << line << std::endl;
            devices = split(line, ' ');

            while (!configReader.eof())
            {
                getline (configReader, line);
                
                lineSplit = split(line, ' ');
                if (lineSplit.size() == 2 
                    && lineSplit[0] == "FILE" 
                    && lineSplit[1] == filename)
                {
                    filenameExist = true;
                    break;
                }
                else
                {
                    configTemp << line << std::endl;
                }
            }

            if(filenameExist)
            {
                std::cout << "Removing the file " << filename << std::endl;

                getline (configReader, line);
                lineSplit = split(line, ' ');
                while (lineSplit.size() == 2 && lineSplit[0] == "FILE-PART")
                {
                    if( remove( lineSplit[1].c_str() ) != 0 )
                    {
                        std::cout << "Error deleting the file " 
                            << lineSplit[1] << std::endl;
                        return false;
                    }
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                } 
                
                std::string zipfile = "";
                for (int d=0; d<devices.size(); d++)
                {
                    zipfile = "";
                    zipfile.append(devices[d]);
                    zipfile.append("://");
                    zipfile.append(filename);
                    zipfile.append(".zip");
                    if (fileExist(zipfile))
                    {
                        if( remove( zipfile.c_str() ) != 0 )
                        {
                            std::cout << "Error deleting the file " 
                                << zipfile << std::endl;
                            return false;
                        }
                    }
                }
                
                while (!configReader.eof())
                {
                    configTemp << line << std::endl;
                    getline (configReader, line);
                }
                configReader.close();
                configTemp.close();
                remove(configFile.c_str());
                rename(configTempName.c_str(), configFile.c_str());
                mirrorConfig(unityX);
                
                return true;
            }
            else
            {
                std::cout << "The file " << filename 
                        << " does not exist." << std::endl;
                return false;
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
        std::cout << "Does not exist a raid X filesystem in " 
            << unityX << std::endl;
        return false;
    }
}


bool removeAll(std::string unityX)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        fstream configReader(configFile, std::fstream::in);
            if (configReader.is_open())
            {
                std::string line;
                std::vector<std::string> allFiles;
                std::vector<std::string> lineSplit;
                
                while (!configReader.eof())
                {
                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                    if (lineSplit.size() == 2 && lineSplit[0] == "FILE")
                    {
                        allFiles.push_back(lineSplit[1]);
                        getline (configReader, line);
                        lineSplit = split(line, ' ');
                        std::cout << lineSplit[1][0] << std::endl;
                    }
                }
                configReader.close();
                
                for(std::string f : allFiles)
                    _remove(unityX, f);
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


bool head100(std::string unityX, std::string filename)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        std::fstream configReader(configFile, std::fstream::in);
        if (configReader.is_open())
        {
            std::string line;
            std::vector<std::string> lineSplit;
            std::vector<std::string> devices;
            bool filenameExist = false;

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
                    filenameExist = true;
                    break;
                }
            }

            if(filenameExist)
            {
                std::cout << "head100 for file " 
                    << filename << ":" << std::endl;

                getline (configReader, line);
                lineSplit = split(line, ' ');
                std::fstream * fileReader = NULL;
                int fileLength = 0;
                int nlines = 0;
                std::string fileLine;
                while (lineSplit[0] == "FILE-PART")
                {
                    fileReader  = new std::fstream(lineSplit[1]
                        , std::fstream::in);

                    while(!fileReader->eof())
                    {
                        getline (*fileReader, fileLine);
                        nlines++;
                        std::cout << fileLine << std::endl;
                        if (nlines == 100)
                            break;
                    }

                    fileReader->close();
                    delete(fileReader);
                    
                    if (nlines == 100)
                        break;

                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                } 
                return true;
            }
            else
            {
                std::cout << "The file " << filename 
                        << " does not exist." << std::endl;
                return false;
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
        std::cout << "Does not exist a raid X filesystem in " 
            << unityX << std::endl;
        return false;
    }
}


bool tail100(std::string unityX, std::string filename)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        std::fstream configReader(configFile, std::fstream::in);
        if (configReader.is_open())
        {
            std::string line;
            std::vector<std::string> lineSplit;
            std::vector<std::string> devices;
            bool filenameExist = false;

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
                    filenameExist = true;
                    break;
                }
            }

            if(filenameExist)
            {
                std::cout << "tail100 for file " 
                    << filename << ":" << std::endl;

                getline (configReader, line);
                lineSplit = split(line, ' ');
                std::fstream * fileReader = NULL;
                std::vector<std::string> allLines;
                std::string fileLine = "";
                while (lineSplit[0] == "FILE-PART")
                {
                    fileReader  = new std::fstream(lineSplit[1]
                        , std::fstream::in);

                    while(!fileReader->eof())
                    {
                        getline (*fileReader, fileLine);
                        allLines.push_back(fileLine);
                    }

                    fileReader->close();
                    delete(fileReader);

                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                } 
                int nlines = 0;
                int show = 100;
                if (allLines.size() > show)
                    for (int li=(allLines.size()-show); 
                        li < allLines.size(); li++)
                        std::cout << allLines[li] << std::endl;
                else
                {
                    for (int li=0; li<allLines.size(); li++)
                    {
                        std::cout << allLines[li] << std::endl;
                        nlines++;
                        if (nlines == show)
                            break;
                    }
                }
                
                return true;
            }
            else
            {
                std::cout << "The file " << filename 
                        << " does not exist." << std::endl;
                return false;
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
        std::cout << "Does not exist a raid X filesystem in " 
            << unityX << std::endl;
        return false;
    }
}


bool grep(std::string unityX, std::string filename, std::string wordMatch)
{
    std:string configFile = unityX;
    configFile.append(CONFIG_FILE);
    if(fileExist(configFile))
    {
        std::fstream configReader(configFile, std::fstream::in);
        if (configReader.is_open())
        {
            std::string line;
            std::vector<std::string> lineSplit;
            std::vector<std::string> devices;
            bool filenameExist = false;

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
                    filenameExist = true;
                    break;
                }
            }

            if(filenameExist)
            {
                std::cout << "grep for file " 
                    << filename << " with word " << wordMatch 
                    << " :" << std::endl;

                getline (configReader, line);
                lineSplit = split(line, ' ');
                std::fstream * fileReader = NULL;
                std::vector<std::string> allLines;
                std::string fileLine = "";
                while (lineSplit[0] == "FILE-PART")
                {
                    fileReader  = new std::fstream(lineSplit[1]
                        , std::fstream::in);

                    while(!fileReader->eof())
                    {
                        getline (*fileReader, fileLine);
                        allLines.push_back(fileLine);
                    }

                    fileReader->close();
                    delete(fileReader);

                    getline (configReader, line);
                    lineSplit = split(line, ' ');
                    if (configReader.eof())
                        break;
                } 
                int lc = 0;
                bool match = false;
                for (std::string line2grep : allLines)
                {
                    lineSplit = split(line2grep, ' ');
                    lc++;
                    for (std::string word : lineSplit)
                    {
                        if (word == wordMatch)
                        {
                            std::cout << "Encontrado " << lc << std::endl;
                            match = true;
                            break;
                        }
                        if (match)
                            break;
                    }
                    if (match)
                        break;
                }
                if (!match)
                    std::cout << "Não encontrado" << std::endl;
                
                return true;
            }
            else
            {
                std::cout << "The file " << filename 
                        << " does not exist." << std::endl;
                return false;
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
    char buffer [fileLength];
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
            dev.append(CONFIG_FILE);
            remove(dev.c_str());
            configWrite =  new ofstream(dev, std::ofstream::out);
            for (int b=0; b<fileLength; b++)
                *configWrite << buffer[b];
            configWrite->close();
            delete(configWrite);
        }
    }   
}
