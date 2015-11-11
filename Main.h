/* 
 * File:   main.h
 * Author: David Young
 *
 * Created on November 8, 2015, 3:01 PM
 */
#pragma once

#ifndef MAIN_H
#define	MAIN_H

#include "RootWindow.h"
#include "External.h"
#include "Types.h"
#include "ExtraUtility.h"
#include <chrono>
#include <deque>
#include <string>
#include <iostream>

#define ERROR_INTERNAL(x) std::cerr<<"[ERROR_INTERNAL] "<<x<<std::endl
#define ERROR_SYSTEM(x) std::cerr<<"[ERROR_SYSTEM] "<<x<<std::endl

const long DEFAULT_SWAP_RATE = 60 * 5;
const unsigned int TIMER_POLLING_RATE = 1000;

#define WALLPAPER_DIR (std::string(getenv("HOME")) + "/.wallpapers/").c_str()
//const char* WALLPAPER_DIR = "~/.wallpapers/";

struct config_vals {
    bool NoScale;
    bool PrintHelp;
    long SwapRate;
    bool NoSwap;
};

/* Handle core functionality. */
int ProcessBackground(RootWindowContainer&, std::string&, config_vals cfg);
int InitImlib(RootWindowContainer&);
int PickFilesToLoad(std::deque<std::string>&);
void HandleTermiSignals(int signo);

#endif	/* MAIN_H */