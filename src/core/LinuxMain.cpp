#include "Platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//#include <X11/Xlib.h>
#include <time.h>

#include <math.h>

#include "graphics/opengl/OpenGLExtensions.h"
#include "Game.h"
#include "input/Input.h"
#include "util/utils.h"


Display* disp;
Window win;
Atom xlib_wm_delete_window;
Cursor blankCursor;
int   lastTime;
float deltaTime;

Game* game;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;


static struct timespec a__time_start = {0, 0};
unsigned int osGetTime(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	if (a__time_start.tv_sec == 0 && a__time_start.tv_nsec == 0)
		a__time_start = ts;
	return //
		(ts.tv_sec - a__time_start.tv_sec) * 1000000 + //
		(ts.tv_nsec - a__time_start.tv_nsec) / 1000;
}


#if defined(__linux__)

void setCursorPos(const int x, const int y){
    XWarpPointer(disp, None, win, 0, 0, 0, 0, x, y);
    XFlush(disp);
}

void captureMouse(const bool value){
	if (GInput->mouseCaptured != value)
	{
		static int mouseX, mouseY;
		if (value){
			XGrabPointer(disp, win, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, win, blankCursor, CurrentTime);

			int rx, ry;
			unsigned int mask;
			Window root, child;
			XQueryPointer(disp, win, &root, &child, &rx, &ry, &mouseX, &mouseY, &mask);
			setCursorPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		} else {
			setCursorPos(mouseX, mouseY);
			XUngrabPointer(disp, CurrentTime);
		}

		GInput->mouseCaptured = value;
	}
}

bool onMouseMove(const int x, const int y, const int deltaX, const int deltaY){
    
    if (GInput->mouseCaptured)
    {

	static bool changed = false;
	if (changed = !changed){
		GInput->mouseDelta.x += (GInput->invertMouse? 1 : -1) * GInput->mouseSensibility * (WINDOW_HEIGHT / 2 - y);
		GInput->mouseDelta.y += (GInput->invertMouse? 1 : -1) * GInput->mouseSensibility * (WINDOW_WIDTH / 2 - x);
		setCursorPos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}
	
    }
    return false;
}

bool onMouseButton(const int x, const int y, const MouseButton button, const bool pressed){
    if (!GInput->mouseCaptured){
	if (button == MOUSE_LEFT && pressed){
	    captureMouse(true);
	    return true;
	}


    }
    return false;
}

bool onKey(const unsigned int key, const bool pressed)
{
    if (key < elementsOf(GInput->keyStates))
    {
	GInput->keyStates[key] = pressed;
    }
    return true;
}

#endif

int os_window_process() {
    while (XPending(disp)) {
	unsigned int key;
	XEvent e;
	XNextEvent(disp, &e);

	switch (e.type)
	{
	    case ConfigureNotify:
	    {
		//shader_resize(e.xconfigure.width, e.xconfigure.height);
		break;
	    }

	    // mouse move event
	    case MotionNotify:
	    {
		static int lastX, lastY;
		onMouseMove(e.xmotion.x, e.xmotion.y, e.xmotion.x - lastX, e.xmotion.y - lastY);
		lastX = e.xmotion.x;
		lastY = e.xmotion.y;
		break;
	    }

	    // mouse button events
	    case ButtonPress:
		onMouseButton(e.xbutton.x, e.xbutton.y, (MouseButton) (e.xbutton.button - 1), true);
		if ((e.xbutton.button -1 ) == MOUSE_RIGHT){
		    GInput->mouseStates[MOUSE_RIGHT] = true;
		}

		if ((e.xbutton.button -1 ) == MOUSE_LEFT){
		    GInput->mouseStates[MOUSE_LEFT] = true;
		}
		break;
	    case ButtonRelease:
	    {
		onMouseButton(e.xbutton.x, e.xbutton.y, (MouseButton) (e.xbutton.button - 1), false);
		if ((e.xbutton.button -1 ) == MOUSE_RIGHT){
		    GInput->mouseStates[MOUSE_RIGHT] = false;
		}

		if ((e.xbutton.button -1 ) == MOUSE_LEFT){
		    GInput->mouseStates[MOUSE_LEFT] = false;
		}

		break;
	    }

	    case KeyRelease:
		key = XLookupKeysym(&e.xkey, 0);
		onKey(key, false);
		break;
	    case KeyPress:
	    {
		key = XLookupKeysym(&e.xkey, 0);
		onKey(key, true);
		if (key == XK_Escape)
		{
		    exit(EXIT_SUCCESS);

		}
		break;
	    }
	    case ClientMessage:
	    case DestroyNotify:
	    case UnmapNotify:
	    {
		return 0;
	    }
	    default:
	    {
		break;
	    }
	}
    }

  return 1;
}


