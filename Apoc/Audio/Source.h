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

#ifndef APOC_AUDIO_SOURCE_H
#define APOC_AUDIO_SOURCE_H

#ifdef ENABLE_OPENAL
#include <AL/alut.h>
#endif

#include <Apoc/Math/Vector.h>

/**
 * \brief Represents a sound source.
 *
 * Those should be created by entities and delete inside entity destructors or at an earlier stage.
 * Sources do not need to be explicitly added to the world.
 */
class Source
{
private:
#ifdef ENABLE_OPENAL
	ALuint source;
#endif

public:
	/**
	 * \brief Constructor.
	 */
	Source();

	/**
	 * \brief Destructor.
	 */
	~Source();

	/**
	 * \brief Set the position of the source.
	 */
	void setPosition(Vector pos);

	/**
	 * \brief Set the velocity of the source.
	 *
	 * The position will NOT be automatically updated. This is only for adjusting the sound
	 * effect, which makes the player hear as though the source was moving.
	 */
	void setVelocity(Vector vel);

	/**
	 * \brief Set the orientation of the source.
	 */
	void setOrientation(Vector orient);

	/**
	 * \brief Attach a sound to this source.
	 */
	void attach(string sound);
};

#endif
