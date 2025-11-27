#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include<algorithm>
#include <time.h>
#include <unistd.h> // <windows.h> 대체
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
        xDistance = newpx - walls[i].x;
        yDistance = newpy - walls[i].y;
        
        if(pow(xDistance, 2) <= 0.25 && pow(yDistance, 2) <= 0.25){
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
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        pair<float, float> position = setPosition(
            px + playerSpeed * ((float)glfwGetTime() - renderTime) * cos(PI/180*ptx), 
            py + playerSpeed * ((float)glfwGetTime() - renderTime) * sin(PI/180*ptx));
        
        px = position.first;
        py = position.second;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        pair<float, float> position = setPosition(
            px - playerSpeed * ((float)glfwGetTime() - renderTime) * cos(PI/180*ptx), 
            py - playerSpeed * ((float)glfwGetTime() - renderTime) * sin(PI/180*ptx));
        
        px = position.first;
        py = position.second;
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        pty += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        pty -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
        if(pty - povVertical/2 < 0){
            pty += 360;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        ptx -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
        if(ptx - povHorizontal/2 < 0){
            ptx += 360;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        ptx += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }


    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        layerToSee += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        layerToSee -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
}

float wallVertics[] = {
    1.0f/mapSize, 1.0f/mapSize, 0.0f, // 위 오른쪽
    -1.0f/mapSize, 1.0f/mapSize, 0.0f, // 위 왼쪽
    1.0f/mapSize, -1.0f/mapSize, 0.0f, // 아래 오른쪽
    -1.0f/mapSize, -1.0f/mapSize, 0.0f, // 아래 왼쪽
};

float rayVertics[] = {
    0.01f, 1.0f, -0.1f, // 위, 오른쪽
    -0.01f, 1.0f, -0.1f, // 위, 왼쪽
    0.01f, 0.0f, -0.1f, // 아래, 오른쪽
    -0.01f, 0.0f, -0.1f // 아래, 왼쪽
};

float wallEyeVertics[] = {
    0.0f, 1.0f, 0.0f, // 위, 오른쪽
    -2.0f/(pixelX), 1.0f, 0.0f, // 위, 왼쪽
    0.0f, 0.0f, 0.0f, // 아래, 오른쪽
    -2.0f/(pixelX), 0.0f, 0.0f // 아래, 왼쪽
};

float pixelVertics[] = {
    0.0f, -2.0f/(pixelY), 0.0f, // 위, 오른쪽
    -2.0f/(pixelX), -2.0f/(pixelY), 0.0f, // 위, 왼쪽
    0.0f, 0.0f, 0.0f, // 아래, 오른쪽
    -2.0f/(pixelX), 0.0f, 0.0f // 아래, 왼쪽
};

unsigned int indices[] = {
    0,1,2,
    1,2,3
};




void drawWall(Shader ourShader, unsigned int VAOW, GLFWwindow* window1){
    glfwMakeContextCurrent(window1);
    glBindVertexArray(VAOW);
    for(unsigned int i = 0; i < walls.size(); i++)
        {
            float wallx = walls[i].x;
            float wally = walls[i].y;

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

void drawRay(Shader ourShader2, unsigned int VAOR, unsigned int VBOR, float ix, float iy, GLFWwindow* window1){
    glfwMakeContextCurrent(window1);
    glBindVertexArray(VAOR);
    glBindBuffer(GL_ARRAY_BUFFER, VBOR);

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
    ourShader2.use();
    ourShader2.setMat4("model", model);
    ourShader2.setVec4("color", glm::vec4(0.7f,0.3f,1.0f,1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void drawPixel(int i, int j, float distance, 
    Shader ourShader, unsigned int VAOP, unsigned int VBOP, GLFWwindow* window){

    if(i <= 0 || j <= 0){
        return;
    }

    glfwMakeContextCurrent(window);
    glBindVertexArray(VAOP);
    glBindBuffer(GL_ARRAY_BUFFER, VBOP);


    // printf("%f, %f, %f\n",(j*2.0f)/pixelX - 1, (i*2.0f)/pixelY - 1, distance);


    glm::mat4 model;
    model = glm::translate(
        model, glm::vec3((j*2.0f)/pixelX - 1,  (i*2.0f)/pixelY - 1, 0)
    );
    float color = distance / (maxSightRange);

    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.0f,0.0f,0.0f,1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ray3D(
    Shader ourShader, Shader ourShader2, 
    unsigned int VAOR, unsigned int  VBOR, 
    unsigned int VAOP, unsigned int VBOP,
    GLFWwindow* window, GLFWwindow* window1){
    glm::vec3 D = glm::vec3(cos(PI/180*pty) * cos(PI/180*ptx), cos(PI/180*pty) * sin(PI/180*ptx),sin(PI/180*pty));
    glm::vec3 Dnr = glm::vec3(sin(PI/180*ptx), -cos(PI/180*ptx), 0);// Srow를 위한 D의 평면직교벡터. 노트에 있는 Dㅗ
    glm::vec3 Dnc = glm::vec3(D.z*Dnr.y - D.y*Dnr.z, D.x*Dnr.z - D.z*Dnr.x, D.y*Dnr.x - D.x*Dnr.y);
    // Scol를 위한 D와 Dㅗ의 외적벡터. 노트에 있는 n

    glm::vec3 Sr = scaleVector(Dnr, 1.0f/(tan(PI/180 * (1.0f/2)*povHorizontal)));
    glm::vec3 Sc = scaleVector(Dnc, 1.0f/(tan(PI/180 * (1.0f/2)*povVertical)));

    glm::vec3 Rd; // ray의 플레이어 좌표 기준 방향벡터
    glm::mat3 scaleX;
    glm::mat3 scaleY;

    for(int i=0;i<pixelY;i++){
        for(int j=0;j<pixelX;j++){
            Rd = D + scaleVector(Sc, (i - (pixelY/2.0f)) / pixelY) + scaleVector(Sr, (j - (pixelX/2.0f)) / pixelX);
            pair<float, glm::vec3> P = wallDistance(Rd);
            // printf("%f\n", ((0.5)*povHorizontal));
            if(P.first != -1){
                if(floor(layerToSee) == (i - pixelY/2)){
                    drawRay(ourShader2, VAOR, VBOR, P.second.x, P.second.y, window1);
                }
                drawPixel(i, j, P.first, ourShader, VAOP, VBOP, window);
            }
        }
    }
}


vector<pair<float, glm::vec2 > > wallRay(
    int i,
    Shader ourShader2, unsigned int VAOR, unsigned int VBOR, GLFWwindow* window1
){
    glm::vec3 W = walls[i];
    glm::vec3 k[] = {
        glm::vec3(1,1,1), 
        glm::vec3(1,1,-1), 
        glm::vec3(1,-1,1), 
        glm::vec3(1,-1,-1), 
        glm::vec3(-1,1,1),
        glm::vec3(-1,1,-1),
        glm::vec3(-1,-1,1),
        glm::vec3(-1,-1,-1),
    };

    vector<pair<float, glm::vec2 > > pixels;
    for(int j=0;j<8;j++){
        pair<float, glm::vec2 > P = getPixelFromVertices(W + scaleVector(k[j], 0.5));
        if(P.first != -1){
            drawRay(ourShader2, VAOR, VBOR, (W + scaleVector(k[j], 0.5)).x, (W + scaleVector(k[j], 0.5)).y, window1);
        }
        
        pixels.push_back(P);
    }

    return pixels;
}


void wallsRay(
    Shader ourShader, Shader ourShader2, 
    unsigned int VAOR, unsigned int  VBOR, 
    unsigned int VAOP, unsigned int VBOP,
    GLFWwindow* window, GLFWwindow* window1){
    
    for(int i=0;i<walls.size(); i++){
        vector<pair<float, glm::vec2 > > pixels = wallRay(i, ourShader2, VAOR, VBOR, window1);

        for(int j=0;j<8;j++){
            if(pixels[j].first != -1){
                drawPixel(floor(pixels[j].second.y), floor(pixels[j].second.x), pixels[j].first,
                ourShader, VAOP, VBOP, window
                );
            }
        }
    }
}


int main(){

    wallCoordinate();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window1 = glfwCreateWindow(600, 600, "raycasting2D", NULL, NULL);
    if (window1 == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window1);

    GLFWwindow* window = glfwCreateWindow(600, 600, "raycasting3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

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
    Shader ourShader;
    ourShader.setShader("shader.vs", "shader.fs");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window1);
    glViewport(0,0,600,600);
    glEnable(GL_DEPTH_TEST);  // depth test 키기
    glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);
    Shader ourShader2;
    ourShader2.setShader("shader.vs", "shader.fs");
    



//window1를 위한 VAO, VBO, EBO 준비
    glfwMakeContextCurrent(window1);
    // wall 단계
    unsigned int EBOW;
    glGenBuffers(1, &EBOW);
    unsigned int VBOW;
    glGenBuffers(1, &VBOW);
    unsigned int VAOW;
    glGenVertexArrays(1, &VAOW);  //VAO는 바인딩된 이후의 vertex속성의 호출을 저장한다
    glBindVertexArray(VAOW);

    // location 0에 넣을 설정들, VBOW를 사용할거고 블라블라블라
    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //그동안은 여기에 있는 enable이 저기 vs쉐이더에 들어가는 0번이라고 생각했는데, 그건 얘의 인덱스니 당연한거고
    //그 진짜 의미는 지금 연결된 GL_ARRAY_BUFFER을 VAO의 0번쨰 location과 연결하는, 즉 
    // VAO - vs사이의 연결이 아니라 VAO[0] - VBO(GL_ARRAY_BUFFER)을 연결하는 거였다.
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertics), wallVertics, GL_STATIC_DRAW);

    // ray 단계
    unsigned int EBOR;
    glGenBuffers(1, &EBOR);
    unsigned int VBOR;
    glGenBuffers(1, &VBOR);
    unsigned int VAOR;
    glGenVertexArrays(1, &VAOR);
    glBindVertexArray(VAOR);

    glBindBuffer(GL_ARRAY_BUFFER, VBOR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOR);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertics), rayVertics, GL_STATIC_DRAW);


    //window에서 픽셀을 그리기 위한 VAO
    glfwMakeContextCurrent(window);
    unsigned int EBOP;
    glGenBuffers(1, &EBOP);
    unsigned int VBOP;
    glGenBuffers(1, &VBOP);
    unsigned int VAOP;
    glGenVertexArrays(1, &VAOP);  
    glBindVertexArray(VAOP);

    glBindBuffer(GL_ARRAY_BUFFER, VBOP);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOP);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(pixelVertics), pixelVertics, GL_STATIC_DRAW);


    

    while(!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window1))
    {
        processInput(window);
        renderTime = (float)glfwGetTime();

        glfwMakeContextCurrent(window1);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        
        drawWall(ourShader2, VAOW, window1);
        // ray3D(ourShader, ourShader2, VAOR, VBOR, VAOP, VBOP, window, window1);
        wallsRay(ourShader, ourShader2, VAOR, VBOR, VAOP, VBOP, window, window1);
        

        glfwSwapBuffers(window);
        glfwSwapBuffers(window1);
        glfwPollEvents();

        // printf("%f\n", (float)glfwGetTime() - renderTime);
    }


    glfwTerminate();
    return 0;
}