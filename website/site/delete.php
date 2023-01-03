<!DOCTYPE html>

<html>
	<head>
		<meta charset="utf-8"/>
		<title>Delete cgi</title>
		<link rel="stylesheet" href="./stylesheet/delete.css"/>
	</head>
	<body>
		<?php
				if (is_dir('./website/site/uploads'))
				{
					$files = scandir('./website/site/uploads');
					foreach ($files as $file)
						if (!($file == "." || $file == ".."))
							echo "<a href='/delete.py?file=$file'><img src='./uploads/$file'/></a><br/>";
				}
				else
					echo "<div>There is no images.</div>";
		?>
		<a class="webserv" href="./index.html">Webserv</a>
	</body>
</html>