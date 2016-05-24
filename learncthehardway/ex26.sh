#!/bin/bash
set -e

cd /tmp
curl -L -O http://archive.apache.org/dist/apr/apr-1.4.6.tar.gz
tar -xzvf apr-1.4.6.tar.gz
cd apr-1.4.6

./configure
make
sudo make install

cd /tmp
rm -rf apr-1.4.6 apr-1.4.6.tar.gz

curl -L -O http://archive.apache.org/dist/apr/apr-util-1.4.1.tar.gz
tar -xzvf apr-util-1.4.1.tar.gz
cd apr-util-1.4.1

./configure --with-apr=/usr/local/apr
make
sudo make install

cd /tmp
rm -rf apr-util-1.4.1* apr-1.4.6*
