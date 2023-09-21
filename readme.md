# Vectorized Poseidon Gold

This repository is a Go wrapper of https://github.com/0xPolygonHermez/goldilocks using commit [353f7c0a44914ad0443c06c91a0f81639bc974c8](https://github.com/0xPolygonHermez/goldilocks/commit/353f7c0a44914ad0443c06c91a0f81639bc974c8) from 21.08.2023

It contains the C wrapper of the goldilocks C++ library. Both compilied as a shared library.

The C wrapper has a Go wrapper as well for easier integration.

The repo contains precompiled shared libraries.

## Supported platforms and architectures:

Lixux - x86_64: Supports three type of instruction sets - regular, AVX2, AVX512
Mac - x86_64: To be added
Mac - arm64: To be added

# Make

## Ubuntu

Install following deps:

libgtest-dev, libomp-dev, libgmp-dev

## macOS

brew install libomp
brew install gmp
