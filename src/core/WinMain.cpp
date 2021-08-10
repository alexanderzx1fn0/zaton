#include "Platform.h"

#include <stdio.h>

#include "graphics/opengl/OpenGLExtensions.h"
#include "Game.h"
#include "input/Input.h"
#include "util/utils.h"

Game* game;

#define window_width 1280
#define window_height 720

#define GETX(l) (int(l & 0xFFFF))
#define GETY(l) (int(l) >> 16)
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// OpenGL
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);


void Query_Performance_Counter(__int64& in) {

	QueryPerformanceCounter((LARGE_INTEGER*)&in);
}
/*
==================
==================
*/
void Query_Performance_Frequency(__int64& in) {

	QueryPerformanceFrequency((LARGE_INTEGER*)&in);
}

// TODO: I neeed to fix that, and move timer to separate file and not use deltaTime
// variable
float deltaTime;

struct timer_ 
{
    __int32 frame_count;
    __int64 counter;
    __int64 prev_counter;
    __int64 frequency;
    float real_delta_time;
    float delta_time;
    double fixed_step;
    double accumulator;
} timer;


void Initialise_Input(timer_&)
{
    {
        // init timer
        timer.frame_count = 0;
        timer.delta_time = 0.0f;
        timer.fixed_step = 1.0 / 60.0;
        timer.accumulator = 0.0;
        Query_Performance_Frequency(timer.frequency);
        Query_Performance_Counter(timer.prev_counter);
    }
}
__int32 Update_Timer(timer_&)
{
    Query_Performance_Counter(timer.counter);
    double delta = (timer.counter - timer.prev_counter) / (double)timer.frequency;

    timer.real_delta_time = (float)delta;
    timer.prev_counter = timer.counter;
    timer.accumulator += delta;
    timer.delta_time = (float)timer.fixed_step;
    __int32 n_steps = (__int32)(timer.accumulator / timer.fixed_step);
    n_steps = MIN(n_steps, 5);
    return n_steps;
}


typedef struct
{
    int width;
    int height;
}_window;

_window win;


const char lpszAppName[] = "zaton";


HWND hwnd;
bool quit = false;


void setCursorPos(const int x, const int y)
{
    POINT point = {x, y};
    ClientToScreen(hwnd, &point);// эта строчка скорее всего
    // чтобы преобразовать значения от 0 до 1 чтобы нужно проверить
    // чтобы не переводить в радианы
    SetCursorPos(point.x, point.y);
}

void onMouseMove(int x, int y, int dx, int dy)
{
    float mouseSensibility = 0.001f;
    static bool changed = false;
    if (changed = !changed) {
        //camera.wx += mouseSensibility * (window_height / 2 - y); // PITCH
        //camera.wy += mouseSensibility * (window_width / 2 - x);  // YAW
        GInput->mouseDelta.x -= mouseSensibility * (window_height / 2 - y);
        GInput->mouseDelta.y -= mouseSensibility * (window_width / 2 - x);  // YAW
        setCursorPos(window_width / 2, window_height / 2);
//        printf("dx: %f, dy: %f\n", camera.wx, camera.wy);
    }
}

HCURSOR curNone, curArrow;

void inputInit() {
    curArrow = (HCURSOR)GetClassLong(hwnd, GCL_HCURSOR);

    uint8 maskA[32 * 32];
    uint8 maskX[32 * 32];
    memset(maskA, 0xFF, sizeof(maskA));
    memset(maskX, 0x00, sizeof(maskX));
    curNone = CreateCursor(0, 0, 0, 32, 32, maskA, maskX);
}

POINT center = { 0, 0 };

