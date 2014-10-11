# Detect some system paramters.

import sys, os
import pickle

def getCompiler():
	print ">Deciding what C++ compiler to use"
	compiler = "g++"
	for token in sys.argv:
		if token.startswith("--host="):
			host = token[7:]
			compiler = host + "-g++"
	print "--Chosen compiler: " + compiler
	return compiler

def checkWindows(cpp_compiler):
	print ">Checking if the host is Windows"
	f = open("temp.cpp", "wb")
	f.write("#ifndef _WIN32\n")
	f.write("#error not windows\n")
	f.write("#endif\n")
	f.close()

	if os.system("%s -c temp.cpp -o temp.o >/dev/null 2>&1" % cpp_compiler) == 0:
		print "--Detected Windows"
		return True
	else:
		print "--Not Windows"
		return False

def checkForOpenCL(cpp_compiler):
	print ">Checking if an OpenCL SDK is available"
	f = open("temp.cpp", "wb")
	f.write("#ifdef __APPLE__\n")
	f.write("#include <OpenCL/cl.h>\n")
	f.write("#else\n")
	f.write("#include <CL/cl.h>\n")
	f.write("#endif\n")
	f.close()

	opencl_enable = ((os.system("%s -c temp.cpp -o temp.o >/dev/null 2>&1" % cpp_compiler) == 0) and ("--no-cl" not in sys.argv))
	if opencl_enable:
		print "--OpenCL enabled"
	else:
		print "--!OpenCL disabled!"
	return opencl_enable

def loadSystemInfo(target):
	sysinfo = {}
	try:
		f = open("build-%s/sys.info" % target, "rb")
		sysinfo = pickle.load(f)
		f.close()
	except:
		pass

	if not sysinfo.has_key("cpp_compiler"):
		sysinfo["cpp_compiler"] = getCompiler()

	if not sysinfo.has_key("is_windows"):
		sysinfo["is_windows"] = checkWindows(sysinfo["cpp_compiler"])

	if not sysinfo.has_key("opencl_enable"):
		sysinfo["opencl_enable"] = checkForOpenCL(sysinfo["cpp_compiler"])

	f = open("build-%s/sys.info" % target, "wb")
	pickle.dump(sysinfo, f)
	f.close()

	return sysinfo
