/**
 * @file SceneGame.h
 * @author MIURA DAIKI
 * @date 2020/12/07
 */
#pragma once
#include "Scene.h"

class C_GameObject;
/**
 * @class C_SceneGame
 * @brief �^�C�g���V�[���p�N���X�B�x�[�X�V�[�����p���B
 */
class C_SceneGame : public C_Scene {
private:
public:
	C_SceneGame();
	~C_SceneGame();
	void SpawnSceneObject();
};