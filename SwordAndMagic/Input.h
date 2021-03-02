/**
 * @file Input.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Main.h"
#include "Singleton.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
constexpr char VK_0	= '0';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_1	= '1';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_2	= '2';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_3	= '3';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_4	= '4';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_5	= '5';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_6	= '6';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_7	= '7';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_8	= '8';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_9	= '9';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_A	= 'A';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_B	= 'B';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_C	= 'C';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_D	= 'D';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_E	= 'E';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_F	= 'F';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_G	= 'G';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_H	= 'H';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_I	= 'I';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_J	= 'J';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_K	= 'K';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_L	= 'L';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_M	= 'M';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_N	= 'N';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_O	= 'O';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_P	= 'P';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_Q	= 'Q';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_R	= 'R';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_S	= 'S';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_T	= 'T';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_U	= 'U';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_V	= 'V';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_W	= 'W';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_X	= 'X';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_Y	= 'Y';	//!< @brief�@�L�[�{�[�h�̃L�[����
constexpr char VK_Z	= 'Z';	//!< @brief�@�L�[�{�[�h�̃L�[����

// JOYSTICKID1,2��<mmsystem.h>�Œ�`��
//#define JOYSTICKID1		0
//#define JOYSTICKID2		1
constexpr char JOYSTICKID3		= 2 ; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID4		= 3	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID5		= 4	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID6		= 5	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID7		= 6	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID8		= 7	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID9		= 8	; //!< @brief�@�p�b�h����
constexpr char JOYSTICKID10	= 9	;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID11	= 10;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID12	= 11;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID13	= 12;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID14	= 13;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID15	= 14;     //!< @brief�@�p�b�h����
constexpr char JOYSTICKID16	= 15;     //!< @brief�@�p�b�h����
constexpr char MOUSEBUTTON_L	= 0	; //!< @brief�@�}�E�X����
constexpr char MOUSEBUTTON_R	= 1	; //!< @brief�@�}�E�X����
constexpr char MOUSEBUTTON_M	= 2	; //!< @brief�@�}�E�X����
constexpr char MOUSEBUTTON_X1	= 3	; //!< @brief�@�}�E�X����
constexpr char MOUSEBUTTON_X2	= 4	; //!< @brief�@�}�E�X����
constexpr int NUM_KEY_MAX = 256;		//!< @brief �L�[�ő吔
constexpr int NUM_JOY_MAX = 32;		//!< @brief �Q�[���p�b�h�ő吔

 /**
  * @class InputManager
  * @brief ���͊Ǘ��p�̃V���O���g���N���X�B
  *		   ���݂͗p�ӂ��Ă��Ȃ����A���͂�ύX�ł���悤�ɂ����肷��ꍇ�͂��̕ӂ��������ɒ�`����Ƃ��������B
  */
class InputManager : public Singleton<InputManager>
{
private:
	WORD				m_wKeyState[NUM_KEY_MAX];			//!< @brief �L�[�{�[�h�̉�����Ԃ�ێ����郏�[�N
	WORD				m_wKeyStateTrigger[NUM_KEY_MAX];	//!< @brief �L�[�{�[�h�̃g���K��Ԃ�ێ����郏�[�N
	WORD				m_wKeyStateRelease[NUM_KEY_MAX];	//!< @brief �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N
	WORD				m_wKeyStateRepeat[NUM_KEY_MAX];		//!< @brief �L�[�{�[�h�̃��s�[�g��Ԃ�ێ����郏�[�N
	int				m_nKeyStateRepeatCnt[NUM_KEY_MAX];	//!< @brief �L�[�{�[�h�̃��s�[�g�J�E���^

	DWORD			m_dwJoyCount; //!< @brief �ڑ�����Ă���Q�[���p�b�h����ێ�
	JOYINFOEX		m_joyState[NUM_JOY_MAX]; //!< @brief �e�Q�[���p�b�h�̉�����Ԃ�ێ�
	DWORD			m_dwJoyButtonTrigger[NUM_JOY_MAX]; //!< @brief �e�Q�[���p�b�h�̃g���K��Ԃ�ێ�
	DWORD			m_dwJoyButtonRelease[NUM_JOY_MAX]; //!< @brief �e�Q�[���p�b�h�̃����[�X��Ԃ�ێ�

	POINT			m_ptMouse; //!< @brief �}�E�X���W

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
