#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 uv;
  
void main(){
  uv = in_uv;
  gl_Position.xyz = vec3(in_position.x, in_position.y, 0.0);
  gl_Position.w = 1.0;
}

