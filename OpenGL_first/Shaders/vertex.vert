#version 460 core
layout (location = 0) in vec3 aPos;		 // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;	 // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // tex coord variable has attribute position 2

out vec3 ourColor;						// specify a color output to the fragment shader
out vec2 TexCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	

	gl_Position = projection * view * model * vec4(aPos, 1.0f);		// giving vec3 to vec4's constructor
	ourColor = aColor;	// set the output variable to your color
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
};
