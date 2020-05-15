import os
path = '.'
for filename in os.listdir(path):
	name, extension = filename.split(".")
	if extension == "obj" or extension == "mtl":
		num = name[8:]
		newFilename = num + "kong" + "." + extension
		print(newFilename)
		os.rename(os.path.join(path, filename), os.path.join(path, newFilename))