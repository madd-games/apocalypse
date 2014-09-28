# model.py
# Compiles an OBJ model file into a C++ file that can be linked
# with Apocalypse.

from material import parseMaterialFile
import sys, os

def compileModel(inFileName, outFileName, modelName, textureNames):
	vertices = []
	texCoords = []
	normals = []

	# Basically, each vertex is a 3-tuple of 0-based indices into
	# the vertices, texCoords and normals lists, respectively.
	# It's not sorted into faces because we will draw in GL_TRIANGLES mode.
	currentObject = ""
	faces = []
	objects = {}
	materials = {}
	materialsToUse = {}

	f = open(inFileName, "r")
	lines = f.read().splitlines()
	f.close()

	# !!! WARNING !!! #
	# Unknown commands are currently ignored!!!
	# Be careful!!!
	# !!! WARNING !!! #

	nameChars = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	for line in lines:
		if not line.startswith('#'):
			tokens = line.split(" ")
			cmd = tokens[0]
			if cmd == "mtllib":
				materials = parseMaterialFile(" ".join(tokens[1:]), textureNames)
			elif cmd == "usemtl":
				materialsToUse[currentObject] = tokens[1]
			elif cmd == "o":
				name = " ".join(tokens[1:])
				if currentObject != "":
					objects[currentObject] = faces
				faces = []
				name = name.replace(".", "_")
				for c in name:
					if c not in nameChars:
						print "ERROR: Object %s contains invalid character in its name: %s" % (name, c)
						sys.exit(1)
				currentObject = name
			elif cmd == "v":
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
				for part in tokens[1:]:
					bits = part.split("/")
					vi = int(bits[0]) - 1
					ti = 0
					ni = 0
					try:
						ti = int(bits[1]) - 1
						ni = int(bits[2]) - 1
					except IndexError:
						pass
					faces.append((vi, ti, ni))

	objects[currentObject] = faces

	def writeToCPP(f, modelName, faces):

		f.write("\n")
		f.write("extern \"C\" Model::Vertex modVertices_%s[] = {\n" % modelName) 
		for vi, ti, ni in faces:
			vertex = vertices[vi]
			tex = (0.0, 0.0, 0.0)
			normal = (0.0, 0.0, 0.0)
			try:
				tex = texCoords[ti]
			except IndexError:
				pass
	
			try:
				normal = normals[ni]
			except IndexError:
				pass
	
			f.write("\t{\n")
			f.write("\t\tVector(%f, %f, %f, %f),\n" % vertex)
			f.write("\t\tVector(%f, %f, %f, 0.0),\n" % tex)
			f.write("\t\tVector(%f, %f, %f, 0.0)\n" % normal)
			f.write("\t},\n")
		f.write("};\n")
		f.write("extern \"C\" const int modCount_%s = %d;\n" % (modelName, len(faces)))

	f = open(outFileName, "wb")
	f.write("// Do not modify this file; it was automatically generated by scripts/model.py!\n")
	f.write("#include <Apoc/Entity/Model.h>\n")
	for objName, faces in objects.items():
		writeToCPP(f, modelName+"__"+objName, faces)

	f.write("Model::ObjDef modObjects_%s[] = {\n" % modelName)
	for objName in objects.keys():
		materialName = materialsToUse.get(objName, "")
		mat = materials.get(materialName, {})
		f.write("\t{\"%s\", modVertices_%s__%s, %d, NULL, \"%s\"},\n" % (objName, modelName, objName, len(objects[objName]),
			mat.get("texture", "empty_texture")))
	f.write("\t{NULL, 0}\n};")
	f.close()
