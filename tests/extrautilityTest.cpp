/* 
 * File:   newsimpletest.cpp
 * Author: master
 *
 * Created on Nov 8, 2015, 12:50:59 PM
 */

#include <stdlib.h>
#include <iostream>
#include "ExtraUtility.h"

/*
 * Simple C++ Test Suite
 */

void ListFilesTest() {
    std::vector<std::string> files = extrautility::ListFilesV(getenv("HOME"));
    std::vector<std::string>::iterator itr = files.begin();
    
    std::cout << "\t=== Files ===" << std::endl;
    while(itr != files.end())
    {
        std::cout << '\t' << *itr << std::endl;
        itr++;
    }
    
    std::cout << "\t=============" << std::endl;
}

void RegexTest() {
    std::vector<std::string> files;
    files.push_back("cake.txt");
    files.push_back("test.keek");
    files.push_back("test.wee");
    files.push_back("test.png");
    std::vector<std::string>::iterator itr = files.begin();
    
    std::cout << "\t=== Files ===" << std::endl;
    while(itr != files.end())
    {
        std::cout << '\t' << *itr << std::endl;
        itr++;
    }
    
    std::cout << "\t=============" << std::endl;
    
    std::cout << "Removing .txt files." <<std::endl;
    extrautility::RegexFilter filter(".+\\.png");
    filter.invert();
    files.erase(std::remove_if(files.begin(), files.end(), filter), files.end());
    
    itr = files.begin();
    std::cout << "\t=== Files ===" << std::endl;
    while(itr != files.end())
    {
        std::cout << '\t' << *itr << std::endl;
        itr++;
    }
    
    std::cout << "\t=============" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% Extra Utility Test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    
    std::cout << "%TEST_STARTED% List Files Test (Extra Utility Test)" << std::endl;
    ListFilesTest();
    std::cout << "%TEST_FINISHED% time=0 List Files (Extra Utility Test)" << std::endl;
    
    
    std::cout << "%TEST_STARTED% List Files Test (Extra Utility Test)" << std::endl;
    RegexTest();
    std::cout << "%TEST_FINISHED% time=0 List Files (Extra Utility Test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}