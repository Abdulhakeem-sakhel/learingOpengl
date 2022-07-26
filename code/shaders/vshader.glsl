#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection*view*model*vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    //ourColor = aColor; // set ourColor to the input color we got from the vertex data
    TexCoord = aTexCoord;
}
