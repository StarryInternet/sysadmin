#!/usr/bin/env python
from __future__ import print_function

import argparse
import os
import subprocess
import sys

from sysadmin.Client import SysAdminClient
from sysadmin.generated import sysadminctl_pb2
from sysadmin.Migrations import load_migrations
from sysadmin.Migrations import MigrationLog
from sysadmin.Migrations import SysAdminMigrator


def main():
    parser = argparse.ArgumentParser(
        description="Migrate sysadmin keys",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument("migration_file", type=str,
                        help="Absolute path to migration file or directory")
    parser.add_argument("--migration_logs", type=str,
                        default="%s/migration_logs" % os.getcwd(),
                        help="log file containing a record of migrations")
    parser.add_argument("--port", type=int, default=4000,
                        help="sysadmin port")
    args = parser.parse_args()

    migration_log = MigrationLog()
    if os.path.exists(args.migration_logs):
        migration_log.load(args.migration_logs)
    client = SysAdminClient('localhost', args.port)
    migrator = SysAdminMigrator(client)
    migration_history = []
    migrations = load_migrations(args.migration_file)
    for i, m in enumerate(migrations):
        if migration_log.check_history(m[1]):
            print("Migration %s has already run, skipping..." % m[0])
            continue
        print("Running migration %s" % (m[0]))
        resp = migrator.migrate(dict(m[1]))
        migration_history.append((m[1], resp.commit.commit_id))
        if resp.status != sysadminctl_pb2.SUCCESS and i == len(m) - 1:
            print("Migration failed, quitting and rolling back")
            migration_history.reverse()
            for migration, cid in migration_history:
                client.rollback(cid)
                migration_log.redo_migration(migration)
            sys.exit(1)
        migration_log.log_migration(m[1])
    migration_log.save(args.migration_logs)
    print("Migrations complete, running hooks")
    if len(migration_history) > 0:
        print(client.firehooks())
    print("Hooks complete, finished migrating")
    print("Syncing filesystem")
    subprocess.check_call(['sync'])
    return 0


if __name__ == '__main__':
    sys.exit(main())
