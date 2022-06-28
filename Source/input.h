/*=====ヘッダコメント=====
*@プログラム名	：input.h
*@説明・概要	：キー入力などについて
*@製作者	：AT_12D_192_51吉田叶聖
*@更新者	：AT_12D_192_51吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
#pragma once
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VK_0 '0'
#define VK_1 '1'
#define VK_2 '2'
#define VK_3 '3'
#define VK_4 '4'
#define VK_5 '5'
#define VK_6 '6'
#define VK_7 '7'
#define VK_8 '8'
#define VK_9 '9'
#define VK_A 'A'
#define VK_B 'B'
#define VK_C 'C'
#define VK_D 'D'
#define VK_E 'E'
#define VK_F 'F'
#define VK_G 'G'
#define VK_H 'H'
#define VK_I 'I'
#define VK_J 'J'
#define VK_K 'K'
#define VK_L 'L'
#define VK_M 'M'
#define VK_N 'N'
#define VK_O 'O'
#define VK_P 'P'
#define VK_Q 'Q'
#define VK_R 'R'
#define VK_S 'S'
#define VK_T 'T'
#define VK_U 'U'
#define VK_V 'V'
#define VK_W 'W'
#define VK_X 'X'
#define VK_Y 'Y'
#define VK_Z 'Z'
#define JOYBUTTON1 0// A
#define JOYBUTTON2 1// B
#define JOYBUTTON3 2// Y
#define JOYBUTTON4 3// X
#define JOYBUTTON5 4// LB
#define JOYBUTTON6 5// RB
#define JOYBUTTON7 6// Back
#define JOYBUTTON8 7// Start
#define JOYBUTTON9 8// LStick押し込み
#define JOYBUTTON10 9// RStick押し込み
#define JOYBUTTON11 10
#define JOYBUTTON12 11
#define JOYBUTTON13 12
#define JOYBUTTON14 13
#define JOYBUTTON15 14
#define JOYBUTTON16 15
#define JOYBUTTON17 16
#define JOYBUTTON18 17
#define JOYBUTTON19 18
#define JOYBUTTON20 19
#define JOYBUTTON21 20
#define JOYBUTTON22 21
#define JOYBUTTON23 22
#define JOYBUTTON24 23
#define JOYBUTTON25 24
#define JOYBUTTON26 25
#define JOYBUTTON27 26
#define JOYBUTTON28 27
#define JOYBUTTON29 28
#define JOYBUTTON30 29
#define JOYBUTTON31 30
#define JOYBUTTON32 31
#define JOYSTICKID3 2
#define JOYSTICKID4 3
#define JOYSTICKID5 4
#define JOYSTICKID6 5
#define JOYSTICKID7 6
#define JOYSTICKID8 7
#define JOYSTICKID9 8
#define JOYSTICKID10 9
#define JOYSTICKID11 10
#define JOYSTICKID12 11
#define JOYSTICKID13 12
#define JOYSTICKID14 13
#define JOYSTICKID15 14
#define JOYSTICKID16 15
#define MOUSEBUTTON_L 0
#define MOUSEBUTTON_R 1
#define MOUSEBUTTON_M 2
#define MOUSEBUTTON_X1 3
#define MOUSEBUTTON_X2 4
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput();
void UninitInput();
void UpdateInput();
bool GetKeyPress(int nKey);
bool GetKeyTrigger(int nKey);
bool GetKeyRelease(int nKey);
bool GetKeyRepeat(int nKey);
DWORD GetJoyCount();
JOYINFOEX *GetJoyState(DWORD dwJoy);
LONG GetJoyX(DWORD dwJoy);
LONG GetJoyY(DWORD dwJoy);
LONG GetJoyZ(DWORD dwJoy);
bool GetJoyButton(DWORD dwJoy, DWORD dwBtn);
bool GetJoyTrigger(DWORD dwJoy, DWORD dwBtn);
bool GetJoyRelease(DWORD dwJoy, DWORD dwBtn);
POINT *GetMousePosition();
bool GetMouseButton(DWORD dwBtn);
bool GetMouseTrigger(DWORD dwBtn);
bool GetMouseRelease(DWORD dwBtn);