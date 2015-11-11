/* 
 * File:   RootWindow.h
 * Author: David Young
 *
 * Created on November 6, 2015, 4:04 PM
 */
#pragma once

#ifndef ROOTWINDOW_H
#define	ROOTWINDOW_H

#include "External.h"

typedef Display const * const Imut_Display;

class RootWindowContainer {
public:
    RootWindowContainer();
    virtual ~RootWindowContainer();
    
    //Only valid for the lifetime of the RootWindowContainer.
    const Window GetRootWindow() const {
        return r_window;
    }
    
    Display* GetDisplay() const {
        return d_display;
    }
    
private:
    /* Use GetRootWindow instead and track this object for this resource's
     * lifetime.
     */
    RootWindowContainer(const RootWindowContainer& orig);
    
    Window r_window;
    int n_screen;
    Display* d_display;
};

#endif	/* ROOTWINDOW_H */

