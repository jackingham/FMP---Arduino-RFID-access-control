<?php
session_start();
if ( !isset ($_SESSION["isAdmin"]))
{
	echo "This page is only for administrators";
}else{
$userID=htmlentities($_POST['userID']);

?>
	<form method="post" action="changescript.php">
	
	<label for="new_user">New UUID (can be blank): </label>
	<input type="text" name="new_UUID">
	
	<input type="hidden" name='userID' value='<?php echo $userID?>'>
	
	<input type="submit" value="Update UUID" />
	</form>
	
	<?php 
}