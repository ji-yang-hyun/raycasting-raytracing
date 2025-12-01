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

// 소수 비교할 때 같다고 할 최소거리
#define equal 0.0001

//맵 기준 위치
float px = 2;
float py = 2;
float pz = playerHeight;
//화면 기준
float ptx = 90; // player theta // 가로
float pty = 0; // 세로 그냥 평면 보는게 0


float layerToSee = 0;


glm::vec2 getTransformPosition(glm::vec2 P, float thetaY, glm::vec2 D){
        float a = D.x * P.x + D.y * P.y;
        float b = -(D.y*P.x) + D.x * P.y;
        float x = D.x * (a/cos(PI/180*thetaY)) + b*(-D.y);
        float y = D.y * (a/cos(PI/180*thetaY)) + b*(D.x);
        glm::vec2 R = glm::vec2(x,y);

        return R;
}

glm::vec3 scaleVector(glm::vec3 V, float k){
    return glm::vec3(V.x * k, V.y * k, V.z * k);
}



pair<float, glm::vec3 > wallDistance(glm::vec3 Rd){
    // 근데 이제 이게 ray가  가는 방향의 역방향으로도 결국 교점이 있으면 찾아버려서, 이걸 고쳐야한다 (탄젠트의 주기성 때문)
    // 어차피 하나의 직선은 사각형에서 두 변만 지날 수 있다
    vector<pair<int, pair<pair<float, glm::vec3>, pair<float, glm::vec3> > > > distanceV; 
    // {i번쨰 벽, { {minD,[minX,minY, minZ], {maxD,[maxX,maxY,minZ]} } } } 벡터 (절단면 구성 위해 필요)

    for(int i = 0; i<walls.size(); i++){
        // 교점의 과표 ix,iy
        // 1,1,1크기의 벽 블록의 중심 W

        //교점 I (interaction)
        glm::vec3 minI;
        glm::vec3 maxI;
        float minDistance = 1000000; // 
        float maxDistance = -1;
        // 좌표계(x+,y+) 기준
        float distance = 0;

        /*
        비교해야하는건 벽의 여섯 면이다. 이걸 다 하긴 좀 그렇고...
        결국 로직 자체는 같다, 하나의 좌표를 대입하고 그게 나머지 좌표들의 범위에 일치하는지 보는 것이다. 대신 그 범위가 평면이 됐을 뿐.
        0.5그거 더하고 빼는거 다시 하자
        */
        // 직선의 표현식 R = P + kRd

        glm::vec3 W = walls[i];
        glm::vec3 P = glm::vec3(px,py,pz);
        glm::vec3 I; // ray가 가르키는 점 하나를 표현하기 위한 벡터 I

        float k; // ray직선의 방향벡터 계수
        

        for(int ki=0;ki<2;ki++){
            float area = 0.5 * pow(-1,ki);
            //x부터
            k = (W.x + area - P.x)/Rd.x;
            if(k>=0){
                I = P + scaleVector(Rd,k);
                
                if(pow(I.y - W.y, 2) <= pow(area,2) && pow(I.z - W.z, 2) <= pow(area,2)){
                    distance = sqrt(pow((I - P).x,2) + pow((I - P).y,2) + pow((I - P).z,2));

                    if(distance < minDistance){
                        minDistance = distance;
                        minI = I;
                    }
                    if(distance > maxDistance){
                        maxDistance = distance;
                        maxI = I;
                    }
                }
            }
            //y
            k = (W.y + area - P.y)/Rd.y;
            if(k>=0){
                I = P + scaleVector(Rd,k);
                
                if(pow(I.x - W.x, 2) <= pow(area,2) && pow(I.z - W.z, 2) <= pow(area,2)){
                    distance = sqrt(pow((I - P).x,2) + pow((I - P).y,2) + pow((I - P).z,2));

                    if(distance < minDistance){
                        minDistance = distance;
                        minI = I;
                    }
                    if(distance > maxDistance){
                        maxDistance = distance;
                        maxI = I;
                    }
                }
            }
            //z
            k = (W.z + area - P.z)/Rd.z;
            if(k>=0){
                I = P + scaleVector(Rd,k);
                
                if(pow(I.y - W.y, 2) <= pow(area,2) && pow(I.x - W.x, 2) <= pow(area,2)){
                    distance = sqrt(pow((I - P).x,2) + pow((I - P).y,2) + pow((I - P).z,2));

                    if(distance < minDistance){
                        minDistance = distance;
                        minI = I;
                    }
                    if(distance > maxDistance){
                        maxDistance = distance;
                        maxI = I;
                    }
                }
            }
        }
        

        
            
        //근데 안 만나는 오브젝트들도 있을거 아냐
        if(minDistance != 1000000000 && maxDistance != -1){
            distanceV.push_back(make_pair(i, 
            make_pair(
                make_pair(minDistance, minI),
                make_pair(maxDistance, maxI)
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
        return make_pair(-1, glm::vec3());
    }
    return distanceV[minI].second.first;
}
#endif