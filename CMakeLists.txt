cmake_minimum_required(VERSION 3.7)
project(CG_Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Headers.h main.cpp
        Shader.cpp Shader.h
        Camera.cpp Camera.h
        GlobeValue.h GlobeValue.cpp
        Mesh.cpp Mesh.h
        Light.cpp Light.h
        Controller.cpp Controller.h
        Texture.cpp Texture.h
        Definition.cpp Definition.h
        Model.cpp Model.h
        CubemapTexture.cpp
        CubemapTexture.h
        SkyBox.cpp SkyBox.h ScreenFBO.cpp ScreenFBO.h Shadow.cpp Shadow.h GBuffer.cpp GBuffer.h)

add_executable(CG_Project ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} glut)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} GLU)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} GLEW)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} glfw3)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} GL)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} X11)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xrandr)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xi)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xxf86vm)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xinerama)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Xcursor)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} SOIL)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} assimp)