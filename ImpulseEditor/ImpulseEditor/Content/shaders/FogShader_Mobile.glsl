#type vertex

#version 300 es
        
layout(location = 0) in highp vec3 a_Position;
layout(location = 1) in lowp vec4 a_Color;
layout(location = 2) in lowp vec2 a_TexCoord;
layout(location = 3) in lowp float a_TexSlot;
layout(location = 4) in mediump vec2 a_TexScale;

out lowp vec4 v_Color;
out lowp vec2 v_TexCoord;
flat out lowp int v_TexSlot;
flat out mediump vec2 v_TexScale;

uniform mat4 u_ViewProjection;
        
void main() {
    v_Color = a_Color;
    v_TexSlot = int(a_TexSlot);
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
flat in lowp int v_TexSlot;
flat in mediump vec2 v_TexScale;

uniform lowp sampler2D u_Textures[1];

uniform highp float u_Time;

lowp int OCTAVES = 4;

lowp float rand(lowp vec2 coord){
	return fract(sin(dot(coord, vec2(56, 78)) * 1000.0) * 1000.0);
}

mediump float noise(vec2 coord){
	lowp vec2 i = floor(coord);
	lowp vec2 f = fract(coord);

	// 4 corners of a rectangle surrounding our point
	lowp float a = rand(i);
	lowp float b = rand(i + vec2(1.0, 0.0));
	lowp float c = rand(i + vec2(0.0, 1.0));
	lowp float d = rand(i + vec2(1.0, 1.0));

	mediump vec2 cubic = f * f * (3.0 - 2.0 * f);

	return mix(a, b, cubic.x) + (c - a) * cubic.y * (1.0 - cubic.x) + (d - b) * cubic.x * cubic.y;
}


mediump float fbm(vec2 coord){
	mediump float value = 0.0;
	mediump float scale = 0.5;

	for(int i = 0; i < OCTAVES; i++){
		value += noise(coord) * scale;
		coord *= 2.0;
		scale *= 0.5;
	}
	return value;
}



void main() {
    mediump vec2 coord = v_TexCoord * 1.25;

	mediump vec2 motion = vec2( fbm(coord + vec2(u_Time * 1.5, 0)) );

	mediump float final = fbm(coord + motion);
    color = vec4(v_Color.xyz, final*v_Color.w);
}
