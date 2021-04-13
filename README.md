# KV File Specification

KV-Spec is a Git repository that provides C++, Python, and MATLAB code to read
KV files. The KV file format is a plain text format for saving data in a clearly
defined way. It is intended to address the problems that can be caused by the
large variety of ways similar files such as CSV files can be formatted, causing
the scripts that read them to require customization for each input file and
careful monitoring. By using KV files are their strict rules for variable
declaration, all ambiguity caused by CSV files or similar formats is avoided.

## Key Features of KV File Format

* Plain text - As a plain text file, you can read or write KV files in any text editor
* Header Statements - KV files support a multi-line string called a header which describes the file's contents. This makes it simple to document the conditions under which the file's data were collected.
* Variable Descriptions - Each variable can be paired with a description. This lets you put information such as units or  detailed descriptions of what the variable describes in the data file without requiring long and complicated variable names.
* Simple - The KV format is very simple and makes it easy to port to new languages, read or write files by hand, or share between systems.

_NOTE:_ This repository is a work in progress and actively being developed as of
April 2021. Installation instructions and full tutorials will be made available
when the first revision of the code is finalized later this year.

For more information, see the [Syntax Rules](Syntax.md).

## Using KV Files

Currently, this specification includes classes to read and write KV files in three
languages: C++, Python, and MATLAB. To use KV files in your programs, first import
the class for your language ("KVFile" in cppkv.hpp for C++, "KVFile" in pykv.py for
Python, and "KvFile" in KvFile.m for MATLAB). Then you can either read an existing
KV file or add variables with the 'add()' function and write them to a file. For
details, see the language specific instructions below.

## Installation

~ In progress ~

### Full Installation

~ In progress ~

### Individual C++ Installation

~ In progress ~

### Individual Python Installation

~ In progress ~

### Individual MATLAB Installation

~ In progress ~

## Tutorial

~ In progress ~
