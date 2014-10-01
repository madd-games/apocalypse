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
uniform float uShininess;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uObjectMatrix;

// Directional light
// Each row in the texture represents a directional light source, and each
// column represents:
// 0 = the direction of the light (mapped from [-1, 1] to [0, 1]).
// 1 = the specular component
// 2 = the diffuse component
uniform sampler2D uDirLightArray;
uniform int uNumDirLights;

in vec2 passTexCoords;
in vec3 passNormal;
in vec4 passVertex;

out vec4 outColor;

// ===== DIRECTIONAL LIGHTS BEGIN ===== //
vec3 getLightDir(in int i)
{
	vec4 sub = texelFetch(uDirLightArray, ivec2(0, i), 0);
	return vec3(sub.x*2-1, sub.y*2-1, sub.z*2-1);
};

vec4 getLightSpecular(in int i)
{
	return texelFetch(uDirLightArray, ivec2(1, i), 0);
};

vec4 getLightDiffuse(in int i)
{
	return texelFetch(uDirLightArray, ivec2(2, i), 0);
};

void computeDirLight(in int i, inout vec4 diffuseLight, inout vec4 specularLight)
{
	vec3 normal = normalize(vec3(uModelMatrix * uObjectMatrix * vec4(passNormal, 0.0)));
	vec3 lightDir = -normalize(getLightDir(i));
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
	{
		vec3 eye = -vec3(uViewMatrix * uModelMatrix * uObjectMatrix * passVertex);
		vec3 hv = normalize(eye - lightDir);
		float NdotHV = max(dot(normal, hv), 0.0);
		specular = pow(NdotHV, uShininess) * uSpecularColor * getLightSpecular(i);
	};
	diffuseLight = max(diffuseLight, NdotL * uDiffuseColor * getLightDiffuse(i));
	specularLight = max(specularLight, specular);
};
// ===== DIRECTIONAL LIGHTS END ===== //

void main()
{
	vec4 diffuseLight = uAmbientLight;
	vec4 specularLight = vec4(0.0, 0.0, 0.0, 1.0);
	int i;
	for (i=0; i<uNumDirLights; i++)
	{
		computeDirLight(i, diffuseLight, specularLight);
	};
	vec4 light = diffuseLight + specularLight;
	light = min(light, vec4(1.0, 1.0, 1.0, 1.0));
	light.w = 1.0;
	vec4 color = texture(uSampler, passTexCoords) * light;
	outColor = color;
};
