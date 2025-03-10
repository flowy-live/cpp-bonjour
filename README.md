# Overview

`cpp-bonjour` is meant to be a pure C++ library that one can use on MacOS (iOS not tested). It uses the stdlib `<dns_sd.h>`.


# How to test
1. Run the broadcaster.
```sh
cd broadcaster
clang++ -o broadcaster main.cpp
./broadcaster
```
You should see output of the service details of the service we are advertising.

2. Scan for services

```sh
cd scanner
clang++ -o scanner main.cpp
./scanner
```
Having done this, the output should include the resolved service.
