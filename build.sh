#!/usr/bin/env bash

cmake -S . -B out -G Ninja --preset linux-debug
cmake --build out
