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
uniform sampler2D uSampler;
uniform vec4 uAmbientLight;
uniform vec4 uDiffuseColor;
uniform vec4 uSpecularColor;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uObjectMatrix;

// Directional light
uniform vec3 uLightDir[2];
uniform vec4 uDiffuseLight[2];
uniform vec4 uSpecularLight[2];

in vec2 passTexCoords;
in vec3 passNormal;
in vec4 passVertex;

out vec4 outColor;

vec4 computeDirLight(in int i)
{
	vec3 normal = normalize(vec3(uModelMatrix * uObjectMatrix * vec4(passNormal, 0.0)));
	vec3 lightDir = -normalize(uLightDir[i]);
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
	{
		vec3 eye = vec3(uViewMatrix * uModelMatrix * uObjectMatrix * passVertex);
		vec3 hv = normalize(eye - lightDir);
		float NdotHV = max(dot(normal, hv), 0.0);
		specular = NdotHV * uSpecularColor * uSpecularLight[i];
	};
	return NdotL * uDiffuseColor * uDiffuseLight[i] + specular;
};

void main()
{
	vec4 light = uAmbientLight;
	int i;
	for (i=0; i<2; i++)
	{
		light = max(light, computeDirLight(i));
	};
	light = min(light, vec4(1.0, 1.0, 1.0, 1.0));
	vec4 color = texture(uSampler, passTexCoords) * light;
	outColor = color;
};
