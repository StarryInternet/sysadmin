#!/usr/bin/env python
import argparse
import sys


def write_args(arg, args, f):
    if args is not None:
        f.write(arg)
        for i in range(len(args)):
            f.write(args[i])
            if i != len(args) - 1:
                f.write(' ')


def main():
    parser = argparse.ArgumentParser(
        description="Test program for sysadmin unit tests")
    parser.add_argument('--template-args', type=str, nargs="*",
                        help="Special Arguments to be sent to templates")
    parser.add_argument('--committed', type=str, nargs="*",
                        help="Key:Value mappings to be slotted into template")
    parser.add_argument('args', type=str, nargs="+",
                        help="The args being dumped to a file")
    args = parser.parse_args()
    with open("argdumper", "w") as f:
        for i in range(len(args.args)):
            f.write(args.args[i])
            if i != len(args.args) - 1:
                f.write(" ")
        write_args(' --template-args ', args.template_args, f)
        write_args(' --committed ', args.committed, f)

        f.write('\n')
    if args.args[0] == "failme":
        sys.exit(1)


if __name__ == '__main__':
    main()
