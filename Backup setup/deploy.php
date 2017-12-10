#!/usr/bin/php
<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');
$client = new rabbitMQClient("deployRabbitServer.ini","deployServer");
$request = array();
$request['type'] = "deploy";
$request['package'] = "BE";
$request['tier'] = "qa";
$request['packageName'] = "backendPackage-v";
$response = $client->send_request($request);
print_r($response);

echo "\n";
?>
