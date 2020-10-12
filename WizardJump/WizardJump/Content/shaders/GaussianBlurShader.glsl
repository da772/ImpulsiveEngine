#type vertex
#version 330


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform vec2 u_BlurDirection;

out lowp vec2 v_BlurDirection;
out highp vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
    v_BlurDirection = u_BlurDirection;
	gl_Position = vec4(a_Position, 1.0);

}


#type fragment
#version 330

out vec4 FragColor;

in vec2 v_TexCoord;
in vec2 v_BlurDirection;

uniform sampler2D u_Texture;

float CalcGauss( float x, float sigma )
{
    if ( sigma <= 0.0 )
        return 0.0;
    return exp( -(x*x) / (2.0 * sigma) ) / (2.0 * 3.14157 * sigma);
}

void main() {
    float sigma = 1;
    vec2 texC     = v_TexCoord;
    vec2 texSize = textureSize(u_Texture, 0);
    vec4 texCol   = texture2D( u_Texture, texC );
    vec4 gaussCol = vec4( texCol.rgb, 1.0 );
    vec2 step     = v_BlurDirection / texSize;
    for ( int i = 1; i <= 32; ++ i )
    {
        float weight = CalcGauss( float(i) / 32.0, sigma * 0.5 );
        if ( weight < 1.0/255.0 )
            break;
        texCol    = texture2D( u_Texture, texC + step * float(i) );
        gaussCol += vec4( texCol.rgb * weight, weight );
        texCol    = texture2D( u_Texture, texC - step * float(i) );
        gaussCol += vec4( texCol.rgb * weight, weight );
    }
    gaussCol.rgb = clamp( gaussCol.rgb / gaussCol.w, 0.0, 1.0 );
    FragColor = vec4( gaussCol.rgb, 1.0 );

}
