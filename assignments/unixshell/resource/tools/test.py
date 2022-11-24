#!/usr/bin/env python3

import os
import argparse
import subprocess
import difflib
import re
import shutil

ROOT = os.path.dirname(__file__)

def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument('-f', '--filter', help='Filter for tests (e.g., simple_test00)')
    return p.parse_args()

def compare_txt(result, expected_file):
    expected = open(expected_file, 'rb').read()

    if result == expected:
        return (True, "")
    else:
        result = result.decode().splitlines(keepends=True)
        expected = expected.decode().splitlines(keepends=True)
        diff = difflib.unified_diff(result, expected, fromfile="result", tofile="expected")
        return (False, '\n'.join(diff))

def compare_re(result, expected_file):
    expected = open(expected_file, 'rb').read()

    # Make lines of string from bytes
    result_lines = result.decode().splitlines(keepends=True)
    expected_lines = expected.decode().splitlines(keepends=True)

    if len(result_lines) != len(expected_lines):
        return (False, '  # of lines = {} (expected: {})'.format(len(result_lines), len(expected_lines)))

    for result_line, expected_line in zip(result_lines, expected_lines):
        r = re.compile(expected_line)
        if not r.match(result_line):
            msgs = [
                '  Result: {}'.format(result_line),
                '  Regular expression: {}'.format(expected_line)]
            return (False, '\n'.join(msgs))
    return (True, '')


    expected = open(expected_file, 'rb').read()
    print(result, expected)

def run_test(filter):
    n_tests = 0
    n_success = 0

    for subdir in ['basic','ishrc']:
        input_dir = os.path.join(ROOT, '../tests/input', subdir)
        answer_dir = os.path.join(ROOT, '../tests/answer', subdir)

        for file in sorted(os.listdir(input_dir)):
            if args.filter and not args.filter in file:
                # Filter out
                continue

            n_tests += 1

            input = os.path.join(input_dir, file)
            name = os.path.splitext(os.path.basename(input))[0]

            if subdir == 'ishrc':
                # Copy input to .ishrc
                ishrc = os.path.join(ROOT, '.ishrc')
                shutil.copy(input, ishrc)
                os.putenv('HOME', os.path.abspath(ROOT))
                input = '/dev/null'

            expected_stdout = os.path.join(answer_dir, name + '.stdout')
            expected_stderr = os.path.join(answer_dir, name + '.stderr')

            exe = './ish'
            assert(os.path.exists(exe))

            try:
                p = subprocess.Popen(exe, stdin=open(input, 'rb'),
                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                stdout, stderr = p.communicate(timeout=5)

                if subdir == 'basic' or subdir == 'ishrc':
                    (res1, diff1) = compare_txt(stdout, expected_stdout)
                    (res2, diff2) = compare_txt(stderr, expected_stderr)
                elif subdir == 'regexp':
                    (res1, diff1) = compare_re(stdout, expected_stdout)
                    (res2, diff2) = compare_re(stderr, expected_stderr)
                else:
                    raise ValueError('No such subdir')

                if res1 and res2:
                    print("[*] PASSED: {}".format(name))
                    n_success += 1
                else:
                    print("[*] FAILED: {}".format(name))

                if not res1:
                    print("[-] Standard outputs are different")
                    print(diff1)

                if not res2:
                    print("[-] Standard errors are different")
                    print(diff2)
            finally:
                if p.poll == None:
                    p.terminate()

    print('[*] Total: {} / {}'.format(n_success, n_tests))

if __name__ == '__main__':
    args = parse_args()
    run_test(args.filter)


