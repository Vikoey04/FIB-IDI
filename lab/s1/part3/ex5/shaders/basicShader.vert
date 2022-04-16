#version 330 core

in vec3 vertex;
in vec3 color;
out vec4 fcolor;

uniform mat4 TG;

void main()  {
    fcolor = vec4(color, 1.0);
    gl_Position = TG * vec4(vertex, 1.0);
}
