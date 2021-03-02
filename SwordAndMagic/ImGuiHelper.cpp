#include "ImGuiHelper.h"
#include "GraphicsManager.h"
#include "WindowManager.h"

/**
* @brief コンストラクタ。ImGuiの初期化処理。
*/
ImGuiNode::ImGuiNode() {
	
}

/**
* @brief 引数つきコンストラクタ。
* @param[in] LabelName そのGUIに貼るラベル名
* @param[in] labelNameSize sizeof(ラベル名)
* @param[in] parentNode 親ノード
*/
ImGuiNode::ImGuiNode(const char* LabelName, ImGuiNode* parentNode, bool isMenu) {
	m_isMenu = isMenu;
	std::string str(LabelName);
	m_szLabelName = str;
	m_parentNode = parentNode;
}


/**
* @brief デストラクタ。ImGuiの終了処理。
*/
ImGuiNode::~ImGuiNode() {
	ImGui_ImplDX11_Shutdown();
}

/**
* @brief 全ての子ノード及び登録されたGUIの削除を行う。子ノードからの呼び出しは非推奨。
*/
void ImGuiNode::ResetAll() {
	guiList.clear();
	guiTreeList.clear();
}

/**
* @brief 開始処理。毎ループのImGui開始処理
*/
void ImGuiNode::NewFrame() {
	//ImGui::SetNextWindowSize(ImVec2(320, 100), ImGuiCond_Once);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

/**
* @brief 描画処理。毎ループの描画登録処理と描画処理
*/
void ImGuiNode::Render() {
	//登録処理
	bool nodeIsOpen = false;

	//最上位レイヤーは常にOpen
	if (!m_parentNode) {
		nodeIsOpen = true;

		//ウィンドウ区切り処理
		ImGui::SetNextWindowSize(ImVec2(320, 320), ImGuiCond_Once);
		ImGui::Begin(m_szLabelName.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
	}

	if (m_parentNode && !m_isMenu) {
		//階層構造
		nodeIsOpen = ImGui::TreeNode(m_szLabelName.c_str());
	}

	if (m_isMenu) {
		//メニューなら
		//ImGui::Begin("test", nullptr, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu(m_szLabelName.c_str())) {
				if (!(guiList.empty())) {
					std::map<std::string, std::unique_ptr<GuiData>>::iterator itr;
					for (itr = guiList.begin(); itr != guiList.end(); itr++) {
						itr->second->Register();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	else {
		if (nodeIsOpen && !(guiList.empty())) {
			std::map<std::string, std::unique_ptr<GuiData>>::iterator itr;
			for (itr = guiList.begin(); itr != guiList.end(); itr++) {
				itr->second->Register();
			}
		}

		if ((nodeIsOpen && !(guiTreeList.empty()))) {
			std::map<std::string, std::unique_ptr<ImGuiNode>>::iterator itr2;
			for (itr2 = guiTreeList.begin(); itr2 != guiTreeList.end(); itr2++) {
				itr2->second->Render();
			}
		}
	}

	if (m_parentNode && nodeIsOpen && !m_isMenu) {
		//階層構造
		ImGui::TreePop();
	}
	else if(!m_parentNode){
		//ウィンドウ区切り処理
		ImGui::End();
	}
}

/**
* @brief Gui追加処理。追加したGuiは次のRender()から登録される。
*/
void ImGuiNode::AddGuiData(std::unique_ptr<GuiData> guiData) {
	std::string name(guiData->GetLabel());
	guiList[name] = std::move(guiData);
}

/**
* @brief 子を追加する処理。
* @param[in] nodeName 子ノード名
* @param[in] size 子ノード名のサイズ
*/
void ImGuiNode::AddChild(const char* nodeName, bool isMenu) {
	std::string name(nodeName);
	guiTreeList[name] = std::make_unique<ImGuiNode>(nodeName, this,isMenu);
}

/**
* @brief ラベル変更処理
* @param[in] LabelName 変更するラベル名
*/
void ImGuiNode::SetLabelName(const char* LabelName) {
	m_szLabelName = LabelName;
}

/**
* @brief 子ノード取得処理。全ての子を検索し、同じ名前の子がある場合は最初に検索にヒットしたものだけを返す仕様。
* @param[in] LabelName 取得する子ノードのラベル名
* @return 取得されたノードへのポインタ
*/
ImGuiNode* ImGuiNode::GetChild(const char* LabelName) {
	if (guiTreeList.empty()) return nullptr;
	if (guiTreeList.find(LabelName) != guiTreeList.end()) {
		//見つかった時(はやい)
		return guiTreeList.at(LabelName).get();
	}

	//見つからなかった時(子を検索するのでちょっと遅い)
	std::map <std::string, std::unique_ptr<ImGuiNode >> ::iterator itr;
	for (itr = guiTreeList.begin(); itr != guiTreeList.end(); itr++) {
		ImGuiNode* child = itr->second->GetChild(LabelName);
		if (child) {
			return child;
		}
	}
	return NULL;
}

/**
* @brief ラベル名取得
* @return そのノードのラベル(最上位レイヤーの時はNULL)
*/
char* ImGuiNode::GetLabel() {
	return (char*)(m_szLabelName.c_str());
}

/**
* @brief GuiData取得処理。後述のGet○○で値を取得できるので、こちらを外部から呼び出す場合は特殊な処理をする時のみ。
* @param[in] LabelName 取得したいGuiDataのラベル名(複数ある場合は先頭の物)
* @return 取得されたGuiDataへのポインタ
*/
GuiData* ImGuiNode::GetGuiData(const char* LabelName) {
	std::string str(LabelName);
	if (guiList.find(str) != guiList.end()) {
		//見つかった時
		return guiList.at(str).get();
	}

	//見つからなかったらnullptr
	return nullptr;
}

/**
* @brief Int型のデータを取得。
* @param[in] 取得したいGuiDataのラベル名
* return 取得された値
*/
int ImGuiNode::GuiGetInt(const char* LabelName) {
	int res = 0;
	GuiData* guiData = GetGuiData(LabelName);
	if (guiData != NULL) {
		guiData->GetValue(res);
		return res;
	}
	return NULL;
}

/**
* @brief float型のデータを取得。
* @param[in] 取得したいGuiDataのラベル名
* return 取得された値
*/
float ImGuiNode:: GuiGetFloat(const char* LabelName) {
	float res = 0.0f;
	GuiData* guiData = GetGuiData(LabelName);
	if (guiData != NULL) {
		guiData->GetValue(res);
		return res;
	}
	return NULL;
}

/**
* @brief bool型のデータを取得。
* @param[in] 取得したいGuiDataのラベル名
* return 取得された値
*/
bool ImGuiNode::GuiGetBool(const char* LabelName) {
	bool res = false;
	GuiData* guiData = GetGuiData(LabelName);
	if (guiData != NULL) {
		guiData->GetValue(res);
		return res;
	}
	return NULL;
}

/**
* @brief string型のデータを取得。
* @param[in] 取得したいGuiDataのラベル名
* return 取得された値
*/
std::string ImGuiNode::GuiGetString(const char* LabelName) {
	std::string res = "";
	GuiData* guiData = GetGuiData(LabelName);
	if (guiData != NULL) {
		guiData->GetValue(res);
		return res;
	}
	return NULL;
}

/**
* @brief コンストラクタ。ImGuiの初期化処理
*/
ImGuiWindowManager::ImGuiWindowManager() {
	
}

/**
* @brief デストラクタ。処理なし
*/
ImGuiWindowManager::~ImGuiWindowManager() {

}

/**
* @brief 全てのウィンドウを描画する
*/
void ImGuiWindowManager::Render() {
	NewFrame();
	std::map <std::string, std::unique_ptr<ImGuiNode>> ::iterator itr;
	for (itr = imGuiWindow.begin(); itr != imGuiWindow.end(); itr++) {
		itr->second->Render();
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/**
* @brief 毎フレームのImGui開始処理
*/
void ImGuiWindowManager::NewFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

/**
* @brief ウィンドウ追加処理
* @param[in] LabelName 追加するウィンドウの名前。同名ウィンドウが既にある場合はインデックス番号を付与。
* @return 追加されたウィンドウデータへのポインタ
*/
ImGuiNode* ImGuiWindowManager::AddWindow(const char* LabelName) {
	std::string name(LabelName);

	//見つかった時は名前にインデックス番号を付加する処理
	if (imGuiWindow.find(name) != imGuiWindow.end()) {
		std::string defaultName = name;
		int indexNo = 0;
		while (imGuiWindow.find(name) != imGuiWindow.end()) {
			name = defaultName + "(" +std::to_string(indexNo) + ")";
			indexNo++;
		}
	}
	imGuiWindow[name] = std::make_unique<ImGuiNode>(name.c_str(), nullptr);
	return imGuiWindow[name].get();
}


/**
* @brief 子ウィンドウ追加処理
* @param[in] parentLabelName 追加するウィンドウの名前。同名は上書きになることに注意。
* @param[in] childLabelName 追加するウィンドウの名前。同名は上書きになることに注意。
*/
void ImGuiWindowManager::AddWindowChild(const char* parentLabelName, const char* childLabelName) {
	ImGuiNode* node = GetWindowData(parentLabelName);
	if (node) {
		GetWindowData(parentLabelName)->AddChild((const char*)childLabelName);
	}
}

/**
* @brief ウィンドウ取得
* @param[in] LabelName 取得したいウィンドウの名前
* @return 取得したウィンドウへのポインタ。見つからなかったらnullptr
*/
ImGuiNode* ImGuiWindowManager::GetWindowData(const char* LabelName) {
	if (imGuiWindow.find(LabelName) != imGuiWindow.end()) {
		//見つかった時
		return imGuiWindow.at(LabelName).get();
	}
	//見つからなかったらnullptr
	return nullptr;
}

/**
* @brief ウィンドウ情報のリセット
* @param[in] LabelName リセットしたいウィンドウの名前
*/
void ImGuiWindowManager::ResetWindow(const char* LabelName) {
	if (imGuiWindow.find(LabelName) != imGuiWindow.end()) {
		//見つかった時
		imGuiWindow.at(LabelName)->ResetAll();
	}
}