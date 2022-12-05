#!/usr/bin/python)

import cgi

form = cgi.FieldStorage()
nom_du_pote		= form.getvalue('nom_du_pote')
forme_de_maison	= form.getvalue('forme_de_maison')

print ("<html>")
print ("<head>")
print ("<title>Les infos !</title>")
print ("</head>")
print ("<body>")
print ("<p>Ta maison s'apelle %s</p>" % (nom_du_pote,))
print ("<p>Et ton pote est %s</p>" % (forme_de_maison,))
print ("</body>")
print ("</html>")