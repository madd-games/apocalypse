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

#version 330
uniform sampler2D uSampler;

uniform vec4 uAmbientLight;

uniform vec4 uDiffuseColor;
uniform vec4 uSpecularColor;
uniform float uShininess;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uObjectMatrix;

// Eye position
uniform vec4 uEyePos;

// The specular map sampler.
uniform sampler2D uSpecularMap;

// The normal map sampler.
uniform sampler2D uNormalMap;

// Directional lights
// Each light is specified by 3 texels:
// the light's direction, the diffuse component, and the specular component.
uniform samplerBuffer uDirLightArray;
uniform int uNumDirLights;

// Points lights
// Each light is specified by 3 texels:
// the light's source position, the diffuse component, and the specular component.
uniform samplerBuffer uPointLightArray;
uniform int uNumPointLights;

// This is 1 for shadow maps.
uniform int uIsShadowMap;

// For sampling the shadow map.
uniform sampler2D uShadowMap;

// Particle systems
uniform int uIsParticle;

// Fog
uniform vec4 uFogColor;
uniform float uFogDensity;

in vec2 passTexCoords;
in vec3 passNormal;
in vec4 passVertex;
in vec4 passShadowCoord;
in vec4 passFragCoord;
in vec3 passVTan;
in vec3 passUTan;

out vec4 outColor;

// DIRECTIONAL LIGHTS

vec3 getLightDir(in int i)
{
	vec4 sub = texelFetch(uDirLightArray, (3*i));
	return sub.xyz;
};

vec4 getDirLightDiffuse(in int i)
{
	return texelFetch(uDirLightArray, (3*i)+1);
};

vec4 getDirLightSpecular(in int i)
{
	return texelFetch(uDirLightArray, (3*i)+2);
};

void computeDirLight(in int i, in vec3 normal, inout vec4 diffuseLight, inout vec4 specularLight)
{
	vec3 lightDir = -normalize(getLightDir(i));
	float NdotL = max(dot(normal, lightDir), 0.0);
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
	{
		vec3 eye = vec3(uEyePos - passVertex);
		vec3 hv = normalize(eye - lightDir);
		float NdotHV = max(dot(normal, hv), 0.0);
		// we must assume that 0^0 = 1.
		if (uShininess == 0.0)
		{
			specular = uSpecularColor * getDirLightSpecular(i);
		}
		else
		{
			specular = pow(NdotHV, uShininess) * uSpecularColor * getDirLightSpecular(i);
		};
	};
	diffuseLight += NdotL * uDiffuseColor * getDirLightDiffuse(i);
	specularLight += specular;
};

// POINT LIGHTS

vec4 getPointLight(in int i)
{
	return texelFetch(uPointLightArray, (3*i));
};

vec4 getPointLightDiffuse(in int i)
{
	return texelFetch(uPointLightArray, (3*i)+1);
};

vec4 getPointLightSpecular(in int i)
{
	return texelFetch(uPointLightArray, (3*i)+2);
};

void computePointLight(in int i, in vec3 normal, inout vec4 diffuseLight, inout vec4 specularLight)
{
	vec4 lightToPoint = passVertex - getPointLight(i);
	vec3 lightDir = -normalize(lightToPoint.xyz);

	float factor = 1.0 / (length(lightToPoint) * length(lightToPoint));

	float NdotL = max(dot(normalize(normal), lightDir), 0.0);
	vec4 specular = vec4(0.0);
	if (NdotL > 0.0)
	{
		vec3 eye = vec3(uEyePos - passVertex);
		vec3 hv = normalize(eye - lightDir);
		float NdotHV = max(dot(normal, hv), 0.0);
		// we must assume that 0^0 = 1.
		if (uShininess == 0.0)
		{
			specular = factor * uSpecularColor * getPointLightSpecular(i);
		}
		else
		{
			specular = factor * pow(NdotHV, uShininess) * uSpecularColor * getPointLightSpecular(i);
		};
	};
	diffuseLight += factor * NdotL * uDiffuseColor * getPointLightDiffuse(i);
	specularLight += specular;
};

void main()
{
	if (uIsShadowMap == 0)
	{
		vec4 shadowCoord = vec4(((passShadowCoord.x/passShadowCoord.w)+1)/2, ((passShadowCoord.y/passShadowCoord.w)+1)/2,
					passShadowCoord.z/passShadowCoord.w, 1.0);
		float depth = texture(uShadowMap, shadowCoord.xy).r;

		vec4 diffuseLight = uAmbientLight;
		vec4 specularLight = vec4(0.0, 0.0, 0.0, 1.0);
		vec3 tmpNormal = passNormal;
		/*vec3 tmpNormalSample = vec3(texture(uNormalMap, passTexCoords)) * vec3(2.0, 2.0, 1.0) + vec3(-1.0, -1.0, 0.0);
		vec3 normalPolygonSpace = normalize(tmpNormalSample);
		vec3 tmpNormal = vec3(
			dot(normalize(passVTan), normalPolygonSpace),
			dot(normalize(passUTan), normalPolygonSpace),
			dot(normalize(passNormal), normalPolygonSpace)
		);*/
		vec3 normal = normalize(vec3(uModelMatrix * uObjectMatrix * vec4(tmpNormal, 0.0)));

		int i;
		for (i=0; i<uNumDirLights; i++)
		{
			computeDirLight(i, normal, diffuseLight, specularLight);
		};
		for (i=0; i<uNumPointLights; i++)
		{
			computePointLight(i, normal, diffuseLight, specularLight);
		};

		/*bool withinTex = (shadowCoord.x > 0) && (shadowCoord.x < 1) && (shadowCoord.y > 0) && (shadowCoord.y < 1);
		if ((depth-0.005) < shadowCoord.z && withinTex)
		{
			diffuseLight = uAmbientLight;
			specularLight = vec4(0.0, 0.0, 0.0, 1.0);
		};*/

		diffuseLight = clamp(diffuseLight, 0, 1);
		specularLight = clamp(specularLight, 0, 1);
		diffuseLight.w = 1.0;
		specularLight.w = 1.0;
		vec4 color = texture(uSampler, passTexCoords) * diffuseLight
				+ texture(uSpecularMap, passTexCoords) * specularLight;
		vec3 vVertex = vec3(uViewMatrix * uModelMatrix * uObjectMatrix * passVertex);
		float fogFactor = clamp(exp2(-uFogDensity * uFogDensity * dot(vVertex, vVertex) * 1.442695), 0, 1);
		color = mix(uFogColor, color, fogFactor);
		outColor = color;
		if (uIsParticle == 1)
		{
			outColor = mix(uFogColor, texture(uSampler, passTexCoords), fogFactor);
		}
		else if (uIsParticle == 2)
		{
			// GUI
			outColor = texture(uSampler, passTexCoords) * uDiffuseColor;
		};

		//float c = dot(passVTan, normalPolygonSpace);
		//outColor = vec4(passVTan+vec3(1.0, 1.0, 1.0)*vec3(0.5, 0.5, 0.5), 1.0);
	}
	else
	{
		float z = passFragCoord.z;
		outColor = vec4(z, z, z, 1.0);
	};
};
