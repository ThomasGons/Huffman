# Huffman

Who have not dreamt to compress files. <br/>
Perhaps whole people but with this project you can do this. <br/>
Using [Huffman Coding](https://brilliant.org/wiki/huffman-encoding/) onto ASCII text files,
the result will take 20 to 90 % less memory.

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
By default, the sample files are in `config/` directory

The building of the project is done with `make` and all useful extensions </br>
are described in the manual reachable with `make help`.

Once executable is created, you can use several options like compress and/or decompress </br>
and ASCII text file. All of these options are in the execution's manual. </br>
easy to reach with `./bin/exe -h <file>` or `./bin/exe --help <file>`.
