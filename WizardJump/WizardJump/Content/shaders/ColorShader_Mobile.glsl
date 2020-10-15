#type vertex
#version 300 es


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


out highp vec2 v_TexCoord;
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection*u_Transform*vec4(a_Position, 1.0);
}


#type fragment
#version 300 es
precision highp float;
layout(location = 0) out vec4 FragColor;
in vec2 v_TexCoord;

uniform vec4 u_Color;


void main() {
    FragColor = u_Color;
}