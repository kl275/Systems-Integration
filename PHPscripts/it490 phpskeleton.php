<?php
	session_start();
	require_once('databaseConnector.php');

	$name = $_POST["name"];
	$email = $_POST["email"];
	$password = $_POST["password"];
?>