# Vectorized Poseidon Gold

This repository is a Go wrapper of https://github.com/0xPolygonHermez/goldilocks using commit [353f7c0a44914ad0443c06c91a0f81639bc974c8](https://github.com/0xPolygonHermez/goldilocks/commit/353f7c0a44914ad0443c06c91a0f81639bc974c8) from 21.08.2023

It contains a C implementation of the goldilocks C++ library.

The C wrapper has a Go wrapper as well for easier integration.

# Testing ARM on x86

1. Install qemu

    apt-get install qemu-system qemu-user

2. Install GCC for arm

    sudo apt-get install gcc-aarch64-linux-gnu

3. Start compiled binary

    qemu-aarch64 -L /usr/aarch64-linux-gnu/ <path>
