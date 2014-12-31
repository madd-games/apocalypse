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

#ifndef APOC_UTILS_ARCHIVE_H
#define APOC_UTILS_ARCHIVE_H

#include <map>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <Apoc/Utils/Utils.h>

using namespace std;

/**
 * \brief A class for handling TAR archives.
 */
class Archive
{
private:
	int octToInt(const char *oct);

	struct TarHeader
	{
		char		filename[100];
		char		mode[8];
		char		owner_uid[8];
		char		group_uid[8];
		char		size[12];
		char		mtime[12];
		char		checksum[8];
		char		type;
		char		link_name[100];
		char		ustar[5];		// the string "ustar"
		char		ustar_garb[3];		// ignore this!
		char		owner_uname[32];
		char		owner_gname[32];
		char		dev_major[8];
		char		dev_minor[8];
		char		prefix[155];
		char		pad[12];		// make the size of this 512
	};

	// used to cache the positions of files inside the archive.
	struct FileDesc
	{
		off_t offset;
		size_t size;
	};

	map<string, FileDesc> arcFiles;
	ifstream ifs;

public:
	/**
	 * \brief Constructor, open an archive.
	 * \param filename The path to the archive.
	 *
	 * If the file does not exist or is not a valid TAR archive, then no file contents are
	 * read and the instance represents an empty archive.
	 */
	Archive(string filename);

	/**
	 * \brief Destructor, close the archive.
	 */
	virtual ~Archive();

	/**
	 * \brief Returns true if the specified file is in this archive.
	 * \param filename The name of the file within the archive.
	 */
	bool hasFile(string filename);

	/**
	 * \brief Get the size of a file.
	 * \param filename The name of the file within the archive.
	 */
	size_t getFileSize(string filename);

	/**
	 * \brief Load file contents into a buffer.
	 * \param filename The name of the file within the archive.
	 * \param buffer The buffer to store file contents.
	 * \param size The buffer size.
	 *
	 * If the buffer is bigger than the file, then the file contents are loaded
	 * into the first <i>size</i> bytes of the buffer, and the rest is untouched.
	 * If the file is bigger than the buffer, then only the first <i>size</i> bytes
	 * of the file are read, and the buffer is filled.
	 */
	void read(string filename, void *buffer, size_t size);
};

/**
 * \brief A class for reading files from the standard path.
 */
class DataFile
{
private:
	Archive *arc;
	size_t size;
	string filename;

public:
	/**
	 * \brief Constructor.
	 * \param filename Name of the file.
	 *
	 * ApocFail() is raised if the file cannot be found in the path.
	 */
	DataFile(string filename);

	/**
	 * \brief Get the size of the file.
	 */
	size_t getSize();

	/**
	 * \brief Read the file contents into a buffer.
	 * \param buffer The buffer to store the file contents.
	 * \param size Size of the buffer.
	 */
	void read(void *buffer, size_t size);
};
	
void ApocAddToPath(string tarFile);

#endif
