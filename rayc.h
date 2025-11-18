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
#define maxSightRange 15

float px = 2;
float py = 2;
float pt = 90; // player theta

pair<float, pair<float, float > > wallDistance(float theta){
    // 근데 이제 이게 ray가  가는 방향의 역방향으로도 결국 교점이 있으면 찾아버려서, 이걸 고쳐야한다 (탄젠트의 주기성 때문)
    // 어차피 하나의 직선은 사각형에서 두 변만 지날 수 있다
    float thetaX = theta;
    vector<pair<int, pair<pair<float, pair<float, float> >, pair<float, pair<float, float> > > > > distanceV; 
    // {i번쨰 벽, { {minD,{minX,minY}, {maxD,{maxX,maxY}} } } } 벡터 (절단면 구성 위해 필요)

    for(int i = 0; i<walls.size(); i++){
        // 교점의 과표 ix,iy
        float ix, iy;
        float x = walls[i].second;
        float y = walls[i].first; 
        float minDistance = sqrt(2) * mapSize + 1; // 나올 수 있는 최대 거리 + 1
        float maxDistance = -1;
        float IXmax, IYmax, IXmin, IYmin = 0;
        // 좌표계(x+,y+) 기준
        float distance = 0;
        int kx, ky;

        //좌표관계에 따라서 반대쪽 벽은 볼 필요조차 없다 -> 근데 절단면 만들거면 필요하다 그러니 나랑 가까운 쪽 한 번, 먼 쪽 한 번 하자.
        // 근데 가까운쪽에서 두 번 만나고 끝일 수도 있다. 아님 그 반대거나, 조심]
        // 이론적으로는 그런데 만약에 ray가 모서리를 스치거나 해버리면 또 이상해질 수 있으니 한 벽당 가장 가까운거, 가장 먼 걸로 나누자.
        // 그리고 그런것들의 집합을 만든 후 마지막에 전체 벽중 가장 가까운거 하나만 꼽는거지.
        // 그리고 붙어있는 친구들 모아줄건데, 결국 대입하는 x,y좌표가 완전히 같을거라 붙어있으면 좌표 자체가 같지 않을까 싶긴 한데 
        //어차피 거리 비교할거다
        // 소수의 경우 == 연산자를 사용하는게 위험한 경우들이 많이 있다. 0.2 == 0.20 이 false인 것 처럼... 그러니 뭉갤 거리를 잡아서 하자.
        // 그리고 한 ray에서 거리와 나타내는 점은 일대일 대응인것을 명심하자
        // ray가 바닥을 뚫을 경우를 생각할 수 있지만, 아직은 그저 map 평면에서의 일이다.

        // 가까운 쪽부터!
        if(x - px >= 0) kx = -1;
        else kx = 1;
        if(y - py >= 0) ky = -1;
        else ky = 1;

        for(int dumb = 0;dumb<2;dumb++){ // 가까운거 두 변, 먼 거 두 변 체크
            iy = tan(PI/180 * thetaX) * (x + 0.5*kx - px) + py;
            ix = (y + 0.5*ky - py) / tan(PI/180 * thetaX) + px;
        
            if((y - iy) <= 0.5 && (y - iy) >= -0.5){
                if((thetaX - 180) * (iy - py) < 0){ // 180만큼 주기성을 가지는 tan x가 반대방향에 있는걸 잡지 않게 해주기
                    distance = sqrt(pow((x + 0.5*kx - px),2) + pow((iy - py),2));
                    
                    if(distance < minDistance){
                        minDistance = distance;
                        IXmin = x + 0.5*kx;
                        IYmin = iy;

                    }
                    if(distance > maxDistance){
                        maxDistance = distance;
                        IXmax = x + 0.5*kx;
                        IYmax = iy;
                    }
                
                }
            }
            if((x - ix) <= 0.5 && (x - ix) >= -0.5){
                if((thetaX - 180) * (y + 0.5*ky - py) < 0){
                    distance = sqrt(pow((y + 0.5*ky - py),2) + pow((ix - px),2));
                   
                    if(distance < minDistance){
                        IYmin = y + 0.5*ky;
                        IXmin = ix;
                        minDistance = distance;
                    }
                    if(distance > maxDistance){
                        IYmax = y + 0.5*ky;
                        IXmax = ix;
                        maxDistance = distance;
                    }
                }
            }

            kx = kx * -1;
            ky = ky * -1;
        }
        //근데 안 만나는 오브젝트들도 있을거 아냐
        if(minDistance != sqrt(2) * mapSize + 1 && maxDistance != -1){
            distanceV.push_back(make_pair(i, 
            make_pair(
                make_pair(minDistance, make_pair(IXmin, IYmin)),
                make_pair(maxDistance, make_pair(IXmax, IYmax))
             )));
        }
    }

    float minI=0;
    float minD = sqrt(2) * mapSize + 1;
    for(int i=0;i<distanceV.size();i++){
        if(distanceV[i].second.first.first <= minD){
            minI = i;
            minD = distanceV[i].second.first.first;
        }
    }
    
    return distanceV[minI].second.first;
}
#endif