#!/bin/sh

cd build && cmake ..;

make;

sudo ./tinyrenderer;
