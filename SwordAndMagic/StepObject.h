/**
 * @file StepObject.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once

typedef unsigned short STEPNUM; 

 /**
  * @class C_StepObject
  * @brief ステップ実行管理用のコンポジション専用クラス。ステップは使用するクラス側で列挙定義して適宜キャストして操作する。
  */
class C_StepObject {
private:
	STEPNUM m_Step; //!< @brief unsigned short型。ステップを示す変数
	STEPNUM m_MaxStep; //!< @brief unsigned short型。最大ステップを保存しておく変数。
	bool m_StepFirst; //!< @brief そのステップが初回実行かどうかを保存しておく変数。
public:
	C_StepObject(STEPNUM maxStep);
	~C_StepObject();
	bool CheckStepFirst();
	bool GetStepFirst();
	STEPNUM GetStep();
	void SetStep(STEPNUM step);
	void SetStepNext();
};
