#! /bin/sh

echo "Enter var a:"
read a
echo "Enter var b:"
read b

if [ -z "$a" ] & [ -z "$b" ]
    then
        echo "Non value about a and b"
        echo "\nPlease enter a and b, try again!"
        exit 0
elif [ $a -eq $b ]
    then
        echo "var a equal var b"
elif [ $a -gt $b ]
    then
        echo "var a great var b"
else
    echo "var a less var b"
exit 0
fi









