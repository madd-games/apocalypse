# kernel.py
# Embeds the OpenCL code as a string inside the Apocalypse binary
# when the OpenCL functionality is enabled.

import sys, os

# Does some pre-processing
class KernelProcessor:
	def __init__(self):
		self.imported = []
		apocMain = None
		gameMain = ""
		try:
			f = open("Apoc/Kernels/ApocMain.cl")
			apocMain = f.read()
			f.close()
		except IOError:
			print "Error: for some reason, Apoc/Kernels/ApocMain.cl could not be opened."
			sys.exit(1)

		try:
			f = open("Game/Kernels/Main.cl")
			gameMain = f.read()
			f.close()
		except IOError:
			pass

		self.finalOutput = "const char *kernelCode = \"\\\n"
		code = self.feed("ApocMain", apocMain) + self.feed("Main", gameMain)
		for line in code.splitlines():
			self.finalOutput += line.replace("\\", "\\\\").replace("\"", "\\\"") + "\\n\\\n"
		self.finalOutput += "\";"

	def feed(self, thismod, code):
		output = ""
		lines = code.splitlines()
		lineno = 0

		for line in lines:
			lineno += 1
			if line.startswith("#use "):
				modname = line[5:]
				if modname not in self.imported:
					f = None
					try:
						f = open("Apoc/Kernels/%s.cl" % modname, "rb")
					except IOError:
						try:
							f = open("Game/Kernels/%s.cl" % modname, "rb")
						except IOError:
							print "Kernel %s, line %d: module %s not found" % (thismod, lineno, modname)
							sys.exit(1)
					data = f.read()
					f.close()
					output += self.feed(modname, data)
					self.imported.append(modname)
			else:
				output += line + "\n"

		return output

def compileKernels(target):
	p = KernelProcessor()
	f = open("temp.cpp", "wb")
	f.write(p.finalOutput)
	f.close()
	os.system("g++ -c temp.cpp -o build-%s/kernels.o -w" % target)
