echo "Packaging Bundle..."

packageNum=`./testRabbitMQClient.php backendPackage-v | xargs`
	cd temp/
	echo "MySQL Password?"
		mkdir mySqlData
			cd mySqlData/
				mysqldump -u root -p login > backup_sql.sql
				cd ..
				cd ..
	
	beScrDIR=/home/it490/Systems-Intergration/RabbitMQ
		cp -a $beScrDIR* temp/

	tar -czvf backendPackage-v"$packageNum".tar.gz -C temp/ . 
		
		rm -r temp/RabbitMQ/
		rm -r temp/mySqlData/
        echo `ls | grep backendPackage-v"$packageNum"`
		echo "Bundle Complete"
	
	# Scp package to the deploy server
	echo "Sending to deploy server..."
	scp -r backendPackage-v"$packageNum".tar.gz sam@192.168.1.13:/home/Sam/rabbitmqphp_example/Deploy_Server/packages/
	
	cp backendPackage-v"$packageNum".tar.gz backendPackages/
	rm backendPackage-v"$packageNum".tar.gz
	php updatePackage.php backendPackage-v"$packageNum".tar.gz "$packageNum"

