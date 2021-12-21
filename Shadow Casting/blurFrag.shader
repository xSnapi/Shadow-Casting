#version 330 core

uniform sampler2D texture;

const float PI2 = 6.28318530718;
    
const float directions  = 16.0;
const float quality     = 5.0;
const float size        = 9.0;

layout(location = 0) out vec4 color;
in vec2 i_tex;

uniform vec2 u_resolution;

void main()
{   
    vec2 radius = size / vec2(800.0, 800.0);

    vec4 pixel = texture2D(texture, i_tex);
    
    for(float d = 0.0; d < PI2; d += PI2 / directions)
    {
		for(float i = 1.0 / quality; i <= 1.0; i += 1.0 / quality)
        {
			pixel += texture2D(texture, i_tex + vec2(cos(d), sin(d)) * radius * i);		
        }
    }
    
    pixel /= quality * directions - 15.0;
    color =  pixel;
};