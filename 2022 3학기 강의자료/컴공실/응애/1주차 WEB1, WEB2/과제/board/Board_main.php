<?php
	// POST 받은 문자열이 있으면 이를 파일에 반영한다.
	$string = $_POST[string];
	if($string) {
		$filename = "Data.txt";
		$fp = fopen($filename, "w");
		fwrite($fp, $string, strlen($string));
		fclose($fp);
	}

	// 게시판 쓰기, 읽기 페이지로 링크를 생성한다.
	echo("
			<a href='Board_write.php'>게시판 쓰기</a><br>
			<a href='Board_read.php'>게시판 읽기</a>"
		);
?>
