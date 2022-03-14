<?php
	// 파일을 열어 기록된 내용을 읽는다.
	$filename = "Data.txt";
	$fp = fopen($filename, "r");
	$data = fread($fp, filesize($filename));
	fclose($fp);

	// 페이지의 제목을 출력한다.
	echo("<strong>게시판 읽기</strong><p>");

	// 1x1 테이블 형태로 내용을 표시한다.
	echo("
		<table border=1 width=500 height=50%>
			<tr>
				<td>".$data."</td>
			</tr>
		</table><p>
		");

	// 메인 화면으로 돌아가는 링크를 생성한다.
	echo("
		<a href='Board_main.php'>되돌아 가기</a>
		");
?>
