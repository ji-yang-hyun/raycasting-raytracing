#include <glad/glad.h> // 가장 먼저 include 필요
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{// 창 크기가 바뀔 때마다 viewport값 바꾸기 위해 호출되는 콜백함수
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window) // 인풋 받기 예제, 루프에 들어간다. 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 안 눌렸으면 GLFW_RELEASE return
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = { // 정점배열
     0.5f,  0.5f, 0.0f,  // 우측 상단
     0.5f, -0.5f, 0.0f,  // 우측 하단
    -0.5f, -0.5f, 0.0f,  // 좌측 하단
    -0.5f,  0.5f, 0.0f   // 좌측 상단
};
unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
    0, 1, 3,   // 첫 번째 삼각형
    1, 2, 3    // 두 번째 삼각형
};   // ebo에서 사용할 배열


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
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 아무것도 안 하고 gl_Position이라는 타입으로 return
    "}\0"; // vertexShader 코드, GLSL이라는 언어

const char *fragmentShaderSource = "#version 330 core\n" // 
    "out vec4 FragColor;\n" // 컬러값 벡터
    "uniform vec4 ourColor;\n" // 유니폼 생성
    "void main()\n"
    "{\n"
   "    FragColor = ourColor;\n" // 일단은 그냥 주황색으로, RGBA값. 0~1
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



    // vertexShader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // 쉐이더 생성

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 소스코드 설정
    glCompileShader(vertexShader); // 컴파일(쉐이더도 코드니까)

    int  success; // 컴파일 에러로그를 위한 부분
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // vertexShader(정점 쉐이더) 끝

    // fragmentShader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "암튼 fragment 쉐이더 에러\n" << infoLog << std::endl;
    }
    // fragmentShader (픽셀 색상 쉐이더) 끝

    //shader progrem -> 쉐이더끼리 연결, 최종 결과 쉐이더
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
        //쉐이더 프로그램 컴파일 로그
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "암튼 쉐이더 프로그램 에러\n" << infoLog << std::endl;
    }
    // glUseProgram(shaderProgram); // 이제 쉐이더 이거 쓸게요~
        //shader progrem에 연결했으니 이제 필요없음
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);  
    //shader progrem 끝




    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  


    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        float timeValue = glfwGetTime(); // 렌더링 시간 초단위 가져오기
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 렌더링 시간 기반 주기적 green값
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // uniform location 찾기(못찾으면 -1)
        glUseProgram(shaderProgram); // 쉐이더 불러와서 유니폼을 바꿔줘야 한다. 왜 불러오는지는 아직 모르겠음, 근데 glUniform함수에 필요
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // glUniform - 유니폼 설정
        /*
        4f: 이 함수는 4개의 float 타입의 값을 원합니다.
        f: 이 함수는 float 타입의 값을 원합니다.
        i: 이 함수는 int 타입의 값을 원합니다.
        ui: 이 함수는 unsigned int 타입의 값을 원합니다.
        3f: 이 함수는 3개의 float 타입의 값을 원합니다.
        fv: 이 함수는float 타입의 vector/배열을 원합니다.
        */
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}