#include "Headers.h"
#include "Shader.h"
#include "Controller.h"
#include "Mesh.h"
#include "Model.h"
#include "SkyBox.h"
#include "Shadow.h"
#include "ScreenFBO.h"
#include "GBuffer.h"
#include "SSAO.h"

int main() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CG_DEMO", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, Controller::KeyCallback);
    glfwSetCursorPosCallback(window, Controller::MouseCallback);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    mainCamera = new Camera(vec3(0, 10, 0));
    // VAOs
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    Shader boxShader("Shaders/box.vs", "Shaders/box.frag");
    // Bind VAO for Screen
    GLuint screenVAO, screenVBO;
    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);
    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    ScreenFBO screenFBO;
    // VAO for floor
    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindVertexArray(0);
    Texture floorTexture("Textures/Objects/wood.png");
    // Init shader
    Shader shaderGeometryPass("Shaders/ssaoGeometry.vs", "Shaders/ssaoGeometry.frag");
    Shader shaderLightingPass("Shaders/ssao.vs", "Shaders/ssaoLight.frag");
    Shader shaderSSAO("Shaders/ssao.vs", "Shaders/ssao.frag");
    Shader shaderSSAOBlur("Shaders/ssao.vs", "Shaders/ssaoBlur.frag");
    Shader screen("Shaders/screen.vs", "Shaders/screenNormal.frag");
    // Set samplers
    shaderLightingPass.Use();
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gPositionDepth"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gAlbedo"), 2);
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "ssao"), 3);
    shaderSSAO.Use();
    glUniform1i(glGetUniformLocation(shaderSSAO.Program, "gPositionDepth"), 0);
    glUniform1i(glGetUniformLocation(shaderSSAO.Program, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderSSAO.Program, "texNoise"), 2);
    // Objects
    Model nanosuit("Models/nanosuit/nanosuit.obj");
    // Lights
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    GBuffer gBuffer;
    SSAO ssao;
    double deltaTime = 0.0f, lastFrame = 0.0f;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Check and call events
        glfwPollEvents();
        Controller::Movement(deltaTime);
        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now

        gBuffer.Record();
        glm::mat4 projection = glm::perspective(mainCamera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 50.0f);
        glm::mat4 view = mainCamera->GetViewMatrix();
        glm::mat4 model;
        shaderGeometryPass.Use();
        glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Floor cube
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // Nanosuit model on the floor
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
        model = glm::rotate(model, -70.0f, glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "texture_diffuse1"), 0);
        nanosuit.Draw(shaderGeometryPass);
        gBuffer.Finish();

        ssao.RecordSSAO();
        shaderSSAO.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPositionDepth);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.noiseTex);
        // Send kernel + rotation
        for (GLuint i = 0; i < 64; ++i)
            glUniform3fv(glGetUniformLocation(shaderSSAO.Program, ("samples[" + std::to_string(i) + "]").c_str()), 1, &ssao.ssaoKernel[i][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderSSAO.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        ssao.FinishSSAO();

        ssao.RecordBlur();
        shaderSSAOBlur.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorTex);
        glBindVertexArray(screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        ssao.FinishBlur();

        shaderLightingPass.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPositionDepth);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);
        glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBlurTex);
        glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "ssaoColorBlurTex"), 3);
        glm::vec3 lightPosView = glm::vec3(mainCamera->GetViewMatrix() * glm::vec4(lightPos, 1.0));
        glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "light.Position"), 1, &lightPosView[0]);
        glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "viewPos"), 1, &mainCamera->Position[0]);
        glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "light.Color"), 1, &lightColor[0]);
        const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const GLfloat linear = 0.09;
        const GLfloat quadratic = 0.032;
        glUniform1f(glGetUniformLocation(shaderLightingPass.Program, "light.Linear"), linear);
        glUniform1f(glGetUniformLocation(shaderLightingPass.Program, "light.Quadratic"), quadratic);
        glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "draw_mode"), 0);

        glBindVertexArray(screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
