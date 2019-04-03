#ifndef UGKEYDEF_H
#define UGKEYDEF_H

/* Key State Masks for Mouse Messages*/ 
#define UG_MK_LBUTTON          0x0001
#define UG_MK_RBUTTON          0x0002
#define UG_MK_SHIFT            0x0004
#define UG_MK_CONTROL          0x0008
#define UG_MK_MBUTTON          0x0010
#define UG_MK_ALT              0x0020


 /* Virtual Keys, Standard Set*/
#define UG_VK_LBUTTON        0x01
#define UG_VK_RBUTTON        0x02
#define UG_VK_CANCEL         0x03
#define UG_VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define UG_VK_BACK           0x08
#define UG_VK_TAB            0x09

#define UG_VK_CLEAR          0x0C
#define UG_VK_RETURN         0x0D

//#define UG_VK_SHIFT          0x10
//#define UG_VK_CONTROL        0x11
//#define UG_VK_MENU           0x12

// 三维组添加组合健定义
#define UG_VK_SHIFT          0x01
#define UG_VK_CONTROL        0x02
#define UG_VK_MENU           0x04

#define UG_VK_PAUSE          0x13
#define UG_VK_CAPITAL        0x14

#define UG_VK_KANA           0x15
#define UG_VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define UG_VK_HANGUL         0x15
#define UG_VK_JUNJA          0x17
#define UG_VK_FINAL          0x18
#define UG_VK_HANJA          0x19
#define UG_VK_KANJI          0x19

#define UG_VK_ESCAPE         0x1B

#define UG_VK_CONVERT        0x1C
#define UG_VK_NONCONVERT     0x1D
#define UG_VK_ACCEPT         0x1E
#define UG_VK_MODECHANGE     0x1F

#define UG_VK_SPACE          0x20
#define UG_VK_PRIOR          0x21
#define UG_VK_NEXT           0x22
#define UG_VK_END            0x23
#define UG_VK_HOME           0x24
#define UG_VK_LEFT           0x25
#define UG_VK_UP             0x26
#define UG_VK_RIGHT          0x27
#define UG_VK_DOWN           0x28
#define UG_VK_SELECT         0x29
#define UG_VK_PRINT          0x2A
#define UG_VK_EXECUTE        0x2B
#define UG_VK_SNAPSHOT       0x2C
#define UG_VK_INSERT         0x2D
#define UG_VK_DELETE         0x2E
#define UG_VK_HELP           0x2F

/* UG_VK_0 thru UG_VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* UG_VK_A thru UG_VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define UG_VK_LWIN           0x5B
#define UG_VK_RWIN           0x5C
#define UG_VK_APPS           0x5D

#define UG_VK_NUMPAD0        0x60
#define UG_VK_NUMPAD1        0x61
#define UG_VK_NUMPAD2        0x62
#define UG_VK_NUMPAD3        0x63
#define UG_VK_NUMPAD4        0x64
#define UG_VK_NUMPAD5        0x65
#define UG_VK_NUMPAD6        0x66
#define UG_VK_NUMPAD7        0x67
#define UG_VK_NUMPAD8        0x68
#define UG_VK_NUMPAD9        0x69
#define UG_VK_MULTIPLY       0x6A
#define UG_VK_ADD            0x6B
#define UG_VK_SEPARATOR      0x6C
#define UG_VK_SUBTRACT       0x6D
#define UG_VK_DECIMAL        0x6E
#define UG_VK_DIVIDE         0x6F
#define UG_VK_F1             0x70
#define UG_VK_F2             0x71
#define UG_VK_F3             0x72
#define UG_VK_F4             0x73
#define UG_VK_F5             0x74
#define UG_VK_F6             0x75
#define UG_VK_F7             0x76
#define UG_VK_F8             0x77
#define UG_VK_F9             0x78
#define UG_VK_F10            0x79
#define UG_VK_F11            0x7A
#define UG_VK_F12            0x7B
#define UG_VK_F13            0x7C
#define UG_VK_F14            0x7D
#define UG_VK_F15            0x7E
#define UG_VK_F16            0x7F
#define UG_VK_F17            0x80
#define UG_VK_F18            0x81
#define UG_VK_F19            0x82
#define UG_VK_F20            0x83
#define UG_VK_F21            0x84
#define UG_VK_F22            0x85
#define UG_VK_F23            0x86
#define UG_VK_F24            0x87

#define UG_VK_NUMLOCK        0x90
#define UG_VK_SCROLL         0x91

/*
 * UG_VK_L* & UG_VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 */
#define UG_VK_LSHIFT         0xA0
#define UG_VK_RSHIFT         0xA1
#define UG_VK_LCONTROL       0xA2
#define UG_VK_RCONTROL       0xA3
#define UG_VK_LMENU          0xA4
#define UG_VK_RMENU          0xA5

#ifdef WINVER
#if(WINVER >= 0x0400)
#define UG_VK_PROCESSKEY     0xE5
#endif /* WINVER >= 0x0400 */
#endif

#define UG_VK_OEM_PLUS       0xBB   // '+'
#define UK_VK_OEM_MINUS      0xBD   // '-'

#define UG_VK_ATTN           0xF6
#define UG_VK_CRSEL          0xF7
#define UG_VK_EXSEL          0xF8
#define UG_VK_EREOF          0xF9
#define UG_VK_PLAY           0xFA
#define UG_VK_ZOOM           0xFB
#define UG_VK_NONAME         0xFC
#define UG_VK_PA1            0xFD
#define UG_VK_OEM_CLEAR      0xFE

#endif // UGKEYDEF_H

