import cgi, os

path = "./website/site/uploads/"

if not os.path.exists(path):
	os.makedirs(path)

form = cgi.FieldStorage()
fi = form["filename"]

if fi.filename:
	fn = os.path.basename(fi.filename)
	open(path + fn, 'wb').write(fi.file.read())
	print("Uploaded!")
else:
	print("Failed!")