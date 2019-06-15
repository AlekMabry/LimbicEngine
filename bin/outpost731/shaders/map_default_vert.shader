#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_norm;

// Values that stay constant for the whole mesh.
uniform mat4 MATRIX_WORLD_VIEW_PROJECTION;
uniform mat3 MATRIX_NORMAL_VIEW_PROJECTION;
uniform vec2 in_texDimensions;

out vec2 uv;
out vec3 norm;
out vec2 texDimensions;
  
void main(){
  uv = in_uv;
  norm = MATRIX_NORMAL_VIEW_PROJECTION * in_norm;
  texDimensions = in_texDimensions;
  // Output position of the vertex, in clip space : MATRIX_WORLD_VIEW_PROJECTION * position
  gl_Position =  MATRIX_WORLD_VIEW_PROJECTION * vec4(in_position,1);
}

