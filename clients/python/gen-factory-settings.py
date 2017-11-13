#!/usr/bin/env python
from __future__ import print_function

import argparse
import os
import shutil
import subprocess
import sys
import time

import yaml
from sysadmin.Client import SysAdminClient
from sysadmin.generated import sysadminctl_pb2
from sysadmin.Migrations import load_migrations
from sysadmin.Migrations import MigrationLog
from sysadmin.Migrations import SysAdminMigrator


class SysAdminRunner(object):
    def Start(self, configFile):
        self.pid = subprocess.Popen(["bin/sysadmin",
                                     configFile])
        time.sleep(2)

    def Stop(self):
        self.pid.kill()


def main():
    parser = argparse.ArgumentParser(
        description="Generate base configs for sysadmin")
    parser.add_argument("factory_settings", type=str,
                        help="File containing base factory settings in "
                             "sysadmin-agnostic format")
    parser.add_argument("--config_file", type=str,
                        help="sysadmin config file for factory settings")
    parser.add_argument("--migration_logs", type=str,
                        default="/etc/sysadmin/migration_log",
                        help="log file containing a record of migrations")
    parser.add_argument("migration_file", type=str,
                        default="/etc/sysadmin/migrations",
                        help="Absolute path to migration file or directory")
    args = parser.parse_args()

    sysadmin = None
    if args.config_file:
        with open(args.config_file) as f:
            config = yaml.load(f)
            dest = config["storage-path"]
            shutil.rmtree(dest)
        sysadmin = SysAdminRunner()
        sysadmin.Start(args.config_file)

    try:
        with open(args.factory_settings) as f:
            config = yaml.load(f)
        client = SysAdminClient('localhost', 4000)
        migrator = SysAdminMigrator(client)
        migrator.migrate(config, sysadminctl_pb2.TEMPLATE_ONLY)
        client.commit(sysadminctl_pb2.TEMPLATE_ONLY)

        # Here we simply log all migrations that existed when this was run
        # to prevent them from being run again
        migration_log = MigrationLog()
        if os.path.exists(args.migration_logs):
            migration_log.load(args.migration_logs)
        migrations = load_migrations(args.migration_file)
        for m in migrations:
            migration_log.log_migration(m[1])
        migration_log.save(args.migration_logs)
    finally:
        if sysadmin:
            sysadmin.Stop()
    print("Configs dumped")
    return 0


if __name__ == '__main__':
    sys.exit(main())
