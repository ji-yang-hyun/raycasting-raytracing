#ifndef MAP
#define MAP

#include<vector>
#include<iostream>
using namespace std;

#define mapSize 10
// 그냥 정사각형 세상으로 하자.

int map[mapSize][mapSize] = {
    {1,1,1,1,1,1,1,1,1,1}, 
    {1,0,0,0,0,0,0,0,0,1}, 
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}, };

vector<glm::vec3> walls;

void wallCoordinate(){
    for(int i=0;i<mapSize;i++){
        for(int j=0;j<mapSize;j++){
            if(map[i][j] == 1){
                // 배열은 y좌표 뒤집히므로 그냥 좌표계를 사용하기 위해 y값을 거꾸로!
                walls.push_back(glm::vec3(j + 0.5, mapSize - i - 0.5, 0.5));
            }
        }
    }
}

#endif