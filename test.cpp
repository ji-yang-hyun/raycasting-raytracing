#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        // 위치              // 컬러
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
    };    

    float texCoords[] = { // 텍스쳐 좌표(정점 좌표와 같은 좌표, 같은 좌표계 사용)
        0.0f, 0.0f,  // 좌측 하단 모서리  
        1.0f, 0.0f,  // 우측 하단 모서리
        0.5f, 1.0f   // 꼭대기 모서리
    };
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // GL_TEXTURE_WRAP_S는 x축을 나타낸다. 텍스쳐 설정에서는 s,t,r을 사용하고 이는 x,y,z를 나타낸다
    /*
    기본적으로 텍스쳐는 0~1사이의 좌푯값을 가지지만, 그 밖의 점을 나타낼 때 어떻게 할지를 설정할 수 있다
    GL_REPEAT: 텍스처의 기본 동작입니다. 이미지를 반복합니다.
    GL_MIRRORED_REPEAT: GL_REPEAT와 같지만 반복할때마다 이미지를 반대로 뒤집습니다.
    GL_CLAMP_TO_EDGE: 0과 1 사이의 좌표를 고정합니다. 결과적으로 큰 좌표가 가장자리에 고정되어 가장자리의 패턴이 늘어나게 됩니다.
    GL_CLAMP_TO_BORDER: 범위 밖의 좌표에 사용자가 지정한 테두리 색이 지정됩니다.

    (GL_CLAMP_TO_BORDER사용했을 때 그 밖의 채우기 색상)
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /*
    GL_TEXTURE_MIN_FILTER : 축소될 때 사용할 필터
    GL_TEXTURE_MAG_FILTER : 확대될 때 사용할 필터
    텍스처에서 픽셀의 rgb값을 해상도에 맞게 설정하는것에서 어떤 값을 사용할지 정하는 옵션중 유명한 것 두 가지
    GL_NEAREST : 말 그대로 가장 가까운 픽셀
    GL_LINEAR : 픽셀 주변 텍셀(텍스쳐 원본에서의 픽셀)의 rgb값의 합성
    */

    // mipmap - 텍스쳐가 작아질 때 사용하기 위해 만들어두는 1/2크기의 또다른 텍스쳐.(들 1/2등비수열 크기를 가짐)
    // mipmap level을 필터링해서 어떤 크기의 mipmap을 사용할지 정해야함
    /*
    GL_NEAREST_MIPMAP_NEAREST: nearest neighbor 보간법으로 mipmap을 필터링하고 텍스처 샘플링도 nearest neghbor 보간법을 사용합니다.
    GL_LINEAR_MIPMAP_NEAREST: nearest neighbor 보간법으로 mipmap을 필터링하고 텍스처 샘플링은 linear 보간법을 사용합니다.
    GL_NEAREST_MIPMAP_LINEAR: linear 보간법으로 mipmap을 필터링하고 텍스처 샘플링은 nearest neighbor 보간법을 사용합니다.
    GL_LINEAR_MIPMAP_LINEAR: linear 보간법으로 mipmap을 필터링하고 텍스처 샘플링도 linear 보간법을 사용합니다.
    
    필터 설정으로 사용 가능
    mipmap은 축소되는것만 저장하니 확대에서는 아무런 쓸모 없다
    */

    Shader ourShader("shader.vs", "shader.fs");


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);  

    glBindVertexArray(VAO);


    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        ourShader.use();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}