<!DOCTYPE html>

<html>
	<head>
		<meta charset="utf-8"/>
		<title>Delete cgi</title>
		<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
		<link rel="stylesheet" href="./stylesheet/delete.css"/>
	</head>
	<body>
		<?php
			function dir_is_empty($dirname)
			{
				if (!is_dir($dirname))
					return false;
				$res = scandir($dirname);
				if ($res === false)
					return false;
				return (!(count($res) == 2));
			}
			if (dir_is_empty('./website/site/uploads'))
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