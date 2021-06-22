#type vertex
#version 330


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out highp vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 330

out vec4 FragColor;


in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform vec2 u_UV;
uniform float u_alphaTolerance;
void main() {
    vec4 c = texture(u_Texture, v_TexCoord * u_UV) * u_Color;
    FragColor = c;
}
