#!/bin/bash

git pull origin main

cmake -S . -B build/

cmake --build build/
