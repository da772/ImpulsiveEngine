#type vertex

#version 300 es
precision lowp float;
        
layout(location = 0) in highp vec3 a_Position;
layout(location = 1) in lowp vec4 a_Color;
layout(location = 2) in lowp vec2 a_TexCoord;
layout(location = 3) in mediump float a_TexSlot;
layout(location = 4) in mediump vec2 a_TexScale;
layout(location = 5) in mediump float a_AlphaChannel;

out lowp vec4 v_Color;
out lowp vec2 v_TexCoord;
flat out lowp int v_TexSlot;
flat out mediump vec2 v_TexScale;
flat out lowp int v_AlphaChannel;

uniform mat4 u_ViewProjection;
        
void main() {
    v_Color = a_Color;
    v_TexSlot = int(a_TexSlot);
    v_TexCoord = a_TexCoord;
    v_TexScale = a_TexScale;
    v_AlphaChannel = int(a_AlphaChannel);
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 300 es
precision lowp float;

layout(location = 0) out lowp vec4 color;

in lowp vec4 v_Color;
in lowp vec2 v_TexCoord;
flat in lowp int v_TexSlot;
flat in mediump vec2 v_TexScale;
flat in lowp int v_AlphaChannel;

uniform lowp sampler2D u_Textures[16];

void main() {
    lowp vec4 texColor = vec4(1.0,1.0,1.0, 1.0);
    switch(v_TexSlot)
        {
            default:
            case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TexScale) ; break;
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
