#ifndef _Common_SDLCompat_h_
#define _Common_SDLCompat_h_

// SDL to Windows compatibility layer
// This header provides Windows-like definitions when using SDL
// to ease the transition from Windows-specific code

#include <SDL2/SDL.h>

// Windows message types - mapped to custom values for compatibility
#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101
#define WM_LBUTTONDOWN      0x0201
#define WM_LBUTTONUP        0x0202
#define WM_RBUTTONDOWN      0x0204
#define WM_RBUTTONUP        0x0205
#define WM_MBUTTONDOWN      0x0207
#define WM_MBUTTONUP        0x0208
#define WM_LBUTTONDBLCLK    0x0203
#define WM_RBUTTONDBLCLK    0x0206
#define WM_MBUTTONDBLCLK    0x0209
#define WM_MOUSEMOVE        0x0200
#define WM_CLOSE            0x0010
#define WM_QUIT             0x0012
#define WM_PAINT            0x000F
#define WM_SIZE             0x0005
#define WM_MOVE             0x0003
#define WM_ACTIVATE         0x0006
#define WM_SYSCOMMAND       0x0112

// Window activation states
#define WA_INACTIVE         0
#define WA_ACTIVE           1
#define WA_CLICKACTIVE      2

// System commands
#define SC_SCREENSAVE       0xF140
#define SC_MONITORPOWER     0xF170

// Virtual key codes - map SDL keys to Windows VK codes
#define VK_SPACE            SDLK_SPACE
#define VK_RETURN           SDLK_RETURN
#define VK_ESCAPE           SDLK_ESCAPE
#define VK_TAB              SDLK_TAB
#define VK_BACK             SDLK_BACKSPACE
#define VK_DELETE           SDLK_DELETE
#define VK_LEFT             SDLK_LEFT
#define VK_RIGHT            SDLK_RIGHT
#define VK_UP               SDLK_UP
#define VK_DOWN             SDLK_DOWN
#define VK_HOME             SDLK_HOME
#define VK_END              SDLK_END
#define VK_PRIOR            SDLK_PAGEUP
#define VK_NEXT             SDLK_PAGEDOWN
#define VK_INSERT           SDLK_INSERT
#define VK_SHIFT            SDLK_LSHIFT
#define VK_CONTROL          SDLK_LCTRL
#define VK_MENU             SDLK_LALT

// Function keys
#define VK_F1               SDLK_F1
#define VK_F2               SDLK_F2
#define VK_F3               SDLK_F3
#define VK_F4               SDLK_F4
#define VK_F5               SDLK_F5
#define VK_F6               SDLK_F6
#define VK_F7               SDLK_F7
#define VK_F8               SDLK_F8
#define VK_F9               SDLK_F9
#define VK_F10              SDLK_F10
#define VK_F11              SDLK_F11
#define VK_F12              SDLK_F12

// Number keys
#define VK_0                SDLK_0
#define VK_1                SDLK_1
#define VK_2                SDLK_2
#define VK_3                SDLK_3
#define VK_4                SDLK_4
#define VK_5                SDLK_5
#define VK_6                SDLK_6
#define VK_7                SDLK_7
#define VK_8                SDLK_8
#define VK_9                SDLK_9

// Letter keys
#define VK_A                SDLK_a
#define VK_B                SDLK_b
#define VK_C                SDLK_c
#define VK_D                SDLK_d
#define VK_E                SDLK_e
#define VK_F                SDLK_f
#define VK_G                SDLK_g
#define VK_H                SDLK_h
#define VK_I                SDLK_i
#define VK_J                SDLK_j
#define VK_K                SDLK_k
#define VK_L                SDLK_l
#define VK_M                SDLK_m
#define VK_N                SDLK_n
#define VK_O                SDLK_o
#define VK_P                SDLK_p
#define VK_Q                SDLK_q
#define VK_R                SDLK_r
#define VK_S                SDLK_s
#define VK_T                SDLK_t
#define VK_U                SDLK_u
#define VK_V                SDLK_v
#define VK_W                SDLK_w
#define VK_X                SDLK_x
#define VK_Y                SDLK_y
#define VK_Z                SDLK_z

// Helper macros for mouse position extraction
#define LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define MAKELPARAM(l, h)    ((long)(((unsigned short)(l)) | ((unsigned long)((unsigned short)(h))) << 16))

// Type definitions for compatibility
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
typedef long                LONG;
typedef int                 BOOL;
typedef void*               LPVOID;
typedef char*               LPSTR;
typedef const char*         LPCSTR;

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

// Point structure for compatibility
typedef struct tagPOINT {
    long x;
    long y;
} POINT, *PPOINT, *LPPOINT;

// Rectangle structure for compatibility
typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT, *LPRECT;

// Paint structure for compatibility
typedef struct tagPAINTSTRUCT {
    void* hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *LPPAINTSTRUCT;

// Message structure for compatibility
typedef struct tagMSG {
    void* hwnd;
    unsigned int message;
    unsigned long wParam;
    long lParam;
    DWORD time;
    POINT pt;
} MSG, *PMSG, *LPMSG;

// Inline functions to replace Windows API calls
inline BOOL GetCursorPos(POINT* lpPoint) {
    if (!lpPoint) return FALSE;
    SDL_GetMouseState(&lpPoint->x, &lpPoint->y);
    return TRUE;
}

inline BOOL SetCursorPos(int x, int y) {
    SDL_WarpMouseInWindow(nullptr, x, y);
    return TRUE;
}

inline DWORD GetLastError() {
    // SDL doesn't have a direct equivalent, return generic error
    return 0;
}

inline void* BeginPaint(void* hwnd, PAINTSTRUCT* lpPaint) {
    // SDL doesn't need paint structs, return dummy
    if (lpPaint) {
        memset(lpPaint, 0, sizeof(PAINTSTRUCT));
    }
    return (void*)1; // Non-null return
}

inline BOOL EndPaint(void* hwnd, const PAINTSTRUCT* lpPaint) {
    // SDL doesn't need paint cleanup
    return TRUE;
}

inline void PostQuitMessage(int nExitCode) {
    SDL_Event quit_event;
    quit_event.type = SDL_QUIT;
    SDL_PushEvent(&quit_event);
}

inline BOOL GetClientRect(void* hwnd, RECT* lpRect) {
    if (!lpRect) return FALSE;
    
    // Get the current window size from SDL
    SDL_Window* window = SDL_GetMouseFocus();
    if (window) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        lpRect->left = 0;
        lpRect->top = 0;
        lpRect->right = w;
        lpRect->bottom = h;
    } else {
        // Fallback values
        lpRect->left = 0;
        lpRect->top = 0;
        lpRect->right = 800;
        lpRect->bottom = 600;
    }
    return TRUE;
}

// Memory debugging compatibility (no-ops for non-Windows)
inline void _CrtSetDbgFlag(int flag) {
    // No-op on non-Windows platforms
}

#define _CRTDBG_ALLOC_MEM_DF    0x01
#define _CRTDBG_LEAK_CHECK_DF   0x20

#endif // _Common_SDLCompat_h_