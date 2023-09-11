#!/bin/bash

chmod u+s bin/*

for file in bin/*; do
    sudo setcap cap_setuid+ep $file
done