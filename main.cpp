#include "Headers.h"
#include "Shader.h"
#include "Controller.h"
#include "Mesh.h"
#include "Model.h"
#include "SkyBox.h"
#include "Shadow.h"
#include "ScreenFBO.h"

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
    // Skybox
    Model skyboxModel("Models/sphere.obj");
    SkyBox skybox(mainCamera, &skyboxModel, skyBoxPath);
    Shader skyoxShader("Shaders/skybox.vs", "Shaders/skybox.frag");
    // Model -> nanosuit
    Model nanosuit("Models/nanosuit/nanosuit.obj");
    Shader modelShader("Shaders/model.vs", "Shaders/model.frag");
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
    Shader blinnShader("Shaders/blinn-phong.vs", "Shaders/blinn-phong.frag");
    // Screen Feature Shader
    Shader screenShader("Shaders/advanceLightWithShadow.vs", "Shaders/advanceLightWithShadow.frag");
    Shader normalShader("Shaders/screen.vs", "Shaders/screenHDR.frag");
    // Shadow Shader
    Shader shadowShader("Shaders/shadow.vs", "Shaders/shadow.frag");
    // Shadow
    Shadow shadow(1024, 1024);
    vec3 lightPos = vec3(0, 6, 0);
    double deltaTime = 0.0f, lastFrame = 0.0f;
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Check and call events
        glfwPollEvents();
        Controller::Movement(deltaTime);
        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now

        glm::mat4 model;
        glm::mat4 view = mainCamera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(mainCamera->Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 0.1f, far_plane = 40.0f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
        lightSpaceMatrix = lightProjection * lightView;

        shadow.Record(shadowShader, lightSpaceMatrix);

        glBindVertexArray(planeVAO);
        model = mat4();
        glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        model = glm::translate(mat4(), vec3(0, 2, 0));
        model = glm::scale(model, vec3(2, 2, 2));
        glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        shadow.Finish();

        screenFBO.Record();

        screenShader.Use();

        projection = glm::perspective(mainCamera->Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        view = mainCamera->GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(screenShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(screenShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(screenShader.Program, "lightPos"), 1, &lightPos[0]);
        glUniform3fv(glGetUniformLocation(screenShader.Program, "viewPos"), 1, &mainCamera->Position[0]);
        glUniformMatrix4fv(glGetUniformLocation(screenShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture.id);
        glUniform1i(glGetUniformLocation(screenShader.Program, "diffuseTexture"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow.shadowTex);
        glUniform1i(glGetUniformLocation(screenShader.Program, "shadowMap"), 1);

        glBindVertexArray(planeVAO);
        model = mat4();
        glUniformMatrix4fv(glGetUniformLocation(screenShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        model = glm::translate(mat4(), vec3(0, 2, 0));
        model = glm::scale(model, vec3(2, 2, 2));
        glUniformMatrix4fv(glGetUniformLocation(screenShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        screenFBO.Finish();

        normalShader.Use();
        glBindVertexArray(screenVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenFBO.texColorBuffer);
        glUniform1i(glGetUniformLocation(normalShader.Program, "screenTexture"), 0);
        glUniform1f(glGetUniformLocation(normalShader.Program, "exposure"), 2.0f);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
//         Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
