#include <glad/glad.h> // 가장 먼저 include 필요
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{// 창 크기가 바뀔 때마다 viewport값 바꾸기 위해 호출되는 콜백함수
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window) // 인풋 받기 예제, 루프에 들어간다. 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 안 눌렸으면 GLFW_RELEASE return
        glfwSetWindowShouldClose(window, true);
}


//GLSL의 입력변수(쉐이더 입력변수) 는 vertex attribute라고 하고 최대개수가 정해져있다.
//GLSL은 int, float, double, uint, bool, vector, matrics 변수 형태를 지원한다
/*
vecn: n개의 float 타입 요소를 가지는 기본적인 vector
bvecn: n개의 boolean 타입 요소를 가지는 vector
ivecn: n개의 integer 타입 요소를 가지는 vector.
uvecn: n개의 unsigned integer 타입 요소를 가지는 vector
dvecn: n개의 double 타입 요소를 가지는 vector

vec -> x,y,z,w로 0,1,2,3인덱스 기본 접근(이게 길이 최대)

vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
이런 문법 사용 가능
*/
/*
쉐이더는 주로 연결해서 쓰기 때문에 시작과 끝인 vertex, fregment를 빼고는 
in, out 키워드로 쉐이더 프로그램 간에 서로 전달해줄 수 있다.
그 방법은두 쉐이더에서 변수의 이름,타입이 같게 만들면 된다.
*/

/*
uniform - opengl의 전체 전역변수 느낌으로 연결된 한 shaderprogrem에서의 모든 쉐이더가 접근 가능하다. c언어 렌더링 루프에서도
index/location을 알면 접근 가능하다
uniform vec4 ourColor; // OpenGL 코드에서 이 변수를 설정할 것입니다.
이렇게 사용 가능. in, out 대신에 사용하면 된다.

모든 부분 쉐이더에서 접근할 수 있지만, 그걸 선언할 때는 그걸 사용하는 코드에서 해야한다.
괜히 전체 쉐이더의 첫 부분에서 사용하지도 않는 uniform을 설정하면 컴파일시 자동으로 지워지니 오류가 난다.
*/
const char *vertexShaderSource = "#version 330 core\n" // opengl버전에 맞는 GLSL버전
    "layout (location = 0) in vec3 aPos;\n" // vec3 형식의 aPos 변수 만들기, 입력 변수 location = 0인 애들 aPos에 넣을게~
    "layout (location = 1) in vec3 aColor;\n" // 컬러도 받을게
    "out vec3 ourColor;"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 아무것도 안 하고 gl_Position이라는 타입으로 return
    "   ourColor = aColor;\n"
    "}\0"; // vertexShader 코드, GLSL이라는 언어

const char *fragmentShaderSource = "#version 330 core\n" // 
    "out vec4 FragColor;\n" // 컬러값 벡터
    "in vec3 ourColor;\n" // 유니폼 생성
    "void main()\n"
    "{\n"
   "    FragColor = vec4(ourColor,1.0);\n" // 일단은 그냥 주황색으로, RGBA값. 0~1
    "}\0";

int main(){
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes); // GLSL 변수 최대개수는 하드웨어마다 달라서 출력해보기
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

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

    Shader ourShader("shader.vs", "shader.fs"); // vs,fs가 무슨 타입인지는 모르겠으나 이렇게들 많이 쓴다고 한다.


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //정점 attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //어느 location에 어떤 데이터가 들어가야 하는지 포인터로 설정, 
    //컬러 attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    /*
    첫 번쨰 파라미터 : 변수가 vertexShader에서 들어갈 location 1
    두 번째 파라미터 : 개수, 세 번째 파라미터 : 형식, 네 번째 파라미터 : 정규화 하지 않음
    다섯 번째 파라미터 : stride, 즉 간격은 하나의 정점 데이터가 6개의 소수(xyzrgb)로 구성돼있으니 6칸만큼의 간격
    여섯 번째 파라미터 : 시작 위치(offset)은 위치 데이터가 다 끝난 후부터 시작이니 앞의 위치데이터 3개의 소수만큼 뒤에서 시작.
    */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);  
    //vertexShader의 0 location 설정했으니 이제 사용해도 된다고 열어주기(기본적으로 닫혀있음)
    //vertexShader의 1 location 설정했으니 이제 사용해도 된다고 열어주기(기본적으로 닫혀있음)

    //이렇게 몇몇 점들의 색상만 주면, 자동으로 fragmentShader에서 컬러의 선형 조합으로 만들어준다.fragment inerpolation
    //이건 fragmentShader의 모든 attribute에 적용된다고 하는데 아직은 무슨 얘긴지 모르겠다. 결국 제한된 인풋을 사용하니 다 선형조합 처리?

    glBindVertexArray(VAO);// static draw이니 VAO 바뀔일 거의 x 그러니 렌더링 루프 밖에서 바인드 해도 됨. (어차피 순차적 진행이니 유지)


    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        ourShader.use();
        /*
        4f: 이 함수는 4개의 float 타입의 값을 원합니다.
        f: 이 함수는 float 타입의 값을 원합니다.
        i: 이 함수는 int 타입의 값을 원합니다.
        ui: 이 함수는 unsigned int 타입의 값을 원합니다.
        3f: 이 함수는 3개의 float 타입의 값을 원합니다.
        fv: 이 함수는float 타입의 vector/배열을 원합니다.
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}