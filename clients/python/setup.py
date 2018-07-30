#!/usr/bin/env python
from __future__ import print_function

import glob
import os
import shutil
import subprocess
import sys

from setuptools import setup

from distutils.command.build import build as _build
from distutils.command.clean import clean as _clean
from distutils.spawn import find_executable


if 'PROTOC' in os.environ and os.path.exists(os.environ['PROTOC']):
    protoc = os.environ['PROTOC']
else:
    protoc = find_executable('protoc')


def generate_proto(source_file, generated_dir):
    print("Generating %s" % source_file)
    if not os.path.exists(source_file):
        print("Can't find required file: %s" % source_file, file=sys.stderr)
        sys.exit(-1)
    if protoc is None:
        print("protoc could not be found. Please install it", file=sys.stderr)
        sys.exit(-1)
    if not os.path.exists(generated_dir):
        os.makedirs(generated_dir)
        open(os.path.join(generated_dir, '__init__.py'), 'w+').close()
    protoc_command = [protoc,
                      '-I../../sysadmin-api',
                      "--python_out=%s" % generated_dir,
                      source_file]
    if subprocess.call(protoc_command) != 0:
        sys.exit(-1)


def clean_command(generated_dir):
    class Clean(_clean):
        def run(self):
            if os.path.exists(generated_dir):
                print('rm -rf %s' % generated_dir)
                shutil.rmtree(generated_dir)
    return Clean


def build_command(generated_dir):
    class Build(_build):
        def run(self):
            for proto_file in glob.glob(os.path.join("../../sysadmin-api",
                                                     "*.proto")):
                generate_proto(proto_file, generated_dir)
            _build.run(self)
    return Build


GENERATED_DIR = os.path.join('sysadmin', 'generated')

setup(
    name='py-sysadmin',
    version='1.2.1',
    description='Control sysadmin',
    author='Eric Brown',
    author_email='oss@starry.com',
    url='https://github.com/StarryInternet/sysadmin',
    install_requires=[
        'Jinja2>=2.8,<3.0',
        'MarkupSafe>=0.23,<2.0',
        'protobuf>=3.0,<4.0',
        'pytz>=2016.1',
        'PyYAML>=3.11,<4.0',
    ],
    extras_require={
        'test': [
            'nose>=1.3.0,<2.0',
        ]
    },
    cmdclass={
        'build': build_command(GENERATED_DIR),
        'clean': clean_command(GENERATED_DIR),
    },
    packages=[
        'sysadmin',
        'sysadmin.generated'
    ],
    scripts=[
        'sysadminctl',
        'templater.py',
        'gen-factory-settings.py',
        'migrate.py',
    ],
    classifiers=[
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7'
    ]
)
