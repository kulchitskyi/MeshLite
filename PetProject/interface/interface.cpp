#include "interface.h"

GUI::GUI(std::shared_ptr<Controller> _controller) 
    : controller(_controller) {}

void GUI::Initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    SetStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");
}

void GUI::SetStyle()
{
    ImGuiIO& io = ImGui::GetIO();
    const char* fontPath = "C:/Users/soapf/Downloads/JetBrainsMono-ExtraLight.ttf"; //TODO
    float fontSize = 26.0f;

    io.Fonts->AddFontFromFileTTF(fontPath, fontSize);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = 1.0;

    style.WindowRounding = 3;
    style.GrabRounding = 1;
    style.GrabMinSize = 20;
    style.FrameRounding = 3;

    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f); 
    style.Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.50f, 1.00f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.50f, 1.00f, 0.27f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.50f, 1.00f, 0.44f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.50f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.50f, 1.00f, 0.68f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.50f, 1.00f, 0.36f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.50f, 1.00f, 0.76f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 0.46f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.50f, 1.00f, 0.43f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.50f, 1.00f, 0.62f);
    style.Colors[ImGuiCol_Header] = ImVec4(1.00f, 0.50f, 1.00f, 0.33f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.50f, 1.00f, 0.42f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.50f, 1.00f, 0.54f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.50f, 1.00f, 0.54f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.50f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.50f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.50f, 1.00f, 0.22f);
}

void GUI::SetNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::SetWindowPos()
{
    ImVec2 window_pos = ImVec2(0, 0);
    ImVec2 window_pos_pivot = ImVec2(0, 0);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
}

void GUI::SetMenu()
{
    ImGui::Begin("Menu");
    if (ImGui::Button("Open model"))
    {
        controller->OpenModel();
    }
    ImGui::Text("% .1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("Model colour scale");
    //not deu
    if (ImGui::ColorPicker4("##Model_colour_scale", modelColor))
    {
        controller->SetModelColor(modelColor);
    }
    ImGui::Text("Intencivity of the light");  
    if(ImGui::SliderFloat("##Intencivity_of_the_light", &lightIntencity, 0.0f, 5.0f))
    {
        controller->SetLigthIntencity(lightIntencity);
    }
    ImGui::Text("X coordinate of light");
    if (ImGui::SliderFloat("##X_coordinate_of_light", &lightDirection[0], -1.0f, 1.0f))
    {
        controller->SetLigthDirection(lightDirection);
    }
    ImGui::Text("Y coordinate of light");
    if (ImGui::SliderFloat("##Y_coordinate_of_light", &lightDirection[1], -1.0f, 1.0f))
    {
        controller->SetLigthDirection(lightDirection);
    }
    ImGui::Text("Z coordinate of light");
    if (ImGui::SliderFloat("##Z_coordinate_of_light", &lightDirection[2], -1.0f, 1.0f))
    {
        controller->SetLigthDirection(lightDirection);
    }
    ImGui::Text("Model size scale");
    
    if (ImGui::SliderFloat("##Model_size_scale", &modelSize, 0.1f, 10.0f))
    {
        controller->SetModelSize(modelSize);
    }
    ImGui::Text("Edge length limit");
    
    if (ImGui::SliderFloat("##Edge_length_limit", &edgeLimit, 0.0001f, 10.0f))
    {
        controller->SetEdgeLimit(edgeLimit);
    }
    if (ImGui::Button("Create lowpoly version"))
    {
        controller->CreateLowPolyVersion();
    }
    ImGui::End();
}

void GUI::RenderInterface()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
