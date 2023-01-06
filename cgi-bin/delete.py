import cgi, os

form = cgi.FieldStorage()

if "file" in form:
	if os.path.exists("./website/site/uploads/" + form['file'].value):
		os.remove("./website/site/uploads/" + form['file'].value)
		print("""\
		<!DOCTYPE html>
		<html>
			<head>
				<meta charset="utf-8"/>
				<title>Deleted!</title>
				<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
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
						padding: 4%;
						padding-bottom: 5%;
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
					<span>Image has been succesfully deleted !</span><br/>
					<a href="/delete.php">Delete an other image ?</a>
					<a href="/images.php">See all images</a><br/>
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
				<title>Not deleted!</title>
				<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
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
						padding: 4%;
						padding-bottom: 5%;
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
					<span>Image has been succesfully not deleted !</span><br/>
					<a href="./delete.php">Delete an image ?</a>
					<a href="/images.php">See all images</a><br/>
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
			<title>Not deleted!</title>
			<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
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
					padding: 4%;
					padding-bottom: 5%;
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
				<span>Image has been succesfully not deleted !</span><br/>
				<a href="./delete.php">Delete an image ?</a>
				<a href="/images.php">See all images</a><br/>
				<a class="home" href="/">Go back to home</a>
			</div>
		</body>
	</html>
	""")