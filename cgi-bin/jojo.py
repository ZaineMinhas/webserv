import cgi

form = cgi.FieldStorage()

if "color" not in form:
	color = "whithe";
else:
	color = form['color'].value

print(color)