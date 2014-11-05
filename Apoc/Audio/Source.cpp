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

#include <Apoc/Audio/Source.h>
#include <Apoc/Audio/Sounds.h>

Source::Source()
{
#ifdef ENABLE_OPENAL
	alGenSources(1, &source);
#endif
};

Source::~Source()
{
#ifdef ENABLE_OPENAL
	alDeleteSources(1, &source);
#endif
};

void Source::setPosition(Vector pos)
{
#ifdef ENABLE_OPENAL
	alSourcefv(source, AL_POSITION, &pos[0]);
#endif
};

void Source::setVelocity(Vector vel)
{
#ifdef ENABLE_OPENAL
	alSourcefv(source, AL_VELOCITY, &vel[0]);
#endif
};

void Source::setOrientation(Vector orient)
{
#ifdef ENABLE_OPENAL
	alSourcefv(source, AL_DIRECTION, &orient[0]);
#endif
};

void Source::attach(string sound)
{
#ifdef ENABLE_OPENAL
	alSourcei(source, AL_BUFFER, Sounds::Get(sound));
#endif
};
