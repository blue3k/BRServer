#!/bin/bash


SF_PATH=$(dirname "$0")

#https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/

#download deb package bundle
wget -qO - https://www.mongodb.org/static/pgp/server-4.4.asc | sudo apt-key add -

echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/4.4 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.4.list

sudo apt-get update

sudo apt-get install -y mongodb-org


#change data path and ip/port setting
#https://docs.mongodb.com/manual/reference/configuration-options/
sudo vi /etc/mongod.conf


