#include "ExtraUtility.h"
#include <dirent.h>

#ifndef __unix__
#ifndef __linux__
#error This source is designed for POSIX compliant systems.
#endif
#endif

using namespace std;

namespace extrautility
{
    ErrorT g_last_error;
    
    std::vector<std::string> ListFilesV(const char* s_directory)
    {
        g_last_error = ErrorT::NONE;
        std::vector<std::string> filen;
        dirent *dir_entry;
        DIR * directory;
        directory = opendir(s_directory);
        
        if(directory)
        {
            while((dir_entry = readdir(directory)) != NULL)
            {
                filen.push_back(dir_entry->d_name);
            }
            
            closedir(directory);
        }
        else
        {
            g_last_error = ErrorT::BAD_DIRECTORY;
        }
        
        return filen;
    }
    
    std::deque<std::string> ListFiles(const char* s_directory)
    {
        g_last_error = ErrorT::NONE;
        std::deque<std::string> filen;
        dirent *dir_entry;
        DIR * directory;
        directory = opendir(s_directory);
        
        if(directory)
        {
            while((dir_entry = readdir(directory)) != NULL)
            {
                filen.push_back(dir_entry->d_name);
            }
            
            closedir(directory);
        }
        else
        {
            g_last_error = ErrorT::BAD_DIRECTORY;
        }
        
        return filen;
    }

    ErrorT GetLastError()
    {
        return g_last_error;
    }
    
    bool RegexFilter::operator ()(std::string str)
    {
        return invert_b != ::regex_match(str, reg);
    }
    
    void RegexFilter::invert()
    {
        invert_b = !invert_b;
    }
    
}