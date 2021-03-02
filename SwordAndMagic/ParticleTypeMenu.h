/**
 * @file ParticleTypeMenu.h
 * @author MIURA DAIKI
 * @date 2021/1/18
 */
#pragma once
#include "Component.h"
#include "ImGuiHelper.h"
#include <windows.h>
#include <wrl/client.h>
#include "Particle.h"


class C_GameObject;
class BasicMeshComponent;
/**
 * @class ParticleTypeMenu
 * @brief パーティクルエディターのメニュー管理用クラス
 */
class ParticleTypeMenu
{
private:

	ImGuiNode* m_guiWindow;
	ImGuiNode* m_guiNodeMenu;

	ImGuiNode* m_guiNodeOffset;
	ImGuiNode* m_guiNodeMove;
	Gui_Button* mainButton;

	OPENFILENAME     ofn;
	OPENFILENAME     ofn2;
	TCHAR            szPath[MAX_PATH];
	TCHAR            szFile[MAX_PATH];

	bool fileOpened;

	int currentParticleType;

	Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_pTexture;		// テクスチャ
public:
	ParticleTypeMenu(std::string LabelName);
	~ParticleTypeMenu();
	void Update();
	void OpenFile();

	//初期位置と速度だけ、常に取得させるようにする
	DirectX::XMFLOAT3 GetSpeed();
	DirectX::XMFLOAT3 GetPos();
};