void inputUpdate() {
    //GInput->mouseDelta = vec3(0.0f);

    POINT p, delta;
    GetCursorPos(&p);

    delta.x = p.x - center.x;
    delta.y = p.y - center.y;

    ScreenToClient(hwnd, &p);
    GInput->mousePos = vec3((float)p.x, (float)p.y, 0.0f);

    if (GInput->mouseCaptured) {
        RECT r;
        GetWindowRect(hwnd, &r);
        center.x = (r.left + r.right) / 2;
        center.y = (r.top + r.bottom) / 2;

	GInput->mouseDelta.x += (GInput->invertMouse? 1 : -1) * GInput->mouseSensibility * (float)delta.y;
	GInput->mouseDelta.y += (GInput->invertMouse? 1 : -1) * GInput->mouseSensibility * (float)delta.x;

        SetCursorPos(center.x, center.y);

        SetClassLong(hwnd, GCL_HCURSOR, (LONG)curNone);
    } else
        SetClassLong(hwnd, GCL_HCURSOR, (LONG)curArrow);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_ACTIVATE :
	{
            //GInput->init();
            break;
        }
            
        case WM_KEYUP:
        case WM_KEYDOWN:
	{
            GInput->keyStates[wParam] = message != WM_KEYUP;
            if (VK_ESCAPE == wParam)
                GInput->mouseCaptured = false;
                //DestroyWindow(hWnd);
            break;
	}
        case WM_LBUTTONDOWN   :
        case WM_LBUTTONDBLCLK :
        case WM_LBUTTONUP     :
	{
            GInput->keyStates[VK_LBUTTON] = message != WM_LBUTTONUP;
	    GInput->mouseStates[MOUSE_LEFT] = message != WM_LBUTTONUP;
            GInput->mouseCaptured = true;
            break;
	}
        case WM_RBUTTONDOWN   :
        case WM_RBUTTONDBLCLK :
        case WM_RBUTTONUP     :
	{
            GInput->keyStates[VK_RBUTTON] = message != WM_RBUTTONUP;
	    GInput->mouseStates[MOUSE_RIGHT] = message != WM_RBUTTONUP;
            break;
	}
        case WM_MBUTTONDOWN   :
        case WM_MBUTTONDBLCLK :
        case WM_MBUTTONUP     :
	{
            GInput->keyStates[VK_MBUTTON] = message != WM_MBUTTONUP;
            break;
	}
        case WM_SIZE :
	{
            //Context::width  = LOWORD(lParam);
            //Context::height = HIWORD(lParam);
            break;
	}
        case WM_DESTROY:
        {
            HDC hdc = GetDC(hwnd);
            HGLRC hglrc = wglGetCurrentContext();
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
            ReleaseDC(hwnd, hdc);

            PostQuitMessage(0);
            break;
        }
        case WM_CLOSE:
	{
            DestroyWindow(hwnd);
            break;
	}
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}


int main(int argc, char* argv[])
{
    int x, y;
    RECT rect;
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = 0;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = "Win32 Game Window";
    RegisterClassEx(&wndclass);


    x = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
    y = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;

    rect.left = x;
    rect.right = x + window_width;
    rect.top = y;
    rect.bottom = y + window_height;


    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); // WS_THICKFRAME to resize
    AdjustWindowRectEx(&rect, style, FALSE, 0);
    hwnd = CreateWindowEx(0, wndclass.lpszClassName, "Game Window", style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, NULL);
    HDC hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempRC);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB, 0,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0,
    };

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(hdc, hglrc);

    init_opengl_extensions();

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    GetClientRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    SetCursorPos(x + width / 2, y + height / 2);


    if (!ZATON_INPUT_Init())
    {
	exit(EXIT_FAILURE);
    }

    game = new Game(window_width, window_height);
    if (!game->initGame())
    {
	exit(EXIT_FAILURE);
    }
    
    Initialise_Input(timer);

    inputInit();

    
// ---- game loop
    float fps_timer = 0.0f;
    __int32 n_frames = 0;
    __int32 frame_rate = 0;
    MSG msg;

    while (!quit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
            quit = true;

// ------------ timer do update
    
        __int32 n_steps = Update_Timer(timer);

        fps_timer += timer.real_delta_time;

        bool is_one_second = fps_timer >= 1.0f;
        fps_timer -= is_one_second ? 1.0f : 0.0f;

        frame_rate = is_one_second ? n_frames : frame_rate;
        n_frames = is_one_second ? 0 : n_frames;

        for (__int32 i_step = 0; i_step < n_steps; i_step++) {

// -------------- update transformation

                deltaTime = timer.fixed_step;
                inputUpdate();
                game->update();


                timer.accumulator -= timer.fixed_step;
                timer.frame_count++;
                n_frames++;

        }

        game->render();

        SwapBuffers(hdc);
/*
        if (vsynch != 0) {
            glFinish();
        }

*/
    }

    ZATON_INPUT_Release();
    delete game;
    game = NULL;

    return msg.wParam;
}
