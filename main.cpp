/* 
 * File:   main.cpp
 * Author: David Young
 *
 * Created on November 6, 2015, 3:33 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include "Main.h"

using namespace std;

bool terminate_process;

config_vals ParseArguments(int argc, char** argv)
{
    enum OPTION_VAL {SWAP, SCALE, HELP};
    int opt = 0;
    config_vals cfg;
    cfg.NoScale = false;
    cfg.PrintHelp = false;
    cfg.SwapRate = DEFAULT_SWAP_RATE;
    cfg.NoSwap = false;
    
    int opt_indx = -1;
    
    option option_a[4];
    option_a[0].name = "help";
    option_a[0].has_arg = no_argument;
    option_a[0].flag = NULL;
    option_a[0].val = OPTION_VAL::HELP;
    
    option_a[1].name = "noscale";
    option_a[1].has_arg = no_argument;
    option_a[1].flag = NULL;
    option_a[1].val = OPTION_VAL::SCALE;
    
    option_a[2].name = "swaprate";
    option_a[2].flag = NULL;
    option_a[2].has_arg = required_argument;
    option_a[2].val = OPTION_VAL::SWAP;
    
    option_a[3].name = NULL;
    option_a[3].flag = 0;
    option_a[3].has_arg = 0;
    option_a[3].val = 0;
    
    while((opt = getopt_long(argc, argv, "hn:r:", option_a, &opt_indx)) != -1)
    {
        switch(opt)
        {
            case OPTION_VAL::SCALE:
            case 'n':
                cfg.NoScale = true;
                break;
            case OPTION_VAL::HELP:
            case 'h':
                cfg.PrintHelp = true;
                break;
            case OPTION_VAL::SWAP:
            case 'r':
                cfg.SwapRate = ::atoi(optarg);
                if(cfg.SwapRate == 0)
                {
                    ERROR_SYSTEM("Option -r [--swaprate] must be a non-zero integer.");
                    exit(-1);
                }
                break;
            case 'p':
                cfg.NoSwap = true;
                break;
            case '?':
            default:
                exit(-1);
        }
    }
    
    return cfg;
}

void PrintHelp()
{
    cout<<"This program can be used to set the background of the root window "
          "in your XClient.\nRotates backgrounds from the `~/.wallpapers folder'.\n"
            "\t-n [--noscale]    Do not scale background images.\n"
            "\t-r [--swaprate]   The rate in seconds that the background rotates.\n"
            "\t-p                Do not swap background images (random pick).\n"
            "\t-h [--help]       Print information on program switches."<<endl;
}

void HandleTermiSignals(int signo)
{
    cout << "Terminating process..." << endl;
    terminate_process = true;
}

void RegisterSignalHandler()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = HandleTermiSignals;
    sa.sa_flags = SA_NOCLDWAIT;
    
    if(sigaction(SIGINT, &sa, NULL) ||
        sigaction(SIGQUIT, &sa, NULL) ||
        sigaction(SIGHUP, &sa, NULL))
    {
        ERROR_SYSTEM("Failed to register signal handler...");
        exit(-1);
    }
}

int main(int argc, char** argv) {
    std::locale::global(std::locale(""));
    RegisterSignalHandler();
    
    terminate_process = false;
    config_vals cfg = ParseArguments(argc, argv);
    
    if(cfg.PrintHelp)
    {
        PrintHelp();
        exit(0);
    }
    
    RootWindowContainer rwc;
    deque<string> files;
    
    if(PickFilesToLoad(files) != 0)
        exit(-1);
    
    InitImlib(rwc);
    
    std::string select_f;
    
    //Early exit -- we got our random file.
    if(cfg.NoSwap)
    {
        select_f = WALLPAPER_DIR + files.front();
        
        if(ProcessBackground(rwc, select_f, cfg) != 0)
        {
            ERROR_SYSTEM("Failed to load a background image. Check for locks.");
            exit(-1);
        }
        
        exit(0);
    }
    
    chrono::time_point<chrono::system_clock> start, now;
    chrono::duration<long> bg_swap_t = std::chrono::seconds(cfg.SwapRate);
    start = chrono::system_clock::now();
    
    while(!terminate_process)
    {
        //Instigate background change
        now = chrono::system_clock::now();
        auto delta = chrono::duration_cast<chrono::seconds>(now - start);
        if(delta.count() > bg_swap_t.count())
        {
            start = now;
            
            std::string t_file = files.front();
            files.pop_front();
            select_f = WALLPAPER_DIR + t_file;
            files.push_back(t_file);
            cout<<"Loading '"<<select_f<<"'..."<<endl;
            
            if(ProcessBackground(rwc, select_f, cfg) != 0)
                ERROR_SYSTEM("Failed to swap background...");
        }
        
        //Check for change in wallpaper directory
        deque<string> n_file_set;
        if(PickFilesToLoad(n_file_set) == 0 && 
                n_file_set.size() != files.size())
            files = n_file_set;
        
        //Poll process
        usleep(TIMER_POLLING_RATE * 1000);
    }
    
    cout << "Exiting..." << endl;
    return 0;
}

int InitImlib(RootWindowContainer& rwc)
{
    const Window root_win = rwc.GetRootWindow();
    Display* display = rwc.GetDisplay();
    
    ::imlib_set_cache_size(MEGABYTE * 2);
    ::imlib_set_color_usage(128);
    ::imlib_context_set_dither(1);
    ::imlib_context_set_display(display);
    ::imlib_context_set_visual(DefaultVisual(display, DefaultScreen(display)));
    ::imlib_context_set_colormap(DefaultColormap(display, DefaultScreen(display)));
    ::imlib_context_set_drawable(root_win);
}

int PickFilesToLoad(deque<string>& files)
{   
    files = extrautility::ListFiles(WALLPAPER_DIR);
    
    if(extrautility::GetLastError() == extrautility::BAD_DIRECTORY)
    {
        ERROR_SYSTEM("The ~/.wallpapers/ directory does not exist."
                " Please create and populate it.");
        return -1;
    }
    //Inverse the set of all files with the following extensions and filter them
    //out.
    extrautility::RegexFilter filter(".+\\.jpg|.+\\.jpeg|.+\\.tiff|.+\\.gif|.+\\.bmp|.+\\.png");
    filter.invert(); 
    files.erase(std::remove_if(files.begin(), files.end(), filter), files.end());
    
    if(files.empty())
    {
        ERROR_SYSTEM("The ~/.wallpapers/ directory is empty.");
        return -1;
    }
    
    std::shuffle(files.begin(), files.end(), 
            std::default_random_engine());
    
    return 0;
}

int ProcessBackground(RootWindowContainer& rwc, string& file, config_vals cfg)
{
    const Window root_win = rwc.GetRootWindow();
    Display* display = rwc.GetDisplay();
    
    //Get Window Attributes
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, root_win, &win_attr);
    
    int try_again = 0;
    Imlib_Image buffer;
    
    //Load buffer image and free it after creating a pixmap version
    while((buffer = imlib_load_image(file.c_str())) == NULL)
    {
        if(3 <= try_again++)
        {
            ERROR_SYSTEM("Failed to load image.");
            return -1;
        }
        
        usleep(1000*5000);
    }
    
    static Pixmap bg_pixm = -1;
    static Pixmap pixm_mask = -1;
    Pixmap temp_pixm = bg_pixm;
    
    imlib_context_set_image(buffer);
    imlib_context_set_dither(0);
    
    if(cfg.NoScale)
    {
        imlib_render_pixmaps_for_whole_image(&bg_pixm, &pixm_mask);
    }
    else
    {
        imlib_render_pixmaps_for_whole_image_at_size(&bg_pixm, &pixm_mask, 
                win_attr.width, win_attr.height);
    }
    
    switch(XSetWindowBackgroundPixmap(display, root_win, bg_pixm))
    {
        case BadMatch:
            ERROR_INTERNAL("Malformed arguments passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
            return -1;
        case BadPixmap:
            ERROR_INTERNAL("Invalid Pixmap passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
            return -1;
        case BadWindow:
            ERROR_INTERNAL("Invalid Window passed to"
                    " XSetWindowBackgroundPixmap in ProcessBackground.");
            return -1;
    }
    
    if(temp_pixm != -1)
        ::XFreePixmap(display, temp_pixm);
    
    imlib_free_image();
    return 0;
}