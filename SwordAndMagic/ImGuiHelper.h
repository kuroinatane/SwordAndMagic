/**
 * @file ImGuiHelper.h
 * @brief ImGui�̋@�\��Z�߂Ă������߂̃N���X�Q�BGuiData�̓t�@�C����������ƃt�@�C���������Ȃ�̂ƁAcpp�Ɋ֐��������Ɠ����֐��������ĉǐ���������Ƃ����ϓ_����C�����C�����B	
 * @author MIURA DAIKI
 * @date 2020/01/18
 */

#pragma once
#include "Singleton.h"
#include <string>
#include <map>
#include <memory>
#include "string.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


/**
 * @class GuiData
 * @brief Gui�̃x�[�X�N���X�B
 */
class GuiData {
public:
	GuiData() {}
	virtual ~GuiData() {}

	/**
	* @brief ImGui�ɓo�^���邽�߂̊֐��B�����͎q�N���X���ōs���̂ŏ������z�֐��B
	*/
	virtual void Register() = 0;

	virtual void GetValue(int& outValue) {}
	virtual void GetValue(float& outValue) {}
	virtual void GetValue(bool& outValue) {}
	virtual void GetValue(std::string& outValue) {}

	std::string GetLabel() {
		return m_szLabelName;
	}
protected:
	std::string m_szLabelName; //!< @brief GUI�̃��x��
};

/**
 * @class ImGuiHelper
 * @brief ImGui�̋@�\���܂Ƃ߂邽�߂̃��b�p�[�N���X�B
 */
class ImGuiNode{
protected:
	std::map<std::string,std::unique_ptr<GuiData >> guiList;	//!< @brief Gui��ێ����郊�X�g
	std::map<std::string,std::unique_ptr<ImGuiNode>> guiTreeList;	//!< @brief Gui�̊e�c���[��ێ����郊�X�g
	std::string m_szLabelName;
	ImGuiNode* m_parentNode;
	bool m_isMenu;
public:
	ImGuiNode();
	ImGuiNode(const char* LabelName, ImGuiNode* parentNode, bool isMenu = false);
	virtual ~ImGuiNode();
	virtual void Render();
	void NewFrame();
	void ResetAll();
	void AddGuiData(std::unique_ptr<GuiData> guiData);
	void AddChild(const char* nodeName, bool isMenu = false);
	ImGuiNode* GetChild(const char* LabelName);
	GuiData* GetGuiData(const char* LabelName);
	int GuiGetInt(const char* LabelName);
	float GuiGetFloat(const char* LabelName);
	bool GuiGetBool(const char* LabelName);
	std::string GuiGetString(const char* LabelName);
	char* GetLabel();
	void SetLabelName(const char* LabelName);

	bool currentNodeIsOpen = false; //!< @brief TreeNode�p�B���߂�TreeNode���u�J���Ă���v����true�A�u���Ă���v����false

	/**
	* @brief Gui�f�[�^�擾�p�B�q�N���X�̏�񂪗~�������ɁB�_�E���L���X�g�𔺂��̂ŗ��p���ցB
	*/
	template <class T> T* GetGuiData()
	{
		T* guiData;

		std::map<std::string, std::unique_ptr<GuiData>>::iterator itr;
		for (itr = guiList.begin(); itr != guiList.end(); std::next(itr, 1)) {
			guiData = dynamic_cast<T*>(itr->second.get());
			if (guiData != nullptr)
				return guiData;
		}
		return nullptr;
	}
};

/**
 * @class Gui_Text
 * @brief �e�L�X�g�\���p�N���X�B
 */
class Gui_Text : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName �\��������������(���̂܂܃��x�����ɂȂ�)
	*/
	Gui_Text(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
	}
	/**
	* @brief �f�X�g���N�^�B������p�Ɋm�ۂ����������̉���B
	*/
	~Gui_Text() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B
	*/
	void Register() {
		ImGui::Text(m_szLabelName.c_str());
	}
};

/**
 * @class Gui_InputField
 * @brief �e�L�X�g���͗p�N���X
 */
class Gui_InputField : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName �\��������������(���̂܂܃��x�����ɂȂ�)
	*/
	Gui_InputField(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
	}
	/**
	* @brief �f�X�g���N�^�B������p�Ɋm�ۂ����������̉���B
	*/
	~Gui_InputField() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B
	*/
	void Register() {
		ImGui::InputText(m_szLabelName.c_str(),buf,256);
	}
	/**
	* @brief �ێ����Ă���l���擾
	* @param[out] outValue �l�̏o�͐�
	*/
	void GetValue(std::string& outValue) {
		if (buf[0] == '\0') {
			std::string str("Default");
			outValue = str;
		}
		else {
			std::string str(buf);
			outValue = str;
		}
	}
private:
	char buf[256] = "";
};

/**
  * @class Gui_Button
  * @brief �e�L�X�g�\���p�N���X�B
  */
