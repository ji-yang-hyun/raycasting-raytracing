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
            // printf("%f \n", xDistance);
            // printf("%f %f\n", newpx, walls[i].second);
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

float wallEyeVertics[] = {
    0.0f, 1.0f, 0.0f, // 위, 오른쪽
    -2.0f/(pixelX), 1.0f, 0.0f, // 위, 왼쪽
    0.0f, 0.0f, 0.0f, // 아래, 오른쪽
    -2.0f/(pixelX), 0.0f, 0.0f // 아래, 왼쪽
};

unsigned int indices[] = {
    0,1,2,
    1,2,3
};




void drawWall(Shader ourShader, unsigned int VAOW, GLFWwindow* window){
    glfwMakeContextCurrent(window);
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

void drawRay(Shader ourShader, unsigned int VAOR, float ix, float iy, GLFWwindow* window){
    glfwMakeContextCurrent(window);
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

void drawWallEye(Shader ourShader2, unsigned int VAOE, float distance, int colNum, GLFWwindow* window2){
    glfwMakeContextCurrent(window2);
    glBindVertexArray(VAOE);

    float heightEyeOver = (wallHeight - playerHeight) / (2 * distance * tan(PI/180*povVertical/2)); 
    float heightEyeBelow = (wallHeight - (wallHeight - playerHeight)) / (2 * distance * tan(PI/180*povVertical/2)); 
    //화면에 보이는 높이, 기준단위는 우리 맵과 같은 좌표계, 눈의 위, 아래로 나눈다.
    //내 시야에서 차지하는 비율이기 때문에 전체 화면 크기인 2(1~-1)을 감안해서 단순히 그냥 사용해주면 된다.
    float color = distance / (maxSightRange);

    if(heightEyeOver >= 0.5){ // 내 눈 위의 시야를 꽉 채운다면
        heightEyeOver = 0.5;
    }
    if(heightEyeBelow >= 0.5){ // 내 눈 아래의 시야를 꽉 채운다면
        heightEyeBelow = 0.5;
    }

    glm::mat4 modelO;
    modelO = glm::scale(modelO, glm::vec3(1,heightEyeOver*2,1));
    modelO = glm::translate(
        modelO, glm::vec3(-((colNum*2.0f)/(pixelX) - 1), 0,0)
    );
    ourShader2.use();
    ourShader2.setMat4("model", modelO);
    ourShader2.setVec4("color", glm::vec4(color,color,color,1.0f));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glm::mat4 modelB;
    modelB = glm::scale(modelB, glm::vec3(1,-heightEyeBelow*2,1));
    modelB = glm::translate(
        modelB, glm::vec3(-((colNum*2.0f)/(pixelX) - 1), 0,0)
    );
    ourShader2.use();
    ourShader2.setMat4("model", modelB);
    ourShader2.setVec4("color", glm::vec4(color,color,color,1.0f));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ray(Shader ourShader, Shader ourShader2, 
    unsigned int VAOR, unsigned int VAOE, 
    GLFWwindow* window, GLFWwindow* window2){
    for(int i=0;i<pixelX;i++){
        float theta = fmod(pt - povHorizontal/2 + dt/2 + i*dt, 360);
        pair<pair<float, float>, float > P;
        P = wallDistance(theta);
        float distance = P.second;
        float ix = P.first.first;
        float iy = P.first.second;
        // printf("각도 : %f, 거리 : %f, 교점 %f, %f \n", theta, distance, ix, iy);
        
        if(distance != -1){
            drawRay(ourShader, VAOR, ix, iy, window);
            drawWallEye(ourShader2, VAOE, distance, i, window2);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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


// window2를 위한 VAO준비, 벽 그릴거다. eye로 보는걸 그리는거니 E.
    glfwMakeContextCurrent(window2);
    unsigned int EBOE;
    glGenBuffers(1, &EBOE);
    unsigned int VBOE;
    glGenBuffers(1, &VBOE);
    unsigned int VAOE;
    glGenVertexArrays(1, &VAOE);  
    glBindVertexArray(VAOE);

    glBindBuffer(GL_ARRAY_BUFFER, VBOE);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOE);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallEyeVertics), wallEyeVertics, GL_STATIC_DRAW);


    while(!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2))
    {
        processInput(window);
        renderTime = (float)glfwGetTime();

        glfwMakeContextCurrent(window);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glfwMakeContextCurrent(window2);
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        drawWall(ourShader, VAOW, window);
        ray(ourShader, ourShader2, VAOR, VAOE, window, window2);

        glfwSwapBuffers(window);
        glfwSwapBuffers(window2);
        glfwPollEvents();
    }


    glfwTerminate();

    setPosition(2,2);
    return 0;
}


/*
두 윈도우가 메인 윈도우일때(glfwMakeContextCurrent)
모든 명령어는 해당 윈도우를 타겟으로 하고 그 사이에 지정되는 변수들도 마찬가지이다.(opengl변수들, 설정들)

따라서 VAO, VBO, EBO, Shader을 포함한것들을 각각 설정해줘야 한다

만약 그런거 없이 그냥 쓰면 그 자리가 비어있기 때문에 안된다.
*/

/*
현상 : 똑같이 다른 설정 없이 그냥 VAOW(window)를 window2에 욱여넣었는데, 벽까지 다 ray가 돼버렸다
내 가설에 의하면 ray도 벽이랑 같이 안 나와야 하는데??
답은 drawray에서 buffer data를 하기 때문이였다.
VAOR를 바인드를 하는데 사실 window2입장에서 이게 뭐지 싶을거다.(아마도 비어있을 것 같은데)
그리고 VBOR에 데이터를 새로 가져오는데 거기서 이제야 VAOR가 정의된다.
이제 그래서 벽을 그릴 때도 그 VAOR를 가져와서 사용하기 때문에 ray, 그것도 방향과 길이가 있는. 이 된다.

그리고 이로써 알 수 있는건 새 VAO를 받았는데 그게 뭔지 모르겠으면 기존에 바인딩돼있던 VAO랑 짬뽕을 시킨다는거다.
그래서 VAOE에서 포인터를, VAOR정의하면서 정점들을, 이렇게 계속 짬뽕시켜 나가는거다.
그래서 애초에 맨 처음에 VAOE에서 포인터 설정을 안하면 동작을 안한다. 짬뽕할게 없기 때문. 
*/ 