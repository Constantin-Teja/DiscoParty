#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;

void main()
{
	world_position = vec3 (Model * vec4(v_position,1));     //obtinem v_position in sp lume
	world_normal = normalize (vec3 ( Model * vec4 (v_normal, 0) ));
	// texcoord = v_texture_coord;
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
