import os
path = '.'
for filename in os.listdir(path):
	name, extension = filename.split(".")
	print(name, extension)
	if extension == "obj" or extension == "mtl":
	    new_filename = prefix + "_" + num + ".png"
	    os.rename(os.path.join(path, filename), os.path.join(path, new_filename))