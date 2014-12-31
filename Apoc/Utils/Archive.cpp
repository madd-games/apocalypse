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

#include <Apoc/Utils/Archive.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

vector<Archive*> apocPath;

int Archive::octToInt(const char *oct)
{
	int out = 0;

	while (*oct != 0)
	{
		out = out * 8 + ((*oct++)-'0');
	};

	return out;
};

Archive::Archive(string filename) : ifs(filename.c_str(), ios::binary)
{
	TarHeader head;

	if (ifs.fail())
	{
		cerr << "[APOC] [ARC] Could not open " << filename << "; ignoring." << endl;
		return;
	};

	while (!ifs.eof())
	{
		ifs.read((char*)&head, 512);
		if (memcmp(head.ustar, "ustar", 5) != 0)
		{
			break;
		};

		// ignore things that are not files (e.g. directories).
		if ((head.type != 0) && (head.type != '0'))
		{
			continue;
		};

		string filename(head.filename);
		arcFiles[filename].offset = ifs.tellg();
		arcFiles[filename].size = (size_t) octToInt(head.size);

		size_t sectors = arcFiles[filename].size / 512;
		if ((arcFiles[filename].size % 512) != 0) sectors++;
		ifs.ignore(sectors*512);
	};

	ifs.clear();
};

Archive::~Archive()
{
	ifs.close();
};

bool Archive::hasFile(string filename)
{
	return arcFiles.count(filename) != 0;
};

size_t Archive::getFileSize(string filename)
{
	if (arcFiles.count(filename) == 0) return 0;
	return arcFiles[filename].size;
};

void Archive::read(string filename, void *buffer, size_t size)
{
	size_t fileSize = getFileSize(filename);
	if (size > fileSize) size = fileSize;
	if (size == 0) return;

	ifs.seekg(arcFiles[filename].offset);
	ifs.read((char*)buffer, size);
	ifs.clear();
};

void ApocAddToPath(string tarFile)
{
	apocPath.push_back(new Archive(tarFile));
};

//
// DataFile
//

DataFile::DataFile(string filename) : filename(filename)
{
	vector<Archive*>::iterator it;
	for (it=apocPath.begin(); it!=apocPath.end(); ++it)
	{
		if ((*it)->hasFile(filename))
		{
			size = (*it)->getFileSize(filename);
			arc = *it;
			return;
		};
	};

	stringstream ss;
	ss << "Cannot find data file in path: " << filename << endl;
	ApocFail(ss.str());
};

size_t DataFile::getSize()
{
	return size;
};

void DataFile::read(void *buffer, size_t size)
{
	arc->read(filename, buffer, size);
};
