#!/usr/bin/env python
from __future__ import print_function

import argparse
import os

from sysadmin import ConfigTemplateRenderer
from sysadmin import LazySysAdmin
from sysadmin import SysAdminClient


def main():
    parser = argparse.ArgumentParser(description="Templater for config files")
    parser.add_argument('--template-args', type=str, nargs="*",
                        help="Special Arguments to be sent to templates")
    parser.add_argument('--committed', type=str, nargs="*",
                        help="Key:Value mappings to be slotted into template")
    parser.add_argument('template', type=str,
                        help="The file path of the jinja template")
    parser.add_argument('dest', type=str,
                        help="The file path of the rendered template")
    args = parser.parse_args()
    lazy = LazySysAdmin(SysAdminClient("127.0.0.1", 4000))
    lazy.prefetch()
    renderer = ConfigTemplateRenderer(args.template)
    fullArgs = {'values': lazy}
    fullArgs['args'] = getattr(args, 'template_args', None)
    fullArgs['committed'] = getattr(args, 'committed', None)
    rendered = renderer.renderTemplate(fullArgs)

    dirname = os.path.dirname(args.dest)
    try:
        if not os.path.exists(dirname):
            os.makedirs(dirname)
    except OSError:
        print("Error making dirs for template %s" % args.dest)
    with open(args.dest, "w") as f:
        f.write(rendered)
        f.flush()
        os.fsync(f.fileno())


if __name__ == '__main__':
    main()
