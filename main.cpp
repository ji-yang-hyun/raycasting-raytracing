#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include<algorithm>
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




void drawWall(float thetaY, Shader ourShader, unsigned int VAOW, unsigned int VBOW, GLFWwindow* window1){
    glfwMakeContextCurrent(window1);
    glBindVertexArray(VAOW);
    glBindBuffer(GL_ARRAY_BUFFER, VBOW);
    for(unsigned int i = 0; i < walls.size(); i++)
    {
        float xw = walls[i].second; // x original
        float yw = walls[i].first; // y original
        
        glm::vec2 D = glm::vec2(cos(PI/180*ptx),sin(PI/180*ptx));
        glm::vec2 P;
        glm::vec2 W = getTransformPosition(glm::vec2(xw,yw), thetaY, D);
        // area에서 x,y의 부호나 방향에 민감하니 각각 해줘야한다.

        int ka[8] = {1,1, -1,1, 1,-1, -1,-1};
        for(int j=0;j<4;j++){
            glm::vec2 A = glm::vec2(ka[2*j]*0.5f, ka[2*j + 1]*0.5f);
            P = getTransformPosition(A, thetaY, D);
            wallVertics[3*j] = std::max(std::min(2*((W.x + P.x)/mapSize)/1.0f - 1.0f, 1.0f), -1.0f);
            
            wallVertics[3*j + 1] = std::max(std::min(2*((W.y + P.y)/mapSize)/1.0f - 1.0f, 1.0f), -1.0f);
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertics), wallVertics, GL_STATIC_DRAW);

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.use();
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0f,0.0f,0.0f,1.0f));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void drawRay(Shader ourShader, unsigned int VAOR, unsigned int VBOR, float ix, float iy, GLFWwindow* window1){
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
    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7f,0.3f,1.0f,1.0f));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ray2D(float thetaY,
    Shader ourShader, Shader ourShader2, 
    unsigned int VAOR, unsigned int  VBOR, unsigned int VAOP,
    GLFWwindow* window, GLFWwindow* window1){
    for(int i=0;i<pixelX;i++){
        float thetaX = fmod(ptx - povHorizontal/2 + dt/2 + i*dt, 360);
        pair<float, pair<float, float> > P;
        P = wallDistance(thetaX, thetaY);
        float distance = P.first;
        float ix = P.second.first;
        float iy = P.second.second;
        
        if(distance != -1){
            drawRay(ourShader2, VAOR, VBOR, ix, iy, window1);
        }
    }
}


// void ray3D(
//     Shader ourShader, Shader ourShader2, 
//     unsigned int VAOR, unsigned int VAOP,
//     GLFWwindow* window, GLFWwindow* window1){
//         for(int i=0;i<pixelY;i++){
//         float thetaY = fmod(pty - povVertical/2 + dt/2 + i*dt, 360);
//         ray2D(thetaY, ourShader, ourShader2, VAOR, VAOP, window, window1);
//     }
// }


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

        
        drawWall(pty, ourShader2, VAOW, VBOW, window1);
        // float thetaY = fmod(pty - povVertical/2 + dt/2 + renderTime*dt, povVertical);
        ray2D(pty, ourShader, ourShader2, VAOR, VBOR, VAOP, window, window1);
        

        glfwSwapBuffers(window);
        glfwSwapBuffers(window1);
        glfwPollEvents();
    }


    glfwTerminate();
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


/*
VAO를 바인드한다고 해서 VBO가 바인드되는게 아닌가보다.
원래는 drawRay에서만 값을 바꾸고, 마지막으로 window1에 바인드됐던게 VBOR이라 괜찮았는데
이번에 drawWall에서 VAOW만 바인드하고 glBufferData를 하니 그 데이터가 window1에 바인드돼있던
마지막 VBO인 VBOR에 들어가고 말았다.
근데 그 상태에서 또 drawelement는 VAOW를 기준으로 VBOW를 그리고 있었으니...
이게 뭔지 잘 모르겠다 ㅋㅋㅋ

그러니까 VAOW에서 VBOW, EBOW에 대한 포인터를 가지고 있어서 가지고 오는데
그걸 직접 바인드하는게 아니라 그냥 VAOW에서 어떤거랑 어떤게 바인드됐었고 그거의 attrib포인터는 이래!
하고 그걸 그냥 기억해온 후 drawelement에 사용할 뿐
지금 현재 바인드 된 GL_ARRAY_BUFFER에 값을 꽂아넣는 glBufferData에서는 여전히 마지막으로 바인드 된
VBOR을 사용한다는것으로 이해하면 되려나?


그니까 정리하면
VAO - VBO, EBO등등을 가지고 있는게 맞고 그걸 사용하지만 직접 각각을 "바인딩" 하지는 않는다
glEnableVertexAttribArray자체가 연결하는건 아니고, attributePointer로 몇번 인덱스에 어떤 모양의 버퍼가 들어갈지 지정하고
glEnableVertexAttribArray로 그 지정된걸 사용하겠다고 하는거다.
그러니까 항상 GL_ARRAY_BUFFER는 마지막으로 바인딩한걸 그대로 가지고 있고, VAO를 바인딩한다고 그걸 따라가지 않는다.
attributePointer로 형식지정 후, 어떤 주소에 있는 VBO를 참조할지 설정 후에 glEnableVertexAttribArray로 연결.
나중에 그 VBO값을 바꾸면 그걸 참조한 VAO에서도 마찬가지로 바뀐다.

BufferData로 GPU에 어떤 정점을 어떻게 그릴지 미리 갖다놓는다. 그리고 그걸 VAO로 참조...
GL_BUFFER_ARRAY는 GPU에 가기 전에 데이터를 뭉탱이로 보관하는거고
나중에 거기있는걸 BufferData를 써서 지금 연결된 VBO주소에(GPU) 보내주는거다.

학습링크
https://rito15.github.io/posts/opengl-study-04/
*/