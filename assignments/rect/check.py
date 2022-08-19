#!/usr/bin/env python3
import argparse
import os
import re
import sys
import tarfile
import shutil
import subprocess

FILES = ['rect.c', 'readme', 'EthicsOath.pdf']

def fatal(msg):
    print(f'[-] ERROR: {msg}')
    sys.exit(1)

def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument('tar', help='a tar.gz file to check before submission')
    return p.parse_args()

if __name__ == '__main__':
    tar = parse_args().tar

    # General check
    if not os.path.exists(tar):
        fatal(f'{tar} does not exist')

    if not tarfile.is_tarfile(tar):
        fatal('Not a tar file')
        sys.exit(1)

    # Check a tar.gz satisifes requirements
    tar = tarfile.open(tar)

    files = set(FILES)
    for f in tar:
        name = f.name
        if not name in files:
            fatal(f'{name} does not exist')

        files.remove(name)

    if files:
        for name in files:
            fatal(f'{name} should be included')

    # Check whether rect.c can be compiled with gcc209
    if not shutil.which('gcc209'):
        fatal('Cannot find gcc')

    with open('.rect.c', 'wb') as f:
        f.write(tar.extractfile('rect.c').read())

    p = subprocess.run(['gcc209', '-o', '.rect', '.rect.c'])

    if p.returncode != 0:
        fatal('Cannot be compiled with gcc209')

    print('[+] Good to submit :)')
