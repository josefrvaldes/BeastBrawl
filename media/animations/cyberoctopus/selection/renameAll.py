import os
path = '.'
contadorObj = 1
contadorMtl = 1
files = os.listdir(path)
files.sort()
#print(files)


for filename in files:
	name, extension = filename.split(".")
	if extension == "obj" or extension == "mtl":
		nameWithoutNumericPart = name[0:len(name) - 6]
		if extension == "obj":
			newName = nameWithoutNumericPart + str(contadorObj).zfill(6) + "." + extension
			contadorObj+=1
		else:
			newName = nameWithoutNumericPart + str(contadorMtl).zfill(6) + "." + extension
			contadorMtl+=1
		print(newName)
		os.rename(os.path.join(path, filename), os.path.join(path, newName))