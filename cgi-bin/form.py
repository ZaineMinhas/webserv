import cgi

form = cgi.FieldStorage()

if "nom_du_pote" not in form:
	nom = "rien du tout"
else:
	nom = form['nom_du_pote'].value

if "forme_de_maison" not in form:
	forme = "rien du tout"
else:
	forme = form['forme_de_maison'].value

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<meta charset='utf-8'/>")
print("<title>Les infos !</title>")
print("<style>")
print("body {")
print("background-color: rgb(246, 246, 246);")
print("display: flex;")
print("flex-direction: column;")
print("align-items: center;")
print("justify-content: center;")
print("min-height: 100vh;")
print("}")
print("img {")
print("justify-content: right;")
print("min-width: 50vw;")
print("}")
print("</style>")
print("</head>")
print("<body>")
print("<p>Ta maison s'apelle %s</p>" % (nom))
print("<p>Et ton pote est de forme %s</p>" % (forme))
print("<img src='https://s.clipartkey.com/mpngs/s/222-2222639_ironicmeme-ironic-png-sunglasses-emoji-smileyface-cool-glasses.png'>")
print("</body>")
print("</html>")