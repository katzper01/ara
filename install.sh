#!/bin/bash

# Install the service
cd service
./install.sh 

cd ..

# Install the app 
cd app
./install.sh
