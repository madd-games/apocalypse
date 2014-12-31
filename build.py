#! /usr/bin/python
# Build the engine and the game.

"""
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
"""

import sys, os
import pickle
from scripts.entity import compileEntity
from scripts.image import compileImage
from scripts.sysinfo import loadSystemInfo
from scripts.kernel import compileKernels
import json

# Figure out which OS we are building on.
# NOTE: Don't bother porting deprecated stuff to Windows unless we really have to.
system = "linux"
if sys.platform.startswith("win"):
        system = "windows"

target = "client"
if len(sys.argv) > 1:
	target = sys.argv[1]
if "--install" in sys.argv:
	target = "sdk"

def makeDir(name):
        if system == "linux":
                os.system("mkdir -p %s" % name)
        else:
                os.system("MD " + name.replace("/", "\\") + " 2>NUL")

makeDir("build-%s" % target)
makeDir("out")
makeDir("cpptemp")
makeDir("gdata")

objectFiles = []

sysinfo = loadSystemInfo(target)
ldopencl = ""
ccopencl = ""
if sysinfo["opencl_enable"]:
	ldopencl = "-lOpenCL"
	ccopencl = "-DENABLE_OPENCL"
	print ">Assemble kernels"
	compileKernels(target, sysinfo["cpp_compiler"])
	objectFiles.append("build-%s/kernels.o" % target)

files = []
def listfiles(dirname):
	for name in os.listdir(dirname):
		path = dirname + "/" + name
		if os.path.isdir(path):
			listfiles(path)
		elif path.endswith(".cpp"):
			files.append(path)

makeDir("Game")
if not os.path.exists("Game/GameImpl.h"):
	f = open("Game/GameImpl.h", "wb")
	f.write("#ifndef GAME_IMPL_H\n")
	f.write("#define GAME_IMPL_H\n\n")
	f.write("#include <Apoc/Game.h>\n\n")
	f.write("class GameImpl : public Game\n")
	f.write("{\npublic:\n\t\n};\n")
	f.write("\n#endif")
	f.close()

if "--with-sdk" not in sys.argv:
	listfiles("Apoc")
if "--install" not in sys.argv:
	listfiles("Game")

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
		if os.path.exists("Game/Entities/%s/model.obj" % name):
			print "!Entity '%s' uses a deprecated format!" % name
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

if os.path.isdir("Game/Images"):
	for name in os.listdir("Game/Images"):
		fullname = "Game/Images/"+name
		#if not cleanTextureNames.has_key(fullname):
		cleanTextureNames[fullname] = [fullname]
		dirtyTextures.append(fullname)

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
expectedContents = []
for name in textureNames:
	inFileName = name
	basename = os.path.basename(inFileName)
	if "." in basename:
		basename = basename.rsplit(".", 1)[0]
	outFileName = "gdata/tex%d_%s" % (i, basename)
	if (name in dirtyTextures) or not os.path.exists(outFileName):
		print ">Compile texture %s" % inFileName
	width, height = compileImage(inFileName, outFileName, str(i), (name not in dirtyTextures) and os.path.exists(outFileName))
	allowMipmaps = "true"
	if inFileName.startswith("Game/Images"):
		allowMipmaps = "false"
	else:
		jsonfilename = inFileName.rsplit(".", 1)[0] + ".json"
		if os.path.exists(jsonfilename):
			print ">Reading %s" % jsonfilename
			jf = open(jsonfilename, "rb")
			options = json.load(jf)
			jf.close()
			allowMipmaps = options.get("allowMipmaps", allowMipmaps)
	f.write("\t{\"%s\", %d, %d, \"gdata/tex%d_%s\", %s},\n" % (name, width, height, i, basename, allowMipmaps))
	expectedContents.append("tex%d_%s" % (i, basename))
	i += 1
f.write("\t{NULL, 0, 0, NULL}\n")
f.write("};")
f.close()

for filename in os.listdir("gdata"):
	if filename not in expectedContents:
		print ">Delete gdata/%s" % filename
		os.system("rm gdata/%s" % filename)

makeDir("Game/Sounds")
f = open("cpptemp/SoundList.cpp", "wb")
f.write("#include <stdlib.h>\n\n")
f.write("const char *apocSoundList[] = {\n")
for name in os.listdir("Game/Sounds"):
	f.write("\t\"Game/Sounds/%s\",\n" % name)
	f.write("\tNULL\n")
