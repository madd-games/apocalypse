#! /usr/bin/python
# Convert a PNG file into an image file with Apocalypse texture data.

from PIL import Image
import sys, os

def compileImage(inFileName, outFileName, imageName, omit = False):
	img = Image.open(inFileName)
	width, height = img.size
	if omit:
		return (width, height)

	f = open(outFileName, "wb")
	for y in range(0, height):
		for x in range(0, width):
			pixel = img.getpixel((x, height-y-1))
			for n in range(0, 3):
				f.write(chr(pixel[n]))
			if len(pixel) == 3:
				f.write("\xFF")
			else:
				f.write(chr(pixel[3]))
	f.close()
	return (width, height)
