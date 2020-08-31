#type vertex

#version 300 es
        
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
        
void main() {
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexSlot = a_TexSlot;
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 300 es

precision highp float;
layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;
in vec2 v_TexScale;

uniform sampler2D u_Textures[16];

void main() {

    vec4 texColor = v_Color;
    switch(int(v_TexSlot))
        {
            default:
            case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TexScale); break;
            case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TexScale); break;
            case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TexScale); break;
            case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TexScale); break;
            case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TexScale); break;
            case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TexScale); break;
            case 6: texColor *= texture(u_Textures[6], v_TexCoord * v_TexScale); break;
            case 7: texColor *= texture(u_Textures[7], v_TexCoord * v_TexScale); break;
            case 8: texColor *= texture(u_Textures[8], v_TexCoord * v_TexScale); break;
            case 9: texColor *= texture(u_Textures[9], v_TexCoord * v_TexScale); break;
            case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TexScale); break;
            case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TexScale); break;
            case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TexScale); break;
            case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TexScale); break;
            case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TexScale); break;
            case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TexScale); break;
        }
    color = texColor;
}
