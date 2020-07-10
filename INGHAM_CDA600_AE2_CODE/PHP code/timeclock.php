<?php

	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 
	$uuid = htmlentities($_GET["uuid"]);

	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$stmt=mysqli_prepare($dbconnect,"SELECT user_ID FROM usertable where card_UUID=?");
	
	$stmt->bind_param('s',$uuid);
	$stmt->execute();
	$result = $stmt->get_result();
	$uRow = $result->fetch_array();
	if ($uRow == ""){
		echo "[Invalid Card]}";
	}else{
		$user_ID = $uRow['user_ID'];
		$stmt2=mysqli_prepare($dbconnect,"INSERT INTO clocktable (userID, timestamp) VALUES (?,?)");
		$timestamp =  date("Y-m-d h:i:sa");
		$stmt2->bind_param('ss', $user_ID, $timestamp);
		$stmt2->execute();
		echo "[Swipe logged]}";
	}
	?>