#type vertex
#version 300 es

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;


out highp vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}


#type fragment
#version 300 es
precision highp float;
layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;


uniform sampler2D u_Texture;
void main() {
    
	 float Pi = 6.28318530718; // Pi*2
    ivec2 iResolution = textureSize(u_Texture, 0);
    // GAUSSIAN BLUR SETTINGS {{{
    float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    float Quality = 4.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
    float Size = 8.0; // BLUR SIZE (Radius)
    // GAUSSIAN BLUR SETTINGS }}}
   
    vec2 Radius = Size/vec2(float(iResolution.x), float(iResolution.y));
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = v_TexCoord;
    // Pixel colour
    vec4 Color = texture(u_Texture, uv);
    
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/Directions)
    {
		for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
			Color += texture( u_Texture, uv+vec2(cos(d),sin(d))*Radius*i);		
        }
    }
    
    // Output to screen
    Color /= Quality * Directions - 15.0;
    FragColor =  Color;
}
