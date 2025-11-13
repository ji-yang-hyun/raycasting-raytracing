#ifndef MAP
#define MAP

#include<vector>
#include<iostream>
using namespace std;

#define mapSizeX 5
#define mapSizeY 5

int map[mapSizeY][mapSizeX] = {{0,0,0,0,0}, {0,0,1,0,0}, {0,1,1,1,0}, {0,0,1,0,0}, {0,0,0,0,0}};

vector<pair<float, float> > walls;
pair<float, float> wall;

void wallCoordinate(){
    for(int i=0;i<mapSizeY;i++){
        for(int j=0;j<mapSizeX;j++){
            if(map[i][j] == 1){
                wall = make_pair(mapSizeY - i - 0.5, j + 0.5);// 배열은 y좌표 뒤집히므로 그냥 좌표계를 사용하기 위해 y값을 거꾸로!
                walls.push_back(wall);
            }
        }
    }
}

#endif