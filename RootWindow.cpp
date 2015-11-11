/* 
 * File:   RootWindow.cpp
 * Author: master
 * 
 * Created on November 6, 2015, 4:04 PM
 */

#include "RootWindow.h"

RootWindowContainer::RootWindowContainer() {
    d_display = XOpenDisplay(NULL);
    n_screen = DefaultScreen(d_display);
    this->r_window = RootWindow(d_display, n_screen);
}


RootWindowContainer::RootWindowContainer(const RootWindowContainer& orig) {
}

RootWindowContainer::~RootWindowContainer() {
    XCloseDisplay(d_display);
}

