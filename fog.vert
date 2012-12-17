uniform float posx;
uniform float posy;
uniform float posz;    
varying vec3 dist;
varying vec3 vertex;
varying vec4 color;
varying float intensity;

void main()
{
    vertex = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vec3 pos = vec3(posx,posy,posz);
    dist = pos - vertex;
    color = gl_Color;
    gl_Position=ftransform();//gl_ModelViewProjectionMatrix*gl_Vertex;
	
    //vec3 n = gl_NormalMatrix*gl_Normal;
    
    //vec3 l = normalize(gl_LightSource[0].position - (gl_ModelViewMatrix*gl_Vertex));
    
    //intensity = dot(n,l);
 
    
    }
