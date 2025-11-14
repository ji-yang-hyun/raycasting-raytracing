#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include"map.h"
#include"rayc.h"

float renderTime;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    float min = (width < height) ? width : height;
    glViewport(0, 0, min, min);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        px += playerSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        px -= playerSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        py += playerSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        py -= playerSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        pt -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        pt += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
}

float wallVertics[] = {
    1.0f/mapSize, 1.0f/mapSize, 0.0f,
    -1.0f/mapSize, 1.0f/mapSize, 0.0f,
    1.0f/mapSize, -1.0f/mapSize, 0.0f,
    -1.0f/mapSize, -1.0f/mapSize, 0.0f,
};

float rayVertics[] = {
    0.01f, 1.0f, 0.1f, // 위, 오른쪽
    -0.01f, 1.0f, 0.1f, // 위, 왼쪽
    0.01f, 0.0f, 0.1f, // 아래, 오른쪽
    -0.01f, 0.0f, 0.1f // 아래, 왼쪽
};

unsigned int indices[] = {
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
                model, glm::vec3((wallx) * 2.0f/mapSize - 1, (wally) * 2.0f/mapSize - 1, 0)
            );
            ourShader.use();
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.0f,0.0f,0.0f,1.0f));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void drawRay(Shader ourShader, unsigned int VAOR, float ix, float iy){
    glBindVertexArray(VAOR);
    rayVertics[0] = 2*(ix/mapSize)/1.0f - 1.0f;
    rayVertics[1] = 2*(iy/mapSize)/1.0f - 1.0f;
    rayVertics[6] = 2*(px/mapSize)/1.0f - 1.0f;
    rayVertics[7] = 2*(py/mapSize)/1.0f - 1.0f;

    float k = 0.005/sqrt(pow((iy - py),2) + pow((ix - px),2)); // 두께를 정하기 위한 상수. 타겟 상수를 법선벡터의 길이로 나눠서 구함.
    glm::vec2 normalVector = glm::vec2(-1 * (iy - py) * k,(ix - px) * k); // 법선벡터, normalvector
    rayVertics[3] = rayVertics[0] + normalVector.x;
    rayVertics[4] = rayVertics[1] + normalVector.y;
    rayVertics[9] = rayVertics[6] + normalVector.x;
    rayVertics[10] = rayVertics[7] + normalVector.y;

    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertics), rayVertics, GL_STATIC_DRAW);
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7f,0.3f,1.0f,1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ray(Shader ourShader, unsigned int VAOR){
    for(int i=0;i<pixelX;i++){
        float theta = fmod(pt - povHorizontal/2 + dt/2 + i*dt, 360);
        pair<pair<float, float>, float > P;
        P = wallDistance(theta);
        float distance = P.second;
        float ix = P.first.first;
        float iy = P.first.second;
        // printf("각도 : %f, 거리 : %f, 교점 %f, %f \n", theta, distance, ix, iy);
        
        if(distance != -1){
            drawRay(ourShader, VAOR, ix, iy);
        }
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
    unsigned int VBOW;
    glGenBuffers(1, &VBOW);
    unsigned int VAOW;
    glGenVertexArrays(1, &VAOW);  
    glBindVertexArray(VAOW);
    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertics), wallVertics, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int EBOW;
    glGenBuffers(1, &EBOW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  


    // ray 단계
    unsigned int VBOR;
    glGenBuffers(1, &VBOR);
    unsigned int VAOR;
    glGenVertexArrays(1, &VAOR);  
    glBindVertexArray(VAOR);
    glBindBuffer(GL_ARRAY_BUFFER, VBOR);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertics), rayVertics, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int EBOR;
    glGenBuffers(1, &EBOR);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOR);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  

/*
    근데 여기서 알 수 있는게, vao만 바꾸고 vbo를 같이 썼더니 처음 vaow도 바뀌는걸 볼 수 있었다.
    따라서 vao는 vbo로의 연결과 포인터를 저장하는거지 그 vbo를 저장하는게 아니기 때문에 원본 vbo가 바뀌면 안된다는 거


    예를 들어 이래버리면 VAOW 쓰는 애들도 바뀌어버린다
    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertics), rayVertics, GL_STATIC_DRAW);
*/

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        renderTime = (float)glfwGetTime();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT); // zbuffer(deptBuffer)사용중이므로 매 루프마다 비워줘야 한다.

        
        // glBindVertexArray(VAOR);
        // glm::mat4 model = glm::mat4(1.0f);
        // ourShader.use();
        // ourShader.setMat4("model", model);
        // ourShader.setVec4("color", glm::vec4(0.0f,0.0f,0.0f,1.0f));
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        drawWall(ourShader, VAOW);
        ray(ourShader, VAOR);

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

/*
depth_testing상태에서 z값이 같은 경우에는 먼저 그린걸 가장 위에 놓는다.
*/