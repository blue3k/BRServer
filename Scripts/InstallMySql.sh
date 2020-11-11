#!/bin/bash


SF_PATH=$(dirname "$0")

#download deb package bundle
wget https://dev.mysql.com/get/Downloads/MySQL-8.0/mysql-server_8.0.21-1ubuntu20.04_amd64.deb-bundle.tar 
mkdir mysql_bundle
tar -xvf mysql-server_8.0.21-1ubuntu20.04_amd64.deb-bundle.tar -C mysql_bundle

cd mysql_bundle

#install
sudo apt install libmecab2
sudo dpkg -i mysql-common_8.0.21-1ubuntu20.04_amd64.deb
sudo dpkg-preconfigure mysql-community-server_8.0.21-1ubuntu20.04_amd64.deb
sudo dpkg -i mysql-community-client-core_8.0.21-1ubuntu20.04_amd64.deb mysql-community-client_8.0.21-1ubuntu20.04_amd64.deb mysql-client_8.0.21-1ubuntu20.04_amd64.deb  
sudo dpkg -i mysql-community-server-core_8.0.21-1ubuntu20.04_amd64.deb mysql-community-server_8.0.21-1ubuntu20.04_amd64.deb


