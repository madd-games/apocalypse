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
import pickle
from scripts.entity import compileEntity
from scripts.image import compileImage

target = "client"
if len(sys.argv) > 1:
	target = sys.argv[1]

compile_line = "g++ -c ${CPP_FILE} -o ${OBJECT_FILE} -D%s -I. -I/usr/local/include/SDL2 -D_REENTRANT" % (target.upper())
link_line = "g++ ${OBJECT_FILES} -o out/%s -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lpthread -lm -ldl -lrt -lGLEW -lGL" % target

files = []
def listfiles(dirname):
	for name in os.listdir(dirname):
		path = dirname + "/" + name
		if os.path.isdir(path):
			listfiles(path)
		elif path.endswith(".cpp"):
			files.append(path)

listfiles("Apoc")
if os.path.isdir("Game"):
	listfiles("Game")
else:
	print "!There is no Game directory! See the manual for more details."

if not os.path.exists("Game/GameImpl.h"):
	print "!Game/GameImpl.h does not exist, will not compile!"
	sys.exit(1)

os.system("mkdir -p build-%s" % target)
os.system("mkdir -p out")
os.system("mkdir -p cpptemp")

entityDiary = {}
try:
	f = open("build-%s/entity.diary" % target, "rb")
	entityDiary = pickle.load(f)
	f.close()
except:
	pass

textureNamesTemp = []
cleanTextureNames = {}
dirtyTextures = []
try:
	f = open("build-%s/texture.names" % target, "rb")
	cleanTextureNames = pickle.load(f)
	f.close()
except:
	pass

if os.path.isdir("Game/Entities"):
	for name in os.listdir("Game/Entities"):
		dirty = False
		newTable = {}
		for filename in os.listdir("Game/Entities/%s" % name):
			mtime = int(os.stat("Game/Entities/%s/%s" % (name, filename)).st_mtime)
			newTable[filename] = mtime
			if not entityDiary.has_key(name):
				dirty = True
			else:
				if not entityDiary[name].has_key(filename):
					dirty = True
				else:
					if entityDiary[name][filename] < mtime:
						dirty = True

		if not cleanTextureNames.has_key(name):
			dirty = True

		if dirty:
			print ">Compile entity %s" % name
			cleanTextureNames[name] = []
			compileEntity(name, cleanTextureNames[name])
			dirtyTextures.extend(cleanTextureNames[name])
			entityDiary[name] = newTable

for key, value in cleanTextureNames.items():
	textureNamesTemp.extend(value)

f = open("build-%s/entity.diary" % target, "wb")
pickle.dump(entityDiary, f)
f.close()

f = open("build-%s/texture.names" % target, "wb")
pickle.dump(cleanTextureNames, f)
f.close()

# Make a non-repetitive list of textures.
textureNames = []
for name in textureNamesTemp:
	if name not in textureNames:
		textureNames.append(name)

f = open("cpptemp/TextureList.cpp", "wb")
f.write("#include <Apoc/Entity/Texture.h>\n\n")
for i in range(0, len(textureNames)):
	f.write("extern const Texture::Texel imgTexels_%d[];\n" % i)
f.write("Texture::Map apocTextureMap[] = {\n")
i = 0
for name in textureNames:
	inFileName = name
	outFileName = "cpptemp/tex%d.cpp" % i
	if name in dirtyTextures:
		print ">Compile texture %s" % inFileName
	width, height = compileImage(inFileName, outFileName, str(i), name not in dirtyTextures)
	f.write("\t{\"%s\", %d, %d, imgTexels_%d},\n" % (name, width, height, i))
	i += 1
f.write("\t{NULL, 0, 0, NULL}\n")
f.write("};")
f.close()

listfiles("cpptemp")

f = open("build.rule", "rb")
rule = f.read()
f.close()

objectFiles = []
depFiles = []
rules = []

for filename in os.listdir("Shaders"):
	if filename.endswith(".glsl"):		# which it should
		shaderName = filename[:-5]
		objectFiles.append("build-%s/glsl_%s.o" % (target, shaderName))
		srule = "build-%s/glsl_%s.o: Shaders/%s\n" % (target, shaderName, filename)
		srule += "\t@python scripts/glsl_embed.py %s %s" % (shaderName, target)
		rules.append(srule)

def makeRule(cppfile):
	objfile = "build-%s/%s.o" % (target, cppfile.replace("/", "__")[:-4])
	depfile = objfile[:-2] + ".d"
	objectFiles.append(objfile)
	depFiles.append(depfile)

	out = rule
	out = out.replace("%DEPFILE%", depfile)
	out = out.replace("%OBJFILE%", objfile)
	out = out.replace("%CPPFILE%", cppfile)
	out = out.replace("%REPLACE%", cppfile.split("/")[-1][:-4]+".o")
	rules.append(out)

for filename in files:
	makeRule(filename)

f = open("build.mk", "wb")
f.write("CFLAGS=-w -D%s -I. -I/usr/local/include/SDL2 -D_REENTRANT\n" % target.upper())
f.write("LDFLAGS=-L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lpthread -lm -ldl -lrt -lGLEW -lGL\n")
f.write("DEPFILES=%s\n" % " ".join(depFiles))
f.write("OBJFILES=%s\n" % " ".join(objectFiles))
f.write("\n")
f.write(".PHONY: all\n")
f.write("all: out/%s\n" % target)
f.write("-include $(DEPFILES)\n")
f.write("\n")
f.write("out/%s: $(OBJFILES)\n" % target)
f.write("\t@echo \">Link $@\"\n")
f.write("\t@$(CXX) -o $@ $(OBJFILES) $(LDFLAGS)\n")
f.write("\n".join(rules))
f.close()

sys.exit(os.system("make -f build.mk"))
