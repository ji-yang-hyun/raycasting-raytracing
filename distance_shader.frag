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


//여기서 pixelCoord는 0,0이 좌측 하단이다. 범위는 0~1
//(pixelCoord - 0.5) / 1.0
//(pixelCoord - 0.5) / 1.0

void main()
{
    vec2 pixelCoord = gl_FragCoord.xy/u_resolution.xy;

    vec3 Rd = D + Sc * (((pixelCoord.x - 0.5) / 1.0)/1.0f) + Sr * ((pixelCoord.y - 0.5) / 1.0);
    
    float k;
    float dst;
    vec3 I;
    vec3 A;
    float minDistance = -1.0f;
    for(int i=0;i<wallCount;i++){
        vec3 W = wallsCoord[i];

        for(int ki=0;ki<2;ki++){
            float area = wallArea * pow(-1,ki);

            // x
            k = (W.x + area - player.x)/Rd.x;

            if(k>=0){
                I = player + Rd*k;
                A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
                if(pow(I.y - W.y, 2) <= pow(area,2) && pow(I.z - W.z, 2) <= pow(area, 2)){
                    dst = length(I - player);
                    if(dst <= minDistance || minDistance<0){
                        minDistance = dst;
                    }
                }
            }

            // y
            k = (W.y + area - player.y)/Rd.y;

            if(k>=0){
                I = player + Rd*k;
                A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
                if(pow(I.x - W.x, 2) <= pow(area,2) && pow(I.z - W.z, 2) <= pow(area, 2)){
                    dst = length(I - player);
                    if(dst <= minDistance || minDistance<0){
                        minDistance = dst;
                    }
                }
            }

            // z
            k = (W.z + area - player.z)/Rd.z;

            if(k>=0){
                I = player + Rd*k;
                A = I - W; // I와 W의 x좌푯값은 area만큼 차이가 난다.
                if(pow(I.x - W.x, 2) <= pow(area,2) && pow(I.y - W.y, 2) <= pow(area, 2)){
                    dst = length(I - player);
                    if(dst <= minDistance || minDistance<0){
                        minDistance = dst;
                    }
                }
            }
        }
    }

    float color = min(max(minDistance/maxSight, 0.0f), 1.0f);
    FragColor = vec4(vec3(color), 1.0f);
}