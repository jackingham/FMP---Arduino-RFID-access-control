<?php

	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 
	$uuid = htmlentities($_GET["uuid"]);
	$hash = htmlentities($_GET["hash"]);
	
	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$stmt=mysqli_prepare($dbconnect,"SELECT hashed_pin FROM usertable where card_UUID=?");
	
	$stmt->bind_param('s',$uuid);
	$stmt->execute();
	$result = $stmt->get_result();
	$hpRow = $result->fetch_array();
	$dbhash = $hpRow["hashed_pin"];
	
	if ($dbhash == $hash){
	echo "[Access granted]}";
	}else{
	echo "[Access denied]}";
	}
?>