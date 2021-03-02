/**
 * @file Animator.h
 * @brief fbx���A�j���[�V���������邽�߂Ɏg�p�BModelData���Ŏg���̂ŃR���|�[�l���g�p���ł͂Ȃ��B
 * @author MIURA DAIKI
 * @date 2020/01/18
 */
#pragma once
#include "main.h"
#include "FbxModel.h"
#include "Singleton.h"
#include <string>
#include <vector>
#include <map>
#include <memory>


class C_Transistor;
 /**
  * @class C_Animator
  * @brief fbx�̃A�j���[�V�����p
  */
class C_Animator {
private:
	LPCSTR m_currentAnimation;
	int	 m_currentAnimFrame;
	int	 m_currentAnimSpeed;
	std::map<LPCSTR, int> m_AnimSpeedList;	//!< @brief �A�j���[�V�������x�Ǘ��p
	std::map<LPCSTR, std::vector<std::unique_ptr<C_Transistor>>> m_transistList; //!< @brief �J�ڐ�A�������X�g
	std::map<std::string, bool> m_bValueList; //!< @brief bool�l���X�g
	std::map<std::string, int> m_nValueList; //!< @brief int�l���X�g
	std::map<std::string, float> m_fValueList; //!< @brief float�l���X�g
public:
	C_Animator(LPCSTR currentAnim);
	~C_Animator();
	void Update();
	LPCSTR GetCurrentAnimation();
	bool GetBool(std::string name);
	int GetInt(std::string name);
	float GetFloat(std::string name);
	void SetBool(std::string name, bool value);
	void SetInt(std::string name, int value);
	void SetFloat(std::string name, float value);
	void AddTransister(LPCSTR modelName, std::unique_ptr<C_Transistor> transister);
	void SetAnimSpeed(LPCSTR modelName, int speed);
	int GetCurrentAnimFrame();
	void SetCurrentAnimSpeed(int speed);
	bool GetAnimEnd();
};

/**
 * @class C_Transistor
 * @brief �A�j���[�V�����̑J�ڊǗ��p
 */
class C_Transistor {
private:
	LPCSTR m_nextAnimation; //!< @brief �J�ڌ�̃A�j���[�V����
	std::string m_valueName; //!< @brief �J�ڗp�X�e�[�g�̖��O
	bool isAuto = false;	//!< @brief true�Ȃ玩���J��
	bool m_valueBool;	//!< @brief �J�ڏ���(bool)
	int m_valueInt;	//!< @brief �J�ڏ���(int)
	float m_valueFloat;	//!< @brief �J�ڏ���(float)
	int m_valueType = 0; 	//!< @brief �J�ڏ����^�C�v�ۑ��p
	bool isOnce = false;
public:
	C_Transistor(LPCSTR nextAnimName,bool doOnce = false);
	C_Transistor(LPCSTR nextAnimName,std::string valueName,bool value, bool doOnce = false);
	C_Transistor(LPCSTR nextAnimName,std::string valueName, int value, bool doOnce = false);
	C_Transistor(LPCSTR nextAnimName,std::string valueName, float value, bool doOnce = false);
	~C_Transistor();
	bool Judge(C_Animator* animator);
	LPCSTR getNextAnimation();
};