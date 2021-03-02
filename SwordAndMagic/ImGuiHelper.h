/**
 * @file ImGuiHelper.h
 * @brief ImGuiの機能を纏めておくためのクラス群。GuiDataはファイル分けするとファイルが多くなるのと、cppに関数を書くと同じ関数が多くて可読性が落ちるという観点からインライン化。	
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
 * @brief Guiのベースクラス。
 */
class GuiData {
public:
	GuiData() {}
	virtual ~GuiData() {}

	/**
	* @brief ImGuiに登録するための関数。実装は子クラス内で行うので純粋仮想関数。
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
	std::string m_szLabelName; //!< @brief GUIのラベル
};

/**
 * @class ImGuiHelper
 * @brief ImGuiの機能をまとめるためのラッパークラス。
 */
class ImGuiNode{
protected:
	std::map<std::string,std::unique_ptr<GuiData >> guiList;	//!< @brief Guiを保持するリスト
	std::map<std::string,std::unique_ptr<ImGuiNode>> guiTreeList;	//!< @brief Guiの各ツリーを保持するリスト
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

	bool currentNodeIsOpen = false; //!< @brief TreeNode用。直近のTreeNodeが「開いている」時はtrue、「閉じている」時はfalse

	/**
	* @brief Guiデータ取得用。子クラスの情報が欲しい時に。ダウンキャストを伴うので乱用厳禁。
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
 * @brief テキスト表示用クラス。
 */
class Gui_Text : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName 表示したい文字列(そのままラベル名になる)
	*/
	Gui_Text(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
	}
	/**
	* @brief デストラクタ。文字列用に確保したメモリの解放。
	*/
	~Gui_Text() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。
	*/
	void Register() {
		ImGui::Text(m_szLabelName.c_str());
	}
};

/**
 * @class Gui_InputField
 * @brief テキスト入力用クラス
 */
class Gui_InputField : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName 表示したい文字列(そのままラベル名になる)
	*/
	Gui_InputField(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
	}
	/**
	* @brief デストラクタ。文字列用に確保したメモリの解放。
	*/
	~Gui_InputField() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。
	*/
	void Register() {
		ImGui::InputText(m_szLabelName.c_str(),buf,256);
	}
	/**
	* @brief 保持している値を取得
	* @param[out] outValue 値の出力先
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
  * @brief テキスト表示用クラス。
  */
class Gui_Button : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName ラベル名
	*/
	Gui_Button(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
		clickCount = 0;
	}
	/**
	* @brief デストラクタ。文字列用に確保したメモリの解放。
	*/
	~Gui_Button() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。と、同時にボタンが押されているかどうかも確認する。
	*/
	void Register() {
		if (ImGui::Button(m_szLabelName.c_str())) {
			clickCount++;
		}
	}

	/**
	* @brief その時点でボタンが押されているかどうかを返す。
	* @return ボタンが押されたかどうか(true/false)
	*/
	bool GetButtonDown() {
		return (bool)(clickCount & 1);
	}

	/**
	* @brief ボタンをトグルする(OnならOff、OffならOn状態に)
	*/
	void SetButtonToggle() {
		clickCount++;
	}

	/**
	* @brief 保持している値を取得
	* @param[out] outValue 値の出力先
	*/
	void GetValue(bool& outValue) {
		outValue = (bool)(clickCount & 1);
	}
private:
	int clickCount;  //!< @brief ボタンが押されたかどうかを保持
};

/**
  * @class Gui_MenuItem
  * @brief メニューの項目定義用クラス。
  */
class Gui_MenuItem : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName ラベル名
	*/
	Gui_MenuItem(const char* LabelName) {
		std::string str(LabelName);
		m_szLabelName = str;
		clickCount = 0;
	}
	/**
	* @brief デストラクタ。
	*/
	~Gui_MenuItem() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。と、同時にメニュー項目が押されて（クリックされて）いるかどうかも確認する。
	*/
	void Register() {
		if (ImGui::MenuItem(m_szLabelName.c_str())) {
			clickCount++;
		}
	}

	/**
	* @brief その時点でそのメニュー項目が押されて（クリックされて）いるかどうかを返す。
	* @return メニュー項目が押されたかどうか(true/false)
	*/
	bool GetMenuPress() {
		return (bool)(clickCount & 1);
	}

	/**
	* @brief ボタンをトグルする(OnならOff、OffならOn状態に)
	*/
	void SetButtonToggle() {
		clickCount++;
	}

	/**
	* @brief 保持している値を取得
	* @param[out] outValue 値の出力先
	*/
	void GetValue(bool& outValue) {
		outValue = (bool)(clickCount & 1);
	}
private:
	int clickCount;  //!< @brief ボタンが押されたかどうかを保持
};


/**
  * @class Gui_SliderInt
  * @brief Int型の数値を表示するスライダー用クラス。
  */
class Gui_SliderInt : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName ラベル名
	* @param[in] min スライダーの最小値
	* @param[in] max スライダーの最大値
	*/
	Gui_SliderInt(const char* LabelName, int min, int max) {
		std::string str(LabelName);
		m_szLabelName = str;;
		minValue = min;
		maxValue = max;
		value = (min + max) / 2;
	}
	/**
	* @brief デストラクタ。文字列用に確保したメモリの解放。
	*/
	~Gui_SliderInt() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。と、同時にスライダーの現在値も取得
	*/
	void Register() {
		ImGui::SliderInt(m_szLabelName.c_str(), &value, minValue, maxValue);
	}

	/**
	* @brief 保持している値を取得
	* @param[out] outValue 値の出力先
	*/
	void GetValue(int& outValue) {
		outValue = value;
	}
private:
	int value;  //!< @brief ボタンが押されたかどうかを保持
	int minValue;  //!< @brief ボタンが押されたかどうかを保持
	int maxValue;  //!< @brief ボタンが押されたかどうかを保持
};

/**
  * @class Gui_SliderFloat
  * @brief Float型の数値を表示するスライダー用クラス。
  */
class Gui_SliderFloat : public GuiData {
public:
	/**
	* @brief コンストラクタ。ラベル名の設定など
	* @param[in] LabelName ラベル名
	* @param[in] min スライダーの最小値
	* @param[in] max スライダーの最大値
	*/
	Gui_SliderFloat(const char* LabelName, float min, float max) {
		std::string str(LabelName);
		m_szLabelName = str;
		minValue = min;
		maxValue = max;
		value = (min+max)/2.0f;
	}
	/**
	* @brief デストラクタ。文字列用に確保したメモリの解放。
	*/
	~Gui_SliderFloat() {
	}
	/**
	* @brief 保持している情報をImGuiに登録。と、同時にスライダーの現在値も取得
	*/
	void Register() {
		ImGui::SliderFloat(m_szLabelName.c_str(), &value, minValue, maxValue);
	}

	/**
	* @brief 保持している値を取得
	* @param[out] outValue 値の出力先
	*/
	void GetValue(float& outValue) {
		outValue = value;
	}
private:
	float value;  //!< @brief ボタンが押されたかどうかを保持
	float minValue;  //!< @brief ボタンが押されたかどうかを保持
	float maxValue;  //!< @brief ボタンが押されたかどうかを保持
};

/**
 * @class ImGuiWindowManager
 * @brief ImGuiのウィンドウを管理するクラス。
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

