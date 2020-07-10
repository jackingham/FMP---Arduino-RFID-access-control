<?php
session_start();
if ( !isset ($_SESSION["isAdmin"]))
{
	echo "This page is only for administrators";
}else{
$userID=htmlentities($_POST['userID']);

$dbusername = "user1";  
$dbpassword = "inghamcda600"; 
$server = "localhost"; 
$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
$dbselect = mysqli_select_db($dbconnect, "users");

$stmt = mysqli_prepare($dbconnect, "DELETE FROM usertable WHERE user_ID=?");
$stmt->bind_param('s', $userID);
$stmt->execute();

header ("Location: admin.php");
}
?>