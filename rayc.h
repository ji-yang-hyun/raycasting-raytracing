#ifndef DISTANCE
#define DISTANCE
#include<stdio.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"map.h"

using namespace std;

#define povHorizontal 120
#define povVertical 120
#define pixelX 300
#define pixelY 300
#define PI 3.1415926
#define playerSpeed 2
#define playerThetaSpeed 45
#define screenSizeX 1000
#define screenSizeY 1000
#define maxSightRange 15
#define playerHeight 0.5

//맵 기준 위치
glm::vec3 player = glm::vec3(2,2,playerHeight); //x,y,z
//화면 기준
glm::vec2 angle = glm::vec2(90, 0); // horizental, vertical 스크린의 x,y방향

glm::vec3 scaleVector(glm::vec3 V, float k){
    return glm::vec3(V.x * k, V.y * k, V.z * k);
}
#endif