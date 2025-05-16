#include "interface.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "interfaceController.h"

#include <algorithm>
#include <string>

GUI::GUI(std::shared_ptr<GUIController> _controller) 
    : controller(_controller) {}

void GUI::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    setStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");
}

void GUI::setStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = 1.0;

    style.WindowRounding = 3;
    style.GrabRounding = 1;
    style.GrabMinSize = 20;
    style.FrameRounding = 3;

    style.Colors[ImGuiCol_Text] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f); 
    style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 1.00f, 1.00f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 1.00f, 1.00f, 0.27f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 1.00f, 1.00f, 0.44f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10f, 1.00f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.10f, 1.00f, 1.00f, 0.68f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.10f, 1.00f, 1.00f, 0.36f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.10f, 1.00f, 1.00f, 0.76f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 0.46f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 1.00f, 1.00f, 0.43f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 1.00f, 1.00f, 0.62f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 1.00f, 1.00f, 0.33f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 1.00f, 1.00f, 0.42f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.10f, 1.00f, 1.00f, 0.54f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.10f, 1.00f, 1.00f, 0.54f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.10f, 1.00f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.10f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.10f, 1.00f, 1.00f, 0.22f);
}

void GUI::setNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::setWindowPos()
{
    ImVec2 window_pos = ImVec2(0, 0);
    ImVec2 window_pos_pivot = ImVec2(0, 0);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
}

void GUI::setMenu()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[0]);
    ImGui::Begin("Menu");
    if (ImGui::Button("Open model"))
    {
        controller->openModel();
    }
    ImGui::Text("% .1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("Model colour scale");
    if (ImGui::ColorPicker4("##Model_colour_scale", _modelColor))
    {
        controller->setModelColor(_modelColor);
    }
    ImGui::Text("Intencivity of the light");  
    if(ImGui::SliderFloat("##Intencivity_of_the_light", &_lightIntencity, 0.0f, 5.0f))
    {
        controller->setLigthIntencity(_lightIntencity);
    }
    ImGui::Text("X coordinate of light");
    if (ImGui::SliderFloat("##X_coordinate_of_light", &_lightDirection[0], -1.0f, 1.0f))
    {
        controller->setLigthDirection(_lightDirection);
    }
    ImGui::Text("Y coordinate of light");
    if (ImGui::SliderFloat("##Y_coordinate_of_light", &_lightDirection[1], -1.0f, 1.0f))
    {
        controller->setLigthDirection(_lightDirection);
    }
    ImGui::Text("Z coordinate of light");
    if (ImGui::SliderFloat("##Z_coordinate_of_light", &_lightDirection[2], -1.0f, 1.0f))
    {
        controller->setLigthDirection(_lightDirection);
    }
    ImGui::Text("Model size scale");
    
    if (ImGui::SliderFloat("##Model_size_scale", &_modelSize, 0.1f, 10.0f))
    {
        controller->setModelSize(_modelSize);
    }
    ImGui::Text("Point count limit");

    if (ImGui::DragInt("##Point_count_limit", &_vertexCountLimit, 3.0f, 3, 10000))
    {
        controller->setVertexLimit(_vertexCountLimit);
    }

    if (ImGui::Button("Create lowpoly version"))
    {
        controller->createLowPolyVersion();
    }
    ImGui::End();
    ImGui::PopFont();
}

void GUI::renderInterface()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
