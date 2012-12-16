uniform sampler2D tex;

    
void main()
{
    vec2 t = gl_TexCoord[0].st;
    gl_FragColor = texture(tex, t);
    
    
}
