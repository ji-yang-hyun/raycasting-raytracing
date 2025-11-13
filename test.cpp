// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <iostream>
// #include"shader_s.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
// {
//     glViewport(0, 0, width, height);
// }  

// void processInput(GLFWwindow *window)
// {
//     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }

// int main(){

//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }    
//     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);

//    float vertices[] = {
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,

//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,

//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
//         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,

//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,

//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
//         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,

//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
//         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
//     };

//     glm::vec3 cubePositions[] = {
//         glm::vec3( 0.0f,  0.0f,  0.0f), 
//         glm::vec3( 2.0f,  5.0f, -15.0f), 
//         glm::vec3(-1.5f, -2.2f, -2.5f),  
//         glm::vec3(-3.8f, -2.0f, -12.3f),  
//         glm::vec3( 2.4f, -0.4f, -3.5f),  
//         glm::vec3(-1.7f,  3.0f, -7.5f),  
//         glm::vec3( 1.3f, -2.0f, -2.5f),  
//         glm::vec3( 1.5f,  2.0f, -2.5f), 
//         glm::vec3( 1.5f,  0.2f, -1.5f), 
//         glm::vec3(-1.3f,  1.0f, -1.5f)  
//     };
    
//     glEnable(GL_DEPTH_TEST);  // depth test 키기


//     Shader ourShader("shader.vs", "shader.fs");

//     unsigned int VBO;
//     glGenBuffers(1, &VBO);
//     unsigned int VAO;
//     glGenVertexArrays(1, &VAO);  
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);

//     glBindVertexArray(VAO);


//     glViewport(0, 0, 800, 600);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//     while(!glfwWindowShouldClose(window))
//     {
//         processInput(window);
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);
//         glClear(GL_DEPTH_BUFFER_BIT); // zbuffer(deptBuffer)사용중이므로 매 루프마다 비워줘야 한다.

//         glm::mat4 trans;
//         ourShader.use();
//         glm::mat4 view          = glm::mat4(1.0f);
//         glm::mat4 projection    = glm::mat4(1.0f);
//         view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//         projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         ourShader.setMat4("projection", projection);
//         ourShader.setMat4("view", view);

//         for(unsigned int i = 0; i < 10; i++)
//         {
//             glm::mat4 model;
//             model = glm::translate(model, cubePositions[i]);
//             float angle = 20.0f * i; 
//             model = glm::rotate(model, glm::radians(((float)glfwGetTime() * glm::radians(50.0f) * 5) + angle), glm::vec3(1.0f, 0.3f, 0.5f));
//             ourShader.setMat4("model", model);

//             glDrawArrays(GL_TRIANGLES, 0, 36);
//         }

        

//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }


//     glfwTerminate();
//     return 0;
// }


// /*
// 사실 정점벡터에 정점이 -1~1사이의 값을 가질 필요 없음.
// 화면에서는
// Orthographic projection에서 설정하는 범위만큼을 보여줌. 3d이기 때문에 평면의 넓이와 그것의 깊이범위까지.
// 그냥 사각형 박스를 설정하고 그 안에 있는걸 보여준다고 생각하면 됨.
// 그럼 그 안에 있는 정점들을 다 보고
// 그걸 각각 -1~1로 자동으로 매핑해줌(NDC로 매핑한다는 것)

// glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
// 가로 넓이 시작,끝
// 세로 높이 시작, 끝
// 깊이 범위 시작, 끝으로 설정

// 근데 이걸 그냥 쓰면 원근감 고려를 안 하니 원근감을 담당하는 projection도 필요함
// */

// /*
// Perspective projection
// 여기서 원근감 담당해줌, 이걸 뭐 어떻게 계산하는지는 몰라도 됨
// glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
// 이건 내가 원근감 계산하는거에 스포같으니 그냥 넘어가자 ㅋㅋㅋ
// 근데 이제 여기서도 하나의 박스 형태로 애들을 clip한다.
// */

// /*
// Orthographic projection과 Perspective projection은 둘중 하나를 선택하는거지 둘 다 하는게 아니다.
// */

// /*
// 이제 결과로서 clipSpace안에 있는 각 정점들이 있을거다
// 이제 이걸 normalized-device coordinates로 변환하기 위해 perspective division를 하고 (이 부분은 저기 Orthographic projection에서!)
// viewPort에 맞춰서 NDC(normalized-device coordinates)를 screenSpace의 좌표로 매핑한다
// */

// /*
// 이제 하나를 3d처럼 렌더링할 수 있으니 아예 3d 세상을 만들자.
// 각각의 위치가 있을거고 그 위치로 이동을 시키기 위한 변환벡터 model을 만든다
// glm::mat4 model;
// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
// 이제 이 model벡터를 사용해서 전체 wordspace에서의 각 오브젝트의 포지션을 설정해줄 수 있다


// 그 다음은 view행렬인데, 카메라 위치를 옮기는 것 같지만 사실 카메라는 가만히 있고 세상을 그 반대로 옮기는것이다
// glm::mat4 view;
// // 우리가 움직이고 싶은 방향과 반대의 방향으로 scene을 이동시키고 있다는 것을 알아두세요.
// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
// */

// /*
// 마지막으로 view, model, projection행렬을 다 uniform으로 vs에 전달해서 곱해주면 된다.
// */


// /*
// 어떤게 뒤에 있고 앞에 있고를 저장하고 뒤에 가려지는 걸 그리지 않기 위해 z버퍼를 사용한다
// 뒤에 있는지 비교하고 그릴지 말지를 하는걸 depth_testing이라고 한다.
// 설정에서 켜주면 된다.
// */