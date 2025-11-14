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

pair<float, float> setPosition(float newpx,float newpy){
    float x = newpx;
    float y = newpy;
    float xDistance, yDistance;
    for(int i=0;i<walls.size();i++){
        xDistance = newpx - walls[i].second;
        yDistance = newpy - walls[i].first;
        
        if(pow(xDistance, 2) <= 0.25 && pow(yDistance, 2) <= 0.25){
            printf("%f \n", xDistance);
            printf("%f %f\n", newpx, walls[i].second);
            x = px;
            y = py;
        }
    }
    return make_pair(x,y);
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

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        pair<float, float> position = setPosition(
            px + playerSpeed * ((float)glfwGetTime() - renderTime) * cos(PI/180*pt), 
            py + playerSpeed * ((float)glfwGetTime() - renderTime) * sin(PI/180*pt));
        
        px = position.first;
        py = position.second;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        pair<float, float> position = setPosition(
            px - playerSpeed * ((float)glfwGetTime() - renderTime) * cos(PI/180*pt), 
            py - playerSpeed * ((float)glfwGetTime() - renderTime) * sin(PI/180*pt));
        
        px = position.first;
        py = position.second;
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        pt -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
        if(pt - povHorizontal/2 < 0){
            pt += 360;
        }
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

    GLFWwindow* window = glfwCreateWindow(600, 600, "raycasting2D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    GLFWwindow* window2 = glfwCreateWindow(600, 600, "raycasting3D", NULL, NULL);
    if (window2 == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window2);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  
    // 어디서 해도 상관없는데 glfwMakeContextCurrent후에 해야함. 그리고 opengl함수 쓰기 전에.
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //공통 설정
    glfwMakeContextCurrent(window);
    glViewport(0,0,600,600);
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("shader.vs", "shader.fs");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window2);
    glViewport(0,0,600,600);
    glEnable(GL_DEPTH_TEST);  // depth test 키기
    glfwSetFramebufferSizeCallback(window2, framebuffer_size_callback);
    Shader ourShader2("shader.vs", "shader.fs");
    



//window를 위한 VAO, VBO, EBO 준비
    glfwMakeContextCurrent(window);
    // wall 단계
    unsigned int EBOW;
    glGenBuffers(1, &EBOW);
    unsigned int VBOW;
    glGenBuffers(1, &VBOW);
    unsigned int VAOW;
    glGenVertexArrays(1, &VAOW);  //VAO는 바인딩된 이후의 vertex속성의 호출을 저장한다
    glBindVertexArray(VAOW);
    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertics), wallVertics, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);는 VBO의 바인딩 후에!
    왜냐면, glVertexAttribPointer은 지금 바인딩 된 buffer(GL_ARRAY_BUFFER)를 어떻게 가르켜야 하는지를 보는 아이다.
    먼저 가르키고 거기에 바인딩이 되는건 안되고 바인딩은 된 후에 포인터를 설정하는 것 같다. 자세히는 모르지만 나름 그럴듯한 순서.

    VAO : VBO바인딩 하고 VBO는 어떻게 생겨먹었고 어디서 값을 가져오면 되는지. + EBO데이터
    "결국 어떤거의 어디서 값을 가져오자"를 저장한다. 그 값 자체는 저장하지 않는다. (VBO값 바꾸몀ㄴ 적용됨)
    VAO는 glVertexAttribPointer뿐만 아니라 뭘(VBOW) 바인딩해야하는지도 기억한다.
    그러니까 말 그대로 그 이후의 흐름 그 자체를 기억한다.
    */

   


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

    while(!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2))
    {
        processInput(window);
        renderTime = (float)glfwGetTime();

        glfwMakeContextCurrent(window);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawWall(ourShader, VAOW);
        ray(ourShader, VAOR);

        glfwSwapBuffers(window);

        glfwMakeContextCurrent(window2);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawWall(ourShader2, VAOW);
        ray(ourShader2, VAOR);

        glfwSwapBuffers(window2);
        glfwPollEvents();
    }


    glfwTerminate();

    setPosition(2,2);
    return 0;
}