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

#define povHorizontal 60
#define povVertical 60
#define dt 1 //delta theta
#define pixelX povHorizontal / dt
#define pixelY povVertical / dt
#define PI 3.1415926
#define playerSpeed 2
#define playerThetaSpeed 45
#define screenSizeX 1000
#define screenSizeY 1000
#define maxSightRange 15

// 소수 비교할 때 같다고 할 최소거리
#define equal 0.0001

//맵 기준 위치
float px = 2;
float py = 2;
float pz = playerHeight;
//화면 기준
float ptx = 90; // player theta // 가로
float pty = 0; // 세로 그냥 평면 보는게 0


glm::vec2 getTransformPosition(glm::vec2 P, float thetaY, glm::vec2 D){
        float a = D.x * P.x + D.y * P.y;
        float b = -(D.y*P.x) + D.x * P.y;
        float x = D.x * (a/cos(PI/180*thetaY)) + b*(-D.y);
        float y = D.y * (a/cos(PI/180*thetaY)) + b*(D.x);
        glm::vec2 R = glm::vec2(x,y);

        return R;
}



pair<float, pair<float, float > > wallDistance(float thetaX, float thetaY){
    // 근데 이제 이게 ray가  가는 방향의 역방향으로도 결국 교점이 있으면 찾아버려서, 이걸 고쳐야한다 (탄젠트의 주기성 때문)
    // 어차피 하나의 직선은 사각형에서 두 변만 지날 수 있다
    vector<pair<int, pair<pair<float, pair<float, float> >, pair<float, pair<float, float> > > > > distanceV; 
    // {i번쨰 벽, { {minD,{minX,minY}, {maxD,{maxX,maxY}} } } } 벡터 (절단면 구성 위해 필요)

    for(int i = 0; i<walls.size(); i++){
        // 교점의 과표 ix,iy
        float xo = walls[i].second; // x original
        float yo = walls[i].first; // y original
        glm::vec2 W = glm::vec2(xo,yo);
        glm::vec2 D = glm::vec2(cos(PI/180*ptx),sin(PI/180*ptx));
        W = getTransformPosition(W, thetaY, D);
        
        float minDistance = 1000000; // 
        float maxDistance = -1;
        float IXmax, IYmax, IXmin, IYmin = 0;
        // 좌표계(x+,y+) 기준
        float distance = 0;

        glm::vec2 area[4];
        int ka[8] = {-1,-1, 1,-1, 1,1, -1,1};
        for(int j=0;j<4;j++){
            glm::vec2 A = glm::vec2(ka[2*j]*0.5f, ka[2*j + 1]*0.5f);
            area[j] = getTransformPosition(A, thetaY, D);
        }

       

        for(int j = 0;j<4;j++){
            float x1 = W.x + area[j].x;
            float x2 = W.x + area[(j+1)%4].x;
            float y1 = W.y + area[j].y;
            float y2 = W.y + area[(j+1)%4].y;

            float ix,iy;
            // tan이 매우 커지면 계산하기 힘들기 때문에 만약에 tan이 1보다 크면 점대칭점 사용해서 구하자.
            // 지금 뭔가 벽이 감지가 되는듯 하다가도 안되고 막 그러는데 일단 지금 보이는 문제는, x1-x2, y1-y2가 0일때 문제가 생기고
            // 그 떄그냥 계산이 튕겨버릴 수 있다. 
            // 그 때들을 잘 예외처리를 해주어야할 것 같고, 또한 그 상태에서 소숫점 비교는 정확하지 않을 수 있다는거 명심!
            // 그리고 지금 되는 애들은 그게 0인 경우지만 점대칭이나 다른 이유로 그 0이 분모에 안 들어가서일거다.
            // 그리고 또 문제가 생길 수 있는게 만약에 (x2-x1)/(y2-y1) - tan(PI/180*thetaX - PI/2)가 0이면... 그 때는 평행하다는건데
            // 그것도 예외처리 해줘야할 것 같다.
            if(pow(tan(PI/180*thetaX), 2) > 1){
                iy = (px - x1 - tan(PI/180*thetaX - PI/2)*py + 
                ((x2-x1)/(y2-y1))*y1)/((x2-x1)/(y2-y1) - tan(PI/180*thetaX - PI/2));
                ix = tan(PI/180*thetaX - PI/2) * (iy - py) + px;
            }
            else{
                ix = (py - y1 - tan(PI/180*thetaX)*px + ((y2-y1)/(x2-x1))*x1)/((y2-y1)/(x2-x1) - tan(PI/180*thetaX));
                iy = tan(PI/180*thetaX) * (ix - px) + py;
            }

            
            // x1 = x2이면 소숫점 조금만 틀어져도 감지가 안 된다. 그러니 iy조건도 넣어서 둘중 하나는 차이가 있을테니 그걸 감지하자
            if((py - iy) * (thetaX - 180) < 0){ // ray 진행방향의 반대방향이다.
                continue;
            }

            if(pow(x1 - x2,2) <= equal){
                if((y1 <= iy && iy <= y2) || (y2 <= iy && iy <= y1)){
                    distance = sqrt(pow((ix - px),2) + pow((iy - py),2));
                

                    if(distance <= 0.0001){
                        printf("(%f, %f), D = %f, (%f, %f) (%f, %f) (%f, %f) (%f)\n",
                        ix, iy, distance, x1, y1, x2, y2, px,  py, tan(PI/180*thetaX));
                        
                        printf("area1 : (%f, %f)   area2 : (%f, %f) \n",
                        area[j].x, area[j].y, area[(j+1)%4].x, area[(j+1)%4].y);
                    }
                    if(distance <= minDistance){
                        minDistance = distance;
                        IXmin = ix;
                        IYmin = iy;
                    }
                    if(distance >= maxDistance){
                        maxDistance = distance;
                        IXmax = ix;
                        IYmax = iy;
                    }
                }
            }
            else{
                if((x1 <= ix && ix <= x2) || (x2 <= ix && ix <= x1)){
                    distance = sqrt(pow((ix - px),2) + pow((iy - py),2));
                    

                    if(distance <= 0.0001){
                        printf("(%f, %f), D = %f, (%f, %f) (%f, %f) (%f, %f) (%f)\n",
                        ix, iy, distance, x1, y1, x2, y2, px,  py, tan(PI/180*thetaX));

                    }
                    if(distance <= minDistance){
                        minDistance = distance;
                        IXmin = ix;
                        IYmin = iy;
                    }
                    if(distance >= maxDistance){
                        maxDistance = distance;
                        IXmax = ix;
                        IYmax = iy;
                    }
                }
            }
        }
        //근데 안 만나는 오브젝트들도 있을거 아냐
        if(minDistance != 1000000000 && maxDistance != -1){
            distanceV.push_back(make_pair(i, 
            make_pair(
                make_pair(minDistance, make_pair(IXmin, IYmin)),
                make_pair(maxDistance, make_pair(IXmax, IYmax))
             )));
        }
    }

    float minI=-1;
    float minDistanceFlat = 1000000000;
    for(int i=0;i<distanceV.size();i++){
        
        if(distanceV[i].second.first.first <= minDistanceFlat){
            minI = i;
            minDistanceFlat = distanceV[i].second.first.first;
        }
    }

    //아무 벽도 안 만나면
    if(minI == -1){
        return make_pair(-1, make_pair(-1, -1));
    }
    return distanceV[minI].second.first;
}
#endif