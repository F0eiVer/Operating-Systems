#!/bin/bash

git pull orign main

cmake -S . -B build/

cmake --build build/
