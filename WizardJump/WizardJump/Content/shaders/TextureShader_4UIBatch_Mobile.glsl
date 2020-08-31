#type vertex

#version 300 es
precision highp float;
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;
layout(location = 4) in vec2 a_TexScale;
layout(location = 5) in float a_AlphaChannel;

out vec3 v_Position;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;
out vec2 v_TexScale;
out float v_AlphaChannel;

uniform mat4 u_ViewProjection;
        
void main() {
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexSlot = a_TexSlot;
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    v_AlphaChannel = a_AlphaChannel;
    gl_Position = vec4(a_Position, 1.0);
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
in float v_AlphaChannel;

uniform sampler2D u_Textures[4];

void main() {
    vec4 texColor = vec4(1.0,1.0,1.0, 1.0);
    switch(int(v_TexSlot))
        {
            default:
            case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TexScale) ; break;
            case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TexScale); break;
            case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TexScale); break;
            case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TexScale); break;
        }
        
    switch(int(v_AlphaChannel))
    {
        default:
        case 4: texColor.a = texColor.a; break;
        case 3: texColor.rgba = vec4(1.0,1.0,1.0, texColor.b); break;
        case 2: texColor.rgba = vec4(1.0,1.0,1.0, texColor.g); break;
        case 1: texColor.rgba = vec4(1.0,1.0,1.0, texColor.r); break;

    }
    texColor = v_Color * (texColor);
    color = texColor;
    //color = vec4(v_TexSlot, v_TexSlot, v_TexSlot, 1.f);
}
