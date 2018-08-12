#!/bin/bash

# base install
apt-get update
apt-get install -y xinetd

# get xinetd
chown root:root *.config
chmod +x *.config
mv *.config /etc/xinetd.d/challenge
chmod 644 /etc/xinetd.d/challenge

# fix permissions
chown challenge:challenge *

# bring up the service
service xinetd restart

# clean up
rm -f babys_first_rop.tar.gz setup.sh

# service specific options
chmod +x /home/challenge/babys_first_rop
