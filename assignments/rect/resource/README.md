# Assignment2: String Manipulation

## Build
```sh
$ gcc209 -o rect rect.c
```

## Testing (Example)
```sh
$ ./rect < ./tests/input/sample00.txt >stdout 2>stderr
$ diff ./tests/answer/sample00.stdout stdout
$ diff ./tests/answer/sample00.stderr stderr
```

You can also compare yours with `samplerect`.

## Submission
1. Make `readme` and put `EthicsOath.pdf` to the current directory.
2. Change your `STUDENT_ID` with yours.
3. If your ID is 20221234, then
```sh
$ mkdir 20221234_assign0
$ mv rect.c readme EthicsOath.pdf 20221234_assign0
$ cd 20221234_assign0
$ tar -zcf 20221234_assign0.tar.gz *
```
4. Check your submission using a given script.
```
$ ./tools/check.py 20221234_assign0.tar.gz
[+] Good to submit :)
```
