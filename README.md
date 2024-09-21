# BigNum Library for Cryptographic Applications

This repository contains a C++ implementation of a BigNum library designed to handle large integer arithmetic, ranging from 512 to 2048 bits. It supports cryptographic operations such as modulo addition, modulo multiplication, and modular inversion, crucial for public key cryptosystems.

## Features

- **Modulo Addition**: Computes the sum of two large numbers modulo a given modulus.
- **Modulo Multiplication**: Multiplies two large numbers modulo a specified modulus.
- **Modular Inversion**: Finds the modular inverse using the Extended Euclidean Algorithm.

## Design Overview

The BigNum library is designed with flexibility and efficiency in mind, enabling operations on numbers with different bit lengths, tailored for cryptographic applications.

### Key Design Decisions
- **Number Representation**: The library uses arrays of integers to represent numbers. Each element in the array represents a portion of the large number, enabling easy handling of numbers with hundreds or thousands of bits.
- **Modular Arithmetic**: Specialized algorithms for handling modulo operations, ensuring both accuracy and performance in arithmetic tasks involving large numbers.
