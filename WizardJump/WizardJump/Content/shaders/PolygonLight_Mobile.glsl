#type vertex
#version 300 es


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_AlphaChannel;


out highp vec2 v_TexCoord;
out float v_AlphaChannel;
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main() {
	v_TexCoord = a_TexCoord;
	v_AlphaChannel = a_AlphaChannel;
	gl_Position = u_ViewProjection*u_Transform*vec4(a_Position, 1.0);
}


#type fragment
#version 300 es


precision mediump float;

out vec4 FragColor;
in vec2 v_TexCoord;
in float v_AlphaChannel;
uniform sampler2D u_Texture;

uniform vec4 u_Color;


void main() {
	vec4 color = texture(u_Texture, v_TexCoord)*u_Color;
    FragColor = vec4(color.xyz,color.w*(1.0-v_AlphaChannel));//texture(u_Texture, v_TexCoord)*u_Color;
}
