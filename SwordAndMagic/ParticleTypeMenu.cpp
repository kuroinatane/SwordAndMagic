#include "ParticleTypeMenu.h"


ParticleTypeMenu::ParticleTypeMenu(std::string LabelName) {
	m_guiWindow = ImGuiWindowManager::GetInstance().AddWindow(LabelName.c_str());
	m_guiWindow->AddChild("MainMenu", true);
	m_guiNodeMenu = m_guiWindow->GetChild("MainMenu");
	m_guiNodeMenu->AddGuiData(std::make_unique<Gui_MenuItem>("SphereLocation"));
	m_guiNodeMenu->AddGuiData(std::make_unique<Gui_MenuItem>("ConstantMove"));
	m_guiNodeMenu->AddGuiData(std::make_unique<Gui_MenuItem>("RandomMove"));
	m_guiNodeMenu->AddGuiData(std::make_unique<Gui_MenuItem>("VortexMove"));
}
ParticleTypeMenu::~ParticleTypeMenu() {

}
void ParticleTypeMenu::Update() {

}
void ParticleTypeMenu::OpenFile() {

}