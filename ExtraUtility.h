/* 
 * File:   ExtraUtility.h
 * Author: David Young
 *
 * Created on November 6, 2015, 6:00 PM
 */
#pragma once

#ifndef EXTRAUTILITY_H
#define	EXTRAUTILITY_H

#include <string>
#include <deque>
#include <regex>

namespace extrautility {
    
    enum ErrorT {
        NONE,
        BAD_DIRECTORY
    };
    
    ErrorT GetLastError();
    
    const int Str_Dque = 0;
    
    std::deque<std::string> ListFiles(const char* directory);
    
    std::vector<std::string> ListFilesV(const char* directory);
    
    class RegexFilter {
    public:
        RegexFilter(const char* str) : reg(str), invert_b(false) {}
        bool operator()(std::string);
        void invert();
    private:
        bool invert_b;
        std::regex reg;
    };
}

#endif	/* EXTRAUTILITY_H */

