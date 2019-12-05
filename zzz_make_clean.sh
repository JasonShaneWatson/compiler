#!/bin/bash
echo -e "Cleaning up junk files from project root..."
rm my.lst 2>/dev/null || true 
rm my.obj 2>/dev/null || true
rm my_output.txt 2>/dev/null || true
rm his_output.txt 2>/dev/null || true
echo -e "Cleaning up build directory..."
rm -rf $1/*
