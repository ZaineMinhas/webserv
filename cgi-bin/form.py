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

print("""\
<!DOCTYPE html>
<html>
	<head>
		<meta charset='utf-8'/>
		<title>Les infos !</title>
		<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
		<style>
			body {
			background-color: rgb(246, 246, 246);
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			min-height: 100vh;
			}
			p { font-size: 2em; }
			img {
			z-index: -1;
			transform: rotate(26deg);
			min-width: 50vw;
			}
			.premier { margin-bottom: -25px; }
		</style>
	</head>
	<body>
		<p class="premier">Ta maison s'apelle %s</p>
		<p>Et ton pote est de forme %s</p>
		<img src='https://s.clipartkey.com/mpngs/s/222-2222639_ironicmeme-ironic-png-sunglasses-emoji-smileyface-cool-glasses.png'>
	</body>
</html>
""" % (nom, forme))