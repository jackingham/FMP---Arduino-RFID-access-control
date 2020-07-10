<?php
session_start();
if ( !isset ($_SESSION["isAdmin"]))
{
	echo "This page is only for administrators";
}else{

$newUser = htmlentities($_POST['new_user']);
$newUUID = htmlentities($_POST['new_UUID']);


$dbusername = "user1";  
$dbpassword = "inghamcda600"; 
$server = "localhost"; 
$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
$dbselect = mysqli_select_db($dbconnect, "users");

$stmt = mysqli_prepare($dbconnect, "INSERT INTO usertable(user_name, card_UUID) VALUES (?,?)");
$stmt->bind_param('ss', $newUser, $newUUID);
$stmt->execute();

header ("Location: admin.php");
}
	
?>