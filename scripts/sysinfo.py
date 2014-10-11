# Detect some system paramters.

import sys, os
import pickle

def checkForOpenCL():
	print ">Checking if an OpenCL SDK is available"
	f = open("temp.cpp", "wb")
	f.write("#ifdef __APPLE__\n")
	f.write("#include <OpenCL/cl.h>\n")
	f.write("#else\n")
	f.write("#include <CL/cl.h>\n")
	f.write("#endif\n")
	f.close()

	opencl_enable = ((os.system("g++ -c temp.cpp -o temp.o") == 0) and ("--no-opencl" not in sys.argv))
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

	if not sysinfo.has_key("opencl_enable"):
		sysinfo["opencl_enable"] = checkForOpenCL()

	f = open("build-%s/sys.info" % target, "wb")
	pickle.dump(sysinfo, f)
	f.close()

	return sysinfo
