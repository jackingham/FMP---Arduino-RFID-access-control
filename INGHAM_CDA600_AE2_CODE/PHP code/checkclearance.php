<?php

	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 
	$uuid = htmlentities($_GET["uuid"]);
	$req_clearance = htmlentities($_GET["clearance"]);

	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$stmt=mysqli_prepare($dbconnect,"SELECT user_ID, clearance_level FROM usertable where card_UUID=?");
	
	$stmt->bind_param('s',$uuid);
	$stmt->execute();
	$result = $stmt->get_result();
	$uRow = $result->fetch_array();
	if ($uRow == ""){
		echo "[Access denied]}";
	}else{
		$user_ID = $uRow['user_ID'];
		$user_clearance = $uRow['clearance_level'];
		if ($user_clearance >= $req_clearance){
			$stmt2=mysqli_prepare($dbconnect,"INSERT INTO door1 (user_ID, timestamp) VALUES (?,?)");
			$timestamp =  date("Y-m-d h:i:sa");
			$stmt2->bind_param('ss', $user_ID, $timestamp);
			$stmt2->execute();
			echo "[Access granted]}";
		} else {
			echo "[Access denied]}";
			}
			
	}
	
	?>