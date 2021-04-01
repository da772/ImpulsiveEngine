#type vertex

#version 300 es
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;
layout(location = 4) in vec2 a_TexScale;
layout(location = 5) in float a_AlphaChannel;
layout(location = 6) in vec2 a_Position2D;

out vec3 v_Position;

out vec4 v_Color;
out vec2 v_TexCoord;
out int v_TexSlot;
out vec2 v_TexScale;
out float v_AlphaChannel;

uniform mat4 u_ViewProjection;
        
void main() {
    v_Color = a_Color;
    v_TexSlot = int(a_TexSlot);
    v_AlphaChannel = a_AlphaChannel;
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

in lowp vec4 v_Color;
in lowp vec2 v_TexCoord;
in lowp int v_TexSlot;
in mediump vec2 v_TexScale;
in lowp float v_AlphaChannel;

uniform lowp sampler2D u_Textures[4];

void main() {
    lowp vec4 texColor = v_Color;
    switch(int(v_TexSlot))
        {
            default:
            case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TexScale); break;
            case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TexScale); break;
            case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TexScale); break;
            case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TexScale); break;
        }

    color = vec4(texColor.xyz, texColor.w*(1.0-v_AlphaChannel));
}
