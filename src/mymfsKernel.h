/**
 * @file mymfsKernel.h
 * 
 * @brief Funcionalidades do software mymfs
 */

#ifndef MYMFSKERNEL_H
#define MYMFSKERNEL_H

#define CONFIG_FILE "/mymfs.config"
#define MAX_SIZE 500000

#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

bool config(std::string unityX, std::string devices);

bool import(std::string unityX, std::string filename);

bool fileExist(std::string filename);

std::vector<std::string> split(const std::string& s, char delimiter);

#endif /* MYMFSKERNEL_H */

