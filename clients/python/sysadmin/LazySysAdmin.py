from sysadmin import UnpackFromProto
from sysadmin.generated import sysadminctl_pb2


def combine_full_key(namespace, key):
    if len(namespace) == 0:
        full_key = "%s" % (key)
    else:
        full_key = "%s.%s" % (namespace, key)
    return full_key


class NamespaceFetcher(object):
    def __init__(self, sysadmin, namespace):
        self.sysadmin = sysadmin
        self.namespace = namespace
        self.sub_namespaces = {}
        self.namespace_items = {}

    def _fetch_update(self, key):
        if key in self.namespace_items:
            return self.namespace_items[key]
        if key in self.sub_namespaces:
            return self.sub_namespaces[key]
        resp = self.sysadmin.get(key)
        if resp.status == sysadminctl_pb2.KEY_NOT_FOUND:
            if key in self.sub_namespaces:
                return self.sub_namespaces[key]
            self.sub_namespaces[key] = NamespaceFetcher(self.sysadmin, key)
            return self.sub_namespaces[key]
        self.namespace_items[key] = UnpackFromProto(resp.get.kvs[0].value)
        return self.namespace_items[key]

    def __iter__(self):
        """Fetch all the paths that start with this node and terminate at a
        node that contains a value. Then iterate along each path. If the parent
        does not contain a corresponding NamespaceFetcher object in its
        sub_namespaces dict then create a new object and add it. Except for the
        last node of each branch. In that case add the name and value to the
        parent's namespace_items dict"""
        # We always issue a wildcard get on iteration, largely cause there's
        # no way to know if we've gotten the whole namespace via single gets
        # TODO, some "hints" to this dict could tell it that we have everything
        resp = self.sysadmin.get(combine_full_key(self.namespace, "*"))
        returned = set()
        for kvs in resp.get.kvs:
            key = kvs.key
            if len(self.namespace) > 0:
                key = kvs.key.replace("%s." % self.namespace, "")
            split_keys = key.split(".")
            nested_dict = self
            for i, skey in enumerate(split_keys):
                running_key = combine_full_key(nested_dict.namespace, skey)
                if i >= len(split_keys) - 1:
                    # The end of a path contains a value, but has no children
                    nested_dict.namespace_items[running_key] = UnpackFromProto(
                        kvs.value)
                elif running_key not in nested_dict.sub_namespaces:
                    fetcher = NamespaceFetcher(self.sysadmin, running_key)
                    nested_dict.sub_namespaces[running_key] = fetcher
                    nested_dict = fetcher
                else:
                    nested_dict = nested_dict.sub_namespaces[running_key]
            if split_keys[0] not in returned:
                returned.add(split_keys[0])
                yield split_keys[0]

    def __getitem__(self, key):
        full_key = combine_full_key(self.namespace, key)
        if full_key in self.namespace_items:
            return self.namespace_items[full_key]
        return self._fetch_update(full_key)

    def iteritems(self):
        for key in self.__iter__():
            yield key, self.__getitem__(key)

    def get(self, key, default):
        value = self.__getitem__(key)
        if type(value) is NamespaceFetcher:
            return default
        return value

    def prefetch(self):
        for _ in self.__iter__():
            pass

    def is_namespace(self, key):
        full_key = combine_full_key(self.namespace, key)
        if full_key in self.sub_namespaces:
            return True
        # maybe we just haven't fetched it yet...
        self.__getitem__(key)
        return full_key in self.sub_namespaces

    def is_value(self, key):
        full_key = combine_full_key(self.namespace, key)
        if full_key in self.namespace_items:
            return True
        # maybe we just haven't fetched it yet...
        self.__getitem__(key)
        return full_key in self.namespace_items


class LazySysAdmin(object):

    def __init__(self, sysadmin):
        self.sysadmin = sysadmin
        self.cached_items = NamespaceFetcher(self.sysadmin, "")

    def __iter__(self):
        return self.cached_items.__iter__()

    def prefetch(self):
        for _ in self.cached_items.__iter__():
            pass

    def iteritems(self):
        return self.cached_items.iteritems()

    def __getitem__(self, key):
        return self.cached_items[key]
