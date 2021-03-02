#include "ImGuiHelper.h"
#include "GraphicsManager.h"
#include "WindowManager.h"

/**
* @brief �R���X�g���N�^�BImGui�̏����������B
*/
ImGuiNode::ImGuiNode() {
	
}

/**
* @brief �������R���X�g���N�^�B
* @param[in] LabelName ����GUI�ɓ\�郉�x����
* @param[in] labelNameSize sizeof(���x����)
* @param[in] parentNode �e�m�[�h
*/
ImGuiNode::ImGuiNode(const char* LabelName, ImGuiNode* parentNode, bool isMenu) {
	m_isMenu = isMenu;
	std::string str(LabelName);
	m_szLabelName = str;
	m_parentNode = parentNode;
}


/**
* @brief �f�X�g���N�^�BImGui�̏I�������B
*/
ImGuiNode::~ImGuiNode() {
	ImGui_ImplDX11_Shutdown();
}

/**
* @brief �S�Ă̎q�m�[�h�y�ѓo�^���ꂽGUI�̍폜���s���B�q�m�[�h����̌Ăяo���͔񐄏��B
*/
void ImGuiNode::ResetAll() {
	guiList.clear();
	guiTreeList.clear();
}

/**
* @brief �J�n�����B�����[�v��ImGui�J�n����
*/
void ImGuiNode::NewFrame() {
	//ImGui::SetNextWindowSize(ImVec2(320, 100), ImGuiCond_Once);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

/**
* @brief �`�揈���B�����[�v�̕`��o�^�����ƕ`�揈��
*/
void ImGuiNode::Render() {
	//�o�^����
	bool nodeIsOpen = false;

	//�ŏ�ʃ��C���[�͏��Open
	if (!m_parentNode) {
		nodeIsOpen = true;

		//�E�B���h�E��؂菈��
		ImGui::SetNextWindowSize(ImVec2(320, 320), ImGuiCond_Once);
		ImGui::Begin(m_szLabelName.c_str(), nullptr, ImGuiWindowFlags_MenuBar);
	}

	if (m_parentNode && !m_isMenu) {
		//�K�w�\��
		nodeIsOpen = ImGui::TreeNode(m_szLabelName.c_str());
	}

	if (m_isMenu) {
		//���j���[�Ȃ�
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
		//�K�w�\��
		ImGui::TreePop();
	}
	else if(!m_parentNode){
		//�E�B���h�E��؂菈��
		ImGui::End();
	}
}

/**
* @brief Gui�ǉ������B�ǉ�����Gui�͎���Render()����o�^�����B
*/
void ImGuiNode::AddGuiData(std::unique_ptr<GuiData> guiData) {
	std::string name(guiData->GetLabel());
	guiList[name] = std::move(guiData);
}

/**
* @brief �q��ǉ����鏈���B
* @param[in] nodeName �q�m�[�h��
* @param[in] size �q�m�[�h���̃T�C�Y
*/
void ImGuiNode::AddChild(const char* nodeName, bool isMenu) {
	std::string name(nodeName);
	guiTreeList[name] = std::make_unique<ImGuiNode>(nodeName, this,isMenu);
}

/**
* @brief ���x���ύX����
* @param[in] LabelName �ύX���郉�x����
*/
void ImGuiNode::SetLabelName(const char* LabelName) {
	m_szLabelName = LabelName;
}

/**
* @brief �q�m�[�h�擾�����B�S�Ă̎q���������A�������O�̎q������ꍇ�͍ŏ��Ɍ����Ƀq�b�g�������̂�����Ԃ��d�l�B
* @param[in] LabelName �擾����q�m�[�h�̃��x����
* @return �擾���ꂽ�m�[�h�ւ̃|�C���^
*/
ImGuiNode* ImGuiNode::GetChild(const char* LabelName) {
	if (guiTreeList.empty()) return nullptr;
	if (guiTreeList.find(LabelName) != guiTreeList.end()) {
		//����������(�͂₢)
		return guiTreeList.at(LabelName).get();
	}

	//������Ȃ�������(�q����������̂ł�����ƒx��)
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
* @brief ���x�����擾
* @return ���̃m�[�h�̃��x��(�ŏ�ʃ��C���[�̎���NULL)
*/
char* ImGuiNode::GetLabel() {
	return (char*)(m_szLabelName.c_str());
}

/**
* @brief GuiData�擾�����B��q��Get�����Œl���擾�ł���̂ŁA��������O������Ăяo���ꍇ�͓���ȏ��������鎞�̂݁B
* @param[in] LabelName �擾������GuiData�̃��x����(��������ꍇ�͐擪�̕�)
* @return �擾���ꂽGuiData�ւ̃|�C���^
*/
GuiData* ImGuiNode::GetGuiData(const char* LabelName) {
	std::string str(LabelName);
	if (guiList.find(str) != guiList.end()) {
		//����������
		return guiList.at(str).get();
	}

	//������Ȃ�������nullptr
	return nullptr;
}

/**
* @brief Int�^�̃f�[�^���擾�B
* @param[in] �擾������GuiData�̃��x����
* return �擾���ꂽ�l
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
* @brief float�^�̃f�[�^���擾�B
* @param[in] �擾������GuiData�̃��x����
* return �擾���ꂽ�l
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
* @brief bool�^�̃f�[�^���擾�B
* @param[in] �擾������GuiData�̃��x����
* return �擾���ꂽ�l
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
* @brief string�^�̃f�[�^���擾�B
* @param[in] �擾������GuiData�̃��x����
* return �擾���ꂽ�l
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
* @brief �R���X�g���N�^�BImGui�̏���������
*/
ImGuiWindowManager::ImGuiWindowManager() {
	
}

/**
* @brief �f�X�g���N�^�B�����Ȃ�
*/
ImGuiWindowManager::~ImGuiWindowManager() {

}

/**
* @brief �S�ẴE�B���h�E��`�悷��
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
* @brief ���t���[����ImGui�J�n����
*/
void ImGuiWindowManager::NewFrame() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

/**
* @brief �E�B���h�E�ǉ�����
* @param[in] LabelName �ǉ�����E�B���h�E�̖��O�B�����E�B���h�E�����ɂ���ꍇ�̓C���f�b�N�X�ԍ���t�^�B
* @return �ǉ����ꂽ�E�B���h�E�f�[�^�ւ̃|�C���^
*/
ImGuiNode* ImGuiWindowManager::AddWindow(const char* LabelName) {
	std::string name(LabelName);

	//�����������͖��O�ɃC���f�b�N�X�ԍ���t�����鏈��
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
* @brief �q�E�B���h�E�ǉ�����
* @param[in] parentLabelName �ǉ�����E�B���h�E�̖��O�B�����͏㏑���ɂȂ邱�Ƃɒ��ӁB
* @param[in] childLabelName �ǉ�����E�B���h�E�̖��O�B�����͏㏑���ɂȂ邱�Ƃɒ��ӁB
*/
void ImGuiWindowManager::AddWindowChild(const char* parentLabelName, const char* childLabelName) {
	ImGuiNode* node = GetWindowData(parentLabelName);
	if (node) {
		GetWindowData(parentLabelName)->AddChild((const char*)childLabelName);
	}
}

/**
* @brief �E�B���h�E�擾
* @param[in] LabelName �擾�������E�B���h�E�̖��O
* @return �擾�����E�B���h�E�ւ̃|�C���^�B������Ȃ�������nullptr
*/
ImGuiNode* ImGuiWindowManager::GetWindowData(const char* LabelName) {
	if (imGuiWindow.find(LabelName) != imGuiWindow.end()) {
		//����������
		return imGuiWindow.at(LabelName).get();
	}
	//������Ȃ�������nullptr
	return nullptr;
}

/**
* @brief �E�B���h�E���̃��Z�b�g
* @param[in] LabelName ���Z�b�g�������E�B���h�E�̖��O
*/
void ImGuiWindowManager::ResetWindow(const char* LabelName) {
	if (imGuiWindow.find(LabelName) != imGuiWindow.end()) {
		//����������
		imGuiWindow.at(LabelName)->ResetAll();
	}
}