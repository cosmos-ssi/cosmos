#!/bin/bash

IMAGE=test.img

DD=dd
GPT=gpt
MKFS=/usr/local/Cellar/dosfstools/4.2/sbin/mkfs.msdos

# 3200 * 32768 = 100 MB
$DD if=/dev/zero of=$IMAGE bs=32768 count=3200

# make a GPT partion table
$GPT create $IMAGE

# windows parition. 512 byte sectors, so 80MB is 163840 sectors
$GPT add -t windows -s 163840 $IMAGE

# a cosmos swap partition, uses the rest of the disk which is about 10MB
GPT add -t 3c453e36-0c03-4b8b-a88f-2228caf8f2b1 $IMAGE

$GPT show $IMAGE

# msdos FAT 16 partition starts at sector 34
$MKFS -F 16 -n COSMOS --offset 34 $IMAGE
