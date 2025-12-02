#ifndef MAP
#define MAP

#include<vector>
#include<iostream>
#include"bad_apple_data.h"
using namespace std;

#define mapSizeX 40
#define mapSizeY 30
#define mapSizeZ 10
// 그냥 정사각형 세상으로 하자.

int screenlayer = 0;

int map[mapSizeZ][colPixelCount][rowPixelCount] = {{{0}}};

vector<glm::vec3> walls;

void wallCoordinate(){
    for(int z=0;z<mapSizeZ;z++){
        for(int y=0;y<mapSizeY;y++){
            for(int x=0;x<mapSizeX;x++){
                if(map[z][y][x] == 1){
                    // 배열은 y좌표 뒤집히므로 그냥 좌표계를 사용하기 위해 y값을 거꾸로!
                    walls.push_back(glm::vec3(x + 0.5, mapSizeY - y - 0.5, z + 0.5));
                    // printf("%f %f %f \n", walls.back().x, walls.back().y, walls.back().z);
                    // printf("%d %d %d\n", x, y, z);
                }
            }
        }
    }
}

#endif