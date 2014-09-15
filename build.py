#! /usr/bin/python
# Build the engine and the game.

"""
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
"""

import sys, os

target = "client"
if len(sys.argv) > 1:
	target = sys.argv[1]

compile_line = "g++ -c ${CPP_FILE} -o ${OBJECT_FILE} -D%s -I." % (target.upper())

files = []
def listfiles(dirname):
	for name in os.listdir(dirname):
		path = dirname + "/" + name
		if os.path.isdir(path):
			listfiles(path)
		else if path.endswith(".cpp"):
			files.append(path)

listfiles("Apoc")

os.system("mkdir -p build-%s" % target)

diary = {}
try:
	f = open("build-%s/diary" % target, "rb")
	data = f.read().split("\n")
	f.close()
	
	for line in data:
		try:
			filename, stamp = line.rsplit(" ", 1)
			diary[filename] = int(stamp)
		except:
			pass
except:
	pass

filesToCompile = []
for filename in files:
	if not diary.has_key(filename):
		filesToCompile.append(filename)
	else:
		stamp = int(os.stat(filename).st_time)
		if diary[filename] != stamp:
			filesToCompiler.append(filename)

for filename in filesToCompile:
	stamp = int(os.stat(filename).st_time)
	cmd = compile_line
	cmd = cmd.replace("${CPP_FILE}", filename)
	objfile = "build-%s/" % target + filename[:-4].replace("/", "__") + ".o"
	cmd = cmd.replace("${OBJECT_FILE}", objfile)
	print ">Compile " + filename
	if os.system(cmd) != 0:
		print "!Compilation of " + filename + " failed"
	diary[filename] = stamp

try:
	f = open("build-%s/diary" % target, "wb")
	for name, stamp in diary.items():
		f.write("%s %d\n" % (name, stamp))
	f.close()
except:
	pass

print "The project is now up-to-date."
