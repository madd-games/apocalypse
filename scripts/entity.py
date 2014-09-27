# entity.py
# Compiles entities.

import sys, os

from image import compileImage
from model import compileModel

def compileEntity(name, textureNames):
	cwd = os.getcwd()
	os.chdir("Game/Entities/%s" % name)
	compileModel("model.obj", "../../../cpptemp/mod_%s.cpp" % name, name, textureNames)
	os.chdir(cwd)
