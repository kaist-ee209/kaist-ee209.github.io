# Assignment4: Assembly Language Programming

- Student ID: 2022xxxx
- Name: xxx


## Build
```sh
$ make
```

## Testing (Example)
```sh
$ ./mydc < ./tests/input/add_test01.txt >stdout 2>stderr
$ diff ./tests/answer/add_test01.stdout stdout
$ diff ./tests/answer/add_test01.stderr stderr
```

## Submission
1. Make `README.md` and put `EthicsOath.pdf` to the current directory.
2. Change your `STUDENT_ID` with yours.
3. Run 
```sh
$ make submit
```
4. Submit your `$(STUDENT_ID)_assign4.tar.gz`.
