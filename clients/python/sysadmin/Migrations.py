from __future__ import print_function

import glob
import hashlib
import os

import yaml
from sysadmin import SysAdminClient
from sysadmin import UnpackFromProto
from sysadmin.generated import sysadminctl_pb2


def load_migrations(path):
    migrations = []
    if os.path.isdir(path):
        migration_paths = glob.glob("%s/*" % path)
        for fp in sorted(migration_paths):
            with open(fp) as f:
                migrations.append((fp, yaml.load(f)))
    else:
        with open(path) as f:
            migrations.append((path, yaml.load(f)))
    return migrations


def hash_migration(migration):
    return hashlib.md5(yaml.dump(migration)).hexdigest()


class MigrationLog(object):
    def __init__(self):
        self.migrations = {}

    def load(self, path):
        with open(path) as f:
            self.migrations = yaml.load(f)
            if not self.migrations:
                self.migrations = {}

    def check_history(self, migration):
        hashed = hash_migration(migration)
        return hashed in self.migrations and self.migrations[hashed]

    def log_migration(self, migration):
        self.migrations[hash_migration(migration)] = True

    def redo_migration(self, migration):
        self.migrations[hash_migration(migration)] = False

    def save(self, path):
        with open(path, "w") as f:
            f.write(yaml.dump(self.migrations))


def nested_setter(sysadmin, config, op):
    def inner_set_configs(current, key_prefix):
        for k, v in current.iteritems():
            key = k if key_prefix == '' else '.'.join([key_prefix, k])
            if isinstance(v, dict):
                inner_set_configs(v, key)
            else:
                op(sysadmin, key, v)
    inner_set_configs(config, '')


class SysAdminMigrator(object):
    def __init__(self, sysadmin):
        self.sysadmin = sysadmin

    def migrate(self, config, commit_mode=sysadminctl_pb2.NO_HOOKS):
        self.default_keys(config.get("default", {}))
        self.new_keys(config.get("new", {}))
        self.remove_keys(config.get("remove", {}))
        self.change_types(config.get("typechange", {}))
        self.change_values(config.get("valuechange", {}))
        self.rename_keys(config.get("rename", {}))
        return self.sysadmin.commit(commit_mode)

    def default_keys(self, keys):
        nested_setter(self.sysadmin, keys, default_add_key)

    def new_keys(self, keys):
        nested_setter(self.sysadmin, keys, maybe_add_key)

    def change_values(self, keys):
        nested_setter(self.sysadmin, keys, change_value)

    def change_types(self, keys):
        nested_setter(self.sysadmin, keys, change_type)

    def rename_keys(self, keys):
        for key in keys:
            oldkey = key.keys()[0]
            newkey = key[oldkey]
            rename_key(self.sysadmin, oldkey, newkey)

    def remove_keys(self, keys):
        for key in keys:
            remove_key(self.sysadmin, key)


def default_add_key(sysadmin, key, value):
    default_key = "default.%s" % key
    if isinstance(value, list):
        head_type = value[0]
        truevalue = value[1:]
        sysadmin.set(default_key, truevalue, head_type)
    else:
        sysadmin.set(default_key, value)


def maybe_add_key(sysadmin, key, value):
    existing = sysadmin.get(key)
    if existing.status == sysadminctl_pb2.SUCCESS:
        print("Key {} already exists, not changing value".format(key))
        return
    if isinstance(value, list):
        head_type = value[0]
        truevalue = value[1:]
        sysadmin.set(key, truevalue, head_type)
    else:
        sysadmin.set(key, value)


def change_value(sysadmin, key, value):
    if isinstance(value, list):
        head_type = value[0]
        truevalue = value[1:]
        sysadmin.set(key, truevalue, head_type)
    else:
        sysadmin.set(key, value)


def change_type(sysadmin, key, newtype):
    existing = sysadmin.get(key)
    if existing.status == sysadminctl_pb2.KEY_NOT_FOUND:
        print("Key {} must exist to have it's type changed".format(key))
        return
    oldtype = str(existing.get.kvs[0].value.WhichOneof("value"))
    value = UnpackFromProto(existing.get.kvs[0].value)
    if "list" in oldtype and "list" not in newtype:
        if len(getattr(existing.get.kvs[0].value, oldtype).list) > 1:
            print("Can't type convert a list larger than one element")
            return
        value = value[0]
    # sysadmin's type system requires that we remove the old value
    # completely before being able to set it with a new type
    cloneClient = SysAdminClient(sysadmin.ip, sysadmin.port, 10)
    cloneClient.erase(key)
    cloneClient.commit(sysadminctl_pb2.NO_HOOKS)
    sysadmin.set(key, value, newtype)


def rename_key(sysadmin, oldkey, newkey):
    existing = sysadmin.get(oldkey)
    if existing.status == sysadminctl_pb2.KEY_NOT_FOUND:
        print("Key {} must exist to be renamed".format(oldkey))
        return
    sysadmin.erase(oldkey)
    sysadmin.set(newkey, UnpackFromProto(existing.get.kvs[0].value))


def remove_key(sysadmin, key):
    sysadmin.erase(key)
