# Assignment 5: A Unix Shell

## Build
```sh
$ make
```

NOTE: You should modify Makefile for your shell.

## Testing (Example)
1. Build `ish` by running `make`.
2. Run `./tools/test.py`.
3. If you want to run specific test, use `-f` option.
For example,

```sh
$ ./tools/test.py -f simple_test01
```

To validate whether your testing works, use `sampleish`.
```sh
$ cp sampleish ish
$ ./tools/test.py
```

This should pass all provided test cases.

## Submission
1. Make `readme` and put `EthicsOath.pdf` to the current directory.
2. Change your `STUDENT_ID` with yours.
3. Run 
```sh
$ make submit
```

WARNING: Currently, we only include `*.c` and `*.h`.
If you have subdirectories, please modify `Makefile` properly.

4. Submit your `$(STUDENT_ID)_assign5.tar.gz`.
