<?php
session_start();

$pw = htmlentities($_POST["password"]);

if($pw== "admin1") {
	$_SESSION["isAdmin"]= "1";
	header ("Location: admin.php");

} else {
	echo "Invalid login.<br>";
	echo "<a href=loginpage.html>Click here to go back to the login page</a>";
}  

?>


