#type vertex
#version 330


layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_AlphaChannel;



out float v_AlphaChannel;
uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

void main() {
	
	v_AlphaChannel = a_AlphaChannel;
	gl_Position = u_ViewProjection*u_Transform*vec4(a_Position, 1.0);
}


#type fragment
#version 330

out vec4 FragColor;

in float v_AlphaChannel;
uniform vec4 u_Color;


void main() {
	
	float dist = v_AlphaChannel;
	float c1 = 1.0;
	float c2 = 1.0;
	float c3 = 1.0;
	float attuniation = 1.0/(c1 + c2*dist + c3*(dist*dist));


    FragColor = vec4(u_Color.xyz,(1.0-attuniation)*u_Color.w);
}
