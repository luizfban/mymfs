/**
 * @file mymfsKernel.h
 * 
 * @brief Funcionalidades do software mymfs
 */

#ifndef MYMFSKERNEL_H
#define MYMFSKERNEL_H

#define CONFIG_FILE "://mymfs.config"
#define MAX_SIZE 512000

#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "zip.h"


using namespace std;

bool config(std::string unityX, std::string devices);

bool import(std::string unityX, std::string filename);

void listall(std::string unityX);

bool _export(std::string unityX, std::string filename, std::string extFile);

bool _remove(std::string unityX, std::string filename);

bool removeAll(std::string unityX);

bool fileExist(std::string filename);

std::vector<std::string> split(const std::string& s, char delimiter);

void zipFile(char * filename, char * filezip);

void mirrorConfig(std::string unityX);

#endif /* MYMFSKERNEL_H */

