#!/bin/bash


SF_PATH=$(dirname "$0")

#download deb package bundle
wget https://apache.claz.org/kafka/2.6.0/kafka_2.12-2.6.0.tgz
sudo mkdir /etc/kafka
sudo tar -xvf kafka_2.12-2.6.0.tgz -C /etc/kafka --strip-components=1

sudo mkdir /etc/kafka/logs
sudo mkdir /data
sudo mkdir /data/kafka-logs
# sudo chown -R <user to run>:<your group> /etc/kafka/logs
# sudo chown -R <user to run>:<your group> /data
# sudo chown -R <user to run>:<your group> /data/kafka-logs


#install

#change data path if needed
sudo vi /etc/kafka/conf/server.properties

sudo cp kafka.service /etc/systemd/system/kafka.service
sudo vi /etc/systemd/system/kafka.service


#sudo systemctl edit kafka

sudo systemctl start kafka

