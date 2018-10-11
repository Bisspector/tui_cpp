#!/bin/sh
clang++ -std=c++0x $1 -o main -lboost_system -pthread -lsqlite3
