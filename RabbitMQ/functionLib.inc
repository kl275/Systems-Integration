<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');


class FunctionLib
{
	private $conn;
	public function connect()
	{
		try
		{
		
			$this->conn = new PDO('mysql:host=localhost;dbname=IT490', 'IT490', 'geronimo');

			return true;
		}
		catch(PDOException $e)
		{
			$message =  'Error: ' . $e->getMessage();
			$client = new rabbitMQClient("testRabbitMQ.ini","testServer");      
			$request = array();
			$request['type'] = "log";
			$request['message'] = $message;
			$response = $client->publish($request);
		}
		
		return false;
	}


	public function getUserInfo($email, $password)
	{
		$stmt = $this->conn->prepare('SELECT Fname, Lname, id FROM IT490 WHERE email = :em AND password = :pw');
		$stmt->bindParam(':em', $email);
		$stmt->bindParam(':pw', $password);
		$stmt->setFetchMode(PDO::FETCH_ASSOC);
		$stmt->execute();
		
		$result = $stmt->fetch();
		echo "User id: " . $result['id'];		
		return $result;
	}
	
// Registers the User if the username 
// does not already exist in the database.

	public function registerUser($password, $firstname, $lastname, $email)
	{

		$database = mysql_connect ('localhost', 'IT490', 'geronimo');
		mysql_select_db('IT490');
		$query = ("SELECT * FROM IT490 where email = '$email'");
		$t = mysql_query($query);
		if(mysql_num_rows($t) > 0)
		{
			echo "Email already exist. Please use a different email.";
			return false;
		}
 		else
		{
			
			$this->conn = new PDO('mysql:host=localhost;dbname=IT490', 'IT490', 'geronimo');
			$stmt = $this->conn->prepare("INSERT INTO registered (Fname, Lname, email, password, id) VALUES (:fname, :lname, :em, :pw, NULL)");
			$stmt->bindParam('fname', $firstname);
			$stmt->bindParam('lname', $lastname);
			$stmt->bindParam(':em', $email);
			$stmt->bindParam(':pw', $password);

			return $stmt->execute();
			return true;
		}
	}
	
// Adds entered drug into the users database
// Rejects if the drug is already in the User's database
 
	public function addUsrDrug($genericName, $productDesc, $recallReportDate, $reason, $brandName, $status, $refLink, $username, $userId)
	{


			
		 	$database = mysql_connect ('localhost', 'root', 'password');
                        mysql_select_db('login');
                        $query = ("SELECT * FROM drugInfo where genericName = '$genericName'");
                        $t = mysql_query($query);
                        if(mysql_num_rows($t) > 0)
        		        {
                        		echo "Drug In Database";
                    
               			}

			elseif($genericName == NULL)
				{
					echo "Error!";
					return true;
				}
			else
			{
				echo $genericName;
				echo $productDesc;


				$this->conn = new PDO('mysql:host=localhost;dbname=IT490', 'IT490', 'geronimo');
       				$stmt = $this->conn->prepare("INSERT INTO drugInfo (genericName, productDesc, recallReportDate, recallData, brandName, status, refLink, drugId) VALUES (:gn, :pd, :rrd, :rd, :bn, :st, :rl, NULL)");
                        	$stmt->bindParam(':gn', $genericName);
                        	$stmt->bindParam(':pd', $productDesc);
				$stmt->bindParam(':rrd', $recallReportDate);
                        	$stmt->bindParam(':rd', $reason);
				$stmt->bindParam(':bn', $brandName);
                        	$stmt->bindParam(':st', $status);
				$stmt->bindParam(':rl', $refLink);
				$stmt->execute();

			}	
		
			
                        $getDrugId = ("SELECT drugId from drugInfo where genericName = '$genericName'");
			$queryResult = mysql_query($getDrugId);
			$row = mysql_fetch_row($queryResult);
			$drugId = $row[0];
			echo "drug ID: ".$drugId."\n";
		
			$checkUserDrugs = ("SELECT * FROM userDrugs where userId = '$userId' AND drugId = '$drugId'");
			$checkUDResults = mysql_query($checkUserDrugs);
			
			if(mysql_num_rows($checkUDResults) > 0)
               			 {
                        		echo "\n"."Username already has drug listed"."\n";
                        		return false; //0
               			 }
			else
				 {	
					$insertUserDrug = ("INSERT INTO userDrugs (username, userId, drugId) VALUES ('$username','$userId','$drugId')");
					mysql_query($insertUserDrug);
				 }
         
			
			return true;
	}
	

// Grabs, from an intermedite table, userId and drugId and pulls data from
// drugInfo and registered tables

	public function getUserDrugs($id)
	{
		
		$drugsArray = array();

		$this->conn = new PDO('mysql:host=localhost;dbname=IT490', 'IT490', 'geronimo');
		$stmt = $this->conn->prepare("select * from userDrugs INNER JOIN drugInfo ON userDrugs.drugId = drugInfo.drugId INNER JOIN registered ON userDrugs.userId = registered.id WHERE userDrugs.userId = '$id'");
                $stmt->setFetchMode(PDO::FETCH_ASSOC);
                $stmt->execute();

                $result = $stmt->fetchAll();
                
		$jsonResult = json_encode($result);
		// echo "User id: " . $result['id'];
		return $jsonResult;
	}


// If the status of the drug changes update the database (in progress)

	public function checkStatus($id)
	{
			$database = mysql_connect ('localhost', 'root', 'password');
                        mysql_select_db('IT490');
                        $query = ("SELECT status FROM drugInfo where id = '$id'");
                        $t = mysql_query($query);

	}

	public function getUserNotifications($id)
	{
		$this->conn = new PDO('mysql:host=localhost;dbname=IT490', 'IT490', 'geronimo');
                $stmt = $this->conn->prepare("select * from userDrugs INNER JOIN drugInfo ON userDrugs.drugId = drugInfo.drugId INNER JOIN registered ON userDrugs.userId = registered.id WHERE userDrugs.userId = '$id' AND drugInfo.status = 'ongoing'");
                $stmt->setFetchMode(PDO::FETCH_ASSOC);
                $stmt->execute();

                $result = $stmt->fetchAll();
		$jsonResult = json_encode($result);
		echo $jsonResult;
                return $jsonResult;
	
	}
	public function __destruct()
	{
		$this->conn = null;
	}
}

?>
