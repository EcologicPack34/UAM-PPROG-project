#version 330 core

in vec3 vColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    // Create a smooth RGB color cycle based on time
    float red   = sin(time * 0.5) * 0.5 + 0.5;
    float green = sin(time * 0.7 + 2.0) * 0.5 + 0.5;
    float blue  = sin(time * 1.3 + 4.0) * 0.5 + 0.5;

    //vec4(red, green, blue, 1.0) * 
    vec4 mixTex = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.3);

    FragColor = vec4(vColor, 1.0) * mixTex;
}