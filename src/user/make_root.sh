#!/bin/bash

IMAGE=test.img

DD=dd
GPT=gpt
MKFS=/usr/local/Cellar/dosfstools/4.2/sbin/mkfs.msdos

$DD if=/dev/zero of=$IMAGE bs=32768 count=3200
$GPT create $IMAGE
$GPT add -t windows $IMAGE
$GPT show $IMAGE

# msdos partition starts at sector 34
$MKFS --offset 34 $IMAGE
