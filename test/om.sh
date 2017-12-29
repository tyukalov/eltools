#!/bin/sh
COMMAND=../bin/om
if [ $1="makesrc" ]
then
    COMMAND=bin/om
fi

echo =======================
echo Positive test
echo =======================
$COMMAND -d -u=20 -r=10
$COMMAND -d -u=20 -i=10
$COMMAND -d -r=20 -i=10
$COMMAND  -u=20 -r=10
$COMMAND  -u=20 -i=10
$COMMAND  -r=20 -i=10
$COMMAND -p -u=20 -r=10
$COMMAND -p -u=20 -i=10
$COMMAND -p -r=20 -i=10
$COMMAND  -u=20 -r=10 -x=10
$COMMAND  -u=20 -i=10 -x=10
$COMMAND  -r=20 -i=10 -x=10
$COMMAND -p -u=20 -r=10 -x=10
$COMMAND -p -u=20 -i=10 -x=10
$COMMAND -p -r=20 -i=10 -x=10
