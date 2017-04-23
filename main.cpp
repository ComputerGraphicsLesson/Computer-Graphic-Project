#include "Header.h"
#include "Callback.h"
#include "Shader.h"
#include "auxiliary.h"
#include "Model.h"

void do_movement();

double deltaTime = 0.0f;
double lastFrame = 0.0f;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CS", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // create shader
    Shader objShader(objVSPath, objFragPath);
    Shader lampShader(lampVSPath, lampFragPath);
    Shader skyboxShader(skyboxVSPath, skyboxFragPath);
    Shader modelShader(modelVSPath, modelFragPath);
    // bind array
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

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

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    vec3 lightPos = vec3(0, 20, 0);

    // bind texture
    GLuint texture1 = loadTexture(containerPicPath, PNG);
    GLuint texture2 = loadTexture(containerSpecularPath, PNG);


    GLuint cubemapTexture = loadCubemap(skyBoxPath);
//    Model ourModel(modelPath);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        drawSkybox(VAO, skyboxShader, cubemapTexture);

        lampShader.Use();
        mat4 view = camera.GetViewMatrix();
        mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)WIDTH, 0.1f, 100.0f);
        GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lampShader.Program, "view");
        GLint projLoc  = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        mat4 model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, vec3(4, 4, 4));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        drawObject(VAO, objShader, lightPos, texture1, texture2);

//        model = glm::mat4();
//        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
//        model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));	// It's a bit too big for our scene, so scale it down
//        drawModel(ourModel, modelShader, model);




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
}
