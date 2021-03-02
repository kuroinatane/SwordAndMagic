/**
 * @file Input.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Main.h"
#include "Singleton.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
constexpr char VK_0	= '0';	//!< @brief　キーボードのキー入力
constexpr char VK_1	= '1';	//!< @brief　キーボードのキー入力
constexpr char VK_2	= '2';	//!< @brief　キーボードのキー入力
constexpr char VK_3	= '3';	//!< @brief　キーボードのキー入力
constexpr char VK_4	= '4';	//!< @brief　キーボードのキー入力
constexpr char VK_5	= '5';	//!< @brief　キーボードのキー入力
constexpr char VK_6	= '6';	//!< @brief　キーボードのキー入力
constexpr char VK_7	= '7';	//!< @brief　キーボードのキー入力
constexpr char VK_8	= '8';	//!< @brief　キーボードのキー入力
constexpr char VK_9	= '9';	//!< @brief　キーボードのキー入力
constexpr char VK_A	= 'A';	//!< @brief　キーボードのキー入力
constexpr char VK_B	= 'B';	//!< @brief　キーボードのキー入力
constexpr char VK_C	= 'C';	//!< @brief　キーボードのキー入力
constexpr char VK_D	= 'D';	//!< @brief　キーボードのキー入力
constexpr char VK_E	= 'E';	//!< @brief　キーボードのキー入力
constexpr char VK_F	= 'F';	//!< @brief　キーボードのキー入力
constexpr char VK_G	= 'G';	//!< @brief　キーボードのキー入力
constexpr char VK_H	= 'H';	//!< @brief　キーボードのキー入力
constexpr char VK_I	= 'I';	//!< @brief　キーボードのキー入力
constexpr char VK_J	= 'J';	//!< @brief　キーボードのキー入力
constexpr char VK_K	= 'K';	//!< @brief　キーボードのキー入力
constexpr char VK_L	= 'L';	//!< @brief　キーボードのキー入力
constexpr char VK_M	= 'M';	//!< @brief　キーボードのキー入力
constexpr char VK_N	= 'N';	//!< @brief　キーボードのキー入力
constexpr char VK_O	= 'O';	//!< @brief　キーボードのキー入力
constexpr char VK_P	= 'P';	//!< @brief　キーボードのキー入力
constexpr char VK_Q	= 'Q';	//!< @brief　キーボードのキー入力
constexpr char VK_R	= 'R';	//!< @brief　キーボードのキー入力
constexpr char VK_S	= 'S';	//!< @brief　キーボードのキー入力
constexpr char VK_T	= 'T';	//!< @brief　キーボードのキー入力
constexpr char VK_U	= 'U';	//!< @brief　キーボードのキー入力
constexpr char VK_V	= 'V';	//!< @brief　キーボードのキー入力
constexpr char VK_W	= 'W';	//!< @brief　キーボードのキー入力
constexpr char VK_X	= 'X';	//!< @brief　キーボードのキー入力
constexpr char VK_Y	= 'Y';	//!< @brief　キーボードのキー入力
constexpr char VK_Z	= 'Z';	//!< @brief　キーボードのキー入力

// JOYSTICKID1,2は<mmsystem.h>で定義済
//#define JOYSTICKID1		0
//#define JOYSTICKID2		1
constexpr char JOYSTICKID3		= 2 ; //!< @brief　パッド入力
constexpr char JOYSTICKID4		= 3	; //!< @brief　パッド入力
constexpr char JOYSTICKID5		= 4	; //!< @brief　パッド入力
constexpr char JOYSTICKID6		= 5	; //!< @brief　パッド入力
constexpr char JOYSTICKID7		= 6	; //!< @brief　パッド入力
constexpr char JOYSTICKID8		= 7	; //!< @brief　パッド入力
constexpr char JOYSTICKID9		= 8	; //!< @brief　パッド入力
constexpr char JOYSTICKID10	= 9	;     //!< @brief　パッド入力
constexpr char JOYSTICKID11	= 10;     //!< @brief　パッド入力
constexpr char JOYSTICKID12	= 11;     //!< @brief　パッド入力
constexpr char JOYSTICKID13	= 12;     //!< @brief　パッド入力
constexpr char JOYSTICKID14	= 13;     //!< @brief　パッド入力
constexpr char JOYSTICKID15	= 14;     //!< @brief　パッド入力
constexpr char JOYSTICKID16	= 15;     //!< @brief　パッド入力
constexpr char MOUSEBUTTON_L	= 0	; //!< @brief　マウス入力
constexpr char MOUSEBUTTON_R	= 1	; //!< @brief　マウス入力
constexpr char MOUSEBUTTON_M	= 2	; //!< @brief　マウス入力
constexpr char MOUSEBUTTON_X1	= 3	; //!< @brief　マウス入力
constexpr char MOUSEBUTTON_X2	= 4	; //!< @brief　マウス入力
constexpr int NUM_KEY_MAX = 256;		//!< @brief キー最大数
constexpr int NUM_JOY_MAX = 32;		//!< @brief ゲームパッド最大数

 /**
  * @class InputManager
  * @brief 入力管理用のシングルトンクラス。
  *		   現在は用意していないが、入力を変更できるようにしたりする場合はその辺りもこちらに定義するといいかも。
  */
