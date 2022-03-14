<?php
	// 페이지의 제목을 출력한다.
	echo("<strong>게시판 쓰기</strong><p>");

	// <form> 태그로 내용을 입력받는다. 입력받은 내용은 메인 페이지로 POST한다.
	echo("
		<form name='inputform' action='Board_main.php' method='POST'>
			<textarea row=7 cols=50 name=string></textarea>
			<br><br>
			<input type='submit' value='등록'>
			<input type='reset' value='다시기입'>
		</form>
		");

	// 메인 페이지로 돌아가는 링크를 생성한다.
	echo("<br>
		<a href='Board_main.php'>되돌아 가기</a>
		");
?>
