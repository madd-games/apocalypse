/*
	Copyright (c) 2014, Madd Games.
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	
	* Redistributions of source code must retain the above copyright notice, this
	  list of conditions and the following disclaimer.
	
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#version 150
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uObjectMatrix;
uniform mat4 uLightMatrix;

// this is 1 for particles.
uniform int uIsParticle;

in vec4 inVertex;
in vec2 inTexCoords;
in vec3 inNormal;
in int inPartPosIndex;		// only used for particles.

out vec2 passTexCoords;
out vec3 passNormal;
out vec4 passVertex;
out vec4 passShadowCoord;
out vec4 passFragCoord;

void main()
{
	passVertex = inVertex;
	passTexCoords = inTexCoords;
	passNormal = inNormal;
	passShadowCoord = uLightMatrix * uModelMatrix * uObjectMatrix * inVertex;
	vec4 fragCoord = uProjectionMatrix * uViewMatrix * uModelMatrix * uObjectMatrix * inVertex;
	if (uIsParticle == 1)
	{
		float xradius = 0.025;
		float yradius = 0.056;
		if (inPartPosIndex == 1)
		{
			fragCoord += vec4(-xradius, yradius, 0.0, 0.0);
			passTexCoords = vec2(0.0, 1.0);
		}
		else if (inPartPosIndex == 2)
		{
			fragCoord += vec4(-xradius, -yradius, 0.0, 0.0);
			passTexCoords = vec2(0.0, 0.0);
		}
		else if (inPartPosIndex == 3)
		{
			fragCoord += vec4(xradius, -yradius, 0.0, 0.0);
			passTexCoords = vec2(1.0, 0.0);
		}
		else
		{
			fragCoord += vec4(xradius, yradius, 0.0, 0.0);
			passTexCoords = vec2(1.0, 1.0);
		};
	};

	passFragCoord = fragCoord;
	gl_Position = fragCoord;
};
