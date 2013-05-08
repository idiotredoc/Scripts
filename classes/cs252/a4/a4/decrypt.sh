#!/bin/sh
key=`cat final`
echo $key
./crypto $key cipherfile.cipher plain.txt

