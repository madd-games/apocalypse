# Detect some system paramters.

import sys, os
import pickle

system_suffix = ">/dev/null 2>&1"
if sys.platform.startswith("win"):
        system_suffix = "2>NUL"

def getCompiler(sysinfo):
	print ">Deciding what C++ compiler to use"
	compiler = "g++"
	sysinfo["ar"] = "ar"
	sysinfo["ranlib"] = "ranlib"
	for token in sys.argv:
		if token.startswith("--host="):
			host = token[7:]
			compiler = host + "-g++"
			sysinfo["ar"] = host + "-ar"
			sysinfo["ranlib"] = host + "-ranlib"
	print "--Chosen compiler: " + compiler
	return compiler

def checkWindows(cpp_compiler):
	print ">Checking if the host is Windows"
	f = open("temp.cpp", "wb")
	f.write("#ifndef _WIN32\n")
	f.write("#error not windows\n")
	f.write("#endif\n")
	f.close()

	if os.system("%s -c temp.cpp -o temp.o %s" % (cpp_compiler, system_suffix)) == 0:
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

	opencl_enable = ((os.system("%s -c temp.cpp -o temp.o %s" % (cpp_compiler, system_suffix)) == 0) and ("--enable-cl" in sys.argv))
	if opencl_enable:
		print "--OpenCL enabled"
	else:
		print "--!OpenCL disabled!"
	return opencl_enable

def checkForOpenAL(cpp_compiler):
	print ">Checking if OpenAL is available"
	f = open("temp.cpp", "wb")
	f.write("#include <AL/alut.h>")
	f.close()
	openal_enable = ((os.system("%s -c temp.cpp -o temp.o %s" % (cpp_compiler, system_suffix)) == 0) and ("--disable-al" not in sys.argv))
	if openal_enable:
		print "--OpenAL enabled"
	else:
		print "--!OpenAL disabled!"
	return openal_enable

def getSystemRoot():
	# The Windows corss-compilation system root.
	# Must be explicitly specified on native Windows builds!
	for par in sys.argv:
		if par.startswith("--sysroot="):
			return par[len("--sysroot="):]

	if sys.platform.startswith("win"):
		print "!You must specify the --sysroot= option on native Windows builds!"
		sys.exit(1)
	else:
		return "/usr/i686-w64-mingw32"

def loadSystemInfo(target):
	sysinfo = {}
	try:
		f = open("build-%s/sys.info" % target, "rb")
		sysinfo = pickle.load(f)
		f.close()
	except:
		pass

	if not (sysinfo.has_key("cpp_compiler") and sysinfo.has_key("ar") and sysinfo.has_key("ranlib")):
		sysinfo["cpp_compiler"] = getCompiler(sysinfo)

	if not sysinfo.has_key("is_windows"):
		sysinfo["is_windows"] = checkWindows(sysinfo["cpp_compiler"])

	if not sysinfo.has_key("opencl_enable"):
		sysinfo["opencl_enable"] = checkForOpenCL(sysinfo["cpp_compiler"])

	if not sysinfo.has_key("openal_enable"):
		sysinfo["openal_enable"] = checkForOpenAL(sysinfo["cpp_compiler"])

	if not sysinfo.has_key("sysroot"):
		sysinfo["sysroot"] = getSystemRoot()

	f = open("build-%s/sys.info" % target, "wb")
	pickle.dump(sysinfo, f)
	f.close()

	return sysinfo
