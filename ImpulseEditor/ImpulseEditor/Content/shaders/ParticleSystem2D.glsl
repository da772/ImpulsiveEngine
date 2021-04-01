#type vertex
#version 330
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;
layout(location = 4) in vec2 a_TexScale;

out vec3 v_Position;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;
out vec2 v_TexScale;

uniform mat4 u_ViewProjection;
uniform float u_Time;
uniform vec2 u_Vel;
uniform float u_Lifespan;
uniform vec4 u_EndColor;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    v_Position = a_Position;
    
    v_TexSlot = a_TexSlot;
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    /*
    float time = u_Time - floor(gl_InstanceID/5.0);
    if (time < 0) {
        return;
    }

    if (time > u_Lifespan) {
        time = time - (u_Lifespan * floor(time/u_Lifespan));
    }
    float r = rand(vec2(gl_InstanceID+floor(u_Time/u_Lifespan),gl_InstanceID+floor(u_Time/u_Lifespan)) );
    float xVar = (r*4)-2;
    float yVar = r;
    gl_Position = u_ViewProjection * vec4( vec2(a_Position.x+((u_Vel.x+xVar)*time), a_Position.y+((u_Vel.y+yVar)*time)), 1.0,1.0);
    */
    float time = u_Time - floor(gl_InstanceID/5.0);
    v_Color = (u_EndColor - a_Color) * time / u_Lifespan + a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position.xy+(u_Vel*time), a_Position.z, 1.0);
    
}

#type fragment
#version 330

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;
in vec2 v_TexScale;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = v_Color;
    if (v_Color.w <= 0) {
        discard;
    }

    
    color = texColor;
    return;
    texColor *= texture(u_Texture, v_TexCoord * v_TexScale);
}
