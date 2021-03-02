#include "SceneController.h"
#include "SceneTitle.h"
#include "SceneGame.h"

/**
 * @brief �V�[����������������
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
 * @brief �V�[�����͎����I�ɔj�������̂Ŗ����I�ɉ������s���K�v�͍��̏��Ȃ�
 */
SceneController::~SceneController() {

}

/**
 * @brief �\��ChangeScene�Őݒ肵���V�[���֔�΂��B
 */
void SceneController::SwapScene() {
	if (currentSceneNumber != nextSceneNumber) {
		switch (nextSceneNumber) {
		case Scene_Title:
			//�^�C�g���V�[���̍쐬�ƌ�X����ւ���
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
 * @brief �V�[���ԍ����w�肵�ĕύX�B���݂̃V�[����unique_ptr�̎Q�Ƃ��؂��̂Ŏ����I�ɔj�������B
 *		  �t�F�[�h�����ޏꍇ�͂ǂ��ŏ������邩�v����
 * @param[in] scene �ύX��̃V�[���ԍ�
 */
void SceneController::ChangeScene(GameScene scene) {
	nextSceneNumber = scene;
}

/**
 * @brief ���݂̃V�[���ւ̐��|�C���^���擾�B���̃N���X�Ƀ|�C���^�̏��L���͓n���Ȃ��̂ŁA����delete�����邱�Ƃ͋֎~�B
 * @return �g�p���̃V�[���ւ̐��|�C���^
 */
C_Scene* SceneController::GetCurrentScene() {
	return currentScene.get();
}

/**
 * @brief ���݂̃V�[���ԍ����擾�B�ǂ̃V�[�������킩��Ηǂ������̎��͊�{�I�ɂ�������g�p���邱�ƁB
 * @return ���݂̃V�[���ԍ�
 */
SceneController::GameScene SceneController::GetCurrentSceneNumber() {
	return currentSceneNumber;
}