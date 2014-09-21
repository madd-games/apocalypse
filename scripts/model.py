# model.py
# Compiles an OBJ model file into a C++ file that can be linked
# with Apocalypse.

inFileName = sys.argv[1]
outFileName = sys.argv[2]

vertices = []
texCoords = []

f = open(inFileName, "r")
