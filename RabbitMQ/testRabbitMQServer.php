#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
require_once('functionLib.inc');

function doLogin($email,$password)
{
    $db = new FunctionLib();    

    if(!$db->connect())
    {	
	return array("returnCode" => '1', 'message'=>"Error connecting to server");
    }


    $info = $db->getUserInfo($email, $password);
    
    if($info)
    {	
	return (array('returnCode' => '0', 'message' => 'Server received request and processed') + $info);
    }
    
    else
    {
	return array("returnCode" => '1', 'message'=>"Login unsuccessful");
    }

}

function doRegister($request)
{
    $db = new FunctionLib();
    
    if($db->registerUser($request['password'], $request['firstname'], $request['lastname'], $request['email']))
    {
	return array("returnCode" => '1', 'message'=>"Registration successful");
    }

    return array("returnCode" => '0', 'message'=>"<br>Registration unsuccessful<br>Username already exist!");
}

function logMessage($request)
{
	$logFile = fopen("log.txt", "a");

	fwrite($logFile, $request['message'] .'\n\n');

	return true;
}
function apiRequest($request)
{
	$client = new rabbitMQClient("apiRabbitMQ.ini","apiServer");
	$apiRe = array();
	$apiRe['type'] = "apiReq";
        $apiRe['drugName'] = $request['drugName'];
	
	$response = $client->send_request($apiRe);
  return $response;

}
      	


function requestProcessor($request)
{
  echo "Request Received".PHP_EOL;
  var_dump($request);
  echo '\n' . 'End Message';
  if(!isset($request['type']))
  {
    return "ERROR: unsupported message type";
  }
  switch ($request['type'])
  {
    case "login":
      return doLogin($request['email'],$request['password']);
    case "register":
      return doRegister($request);
    case "log":
      return logMessage($request);
   case "apiReq":
      return apiRequest($request);
  }
  return array("returnCode" => '0', 'message'=>"Server received request and processed");
}

$server = new rabbitMQServer("testRabbitMQ.ini","testServer");

$server->process_requests('requestProcessor');
exit();
?>
