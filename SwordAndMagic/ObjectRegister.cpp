#include "ObjectRegister.h"

/**
 * @brief コンストラクタ。オブジェクトの番号管理用配列を初期化します
 */
ObjectRegister::ObjectRegister() {
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (objNumberList[i]) continue;
		objNumberList[i] = false;
	}
}

/**
 * @brief デストラクタ。現状特にやることはありません。
 */
ObjectRegister::~ObjectRegister() {

}

/**
 * @brief オブジェクトを番号で登録します。同じ番号のオブジェクトは同時には存在できません。
 * @return 登録されたオブジェクト番号
 */
int ObjectRegister::RegisterObject() {
	for (int i = 0; i < MAX_OBJECT; i++) {
		if (objNumberList[i]) continue;
		objNumberList[i] = true;
		return i;
	}
	return -1;//エラー
}

/**
 * @brief オブジェクトの登録を解除します。オブジェクトが削除された時に。
 * @param[in] 登録解除するオブジェクト番号
 */
void ObjectRegister::UnregisterObject(int objNo) {
	if (objNo < 0 || objNo >= MAX_OBJECT) return;
	objNumberList[objNo] = false;
}