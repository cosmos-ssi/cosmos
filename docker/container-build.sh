#!/bin/bash

# this is typically built by the CI only.
# For users, ktool will pull it from ghcr.io directly.
docker build -t ghcr.io/cosmos-ssi/cosmos-buildenv:v2 .