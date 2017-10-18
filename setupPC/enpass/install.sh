#!/bin/bash

# install enpass source: https://www.enpass.io/kb/how-to-install-on-linux/

sudo echo "deb http://repo.sinew.in/ stable main" > \
  /etc/apt/sources.list.d/enpass.list


# And import key that is used to sign the release:
sudo wget -O - https://dl.sinew.in/keys/enpass-linux.key | apt-key add -


# install like any other software package
sudo apt-get update

sudo apt-get install enpass




