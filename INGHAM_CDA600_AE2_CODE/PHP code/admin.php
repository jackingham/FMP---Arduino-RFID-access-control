<?php
session_start();

if ( !isset ($_SESSION["isAdmin"]))
{
	echo "This page is only for administrators";
}else{
    
	echo "<h1>Admin page</h1>";
	echo "<h2>Add new user</h2>";
	?>
	<form method="post" action="adduser.php">
	
	<label for="new_user">User name: </label>
	<input type="text" name="new_user">
	
	<label for="new_UUID">Card UUID: </label>
	<input type="text" name="new_UUID"><br>
	
	<input type="submit" value="Add user" />
	</form>
	
	<?php
	
	echo "<h2> Current users </h2>";
	$dbusername = "user1";  
    $dbpassword = "inghamcda600"; 
    $server = "localhost"; 

	$dbconnect = mysqli_connect($server, $dbusername, $dbpassword);
    $dbselect = mysqli_select_db($dbconnect, "users");

	$sql="SELECT * FROM usertable";

	$records=mysqli_query($dbconnect,$sql);
	$json_array=array();
	
	while($row=mysqli_fetch_assoc($records))
	{
		echo "<p>";
		echo "Username: ". $row["user_name"] ."<br> ";
		echo "UUID: " . $row["card_UUID"] . "<br> " ; 
		echo "</p>";
		$id = $row["user_ID"];
		?>
		<form method="post" action="removeuser.php">
		<input type="submit" value="Remove user" />
		<input type="hidden" name='userID' value='<?php echo $id?>'>
		</form>
		
		<form method="post" action="changeUUID.php">
		<input type="submit" value="Change UUID" />
		<input type="hidden" name='userID' value='<?php echo $id?>'>
		</form>
		<?php
		echo "</p>";
		
	}
	
	
	
}
?>
