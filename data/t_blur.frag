const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    // TODO: Step 2 - Fill this in!
    vec2 t = gl_TexCoord[0].st;
    vec4 frag = texture(tex,t);
    
    for(int i=0;i<MAX_KERNEL_SIZE;i++) {
	for(int j=0;j<MAX_KERNEL_SIZE;j++) {
	    vec2 loc = offsets[i,j] + t;
	    vec4 val = texture(tex, loc);
	    float k = kernel[i,j];
		frag+=k*val;
	}
    }
    gl_FragColor=texture(tex,t);
}