class InputManager : public Singleton<InputManager>
{
private:
	WORD				m_wKeyState[NUM_KEY_MAX];			//!< @brief キーボードの押下状態を保持するワーク
	WORD				m_wKeyStateTrigger[NUM_KEY_MAX];	//!< @brief キーボードのトリガ状態を保持するワーク
	WORD				m_wKeyStateRelease[NUM_KEY_MAX];	//!< @brief キーボードのリリース状態を保持するワーク
	WORD				m_wKeyStateRepeat[NUM_KEY_MAX];		//!< @brief キーボードのリピート状態を保持するワーク
	int				m_nKeyStateRepeatCnt[NUM_KEY_MAX];	//!< @brief キーボードのリピートカウンタ

	DWORD			m_dwJoyCount; //!< @brief 接続されているゲームパッド数を保持
	JOYINFOEX		m_joyState[NUM_JOY_MAX]; //!< @brief 各ゲームパッドの押下状態を保持
	DWORD			m_dwJoyButtonTrigger[NUM_JOY_MAX]; //!< @brief 各ゲームパッドのトリガ状態を保持
	DWORD			m_dwJoyButtonRelease[NUM_JOY_MAX]; //!< @brief 各ゲームパッドのリリース状態を保持

	POINT			m_ptMouse; //!< @brief マウス座標

private:
	HRESULT InitKeyboard();
	void UninitKeyboard();
	HRESULT UpdateKeyboard();

	HRESULT InitJoystick();
	void UninitJoystick();
	HRESULT UpdateJoystick();

	HRESULT InitMouse();
	void UninitMouse();
	HRESULT UpdateMouse();

public:
	HRESULT Init();
	void Uninit();
	void Update();

	bool GetKeyPress(int nKey);
	bool GetKeyTrigger(int nKey);
	bool GetKeyRelease(int nKey);
	bool GetKeyRepeat(int nKey);

	DWORD GetJoyCount();
	JOYINFOEX* GetJoyState(DWORD dwJoy);
	LONG GetJoyX(DWORD dwJoy);
	LONG GetJoyY(DWORD dwJoy);
	LONG GetJoyZ(DWORD dwJoy);
	bool GetJoyButton(DWORD dwJoy, DWORD dwBtn);
	bool GetJoyTrigger(DWORD dwJoy, DWORD dwBtn);
	bool GetJoyRelease(DWORD dwJoy, DWORD dwBtn);

	POINT* GetMousePosition();
	bool GetMouseButton(DWORD dwBtn);
	bool GetMouseTrigger(DWORD dwBtn);
	bool GetMouseRelease(DWORD dwBtn);
};

//=======================================================================================
//	End of File
//=======================================================================================
