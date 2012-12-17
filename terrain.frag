
uniform float dirtMin, dirtMax;
uniform float grassMin, grassMax;
uniform float rockMin, rockMax;
uniform float snowMin, snowMax;
    
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying float height;
varying float intensity;

float getGrassWeight() {
    float grassRange = grassMax - grassMin;
    float grassWeight = (grassRange-abs(height-grassMax))/grassRange;
    grassWeight = max(0.0,grassWeight);
    return grassWeight;
}
float getSnowWeight() {
    float snowRange = snowMax - snowMin;
    float snowWeight = (snowRange-abs(height-snowMax))/snowRange;
    snowWeight = max(0.0,snowWeight);
    return snowWeight;
}
float getDirtWeight() {
    float dirtRange = dirtMax - dirtMin;
    float dirtWeight = (dirtRange-abs(height-dirtMax))/dirtRange;
    dirtWeight = max(0.0,dirtWeight);
    return dirtWeight;
}
float getRockWeight() {
    float rockRange = rockMax - rockMin;
    float rockWeight = (rockRange-abs(height-rockMax))/rockRange;
    rockWeight = max(0.0,rockWeight);
    return rockWeight;
}
vec4 sampleTextures()
{
    vec4 gsample = texture2D(grassTexture, gl_TexCoord[0].st);
    vec4 rsample = texture2D(rockTexture, gl_TexCoord[0].st);
    vec4 ssample = texture2D(snowTexture, gl_TexCoord[0].st);
    vec4 dsample = texture2D(dirtTexture, gl_TexCoord[0].st);
    vec4 total = gsample*getGrassWeight() + 
		 ssample*getSnowWeight() +
		 rsample*getRockWeight() +
		 dsample*getDirtWeight();
    return total;
}

void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
