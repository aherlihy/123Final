uniform float posx;
uniform float posy;
uniform float posz; 

varying vec3 dist;

varying float intensity;
varying vec4 color;

void main()
{
    const float rad = 100.50;
    
    float d = length(dist);
    float a = clamp(((rad - d) / rad), 0.0, 1.0);
    
    vec4 dval = color;
    vec4 dter = dval * d/100.0 + (0.8 * (1.0 - (d/100.0)));
    dter.a = dval.a;//retain alpha values
    
    float intensity = clamp(d/300.0, 0, 1);
    vec4 newcolor = vec4(0.8, 0.8, 0.8, 1.0);
    
    
    
    gl_FragColor = (1-intensity) * color + (intensity)*newcolor;   
} 
