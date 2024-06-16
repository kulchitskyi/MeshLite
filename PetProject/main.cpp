#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "FileDialog.h"
#include "lowpoly/lowPolyGenerator.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(std::vector<std::string> faces);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

class Conditions
{
public:
    bool drawModel = false;
    bool loadModel = false;
    bool isMRpressed = false;

    float modelSize = 1.0f;
    float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float edgeLengthLimit = 0.001f;
    float lightIntencivity = 0.4f;
    std::string path = "";
};

Conditions cond;

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lowpoly generator", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Shader modelShader("vs.txt", "fs.txt");

    Model Model(cond.path);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO(); 
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    glm::vec3 lightDiraction = { -0.2f, -1.0f, -0.3f };
    
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 window_pos = ImVec2(0, 0);
        ImVec2 window_pos_pivot = ImVec2(0, 0);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

        modelShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        modelShader.setVec3("viewPos", camera.Position);

        modelShader.setVec3("dirLight.direction", lightDiraction);
        modelShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        modelShader.setVec3("dirLight.diffuse", cond.lightIntencivity, cond.lightIntencivity, cond.lightIntencivity);
        modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        if (cond.drawModel) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
            model = glm::scale(model, glm::vec3(cond.modelSize, cond.modelSize, cond.modelSize));
            modelShader.setMat4("model", model);
            Model.Draw(modelShader);
        }

        static float color[4] = { 0.4f, 0.7f, 0.0f, 1.0f };

        ImGui::Begin("Menu");
        if (ImGui::Button("Open model"))
        {
            FileDialog fileDialog;
            std::string newPath = fileDialog.OpenFile("OBJ files (*.obj)\0*.obj\0"); //filters for .obj only
            if (newPath != "")
            {
                Model.loadModel(newPath);
                cond.loadModel = false;
                cond.path = newPath;
                std::cout << "new path : " << newPath;
            }
        }

        ImGui::Text( "% .1f FPS", ImGui::GetIO().Framerate);
        ImGui::Checkbox("Draw model", &cond.drawModel);
        ImGui::Text("Model colour scale");
        if (ImGui::ColorPicker4("           ", color))
        {
            modelShader.setFloat("color[0]", color[0]);
            modelShader.setFloat("color[1]", color[1]);
            modelShader.setFloat("color[2]", color[2]);
            modelShader.setFloat("color[3]", color[3]);
        }
        ImGui::Text("Intencivity of the light");
        ImGui::SliderFloat("        ", &cond.lightIntencivity, 0.0f, 5.0f);
        ImGui::Text("X coordinate of light");
        ImGui::SliderFloat(" ", &lightDiraction[0], -1.0f, 1.0f);
        ImGui::Text("Y coordinate of light");
        ImGui::SliderFloat("  ", &lightDiraction[1], -1.0f, 1.0f);
        ImGui::Text("Z coordinate of light");
        ImGui::SliderFloat("   ", &lightDiraction[2], -1.0f, 1.0f);
        ImGui::Text("Model size scale");
        ImGui::SliderFloat("    ", &cond.modelSize, 0.1f, 10.0f);
        ImGui::Text("Edge length limit");
        ImGui::SliderFloat("     ", &cond.edgeLengthLimit, 0.0001f, 10.0f);
        if (ImGui::Button("Create lowpoly version"))
        {
            VDPL::GeneratorInterface generator(cond.path, cond.edgeLengthLimit);
            std::cout << "PATH:" << generator.lowpolyFilePath;
            cond.path = generator.lowpolyFilePath;
            Model.loadModel(cond.path);
            cond.loadModel = false;
        }
        ImGui::End();
   

        modelShader.setVec4("color", color[0], color[1], color[2], color[3]);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        cond.isMRpressed = true;
    else
        cond.isMRpressed = false;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (cond.isMRpressed)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
