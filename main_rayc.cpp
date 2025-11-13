// #ifndef DISTANCE
// #define DISTANCE
// #include<stdio.h>
// #include<iostream>
// #include<vector>
// #include<cmath>
// #include"map.cpp"

// using namespace std;

// #define povHorizontal 180
// #define povVertical 120
// #define dt 5 //delta theta
// #define pixelX povHorizontal / dt
// #define pixelY povVertical / dt
// #define PI 3.1415926

// int screen[pixelY][pixelX] = {};
// float px = 0;
// float py = 0;

// float wallDistance(float theta){
//     float minDistance = -1;
//     float thetaF = theta;
//     for(int i = 0; i<walls.size(); i++){
//         float x = walls[i].second;
//         float y = walls[i].first; 
//         // 좌표계(x+,y+) 기준
//         float y1 = tan(PI/180 * thetaF) * (x + 0.5 - px) + py; // 오른쪽 벽 판별
//         float y2 = tan(PI/180 * thetaF) * (x - 0.5 - px) + py; // 왼쪽 벽 판별
//         float x1 = (y - 0.5 - py) / tan(PI/180 * thetaF) + px; // 아래쪽 벽 판별
//         float x2 = (y + 0.5 - py) / tan(PI/180 * thetaF) + px; // 위쪽 벽 판별
//         float distance = 0;
        
//         if((y - y1) < 0.5 && (y - y1) > -0.5){
//             distance = sqrt(pow((x + 0.5 - px),2) + pow((y1 - py),2));
//             if(minDistance == -1 || minDistance > distance){
//                 minDistance = distance;
//             }
//         }
//         if((y - y2) < 0.5 && (y - y2) > -0.5){
//             distance = sqrt(pow((x - 0.5 - px),2) + pow((y2 - py),2));
//             if(minDistance == -1 || minDistance > distance){
//                 minDistance = distance;
//             }
//         }
//         if((x - x1) < 0.5 && (x - x1) > -0.5){
//             distance = sqrt(pow((y - 0.5 - py),2) + pow((x1 - px),2));
//             if(minDistance == -1 || minDistance > distance){
//                 minDistance = distance;
//             }
//         }
//         if((x - x2) < 0.5 && (x - x2) > -0.5){
//             distance = sqrt(pow((y + 0.5 - py),2) + pow((x1 - px),2));
//             if(minDistance == -1 || minDistance > distance){
//                 minDistance = distance;
//             }
//         }
//     }
//     return minDistance;
// }

// #endif

// // int main(){
// //     wallCoordinate();
// //     for(int i=0;i<pixelX;i++){
// //         float theta = 90 - povHorizontal/2 + dt/2 + i*dt;
// //         float distance = wallDistance(theta);
// //         printf("각도 : %f, 거리 : %f\n", theta, distance);
// //     }
// //     return 0;
// // }