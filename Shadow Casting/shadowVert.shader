varying vec2 i_tex;

const float scalingFactor = 0.375;

void main()
{
	vec4 pos    = gl_ModelViewProjectionMatrix * gl_Vertex;
	i_tex		= gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_Position = vec4(pos.x / scalingFactor, pos.y / scalingFactor, 0, 1);
};