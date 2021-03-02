/**
 * @file Component.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "main.h"

class C_GameObject;

/**
 * @class C_Component
 * @brief コンポーネントのベースクラス。継承前提なので基本的に中身は何も無し。
		　全ての関数は純粋仮想関数ではなく空実装となっているので、オーバーライドは自由。
 */
class C_Component {
protected:
public:
	C_Component() { return; }
	virtual ~C_Component() { return;  }
	virtual void Start(void) { return; }
	virtual void Update(void) { return; }
	virtual void Draw(void) { return; }
	virtual void OnDestroy(void) { return; }
	virtual void OnCollision(C_GameObject* col) { return; }
	virtual void OnClick() { return; }
};