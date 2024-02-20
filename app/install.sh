#!/bin/bash

echo "Building app..."

cargo build
sudo cp ./target/debug/ara /usr/bin/ara
