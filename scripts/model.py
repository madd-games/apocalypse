# model.py
# Compiles an OBJ model file into a C++ file that can be linked
# with Apocalypse.

import sys, os

inFileName = sys.argv[1]
outFileName = sys.argv[2]

vertices = []
texCoords = []
normals = []

# Basically, each faces is a 3-list of 3-tuples 0-based indices into
# the vertices, texCoords and normals lists, respectively.
faces = []

f = open(inFileName, "r")
lines = f.read().splitlines()
f.close()

# !!! WARNING !!! #
# Unknown commands are currently ignored!!!
# Be careful!!!
# !!! WARNING !!! #

for line in lines:
	if not line.startswith('#'):
		tokens = line.split(" ")
		cmd = tokens[0]
		if cmd == "v":
			x = float(tokens[1])
			y = float(tokens[2])
			z = float(tokens[3])
			w = 1.0
			try:
				w = float(tokens[4])
			except IndexError:
				pass
			vertices.append((x, y, z, w))
		elif cmd == "vt":
			u = float(tokens[1])
			v = float(tokens[2])
			w = 0.0
			try:
				w = float(tokens[3])
			except IndexError:
				pass
			texCoords.append((u, v, w))
		elif cmd == "vn":
			x = float(tokens[1])
			y = float(tokens[2])
			z = float(tokens[3])
			normals.append((x, y, z))
		elif cmd == "f":
			if (len(tokens) != 4):
				print "ERROR: All faces must be triangles!"
				sys.exit(1)
			theFace = []
			for part in tokens[1:]:
				bits = part.split("/")
				vi = int(bits[0]) - 1
				ti = 0
				ni = 0
				try:
					ti = int(bits[1]) - 1
					vi = int(bits[2]) - 1
				except IndexError:
					pass
				theFace.append((vi, ti, ni))
			faces.append(theFace)

