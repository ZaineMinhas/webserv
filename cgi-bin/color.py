import cgi

form = cgi.FieldStorage()

if "color" not in form:
	color = "whithe";
else:
	color = form['color'].value

print("""\
	<!DOCTYPE html>
	<html>
		<head>
			<meta charset="utf-8"/>
			<title>La couleur ...</title>
			<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
			<style>
				@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300&display=swap');
					body {
						font-family: 'Poppins', sans-serif;
						background-color: %s;
						min-height: 100vh;
						display: flex;
						align-items: center;
						justify-content: center;
						margin: auto;
					}
					div {
						background: rgba(255, 255, 255, 0.8);
						border-radius: 20px;
						width: 450px;
						padding: 35px;
						padding-bottom: 40px;
						text-align: center;
					}
					span {
						font-size: 2em;
						position: relative;
						bottom: 2vh;
					}
					a {
						border: 3px rgba(255, 255, 255, 0.9) solid;
						border-radius: 100px;
						background-color: rgba(255, 255, 255, 0.9);
						color: black;
						width: 15em;
						height: 2em;
						margin-bottom: 10px;
						transition: background-color 0.3s ease-out;
						transition: border-color 0.1s ease-out;
						transition: color 0.1s ease-out;
						text-decoration: none;
						position: relative;
						top: 1vh;
						padding: 2px;
					}		
					a:hover {
						border: 3px %s80 solid;
						background: %s80;
						color: black;
						cursor: pointer;
					}
					color {
						color: %s;
					}
			</style>
		</head>
		<body>
			<div>
				<span>Your color: <color>%s</color></span><br/>
				<a href="/color_cgi.html">Try an other color ?</a>
				<a class="home" href="/">Go back to home</a>
			</div>
		</body>
	</html>
""" % (color, color, color, color, color))