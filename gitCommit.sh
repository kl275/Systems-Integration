#!/bin/bash

echo "What's the commit message?";
read message;

git add *;
git commit -m "case 
$message";
git push;
