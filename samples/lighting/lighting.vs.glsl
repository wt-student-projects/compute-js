#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 normal;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0);
  normal = (model * vec4(normals, 1.0)).xyz;
}