class Gui_Button : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName ���x����
	*/
	Gui_Button(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
		clickCount = 0;
	}
	/**
	* @brief �f�X�g���N�^�B������p�Ɋm�ۂ����������̉���B
	*/
	~Gui_Button() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B�ƁA�����Ƀ{�^����������Ă��邩�ǂ������m�F����B
	*/
	void Register() {
		if (ImGui::Button(m_szLabelName.c_str())) {
			clickCount++;
		}
	}

	/**
	* @brief ���̎��_�Ń{�^����������Ă��邩�ǂ�����Ԃ��B
	* @return �{�^���������ꂽ���ǂ���(true/false)
	*/
	bool GetButtonDown() {
		return (bool)(clickCount & 1);
	}

	/**
	* @brief �{�^�����g�O������(On�Ȃ�Off�AOff�Ȃ�On��Ԃ�)
	*/
	void SetButtonToggle() {
		clickCount++;
	}

	/**
	* @brief �ێ����Ă���l���擾
	* @param[out] outValue �l�̏o�͐�
	*/
	void GetValue(bool& outValue) {
		outValue = (bool)(clickCount & 1);
	}
private:
	int clickCount;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
};

/**
  * @class Gui_MenuItem
  * @brief ���j���[�̍��ڒ�`�p�N���X�B
  */
class Gui_MenuItem : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName ���x����
	*/
	Gui_MenuItem(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
		clickCount = 0;
	}
	/**
	* @brief �f�X�g���N�^�B
	*/
	~Gui_MenuItem() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B�ƁA�����Ƀ��j���[���ڂ�������āi�N���b�N����āj���邩�ǂ������m�F����B
	*/
	void Register() {
		if (ImGui::MenuItem(m_szLabelName.c_str())) {
			clickCount++;
		}
	}

	/**
	* @brief ���̎��_�ł��̃��j���[���ڂ�������āi�N���b�N����āj���邩�ǂ�����Ԃ��B
	* @return ���j���[���ڂ������ꂽ���ǂ���(true/false)
	*/
	bool GetMenuPress() {
		return (bool)(clickCount & 1);
	}

	/**
	* @brief �{�^�����g�O������(On�Ȃ�Off�AOff�Ȃ�On��Ԃ�)
	*/
	void SetButtonToggle() {
		clickCount++;
	}

	/**
	* @brief �ێ����Ă���l���擾
	* @param[out] outValue �l�̏o�͐�
	*/
	void GetValue(bool& outValue) {
		outValue = (bool)(clickCount & 1);
	}
private:
	int clickCount;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
};


/**
  * @class Gui_SliderInt
  * @brief Int�^�̐��l��\������X���C�_�[�p�N���X�B
  */
class Gui_SliderInt : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName ���x����
	* @param[in] min �X���C�_�[�̍ŏ��l
	* @param[in] max �X���C�_�[�̍ő�l
	*/
	Gui_SliderInt(const char* LabelName, int min, int max) {
		std::string str(LabelName);
		m_szLabelName = str;;
		minValue = min;
		maxValue = max;
		value = (min + max) / 2;
	}
	/**
	* @brief �f�X�g���N�^�B������p�Ɋm�ۂ����������̉���B
	*/
	~Gui_SliderInt() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B�ƁA�����ɃX���C�_�[�̌��ݒl���擾
	*/
	void Register() {
		ImGui::SliderInt(m_szLabelName.c_str(), &value, minValue, maxValue);
	}

	/**
	* @brief �ێ����Ă���l���擾
	* @param[out] outValue �l�̏o�͐�
	*/
	void GetValue(int& outValue) {
		outValue = value;
	}
private:
	int value;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
	int minValue;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
	int maxValue;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
};

/**
  * @class Gui_SliderFloat
  * @brief Float�^�̐��l��\������X���C�_�[�p�N���X�B
  */
class Gui_SliderFloat : public GuiData {
public:
	/**
	* @brief �R���X�g���N�^�B���x�����̐ݒ�Ȃ�
	* @param[in] LabelName ���x����
	* @param[in] min �X���C�_�[�̍ŏ��l
	* @param[in] max �X���C�_�[�̍ő�l
	*/
	Gui_SliderFloat(const char* LabelName, float min, float max) {
		std::string str(LabelName);
		m_szLabelName = str;
		minValue = min;
		maxValue = max;
		value = (min+max)/2.0f;
	}
	/**
	* @brief �f�X�g���N�^�B������p�Ɋm�ۂ����������̉���B
	*/
	~Gui_SliderFloat() {
	}
	/**
	* @brief �ێ����Ă������ImGui�ɓo�^�B�ƁA�����ɃX���C�_�[�̌��ݒl���擾
	*/
	void Register() {
		ImGui::SliderFloat(m_szLabelName.c_str(), &value, minValue, maxValue);
	}

	/**
	* @brief �ێ����Ă���l���擾
	* @param[out] outValue �l�̏o�͐�
	*/
	void GetValue(float& outValue) {
		outValue = value;
	}
private:
	float value;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
	float minValue;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
	float maxValue;  //!< @brief �{�^���������ꂽ���ǂ�����ێ�
};

/**
 * @class ImGuiWindowManager
 * @brief ImGui�̃E�B���h�E���Ǘ�����N���X�B
 */
class ImGuiWindowManager : public Singleton<ImGuiWindowManager> {
private:
	std::map<std::string,std::unique_ptr<ImGuiNode>> imGuiWindow;
public:
	ImGuiWindowManager();
	~ImGuiWindowManager();
	void NewFrame();
	void Render();
	ImGuiNode* AddWindow(const char* LabelName);
	ImGuiNode* AddNodeEditor(const char* LabelName);
	ImGuiNode* GetWindowData(const char* LabelName);
	void AddWindowChild(const char* LabelName, const char* ParentLabelName);
	void ResetWindow(const char* LabelName);
};

