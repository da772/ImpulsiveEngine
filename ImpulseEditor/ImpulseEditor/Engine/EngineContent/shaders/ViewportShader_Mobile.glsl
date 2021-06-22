#type vertex
#version 300 es


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


out highp vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 300 es
precision highp float;

layout(location = 0) out vec4 FragColor;
in vec2 v_TexCoord;

uniform lowp sampler2D u_Texture;
void main() {
    vec4 c = texture(u_Texture, v_TexCoord);
    FragColor = c;
}
