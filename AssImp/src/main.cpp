#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <src/camera.h>
#include <src/window.h>
#include <src/shader.h>
#include <src/model.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearErrors() {
    while (glGetError() != GL_NO_ERROR) {}
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGl Error] (" << error << " | 0x" << std::hex << error << std::dec << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
Camera camera = Camera();
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseLocked = false;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);

    if (glfwGetMouseButton(window, 1)) {
        if (mouseLocked)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0, 0);
        }
        mouseLocked = !mouseLocked;
    }
    if (mouseLocked) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        camera.ProcessMouseMovement((float)x, (float)-y, true);
        glfwSetCursorPos(window, 0, 0);
    }
}

int main() {
    Window window = Window(1600, 1000);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.windowHandle, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    float vertices[] = {
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    };

    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    unsigned int VBO;
    GLCall(glGenBuffers(1, &VBO));

    GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));

    unsigned int lightVAO;
    GLCall(glGenVertexArrays(1, &lightVAO));

    GLCall(glBindVertexArray(lightVAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glEnableVertexAttribArray(2));

    Shader lightShader("res\\basicVertex.shader", "res\\fragment.shader");
    Shader shader("res\\textureVertex.shader", "res\\shadedFragment.shader");

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("res\\container.jpg", &width, &height, &nrChannels, 0);
    unsigned int texture;
    GLCall(glGenTextures(1, &texture));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    const float radius = 10.0f;

    Shader ourShader("res/1.model_loading.vs", "res/1.model_loading.fs");
    Model ourModel("C:/Users/Me/MyStuff/Dev/C/AssImp/AssImp/res/model/backpack.obj");

    while (!glfwWindowShouldClose(window.windowHandle))
    {
        processInput(window.windowHandle);

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window.screenWidth != 0 && window.screenHeight != 0) //Window is minimized
        {
            GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            glm::mat4 model = glm::mat4(1.0f);

            glm::mat4 view = camera.GetViewMatrix();

            glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)window.screenWidth / window.screenHeight, 0.1f, 100.0f);

            /*shader.use();
            shader.setMat4f("model", model);
            //shader.setMat3f("normalModel", normalModel);
            shader.setMat4f("view", view);
            shader.setMat4f("projection", projection);
            shader.setInt("texture1", 0);
            shader.setVec3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            shader.setVec3f("lightPos", lightPos);
            shader.setVec4f("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            shader.setVec3f("cameraPos", camera.Position);

            GLCall(glBindVertexArray(VAO));
            glDrawArrays(GL_TRIANGLES, 0, 36);*/
            static glm::vec3 lightPos1 = glm::vec3(0, 0, 0);
            static glm::vec3 lightPos2 = glm::vec3(0, 0, 0);
            static glm::vec3 lightPos3 = glm::vec3(0, 0, 0);
            static glm::vec3 lightColor1 = glm::vec3(0, 0, 0);
            static glm::vec3 lightColor2 = glm::vec3(0, 0, 0);
            static glm::vec3 lightColor3 = glm::vec3(0, 0, 0);
            static float lightAttenuation1 = 0.5f;
            static float lightAttenuation2 = 0.5f;
            static float lightAttenuation3 = 0.5f;

            ourShader.use();
            ourShader.setMat4f("projection", projection);
            ourShader.setMat4f("view", view);
            ourShader.setMat4f("model", model);
            ourShader.setVec3f("lights[0].viewPosition", glm::vec3(view * glm::vec4(lightPos1, 1.0)));
            ourShader.setVec3f("lights[0].color", lightColor1);
            ourShader.setFloat("lights[0].attenuationCoefficient", lightAttenuation1);
            ourShader.setVec3f("lights[1].viewPosition", glm::vec3(view * glm::vec4(lightPos2, 1.0)));
            ourShader.setVec3f("lights[1].color", lightColor2);
            ourShader.setFloat("lights[1].attenuationCoefficient", lightAttenuation2);
            ourShader.setVec3f("lights[2].viewPosition", glm::vec3(view * glm::vec4(lightPos3, 1.0)));
            ourShader.setVec3f("lights[2].color", lightColor3);
            ourShader.setFloat("lights[2].attenuationCoefficient", lightAttenuation3);
            ourModel.Draw(ourShader);

            
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos1);
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

            lightShader.use();
            lightShader.setMat4f("model", model);
            lightShader.setMat4f("view", view);
            lightShader.setMat4f("projection", projection);


            GLCall(glBindVertexArray(lightVAO));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos2);
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

            lightShader.use();
            lightShader.setMat4f("model", model);
            lightShader.setMat4f("view", view);
            lightShader.setMat4f("projection", projection);


            GLCall(glBindVertexArray(lightVAO));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos3);
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

            lightShader.use();
            lightShader.setMat4f("model", model);
            lightShader.setMat4f("view", view);
            lightShader.setMat4f("projection", projection);


            GLCall(glBindVertexArray(lightVAO));
            glDrawArrays(GL_TRIANGLES, 0, 36);



            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            bool stupid = true;
            ImGui::ShowDemoWindow(&stupid);

            ImGui::Begin("Test");
            ImGui::DragFloat3("Light Position1", glm::value_ptr(lightPos1), 0.025f, 0.0f);
            ImGui::SliderFloat3("Light Color1", glm::value_ptr(lightColor1), 0.0f, 1.0f);
            ImGui::SliderFloat("Light Attenuation1", &lightAttenuation1, 0.0f, 1.0f);
            ImGui::DragFloat3("Light Position2", glm::value_ptr(lightPos2), 0.025f, 0.0f);
            ImGui::SliderFloat3("Light Color2", glm::value_ptr(lightColor2), 0.0f, 1.0f);
            ImGui::SliderFloat("Light Attenuation2", &lightAttenuation2, 0.0f, 1.0f);
            ImGui::DragFloat3("Light Position3", glm::value_ptr(lightPos3), 0.025f, 0.0f);
            ImGui::SliderFloat3("Light Color3", glm::value_ptr(lightColor3), 0.0f, 1.0f);
            ImGui::SliderFloat("Light Attenuation3", &lightAttenuation3, 0.0f, 1.0f);

            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwSwapBuffers(window.windowHandle);
            //glClear();
            //glFlush();
            glFinish(); //out of the three, this is only way vsync works, no idea why
            //std::cout << 1/deltaTime << std::endl;
        }

        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}