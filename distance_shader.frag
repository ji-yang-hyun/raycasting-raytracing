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
    
    float k;
    float dst;
    vec3 I;
    float minDistance = -1.0f;
    vec3 W;

    for(int i=0;i<wallCount;i++){
        W = wallsCoord[i];

        int d;
        for(int axis=0;axis<3;axis++){
            if(player[axis%3] - W[axis%3] > 0){
                d = 1;
            }
            else d = -1;
            k = (W[axis%3] + wallArea*d - player[axis%3])/Rd[axis%3];

            if(k>=0){
                I = player + Rd*k;
                if(pow(I[(axis+1)%3] - W[(axis+1)%3], 2) <= pow(wallArea,2) 
                    && pow(I[(axis+2)%3] - W[(axis+2)%3], 2) <= pow(wallArea, 2)){
                    dst = length(I - player);
                    if(dst <= minDistance || minDistance<0){
                        minDistance = dst;
                    }
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