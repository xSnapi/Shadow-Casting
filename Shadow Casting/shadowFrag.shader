#version 330 core

uniform sampler2D texture;

layout(location = 0) out vec4 color;

in vec2 i_tex;

uniform vec2 u_mouse;

const vec2 resolution = vec2(800.0, 800.0);

void main()
{
	vec2 uv		 = gl_FragCoord.xy / resolution;
	vec2 mouse = u_mouse / resolution;
	mouse.y    = 1.0 - mouse.y;

	vec4 pixel = texture2D(texture, i_tex);

	float circle = 1.0 - length(uv - mouse) * 2.0;

	color = vec4(vec3(0.1), circle * (1.0 - pixel.b) * 1.35);
};