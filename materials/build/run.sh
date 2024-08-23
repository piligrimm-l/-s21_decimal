#!/bin/bash
cd ../..
sudo docker rmi -f school21/miniverter:1.0 && sudo docker build . -t school21/miniverter:1.0 -f materials/build/Dockerfile && sudo docker run school21/miniverter:1.0 
