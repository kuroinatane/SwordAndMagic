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
  * @brief �V�[���ύX�A���݂̃V�[���̎擾�݂̂�S���N���X�B���̂�Controller�ɂ���̂�Changer�ɂ���̂��l����
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
	std::unique_ptr<C_Scene> currentScene; //!< @brief ���݂̃V�[���ւ̃|�C���^
	GameScene  currentSceneNumber;//!< @brief ���݂̃V�[���ԍ�
	GameScene  nextSceneNumber;//!< @brief ���݂̃V�[���ԍ�
};