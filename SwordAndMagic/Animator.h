/**
 * @file Animator.h
 * @brief fbxをアニメーションさせるために使用。ModelData内で使うのでコンポーネント継承ではない。
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
  * @brief fbxのアニメーション用
  */
class C_Animator {
private:
	LPCSTR m_currentAnimation;
	int	 m_currentAnimFrame;
	int	 m_currentAnimSpeed;
	std::map<LPCSTR, int> m_AnimSpeedList;	//!< @brief アニメーション速度管理用
	std::map<LPCSTR, std::vector<std::unique_ptr<C_Transistor>>> m_transistList; //!< @brief 遷移先、条件リスト
	std::map<std::string, bool> m_bValueList; //!< @brief bool値リスト
	std::map<std::string, int> m_nValueList; //!< @brief int値リスト
	std::map<std::string, float> m_fValueList; //!< @brief float値リスト
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
 * @brief アニメーションの遷移管理用
 */
class C_Transistor {
private:
	LPCSTR m_nextAnimation; //!< @brief 遷移後のアニメーション
	std::string m_valueName; //!< @brief 遷移用ステートの名前
	bool isAuto = false;	//!< @brief trueなら自動遷移
	bool m_valueBool;	//!< @brief 遷移条件(bool)
	int m_valueInt;	//!< @brief 遷移条件(int)
	float m_valueFloat;	//!< @brief 遷移条件(float)
	int m_valueType = 0; 	//!< @brief 遷移条件タイプ保存用
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