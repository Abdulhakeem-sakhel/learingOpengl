#version 330 core
out vec4 FragColor;  
in vec3 ourColor; // the input variable from the vertex shader (same name and same type)  
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(ourColor,1.0);
    FragColor = texture(ourTexture, TexCoord);

} 