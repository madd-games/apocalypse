# material.py
# Parse MTL files.

import sys, os

matNameChars = "._abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

def parseMaterialFile(matfile, textureNames):
	f = open(matfile, "r")
	lines = f.read().splitlines()
	f.close()

	materials = {}
	currentMaterial = {}
	currentMaterialName = ""

	for line in lines:
		tokens = line.split(" ")
		cmd = tokens[0]

		if cmd == "newmtl":
			name = tokens[1]
			if currentMaterialName != "":
				materials[currentMaterialName] = currentMaterial
			for c in name:
				if c not in matNameChars:
					print "ERROR: Material name %s contains illegal character %s" % (name, c)
					sys.exit(1)
			currentMaterialName = name
			currentMaterial = {}
		elif cmd == "map_Kd":
			# We take this as the texture.
			name = os.path.abspath(" ".join(tokens[1:]))
			currentMaterial["texture"] = name
			textureNames.append(name)

	materials[currentMaterialName] = currentMaterial
	return materials
