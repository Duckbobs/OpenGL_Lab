#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Gui.h"

void Gui::Test()
{
	static bool checkBox = false;
	static float sliderFloat = 0.f;
	static int sliderInt = 0;
	ImGui::Begin("Window");
	ImGui::Checkbox("Toggle", &checkBox);
	ImGui::SliderFloat("Slider###SliderFloat", &sliderFloat, 0.0f, 100.0f);
	ImGui::SliderInt("Slider###SliderInt", &sliderInt, 0, 100);
	ImGui::End();
}