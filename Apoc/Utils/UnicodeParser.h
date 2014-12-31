/*
	Copyright (c) 2014-2015, Madd Games.
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

#ifndef APOC_UTILS_UNICODE_PARSER_H
#define APOC_UTILS_UNICODE_PARSER_H

#include <string>

using namespace std;

/**
 * \brief Parses UTF-8 strings.
 *
 * Initialise instances of this class on the stack only, and use it only within the scope
 * of a function. Upon exiting a function scope, the state of a UnicodeParser object is
 * undefined.
 */
class UnicodeParser
{
private:
	const char *data;

public:
	/**
	 * \brief Constructor.
	 *
	 * Initialise this parser with a string.
	 */
	UnicodeParser(string str);

	/**
	 * \brief Read the next character in the UTF-8 sequence.
	 * \return Returns the Unicode codepoint.
	 */
	unsigned long next();

	/**
	 * \brief Returns true if this is the end of the string.
	 */
	bool end();
};

#endif
