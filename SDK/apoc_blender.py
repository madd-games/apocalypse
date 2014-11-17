# Apocalypse add-on for Blender, allows exporting of APM files and stuff.

bl_info = {
    "name": "Apocalypse Model (APM) format",
    "author": "Madd Games",
    "blender": (2, 69, 0),
    "location": "File > Import-Export",
    "description": "Export models for use with the Apocalypse Engine.",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "support": 'OFFICIAL',			# unsure about this one.
    "category": "Import-Export"}

DEBUG = True

import bpy
import struct
import sys, os
import json
from PIL import Image

from bpy.props import (StringProperty,
                       FloatProperty,
                       IntProperty,
                       BoolProperty,
                       EnumProperty,
                       )
from bpy_extras.io_utils import (ImportHelper,
                                 ExportHelper,
                                 axis_conversion,
                                 )

nextTexIndex = 1
texIndices = {}

def getTextureIndex(filepath):
	global nextTexIndex
	if not os.path.exists(filepath):
		return 0
	if filepath not in texIndices:
		texIndices[filepath] = nextTexIndex
		nextTexIndex += 1
	return texIndices[filepath]

class ExportAPM(bpy.types.Operator, ExportHelper):
	bl_idname = "export_mesh.apm"
	bl_label = "Export APM"

	filename_ext = ".apm"
	filter_glob = StringProperty(
		default="*.apm",
		options={'HIDDEN'}
	)

	def execute(self, context):
		scene = bpy.context.scene
		filename = self.filepath
		objlist = list(bpy.data.objects)

		f = open(filename, "wb")

		# Write the APM header
		f.write(bytes("APM\0", "ascii"))
		# Report sizeof(Vertex) as 48, so that u-tangents and v-tangents are skipped as they are
		# auto-computed.
		# Also, zeros will be put in place of the texture table offset and size for now, we'll change
		# it later on in this script when we know the values!
		f.write(struct.pack("IIIII", 24, len(objlist), 48, 0, 0))

		for obj in objlist:
			f.write(bytes(obj.name, "utf-8"))
			f.write(bytes("\0", "ascii"))

			mesh = obj.to_mesh(scene, True, 'PREVIEW', calc_tessface=True)
			mesh.calc_normals_split()
			mesh.transform(obj.matrix_world)

			uvdata = None
			try:
				mesh.tessface_uv_textures.active.data[0]
				uvdata = mesh.tessface_uv_textures.active.data
			except:
				pass

			vertices = []
			uvs = []
			normals = []
			for face in mesh.tessfaces:
				if len(face.vertices) < 3:
					# Ignore faces that can't even make triangles.
					continue
				else:
					if len(face.vertices) == 4:
						vertices.extend((face.vertices[0], face.vertices[1], face.vertices[2]))
						vertices.extend((face.vertices[2], face.vertices[3], face.vertices[0]))
						normals.extend([face.normal]*6)

						if uvdata is not None:
							i = face.index
							uvs.extend((uvdata[i].uv1, uvdata[i].uv2, uvdata[i].uv3))
							uvs.extend((uvdata[i].uv3, uvdata[i].uv4, uvdata[i].uv1))
					else:
						vertices.extend(face.vertices)
						normals.extend([face.normal]*3)
						if uvdata is not None:
							i = face.index
							uvs.extend((uvdata[i].uv1, uvdata[i].uv2, uvdata[i].uv3))

			mat = obj.active_material
			diffuseColor = (255, 255, 255, 255)
			specularColor = (0, 0, 0, 255)
			shininess = 0.0
			if mat is not None:
				diffuseColor = (int(mat.diffuse_color[0]*255.0), int(mat.diffuse_color[1]*255.0), int(mat.diffuse_color[2]*255.0), 255)
				specularColor = (int(mat.specular_color[0]*mat.specular_intensity*255.0), int(mat.specular_color[1]*mat.specular_intensity*255.0), int(mat.specular_color[2]*mat.specular_intensity*255.0), 255)
				shininess = (mat.specular_hardness - 1) * 1.9607843137254901

			colorIndex = 0
			specularIndex = 0
			normalIndex = 0
			illumIndex = 0
			warpIndex = 0
			try:
				filepath = obj.active_material.active_texture.image.filepath
				colorIndex = getTextureIndex(filepath)
				if filepath.endswith(".png"):
					basename = filepath[:-4]
					specularIndex = getTextureIndex(basename+"_spec.png")
					normalIndex = getTextureIndex(basename+"_normal.png")
					illumIndex = getTextureIndex(basename+"_illum.png")
					warpIndex = getTextureIndex(basename+"_warp.png")
			except:
				pass

			f.write(struct.pack("I", len(vertices)))		# number of vertices
			f.write(struct.pack("I", colorIndex))			# index of the color texture
			f.write(struct.pack("I", specularIndex))		# index of the specular texture
			f.write(struct.pack("I", normalIndex))			# index of the normal map
			f.write(struct.pack("BBBB", *diffuseColor))		# the diffuse color
			f.write(struct.pack("BBBB", *specularColor))		# the specular color
			f.write(struct.pack("f", shininess))			# the specular exponent
			f.write(struct.pack("I", 40))				# size of this structure
			f.write(struct.pack("I", illumIndex))			# index of the illumination map
			f.write(struct.pack("I", warpIndex))			# index of the warp map

			for i, verti in enumerate(vertices):
				uv = (0.0, 0.0)
				if uvdata is not None:
					uv = (uvs[i][0], uvs[i][1])

				# Swap Y and Z to make it agree with Apocalypse space.
				vert = (mesh.vertices[verti].co[0], mesh.vertices[verti].co[2], mesh.vertices[verti].co[1], 1.0,
					uv[0], uv[1], 0.0, 0.0,	 # UV
					normals[i][0], normals[i][2], normals[i][1], 0.0)
				f.write(struct.pack("f"*12, *vert))

		# OK, now we know where we shall write the texture table, so update the
		# APM header.
		ttOffset = f.tell()
		f.seek(16)
		f.write(struct.pack("II", ttOffset, len(texIndices)))
		f.seek(ttOffset)

		# So yeah, write the texture table.
		for filepath, index in texIndices.items():
			texflags = 0
			if filepath.endswith(".png"):
				try:
					jf = open(filepath[:-4]+".json", "r")
					data = json.load(jf)
					jf.close()
					if data["allowMipmaps"] == "false":
						texflags = texflags | 1
				except:
					pass

			img = Image.open(filepath)
			width, height = img.size
			f.write(struct.pack("IIIH", index, width, height, texflags))

			for y in range(0, height):
				for x in range(0, width):
					pixel = img.getpixel((x, height-y-1))
					if len(pixel) == 3:
						pixel = (pixel[0], pixel[1], pixel[2], 255)
					f.write(struct.pack("BBBB", *pixel))

		f.close()
		return {'FINISHED'}

def menu_func_export(self, context):
	self.layout.operator(ExportAPM.bl_idname, text="Apocalypse Model (.apm)")

def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_export.append(menu_func_export)

if __name__ == "__main__":
	register()
