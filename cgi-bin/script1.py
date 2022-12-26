import cgi

form = cgi.FieldStorage()

nom = form['nom_du_pote'].value
forme = form['forme_de_maison'].value

if nom == "": #ca marche pas
    nom = "rien du tout"
if forme == "": #ca marche pas
    forme = "rien du tout"

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<meta charset='utf-8'/>")
print("<title>Les infos !</title>")
print("<style>")
print("body {")
print("background-color: rgba(246, 246, 246);")
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