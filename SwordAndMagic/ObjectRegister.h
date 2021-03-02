/**
 * @file ObjectRegister.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Singleton.h"

constexpr int MAX_OBJECT = 1024; //!< @brief 同時に存在できるオブジェクトの最大数

 /**
  * @class ObjectRegister
  * @brief オブジェクト登録用のシングルトンクラス。
  *        生成するオブジェクトに番号を振り分け、破棄されるオブジェクトの番号は返却する。
  */
class ObjectRegister : public Singleton<ObjectRegister> {
private:
	bool objNumberList[MAX_OBJECT]; //!< @brief オブジェクト番号管理用配列
public:
	ObjectRegister();
	~ObjectRegister();
	int RegisterObject();
	void UnregisterObject(int objNo);
};
