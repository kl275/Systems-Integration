<?php
session_start();
$servername = "zanzibar";
$username = "IT490";
$password = "coffee";
$dbname = "IT490";

// Create connection
$conn = new mysqli($servername, $username, $password);

// Check connection
	if ($conn->connect_error)
	{
		die("Connection failed: " . $conn->connect_error);
	} 
	echo "Connected successfully";
?>

