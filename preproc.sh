#!/bin/bash

chmod u+s bin/*
sudo setcap cap_setuid+ep bin/setacl
sudo setcap cap_setuid+ep bin/getacl
sudo setcap cap_setuid+ep bin/fput
# sudo setcap cap_setuid+ep bin/fget
