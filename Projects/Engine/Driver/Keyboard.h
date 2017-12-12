
#ifndef _Driver_Keyboard_h_
#define _Driver_Keyboard_h_


// Library includes
#include <windows.h>

// Project includes

// Forward declarations

// Namespace declarations


enum KEY_asciiCode
{
	KeyCode_UNKNOWN		= 0,
	KeyCode_L_BUTTON	= VK_LBUTTON,
	KeyCode_R_BUTTON	= VK_RBUTTON, 

	KeyCode_F1			= VK_F1,
	KeyCode_F2,
	KeyCode_F3,
	KeyCode_F4,
	KeyCode_F5,
	KeyCode_F6,
	KeyCode_F7,
	KeyCode_F8,
	KeyCode_F9,
	KeyCode_F10,
	KeyCode_F11,
	KeyCode_F12,
	KeyCode_lastSpecialKey,

	KeyCode_ESCAPE		= VK_ESCAPE,
	KeyCode_SPACE		= VK_SPACE,
	KeyCode_PLUS		= VK_ADD,
	KeyCode_MINUS		= VK_SUBTRACT,

	KeyCode_a			= 97,
	KeyCode_b,
	KeyCode_c,
	KeyCode_d,
	KeyCode_e,
	KeyCode_f,
	KeyCode_g,
	KeyCode_h,
	KeyCode_i,
	KeyCode_j,
	KeyCode_k,
	KeyCode_l,
	KeyCode_m,
	KeyCode_n,
	KeyCode_o,
	KeyCode_p,
	KeyCode_q,
	KeyCode_r,
	KeyCode_s,
	KeyCode_t,
	KeyCode_u,
	KeyCode_v,
	KeyCode_w,
	KeyCode_x,
	KeyCode_y,
	KeyCode_z,
};


bool isKeyDown(int keycode);


#endif
