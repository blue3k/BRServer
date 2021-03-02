#!/bin/bash


SF_PATH=$(dirname "$0")

sudo apt install default-jdk

#download deb package bundle
wget http://apache.mirrors.hoobly.com/zookeeper/zookeeper-3.6.2/apache-zookeeper-3.6.2-bin.tar.gz
sudo mkdir /etc/zookeeper
sudo tar -xvf apache-zookeeper-3.6.2-bin.tar.gz -C /etc/zookeeper --strip-components=1

sudo mkdir /data
sudo mkdir /data/zookeeper


#install
#sudo chown -R <user to run>:<your group> /data
#sudo chown -R <user to run>:<your group> /etc/zookeeper

sudo mkdir /etc/zookeeper/logs
sudo chmod -R g+rw /etc/zookeeper/logs
sudo chmod -R g+rw /data

sudo cp /etc/zookeeper/conf/zoo_sample.cfg /etc/zookeeper/conf/zoo.cfg
sudo vi /etc/zookeeper/conf/zoo.cfg

sudo cp zookeeper.service /etc/systemd/system/zookeeper.service
sudo vi /etc/systemd/system/zookeeper.service


sudo systemctl edit zookeeper
#/etc/systemd/system/zookeeper.service.d/override.conf:
#[Service]
#Environment="ZOO_LOG_DIR=/StreamDB/logs"


sudo systemctl start zookeeper
sudo systemctl enable zookeeper

