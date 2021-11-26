# Huffman Coding

This project is an implementation of the [Huffman Coding](https://brilliant.org/wiki/huffman-encoding/).
It allows you to lossless compress ASCII text files, until 20 to 90 percents.

## Table of Contents

- [Install](#install)
- [Usage](#usage)


## Install 

If you have not installed or configured git,
please refer to [getting started with git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).

Once done, you can clone the git repository with
```bash
git clone https://github.com/Tucobad/Huffman # with https protocol
```

or

```bash
git clone git@github.com:Tucobad/Huffman.git  # with ssh protocol
```

## Usage


The structure of the project is convenient, thus you can easily find your way. </br>
By default, there are some samples in `config/` directory (50kb to 2mb).

The building of the project is done with `make` and useful extensions </br>
to build the project are described in the manual reachable with `make help`.

Once executable is created, you can use several options like compress and/or decompress </br>
and ASCII text file. All of these options are in the execution's manual. </br>
easy to reach with `./bin/exe -h <file>` or `./bin/exe --help <file>`.
