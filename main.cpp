#include "Header.h"
#include "Callback.h"
#include "Shader.h"
#include "auxiliary.h"
#include "Model.h"

void do_movement();

double deltaTime = 0.0f;
double lastFrame = 0.0f;

bool skyboxEnable = false;
bool modelEnable = false;
int lightKind = 0;

int main() {
    // Create Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CS", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // Set Callback Function
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Init GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    // Set Window
    glViewport(0, 0, WIDTH, HEIGHT);
    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    // Enable Anti Aliasing
    glEnable(GL_MULTISAMPLE);
    // Create Shader
    Shader lampShader(lampVSPath, lampFragPath);
    Shader skyboxShader(skyboxVSPath, skyboxFragPath);
    Shader modelShader(modelVSPath, modelFragPath);
    // Create VAO and VBO
    GLuint VAO, VBO, lightVAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind Cube's VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    // Bind light's VAO
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Light directLight(vec3(-0.2, -1.0, -0.3), vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0));
    Light pointLight(vec3(0, 30, 0), vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), 1.0, 0.00001, 0.000001);
    Light spotLight(vec3(0.1f, 0.1f, 0.1f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f), 12.5, 17.5, 1.0, 0.0001, 0.000001);

    // Bind texture
    GLuint diffTex = loadTexture(containerPicPath, PNG);
    GLuint specularTex = loadTexture(containerSpecularPath, PNG);
    GLuint skyboxTex = loadCubemap(skyBoxPath);
    Model ourModel(modelPath);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (skyboxEnable)
            drawSkybox(VAO, skyboxShader, skyboxTex);

        if (lightKind == 0) {
            drawObject(VAO, directLight, diffTex, specularTex);
        } else if (lightKind == 1) {
            drawObject(VAO, pointLight, diffTex, specularTex);
            pointLight.Draw(VAO, lampShader);
        } else {
            drawObject(VAO, spotLight, diffTex, specularTex);
        }

        mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));	// It's a bit too big for our scene, so scale it down
        if (modelEnable) {
            drawModel(ourModel, modelShader, model);
        }
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void do_movement() {
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_T])
        skyboxEnable = !skyboxEnable;
    if(keys[GLFW_KEY_L])
        lightKind = (lightKind + 1) % 3;
    if(keys[GLFW_KEY_M])
        modelEnable = !modelEnable;
}
