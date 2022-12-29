import cgi, os

path = "./website/site/uploads/"

form = cgi.FieldStorage()
fi = form['filename']

if fi.filename:
	if not os.path.exists(path):
		os.makedirs(path)
	fn = os.path.basename(fi.filename)
	open(path + fn, 'wb').write(fi.file.read())
	print("""\
	<!DOCTYPE html>
	<html>
		<head>
			<meta charset="utf-8"/>
			<title>Uploaded!</title>
			<style>
				@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300&display=swap');
				body {
					font-family: 'Poppins', sans-serif;
					background: linear-gradient(to right, blue, magenta);
					min-height: 100vh;
					display: flex;
					align-items: center;
					justify-content: center;
					margin: auto;
				}
				div {
					background: rgba(2, 255, 23, 0.5);
					border-radius: 20px;
					width: 450px;
					padding: 3%;
					text-align: center;
				}
				span {
					font-size: 2em;
					position: relative;
					bottom: 2vh;
				}
				a {
					border: 3px rgb(119, 178, 85) solid;
					border-radius: 100px;
					background-color: rgb(119, 178, 85);
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
					padding: 0.5%;
				}
				a:hover {
					border: 3px rgba(2, 255, 23, 0.5) solid;
					background: rgba(2, 255, 23, 0.5);
					color: black;
					cursor: pointer;
				}
				.home { top: 2.5vh; }
			</style>
		</head>
		<body>
			<div>
				<span>Image has been succesfully uploaded !</span><br/>
				<a href="/upload_cgi.html">Upload a other image ?</a>
				<a href="/uploads">See all images</a><br/>
				<a class="home" href="/">Go back to home</a>
			</div>
		</body>
	</html>
	""")
else:
	print("""\
	<!DOCTYPE html>
	<html>
		<head>
			<meta charset="utf-8"/>
			<title>Uploaded!</title>
			<style>
				@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300&display=swap');
				body {
					font-family: 'Poppins', sans-serif;
					background: linear-gradient(to right, blue, magenta);
					min-height: 100vh;
					display: flex;
					align-items: center;
					justify-content: center;
					margin: auto;
					color: whitesmoke;
				}
				div {
					background: rgba(255, 19, 19, 0.5);
					border-radius: 20px;
					width: 450px;
					padding: 3%;
					text-align: center;
				}
				span {
					font-size: 2em;
					position: relative;
					bottom: 2vh;
				}
				a {
					border: 3px rgb(255, 136, 0) solid;
					border-radius: 100px;
					background-color: rgb(255, 136, 0);
					color: whitesmoke;
					width: 15em;
					height: 2em;
					margin-bottom: 10px;
					transition: background-color 0.3s ease-out;
					transition: border-color 0.1s ease-out;
					transition: color 0.1s ease-out;
					text-decoration: none;
					position: relative;
					top: 1vh;
					padding: 0.5%;
				}
				a:hover {
					border: 3px rgba(255, 0, 0, 0.6) solid;
					background: rgba(255, 0, 0, 0.6);
					cursor: pointer;
				}
				.home { top: 2.5vh; }
			</style>
		</head>
		<body>
			<div>
				<span>Image has been succesfully not uploaded !</span><br/>
				<a href="./upload_cgi.html">Upload a image ?</a>
				<a href="/uploads">See all images</a><br/>
				<a class="home" href="/">Go back to home</a>
			</div>
		</body>
	</html>
	""")
