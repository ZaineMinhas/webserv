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
						echo "<a href='/delete.py?file=$file'>$file</a><br/>";
				}
				else
					echo "there is no images";
			?>
	</body>
</html>