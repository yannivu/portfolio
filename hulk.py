#!/usr/bin/env python3

import concurrent.futures
import hashlib
import os
import string
import sys
import getopt
from tqdm import tqdm

# Constants

ALPHABET = string.ascii_lowercase + string.digits

# Functions

def usage(exit_code=0):
    progname = os.path.basename(sys.argv[0])
    print(f'''Usage: {progname} [-a ALPHABET -c CORES -l LENGTH -p PATH -s HASHES]
    -a ALPHABET Alphabet to use in permutations
    -c CORES    CPU Cores to use
    -l LENGTH   Length of permutations
    -p PREFIX   Prefix for all permutations
    -s HASHES   Path of hashes file''')
    sys.exit(exit_code)

def md5sum(s):
    ''' Compute md5 digest for given string. '''
    # TODO: Use the hashlib library to produce the md5 hex digest of the given
    # string.

    return hashlib.md5(s.encode()).hexdigest()

def permutations(length, alphabet=ALPHABET):
    ''' Recursively yield all permutations of alphabet up to given length. '''
    # TODO: Use yield to create a generator function that recursively produces
    # all the permutations of the given alphabet up to the provided length.
    if length == 0:   # Base case
        yield ''
    else:             # Recursive case
        for char in alphabet:
            for perm in permutations(length-1, alphabet):
                yield char + perm #the combination of each letter with each permutation of length - 1

def flatten(sequence):
    ''' Flatten sequence of iterators. '''
    # TODO: Iterate through sequence and yield from each iterator in sequence.
    for iterator in sequence:
        yield from iterator

def crack(hashes, length, alphabet=ALPHABET, prefix=''):
    ''' Return all password permutations of specified length that are in hashes
    by sequentially trying all permutations. '''
    # TODO: Return list comprehension that iterates over a sequence of
    # candidate permutations and checks if the md5sum of each candidate is in
    # hashes.

    candidates = (prefix+p for p in permutations(length, alphabet))
    
    return [candidate for candidate in candidates if md5sum(candidate) in hashes]

def whack(arguments):
    ''' Call the crack function with the specified list of arguments '''
    return crack(*arguments)

def smash(hashes, length, alphabet=ALPHABET, prefix='', cores=1):
    ''' Return all password permutations of specified length that are in hashes
    by concurrently subsets of permutations concurrently.
    '''
    # TODO: Create generator expression with arguments to pass to whack and
    # then use ProcessPoolExecutor to apply whack to all items in expression.

    arguments = ((hashes, length-1, alphabet, prefix+char) for char in alphabet)

    with concurrent.futures.ProcessPoolExecutor(cores) as executor:
        results = (executor.map(whack, arguments))
    
    return flatten(results)

# Main Execution

def main():
    arguments   = sys.argv[1:]
    alphabet    = ALPHABET
    cores       = 1
    hashes_path = 'hashes.txt'
    length      = 1
    prefix      = ''

    # TODO: Parse command line arguments

    try:
        opts, args = getopt.getopt(arguments, 'a:c:l:p:s:')
    except getopt.GetoptError as e:
        print(e, file=sys.stderr)
        usage(1)

    for opt, arg in opts:
        if opt == '-a':
            alphabet = arg
        elif opt == '-c':
            cores = int(arg)
        elif opt == '-l':
            length = int(arg)
        elif opt == '-p':
            prefix = arg
        elif opt == '-s':
            hashes_path = arg
        else:
            usage(1)

    # TODO: Load hashes set

    file = open(hashes_path, 'r')
    hashes = set(line.strip() for line in file)
    file.close()

    # TODO: Execute crack or smash function

    if cores == 1:
        passwords = crack(hashes, length, alphabet, prefix)
    else:
        passwords = smash(hashes, length, alphabet, prefix, cores)

    for password in passwords:
        print(password)

    # TODO: Print all found passwords

if __name__ == '__main__':
    main()
