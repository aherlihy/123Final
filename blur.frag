const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    // TODO: Step 2 - Fill this in!
    vec2 t = gl_TexCoord[0].st;
    //gl_FragColor = texture(tex, t);
    
   int intensities[256];
   int r_vals[256];
  int g_vals[256];
   int b_vals[256];
 //  for(int i=0;i<256;i++) {
 //       intensities[i]=0;
  //      r_vals[i]=0;
  //      g_vals[i]=0;
  //      b_vals[i]=0;
 //  }
    
    
    for(int i=0;i<MAX_KERNEL_SIZE;i++) {
	    for(int j=0;j<MAX_KERNEL_SIZE;j++) {
	        vec2 loc = offsets[i,j] + t;
	        vec4 val = texture(tex, loc);
	        
	        float intensity_f = (0.299*val.r + 0.587*val.g + 0.114*val.b);
                int intensity = intensity_f;
	            intensities[intensity]++;
                r_vals[intensity]+=val.r;
                g_vals[intensity]+=val.g;
                b_vals[intensity]+=val.b;
	    }
   }
    int max_int = 0;
    for(int k = 0;k<256;k++) {
       if(intensities[k]>intensities[max_int]) {
            max_int = k;
	}
    }
   // gl_FragColor=vec4(r_vals[max_int]/intensities[max_int], g_vals[max_int]/intensities[max_int], b_vals[max_int]/intensities[max_int], 1.0);
    gl_FragColor=texture(tex,t);
    }
