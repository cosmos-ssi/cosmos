#!/bin/sh

echo "cleaning all generated files"
find . -type f -name '*.o' -exec rm {} +
find . -type f -name '*.a' -exec rm {} +
find . -type f -name '*.ar' -exec rm {} +
find . -type f -name '*.bin' -exec rm {} +
find . -type f -name '*.elf' -exec rm {} +
find . -type f -name '*.map' -exec rm {} +

echo "cleaning log files"
find . -type f -name '*.log' -exec rm {} +

echo "cleaning packet capture files"
find . -type f -name 'net?.dat' -exec rm {} +