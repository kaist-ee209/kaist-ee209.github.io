#!/bin/bash
./sgrep $(python3 -c'print("a"*2048)') < files/google.txt
