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

#ifndef APOC_VIDEO_SHADER_ARRAY_H
#define APOC_VIDEO_SHADER_ARRAY_H

#include <Apoc/Video/OpenGL.h>
#include <vector>
#include <map>

using namespace std;

/**
 * \brief Manages arrays for GLSL shaders.
 * \param T Type of elements.
 * 
 * This class essentially manages an OpenGL buffer texture. The contents of such textures
 * are either instances of Vector, or structures of instances of Vector.
 * The array is unordered. When an element is deleted for example, the indices of elements
 * that came after it do not change; and an object created later may reclaim its index.
 * This is used for storing lists of lights for example.
 */
template<class T>
class ShaderArray
{
private:
	GLuint vbo;
	GLuint tex;
	GLuint texUnit;
	vector<T> cpuBuffer;
	
	// Maps element keys to indices into the cpuBuffer vector.
	map<int, unsigned int> elementMap;
	
	int getFirstFreeKey()
	{
		int x = 0;
		while (elementMap.count(x) != 0) x++;
		return x;
	};
	
	void refreshBuffer()
	{
		glBindBuffer(GL_TEXTURE_BUFFER, vbo);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(T)*cpuBuffer.size(), &cpuBuffer[0], GL_DYNAMIC_DRAW);
	};
	
public:
	ShaderArray(GLuint texUnit) : texUnit(texUnit)
	{
		glActiveTexture(texUnit);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_BUFFER, tex);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_TEXTURE_BUFFER, vbo);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo);
	};
	
	~ShaderArray()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteTextures(1, &tex);
	};
	
	/**
	 * \brief Add a new element.
	 * \param val The initial value of this element.
	 * 
	 * \return A key with which you may later access this element.
	 */
	int add(T val)
	{
		int key = getFirstFreeKey();
		unsigned int index = cpuBuffer.size();
		elementMap[key] = index;
		cpuBuffer.push_back(val);
		refreshBuffer();
		return key;
	};
	
	/**
	 * \brief Remove an element.
	 * \param key The element key.
	 */
	void remove(int key)
	{
		if (elementMap.count(key) == 0) return;
		
		unsigned int index = elementMap[key];
		cpuBuffer.erase(cpuBuffer.begin() + index);
		
		// All elements with indices above index must now be moved down.
		map<int, unsigned int>::iterator it;
		for (it=elementMap.begin(); it!=elementMap.end(); ++it)
		{
			if (it->second >= index)
			{
				it->second--;
			};
		};
		
		// Remove the key from the map.
		elementMap.erase(key);
		
		// Refresh the GPU buffer.
		refreshBuffer();
	};
	
	/**
	 * \brief Update the value of an element.
	 * \param key The element key.
	 * \param val The new value for this element.
	 */
	void set(int key, T val)
	{
		if (elementMap.count(key) == 0) return;
		
		unsigned int index = elementMap[key];
		cpuBuffer[index] = val;
		
		// Change just this element on the GPU.
		glBindBuffer(GL_TEXTURE_BUFFER, vbo);
		glBufferSubData(GL_TEXTURE_BUFFER, sizeof(T)*index, sizeof(T), &cpuBuffer[index]);
	};
	
	/**
	 * \brief Bind this array to the current active texture unit.
	 * 
	 * Please note that if you created the texture while the texture unit was active,
	 * and have not bound or created other arrays to this texture unit afterwards, then
	 * there is no need to re-bind this array to that unit, even if you update the array
	 * contents.
	 */
	void bind()
	{
		glBindTexture(GL_TEXTURE_BUFFER, tex);
	};

	/**
	 * \brief Return the number of elements in this array.
	 */
	int count()
	{
		return (int) cpuBuffer.size();
	};
};

#endif
