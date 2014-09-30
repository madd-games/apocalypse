#! /usr/bin/python
# glsl_embed.py
# Embed a GLSL shader by creating an object file.

import sys, os

shaderName = sys.argv[1]
target = sys.argv[2]

print ">Embed shader %s" % shaderName

f = open("Shaders/%s.glsl" % shaderName, "r")
lines = f.read().splitlines()
f.close()

f = open("temp.cpp", "wb")
f.write("extern \"C\" const char *%s = \"\\\n" % shaderName)

for line in lines:
	f.write("%s\\n\\\n" % line.replace('"', '\\"'))

f.write("\";")
f.close()

os.system("g++ -c temp.cpp -o build-%s/glsl_%s.o -w" % (target, shaderName))

