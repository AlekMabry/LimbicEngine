#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec2 lightmap_vertexUV;

// Values that stay constant for the whole mesh.
uniform mat4 MATRIX_WORLD_VIEW_PROJECTION;
uniform vec2 in_texDimensions;

out vec2 UV;
out vec2 texDimensions;
  
void main(){
  UV = vertexUV;
  texDimensions = in_texDimensions;
  // Output position of the vertex, in clip space : MATRIX_WORLD_VIEW_PROJECTION * position
  gl_Position =  MATRIX_WORLD_VIEW_PROJECTION * vec4(vertexPosition_modelspace,1);
}

