/* 
 * File:   newsimpletest.cpp
 * Author: master
 *
 * Created on Nov 10, 2015, 2:34:44 PM
 */

#include <stdlib.h>
#include <iostream>
#include <string>
#include "Types.h"
#include "External.h"
#include "RootWindow.h"

#define ERROR_INTERNAL(x) std::cerr<<"[ERROR_INTERNAL] "<<x
#define ERROR_SYSTEM(x) std::cerr<<"[ERROR_SYSTEM] "<<x

/*
 * Simple C++ Test Suite
 */

int ImlibBgTest()
{
    RootWindowContainer rwc;
    const Window root_win = rwc.GetRootWindow();
    Display* display = rwc.GetDisplay();
    
    ::imlib_set_cache_size(MEGABYTE * 2);
    ::imlib_set_color_usage(128);
    ::imlib_context_set_dither(1);
    ::imlib_context_set_display(display);
    ::imlib_context_set_visual(DefaultVisual(display, DefaultScreen(display)));
    ::imlib_context_set_colormap(DefaultColormap(display, DefaultScreen(display)));
    ::imlib_context_set_drawable(root_win);
    
    std::string file = "/home/master/.wallpapers/maxresdefault.jpg";
    
    //Get Window Attributes
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, root_win, &win_attr);
    
    //Load buffer image and free it after creating a pixmap version
    Imlib_Image buffer = imlib_load_image(file.c_str());
    static Pixmap bg_pixm = -1;
    static Pixmap pixm_mask = -1;
    Pixmap temp_pixm = bg_pixm;
    
    imlib_context_set_image(buffer);
    imlib_context_set_dither(0);
    imlib_render_pixmaps_for_whole_image_at_size(&bg_pixm, &pixm_mask, 
            win_attr.width, win_attr.height);
    
    switch(XSetWindowBackgroundPixmap(display, root_win, bg_pixm))
    {
        case BadMatch:
            ERROR_INTERNAL("Malformed arguments passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
            break;
        case BadPixmap:
            ERROR_INTERNAL("Invalid Pixmap passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
            break;
        case BadWindow:
            ERROR_INTERNAL("Invalid Window passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
    }
    
    if(temp_pixm != -1)
        ::XFreePixmap(display, temp_pixm);
    
    imlib_free_image();
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% ImlibBgTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% ImlibBgTest (ImlibBgTest)" << std::endl;
    ImlibBgTest();
    std::cout << "%TEST_FINISHED% time=0 test1 (ImlibBgTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