typedef GLXContext (*glXCreateContextAttribsARBProc)
    (Display*, GLXFBConfig, GLXContext, Bool, const int*);


int main()
{
    disp = 0;
    win = 0;

    /* Create_display_and_window
       -------------------------
       Skip if you already have a display and window */
    disp = XOpenDisplay(0);
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
                              10, 10,   /* x, y */
                              WINDOW_WIDTH, WINDOW_HEIGHT, /* width, height */
                              0, 0,     /* border_width, border */
                              0);       /* background */

    /* Create_the_modern_OpenGL_context
       -------------------------------- */
    static int visual_attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
	GLX_ALPHA_SIZE, 8,
	GLX_DEPTH_SIZE, 24,
	GLX_STENCIL_SIZE, 8,
        None
    };

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(disp,
                                         DefaultScreen(disp),
                                         visual_attribs, &num_fbc);
    if (!fbc) {
        printf("glXChooseFBConfig() failed\n");
        exit(1);
    }

    /* If we were on Windows (i.e. WGL), we would need to create an old
       dummy OpenGL context here, before calling GetProcAddress(). This is
       unnecessary on Linux (GLX).

       For details, refer to the spec
       (https://www.khronos.org/registry/OpenGL/extensions/ARB/GLX_ARB_get_proc_address.txt)
       which says:
           > Are function pointers context-independent? Yes. The pointer to an
           > extension function can be used with any context [...]

       This is in direct contrast to WGL's wglGetProcAddress. */

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    /* If we were on Winows, we would destroy the dummy context here. Again,
       this is unnecessary on Linux.*/

    if (!glXCreateContextAttribsARB) {
        printf("glXCreateContextAttribsARB() not found\n");
        exit(1);
    }

    /* Set desired minimum OpenGL version */
    static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };
    /* Create modern OpenGL context */
    GLXContext ctx = glXCreateContextAttribsARB(disp, fbc[0], NULL, true,
                                                context_attribs);
    if (!ctx) {
        printf("Failed to create OpenGL context. Exiting.\n");
        exit(1);
    }

    // Create a blank cursor for cursor hiding
    XColor dummy;
    char data = 0;
    Pixmap blank = XCreateBitmapFromData(disp, win, &data, 1, 1);
    blankCursor = XCreatePixmapCursor(disp, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(disp, blank);
    XGrabKeyboard(disp, win, True, GrabModeAsync, GrabModeAsync, CurrentTime);

    /* Show_the_window
       --------------- */
    XMapWindow(disp, win);
    XFlush(disp);
    glXMakeCurrent(disp, win, ctx);



    XSelectInput(disp, win, StructureNotifyMask | KeyReleaseMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask |
		PointerMotionMask );

    lastTime = osGetTime();


    init_opengl_extensions();

    if (!ZATON_INPUT_Init())
    {
	exit(EXIT_FAILURE);
    }

    game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT);
    game->mWidth = WINDOW_WIDTH;
    game->mHeight = WINDOW_HEIGHT;
    if (!game->initGame())
    {
	exit(EXIT_FAILURE);
    }

    /* Application_loop
       ---------------- */
    while (os_window_process() != 0) {
            int time = osGetTime();
            if (time == lastTime) continue;

            deltaTime = (time - lastTime) / 1000000.0f; // it's ~0.016
            lastTime = time;

	    //printf("%f %f\n", GInput->mouseDelta.x, GInput->mouseDelta.y);
	    game->update();
	    game->render();


	    glXSwapBuffers(disp, win);
    }


    
    ZATON_INPUT_Release();
    delete game;
    game = NULL;
    // close window
    glXMakeContextCurrent(disp, 0, 0, 0);
    glXDestroyContext(disp, ctx);
    XDestroyWindow(disp, win);
    XCloseDisplay(disp);

    return EXIT_SUCCESS;
}


