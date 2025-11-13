#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"map.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float wallVertics[] = {
    1.0f/mapSizeX, 1.0f/mapSizeY, 0.0f,
    -1.0f/mapSizeX, 1.0f/mapSizeY, 0.0f,
    1.0f/mapSizeX, -1.0f/mapSizeY, 0.0f,
    -1.0f/mapSizeX, -1.0f/mapSizeY, 0.0f,
};

unsigned int wallIndices[] = {
    0,1,2,
    1,2,3
};


void drawWall(Shader ourShader, unsigned int VAOW){
    glBindVertexArray(VAOW);
    for(unsigned int i = 0; i < walls.size(); i++)
        {
            float wallx = walls[i].second;
            float wally = walls[i].first;

            glm::mat4 model;
            model = glm::translate(
                model, glm::vec3((wallx) * 2.0f/mapSizeX - 1, (wally) * 2.0f/mapSizeY - 1, 0)
            );
            ourShader.use();
            ourShader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}


int main(){

    wallCoordinate();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);  // depth test 키기
    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("shader.vs", "shader.fs");


    // wall 단계
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAOW;
    glGenVertexArrays(1, &VAOW);  
    glBindVertexArray(VAOW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertics), wallVertics, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  


    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT); // zbuffer(deptBuffer)사용중이므로 매 루프마다 비워줘야 한다.

        glm::mat4 model = glm::mat4(1.0f);
        
        ourShader.use();
        ourShader.setMat4("model", model);

        drawWall(ourShader, VAOW);

        // glBindVertexArray(VAOW);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);


        // for(unsigned int i = 0; i < walls.size(); i++)
        // {
        //     float wallx = walls[i].second;
        //     float wally = walls[i].first;

        //     glm::mat4 model;
        //     model = glm::translate(model, glm::vec3(wallx - 0.5, wally - 0.5, 0));
        //     ourShader.use();
        //     ourShader.setMat4("model", model);

        //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}


/*
파일을 include한다는 것은 그걸 앞쪽에 그대로 복사한다는것과 같다
그래서 같은걸 여러 파일에서 include한다면 여기저기 파일에 복사돼서 여기저기서 같은 변수가 생성되는것과 같은 효과가 나온다.
그래서 우리는 
#ifndef로 이게 복사가 된 적이 있는지 없는지, 있다면 그냥 실행하지 않고 넘어갈 수 있게 해야한다

컴파일 후의 마지막 목적파일 .o파일.

근데 이게 h파일만 가능한 것 같다. 왜냐면 이게 h파일은 목적파일 o파일이 나오지 않는것에 비해 cpp파일은 그 자체로 목적파일이 나와버리니
#ifndef를 사용해도 최소 2개의 목적파일에서 생성하는것처럼 되어버리는 듯.
그동안 그냥 main굴릴때는 몰랐는데 컴파일, 빌드 하려면 그런 것 같다
*/