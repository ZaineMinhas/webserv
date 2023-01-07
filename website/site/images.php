<html>
	<head>
		<meta charset="utf-8"/>
		<title>Delete cgi</title>
		<link rel="stylesheet" href="./stylesheet/images.css"/>
		<link rel="shortcut icon" type="image/x-icon" href="/images/favicon.ico"/>
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
			if (dir_is_empty($_SERVER['UPLOAD_DIR']))
			{
				$files = scandir($_SERVER['UPLOAD_DIR']);
				foreach ($files as $file)
				{
					if (!($file == "." || $file == ".."))
					{
						$file = urlencode($file);
						echo "<a href='./uploads/$file'><img src='./uploads/$file'/></a>";
					}
				}
			}
			else
				echo "<div>There is no images.</div>";
		?>
		<a class="webserv" href="./index.html">Webserv</a>
	</body>
</html>