f.write("};")
f.close()

if "--install" not in sys.argv:
	listfiles("cpptemp")

rulefile = "linux.rule"
if sys.platform.startswith("win"):
	rulefile = "windows.rule"
f = open(rulefile, "rb")
rule = f.read()
f.close()

depFiles = []
rules = []

if "--with-sdk" not in sys.argv:
	for filename in os.listdir("Shaders"):
		if filename.endswith(".glsl"):		# which it should
			shaderName = filename[:-5]
			objectFiles.append("build-%s/glsl_%s.o" % (target, shaderName))
			srule = "build-%s/glsl_%s.o: Shaders/%s\n" % (target, shaderName, filename)
			srule += "\t@python scripts/glsl_embed.py $(CXX) %s %s" % (shaderName, target)
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

libdirs = "-L" + sysinfo["sysroot"] + "/lib"
incdirs = "-I" + sysinfo["sysroot"] + "/include/SDL2"

if sysinfo["is_windows"]:
	sdl_ldflags="-static " + libdirs + " -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -lglew -lopengl32"
	sdl_cflags=incdirs + " -DGLEW_STATIC"
else:
	sdl_ldflags="-L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lpthread -lm -ldl -lrt  -lGLEW -lGL"
	sdl_cflags="-I/usr/include/SDL2 -D_REENTRANT"

if sysinfo["openal_enable"]:
	sdl_cflags += " -DENABLE_OPENAL"
	sdl_ldflags += " -lalut -lopenal"

exe = ""
if sysinfo["is_windows"]:
	exe = ".exe"

ldapoc = ""
if "--with-sdk" in sys.argv:
	ldapoc = "-lapocalypse"

ccapoc = ""
if "--with-sdk" in sys.argv:
	ccapoc = "-I/usr/src/madd-games/apoc"

f = open("build.mk", "wb")
f.write("CXX=%s\n" % sysinfo["cpp_compiler"])
f.write("CFLAGS=-D_USE_MATH_DEFINES -w -D%s -I. %s -ggdb %s -std=c++11 %s\n" % (target.upper(), sdl_cflags, ccopencl, ccapoc))
f.write("LDFLAGS=%s %s %s\n" % (ldapoc, sdl_ldflags, ldopencl))
f.write("DEPFILES=%s\n" % " ".join(depFiles))
f.write("OBJFILES=%s\n" % " ".join(objectFiles))
f.write("\n")
f.write(".PHONY: all\n")
f.write("all: out/%s%s\n" % (target, exe))
f.write("-include $(DEPFILES)\n")
f.write("\n")
f.write("out/%s%s: $(OBJFILES)\n" % (target, exe))
if sys.platform.startswith("win"):
	f.write("\t@echo ^>Link $@\n")
else:
	f.write("\t@echo \">Link $@\"\n")
f.write("\t@$(CXX) -o $@ $(OBJFILES) $(LDFLAGS)\n")
f.write("\n".join(rules))
f.write("out/libapocalypse.a: $(OBJFILES)\n")
if sysinfo["is_windows"]:
	f.write("\t@echo ^>Bundle out/libapocalypse.a\n")
else:
	f.write("\t@echo \">Bundle out/libapocalypse.a\"\n")
f.write("\t@%s rs out/libapocalypse.a $(OBJFILES)\n" % sysinfo["ar"])
f.write("\t@%s out/libapocalypse.a\n" % sysinfo["ranlib"])
f.close()

makeDir("Game/Resources")
makeDir("Game/Models")
print ">Package data as out/%s.tar" % target
if os.system("tar -cf out/%s.tar gdata Game/Sounds Game/Resources Game/Models" % target) != 0:
	print "!BUILD FAILED!"
	sys.exit(1)
if "--no-make" not in sys.argv:
	make = "make"
	if sys.platform.startswith("win"):
		make = "mingw32-make"
	target = "all"
	if "--install" in sys.argv:
		target = "out/libapocalypse.a"
	status = os.system("%s -f build.mk %s" % (make, target))
	if status != 0:
		print "!Build failed!"
		sys.exit(status)
	if "--install" in sys.argv:
		print ">Install the SDK"
		if os.system("cp out/libapocalypse.a /usr/lib/") != 0:
			print "!Installation failed!"
			sys.exit(1)
		if os.system("cd SDK/unix && sh install-sdk") != 0:
			print "!Installation failed!"
			sys.exit(1)
		print "Installation complete."
