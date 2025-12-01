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

bool isColide(glm::vec3 move){
    glm::vec3 position = player + move;
    if(map[(int)floor(position.z)][mapSizeY - (int)floor(position.y) - 1][(int)floor(position.x)] == 1){
        return true;
    }
    return false;
}

void processInput(GLFWwindow *window)
{
    glm::vec3 Dxy = glm::vec3(cos(PI/180*angle.x), sin(PI/180*angle.x),0);
    glm::vec3 Dz = glm::vec3(0,0,1);
    float moveDst = playerSpeed * ((float)glfwGetTime() - renderTime);
    glm::vec3 move;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        move = scaleVector(Dxy, moveDst);

        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        move = scaleVector(Dxy, -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        move = scaleVector(glm::vec3(-Dxy.y, Dxy.x, 0), moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        move = scaleVector(glm::vec3(-Dxy.y, Dxy.x, 0), -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        move = scaleVector(Dz, moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        move = scaleVector(Dz, -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }



    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        angle.y += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        angle.y -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        angle.x -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        angle.x += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
}

float screenVertices[] = {
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
};

unsigned int indices[] = {
    0,1,2,
    1,2,3
};


void setUniform(Shader ourShader){
    glm::vec3 D = glm::vec3(cos(PI/180*angle.y) * cos(PI/180*angle.x), cos(PI/180*angle.y) * sin(PI/180*angle.x),sin(PI/180*angle.y));
    glm::vec3 Dnr = glm::vec3(sin(PI/180*angle.x), -cos(PI/180*angle.x), 0);// Srow를 위한 D의 평면직교벡터. 노트에 있는 Dㅗ
    glm::vec3 Dnc = glm::vec3(D.z*Dnr.y - D.y*Dnr.z, D.x*Dnr.z - D.z*Dnr.x, D.y*Dnr.x - D.x*Dnr.y);
    glm::vec3 Sr = scaleVector(Dnr, 1.0f/(tan(PI/180 * (1.0f/2)*povHorizontal)));
    glm::vec3 Sc = scaleVector(Dnc, 1.0f/(tan(PI/180 * (1.0f/2)*povVertical)));

    ourShader.use();
    ourShader.setVec3("player", player);
    ourShader.setVec3("D", D);
    ourShader.setVec3("Sr", Sr);
    ourShader.setVec3("Sc", Sc);
}
int main(){

    wallCoordinate();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glViewport(0,0,600,600);
    glEnable(GL_DEPTH_TEST);
    Shader ourShader;
    ourShader.setShader("shader.vs", "distance_shader.frag");

    // wall Data uniform 보내주기
    ourShader.use();
    glUniform3fv(glGetUniformLocation(ourShader.ID, "wallsCoord"), walls.size(), glm::value_ptr(walls[0])); 
    ourShader.setInt("wallCount", walls.size());
    ourShader.setFloat("wallArea", 0.5);
    ourShader.setFloat("maxSight", maxSightRange);
    ourShader.setVec2("u_resolution", glm::vec2(600,600));


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //스크린을 그리기 위한 버퍼 설정
    unsigned int EBOS;
    glGenBuffers(1, &EBOS);
    unsigned int VBOS;
    glGenBuffers(1, &VBOS);
    unsigned int VAOS;
    glGenVertexArrays(1, &VAOS);  
    glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderTime = (float)glfwGetTime();
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        setUniform(ourShader);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}