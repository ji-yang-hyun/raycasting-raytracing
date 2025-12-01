#version 330 core
out vec4 FragColor;

uniform vec3 wallsCoord[1000];
uniform vec3 player;
uniform int wallCount;
uniform float wallArea;
uniform vec3 D;
uniform vec3 Sc;
uniform vec3 Sr;
uniform vec2 u_resolution;
uniform float maxSight;

void main()
{
    vec2 pixelCoord = gl_FragCoord.xy/u_resolution.xy;

    vec3 Rd = D + Sr * ((pixelCoord.x - 0.5) / 1.0f) + Sc * ((pixelCoord.y - 0.5) / 1.0f);
    int cnt = 0;
    
    float k;
    float dst;
    vec3 I;
    vec3 A;
    float minDistance = -1.0f;
    vec3 W;
    vec3 visableArea;

    for(int i=0;i<wallCount;i++){
        W = wallsCoord[i];

        visableArea = player - W;
        if(visableArea.x > 0) visableArea.x = 1;
        else visableArea.x = -1;
        if(visableArea.y > 0) visableArea.y = 1;
        else visableArea.y = -1;
        if(visableArea.z > 0) visableArea.z = 1;
        else visableArea.z = -1;
        visableArea = visableArea * wallArea;


        // x
        k = (W.x + visableArea.x - player.x)/Rd.x;

        if(k>=0){
            I = player + Rd*k;
            A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
            if(pow(I.y - W.y, 2) <= pow(visableArea.y,2) && pow(I.z - W.z, 2) <= pow(visableArea.y, 2)){
                dst = length(I - player);
                if(dst <= minDistance || minDistance<0){
                    minDistance = dst;
                }
            }
        }

        // y
        k = (W.y + visableArea.y - player.y)/Rd.y;

        if(k>=0){
            I = player + Rd*k;
            A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
            if(pow(I.x - W.x, 2) <= pow(visableArea.x,2) && pow(I.z - W.z, 2) <= pow(visableArea.z, 2)){
                dst = length(I - player);
                if(dst <= minDistance || minDistance<0){
                    minDistance = dst;
                }
            }
        }

        // z
        k = (W.z + visableArea.z - player.z)/Rd.z;

        if(k>=0){
            I = player + Rd*k;
            A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
            if(pow(I.x - W.x, 2) <= pow(visableArea.x,2) && pow(I.y - W.y, 2) <= pow(visableArea.y, 2)){
                dst = length(I - player);
                if(dst <= minDistance || minDistance<0){
                    minDistance = dst;
                }
            }
        }
    }

    float color;
    color = min(minDistance/maxSight, 1.0f);
    if(minDistance < 0){
        color = 1.0f;
    }

    FragColor = vec4(vec3(color), 1.0f);
}