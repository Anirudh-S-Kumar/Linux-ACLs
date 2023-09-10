#!/bin/bash

chmod u+s bin/*
sudo setcap cap_setuid+ep bin/setacl
sudo setcap cap_setuid+ep bin/getacl
sudo setcap cap_setuid+ep bin/fput
sudo setcap cap_setuid+ep bin/fget
sudo setcap cap_setuid+ep bin/create_dir
sudo setcap cap_setuid+ep bin/acl_cd

sudo cp bin/* /usr/bin
