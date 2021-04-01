#type vertex
#version 300 es
        
layout(location = 0) in highp vec3 a_Position;
layout(location = 1) in lowp vec4 a_Color;
layout(location = 2) in lowp vec2 a_TexCoord;
layout(location = 3) in lowp float a_TexSlot;
layout(location = 4) in mediump vec2 a_TexScale;

out highp vec3 v_Position;

out lowp vec4 v_Color;
out lowp vec2 v_TexCoord;
out lowp float v_TexSlot;
out mediump vec2 v_TexScale;

uniform mat4 u_ViewProjection;
uniform highp float u_Time;
uniform highp vec2 u_Vel;
uniform highp float u_Lifespan;
uniform lowp vec4 u_EndColor;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    v_Position = a_Position;
    
    v_TexSlot = a_TexSlot;
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    /*
    highp float id = float(gl_InstanceID);
    highp float fl = float(floor(u_Time/u_Lifespan));
    highp float time = u_Time - float(floor(id/5.0));
    if (time < 0.0) {
        return;
    }

    if (time > u_Lifespan) {
        time = time - (u_Lifespan * fl);
    }
    highp float r = rand(vec2(id+fl,id+fl) );
    highp float xVar = (r*4.0)-2.0;
    highp float yVar = r;
    gl_Position = u_ViewProjection * vec4( vec2(a_Position.x+((u_Vel.x+xVar)*time), a_Position.y+((u_Vel.y+yVar)*time)), 1.0,1.0);
    //v_Color = (u_EndColor - a_Color) * time / u_Lifespan + a_Color;
    */
    highp float id = float(gl_InstanceID);
    highp float time = u_Time;
    v_Color = a_Color;    
    highp vec4 pos = vec4(a_Position.xy+(u_Vel*u_Time), a_Position.z, 1.0);
    gl_Position = u_ViewProjection * pos;
}

#type fragment
#version 300 es

precision lowp float;
layout(location = 0) out vec4 color;

in highp vec3 v_Position;
in lowp  vec4 v_Color;
in lowp  vec2 v_TexCoord;
in lowp  float v_TexSlot;
in mediump vec2 v_TexScale;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = v_Color;
    if (v_Color.w <= 0.0) {
        discard;
    }

    
    color = texColor;
    return;
    texColor *= texture(u_Texture, v_TexCoord * v_TexScale);
}
