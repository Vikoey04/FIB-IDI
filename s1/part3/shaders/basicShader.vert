#version 330 core

in vec3 vertex;

//inicialitzem el uniform
uniform float val;

void main()  {
    //gl_Position = vec4 (vertex, 1.0);
    gl_Position = vec4 (vertex * val, 1.0);
}
