/* 
        The OpenSource SpringBoard clone for PocketDarwin
        This program uses Xwayland and libX11!!
*/

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstring>

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Failed to open X display" << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    // Create a simple window
    Window window = XCreateSimpleWindow(display, root, 10, 10, 400, 300, 1,
                                        BlackPixel(display, screen), WhitePixel(display, screen));

    // Select input events
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Map the window to the display
    XMapWindow(display, window);

    // Main event loop
    while (true) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Redraw the window when exposed
            XFillRectangle(display, window, DefaultGC(display, screen), 20, 20, 360, 260);
            XDrawString(display, window, DefaultGC(display, screen), 50, 50,
                        "Hello from freeboard!", strlen("Hello from freeboard!"));
        } else if (event.type == KeyPress) {
            break; // Exit on key press
        }
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}