from sysadmin import SysAdminClient
from sysadmin import UnpackFromProto
from sysadmin.generated import sysadminctl_pb2


def _transformKeyForJinja(key):
    return key.replace(".", "_")


def FetchAllValues(keys):
    client = SysAdminClient("127.0.0.1", 4000)
    output = {}
    for arg in keys:
        resp = client.get(arg)
        if resp.status == sysadminctl_pb2.SUCCESS:
            for value in resp.get.kvs:
                output[value.key] = UnpackFromProto(value.value)
        elif "last." not in arg:
            raise Exception("Could not find required key. Key %s, Error: %s" %
                            (arg, resp.status))
    return condense(output)


def condense(uncondensed):
    def innerCondense(partialDict, keys, value):
        if len(keys) <= 0:
            return
        if len(keys) == 1:
            if keys[0] in partialDict:
                partialDict["{" + keys[0] + "}"] = value
            else:
                partialDict[keys[0]] = value
        else:
            dict_instance = isinstance(partialDict[keys[0]], dict)
            if keys[0] in partialDict and not dict_instance:
                partialDict["{" + keys[0] + "}"] = partialDict[keys[0]]
                del partialDict[keys[0]]
            if keys[0] not in partialDict:
                partialDict[keys[0]] = {}
            innerCondense(partialDict[keys[0]], keys[1:], value)
    output = {}
    for key, value in uncondensed.iteritems():
        keyparts = key.split(".")
        innerCondense(output, keyparts, value)
    return output
