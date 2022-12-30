<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8"/>
		<title>Test</title>
		<?php
			$bg = array('pizza-back2.jpg', 'pizza-back3.jpg', 'shrekpizza.jpg', 'volcano.jpg');
			$i = rand(0, count($bg)-1);
			$selectedBg = "$bg[$i]";
		?>
		<style>
			body { background-image: url("./images/<?php echo $selectedBg; ?>"); }
		</style>
	</head>
	<body>
		<div>voila les background</div>
	</body>
</html>