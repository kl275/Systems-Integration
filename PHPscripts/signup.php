<?php
	session_start();
	require_once('databaseConnector.php');

	$name = $_POST["user"];
	$password = $_POST["password"];
	$email = $_POST["email"];

	/using MD5 for security before inserting into database.
	$password = md5($password);

	$statement = "INSERT INTO IT490 (name, email, password) VALUES ('$name, $email, $password')";
	// use exec() because no results are returned
	if ($conn->exec($statement) )
	{
		echo "User account successfully made!"
	}
?>

