#ifndef __PACMAN_H_
#define __PACMAN_H_
#include <string>

static const std::string spriteVertexShaderSrc = R"(
#version 430 core
layout(location = 0) in vec2 aPosition;
layout(location = 2) in vec2 aTexCoord;

/** Outputs */
out vec2 TexCoord;

/** Uniforms */
uniform mat4 u_TransformationMat;

void main() 
{
	// Position
	gl_Position = u_TransformationMat * vec4(aPosition, 1.0f, 1.0f);

	// Texture coordinates
	TexCoord = aTexCoord;
}

)";


static const std::string spriteFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec2 TexCoord;

/** Outputs */
out vec4 outColor;

uniform sampler2D ourTexture;

void main() 
{
	outColor = texture(ourTexture, TexCoord);
}
)";

#endif
