FROM ubuntu:20.04

# Install cross-compiler prerequisites
RUN set -x \
	&& apt-get update \
	&& apt-get install -y wget make nasm patch clang-format build-essential \
	binutils gcc-10 git iwyu curl \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /work
