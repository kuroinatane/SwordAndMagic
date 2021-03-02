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
 * @brief タイトルシーン用クラス。ベースシーンを継承。
 */
class C_SceneGame : public C_Scene {
private:
public:
	C_SceneGame();
	~C_SceneGame();
	void SpawnSceneObject();
};