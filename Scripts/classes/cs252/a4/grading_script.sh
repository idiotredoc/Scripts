#! /bin/sh
#:
if test $# -eq 0
then
     echo "error, usage: grading_script.sh <login> "
     exit 0;
fi

# set the random file environmnet.
echo '>>>' grading .... $1
cp /home/cs472/randomfile .
RANDFILE=randomfile
export RANDFILE 
#
echo .... unziping all submitted files
echo "unzip $1A4Files.zip"
unzip $1A4Files.zip
#
echo .... converting symPasswd.base64 to symPasswd.cipher
echo "openssl enc -base64 -d -in symPasswd.base64 -out symPasswd.cipher "
openssl enc -base64 -d -in symPasswd.base64 -out symPasswd.cipher

#
echo .... get the SYMMYTRIC  passwd
echo "openssl rsautl -decrypt -inkey cs472PrivateKey.pem -in symPasswd.cipher -out  symPasswd.txt"
openssl rsautl -decrypt -inkey cs472PrivateKey.pem -in symPasswd.cipher -out  symPasswd.txt
echo '>>>' "$user" passwd is .....  :  `cat symPasswd.txt`
#
echo .... decrypt A4.zip
echo "openssl enc -des3  -d -salt -a -out A4.zip -in A4Zip.base64"
openssl enc -des3  -d -salt -a -out A4.zip -in A4Zip.base64
#
echo  ... verify the signature of the MD of A4.zip
echo "openssl enc -base64  -d -in A4ZipMdSignature.base64 -out A4ZipMdSignature.cipher"
openssl enc -base64  -d -in A4ZipMdSignature.base64 -out A4ZipMdSignature.cipher
echo "openssl dgst -sha1 -verify $1PublicKey.pem -signature  A4ZipMdSignature.cipher A4.zip"
openssl dgst -sha1 -verify $1PublicKey.pem -signature  A4ZipMdSignature.cipher A4.zip
#
echo ... unziping A4 files
echo unzip A4.zip
unzip A4.zip
echo ....done
