import cgi

form = cgi.FieldStorage()

nom_du_pote		= form["nom_du_pote"].value
forme_de_maison	= form["forme_de_la_maison"].value

print("<html>")
print("<head>")
print("<title>Les infos !</title>")
print("</head>")
print("<body>")
print("<p>Ta maison s'apelle %s</p>" % (nom_du_pote,))
print("<p>Et ton pote est %s</p>" % (forme_de_maison,))
print("</body>")
print("</html>")