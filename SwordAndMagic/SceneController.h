#pragma once
#include "Singleton.h"
#include "Scene.h"
#include <memory>
/**
 * @file SceneController.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */

class C_Camera;
 /**
  * @class SceneController
  * @brief シーン変更、現在のシーンの取得のみを担うクラス。名称をControllerにするのかChangerにするのか考え中
  */
class SceneController : public Singleton<SceneController> {
public:
	SceneController();
	~SceneController();
	enum GameScene {
		Scene_None,
		Scene_Debug,
		Scene_Title,
		Scene_Game
	};
	void SwapScene();
	void ChangeScene(GameScene scene);
	C_Scene* GetCurrentScene();
	GameScene GetCurrentSceneNumber();
private:
	std::unique_ptr<C_Scene> currentScene; //!< @brief 現在のシーンへのポインタ
	GameScene  currentSceneNumber;//!< @brief 現在のシーン番号
	GameScene  nextSceneNumber;//!< @brief 現在のシーン番号
};