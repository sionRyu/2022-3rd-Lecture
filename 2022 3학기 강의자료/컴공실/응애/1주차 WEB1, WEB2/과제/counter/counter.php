<?php
	// 카운트를 기록할 파일 이름
	$filename = "counter.txt";

	// 기존 카운트를 읽어온다. 
	$fp = fopen($filename, "r");
	$count = fread($fp, filesize($filename));
	fclose($fp);

	// 쿠키를 확인해서 카운트를 증가시킨다.
	if(!$_COOKIE["ip"]) {
		setcookie("ip", $_SERVER["REMOTE_ADDR"]);
		$count = $count+1;
	}

	// 그림파일을 이용해 숫자를 한 자리씩 출력한다.
	echo("방문자 수: ");
	$string = (string)$count;
	for($i = 0; $i < strlen($string); $i++) {
		$ch = substr($string, $i, 1);
		if('0' <= $ch && $ch <= '9') {
			$tag = "<img src = \"./img/".$ch.".JPG\" alt=".$ch.">";
			echo($tag);
		}
	}

	// 새로운 카운트를 파일에 기록한다.
	$fp = fopen($filename, "w");
	fwrite($fp, (string)$count, strlen($count));
	fclose($fp);
?>
