#include "SceneController.h"
#include "SceneTitle.h"
#include "SceneGame.h"

/**
 * @brief シーン情報を初期化する
 */
SceneController::SceneController() {
	currentSceneNumber = Scene_Title;
	nextSceneNumber = Scene_Title;
	currentScene = std::make_unique<C_SceneTitle>();
	//ChangeScene(Scene_Title);
	//SwapScene();
	/*currentSceneNumber = Scene_Debug;
	ChangeScene(Scene_Debug);*/
}

/**
 * @brief シーン情報は自動的に破棄されるので明示的に何かを行う必要は今の所ない
 */
SceneController::~SceneController() {

}

/**
 * @brief 予めChangeSceneで設定したシーンへ飛ばす。
 */
void SceneController::SwapScene() {
	if (currentSceneNumber != nextSceneNumber) {
		switch (nextSceneNumber) {
		case Scene_Title:
			//タイトルシーンの作成と後々入れ替える
			currentScene = std::make_unique<C_SceneTitle>();
			break;
		case Scene_Game:
			currentScene = std::make_unique<C_SceneGame>();
			break;
		default:
			break;
		}
		currentSceneNumber = nextSceneNumber;
		currentScene->Init();
	}
}
/**
 * @brief シーン番号を指定して変更。現在のシーンはunique_ptrの参照が切れるので自動的に破棄される。
 *		  フェードを挟む場合はどこで処理するか要検討
 * @param[in] scene 変更先のシーン番号
 */
void SceneController::ChangeScene(GameScene scene) {
	nextSceneNumber = scene;
}

/**
 * @brief 現在のシーンへの生ポインタを取得。他のクラスにポインタの所有権は渡さないので、他でdeleteをすることは禁止。
 * @return 使用中のシーンへの生ポインタ
 */
C_Scene* SceneController::GetCurrentScene() {
	return currentScene.get();
}

/**
 * @brief 現在のシーン番号を取得。どのシーンかがわかれば良いだけの時は基本的にこちらを使用すること。
 * @return 現在のシーン番号
 */
SceneController::GameScene SceneController::GetCurrentSceneNumber() {
	return currentSceneNumber;
}