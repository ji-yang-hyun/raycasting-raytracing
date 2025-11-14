#ifndef DISTANCE
#define DISTANCE
#include<stdio.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include"map.h"

using namespace std;

#define povHorizontal 60
#define povVertical 120
#define dt 1 //delta theta
#define pixelX povHorizontal / dt
#define pixelY povVertical / dt
#define PI 3.1415926
#define playerSpeed 2
#define playerThetaSpeed 45
#define screenSizeX 1000
#define screenSizeY 1000

float px = 2;
float py = 2;
float pt = 90; // player theta

pair<pair<float, float >, float> wallDistance(float theta){
    // 근데 이제 이게 ray가  가는 방향의 역방향으로도 결국 교점이 있으면 찾아버려서, 이걸 고쳐야한다 (탄젠트의 주기성 때문)
    float minDistance = -1;
    float thetaF = theta;
    // 교점의 x,y좌표
    float IX = 0;
    float IY = 0;
    for(int i = 0; i<walls.size(); i++){
        float x = walls[i].second;
        float y = walls[i].first; 
        // 좌표계(x+,y+) 기준
        float distance = 0;
        int kx, ky;

        //좌표관계에 따라서 반대쪽 벽은 볼 필요조차 없다
        if(x - px >= 0) kx = -1;
        else kx = 1;
        if(y - py >= 0) ky = -1;
        else ky = 1;

        float iy = tan(PI/180 * thetaF) * (x + 0.5*kx - px) + py;
        float ix = (y + 0.5*ky - py) / tan(PI/180 * thetaF) + px;
        
        if((y - iy) <= 0.5 && (y - iy) >= -0.5){
            if((thetaF - 180) * (iy - py) < 0){ // 180만큼 주기성을 가지는 tan x가 반대방향에 있는걸 잡지 않게 해주기
                distance = sqrt(pow((x + 0.5*kx - px),2) + pow((iy - py),2));
                if(minDistance == -1 || minDistance > distance){
                    IX = x + 0.5*kx;
                    IY = iy;
                    minDistance = distance;
                }
            }
        }
        if((x - ix) <= 0.5 && (x - ix) >= -0.5){
            if((thetaF - 180) * (y + 0.5*ky - py) < 0){
                distance = sqrt(pow((y + 0.5*ky - py),2) + pow((ix - px),2));
                if(minDistance == -1 || minDistance > distance){
                    IY = y + 0.5*ky;
                    IX = ix;
                    minDistance = distance;
                }
            }
        }
    }

    return make_pair(make_pair(IX,IY) , minDistance);
}

#endif