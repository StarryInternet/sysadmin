import random
import socket
import struct

from sysadmin.generated import sysadminctl_pb2


def UnpackFromProto(proto):
    attrName = proto.WhichOneof("value")
    if attrName is None:
        return "EMPTY_VALUE"
    value = getattr(proto, attrName)
    try:
        return list(getattr(value, "list"))
    except AttributeError:
        return _convertType(value, proto.WhichOneof("value"))


def rand_uint32():
    return random.randint(0, 2**32 - 1)


def _unpackIntoProto(proto, value):
    if isinstance(value, str):
        proto.strval = value
    elif value is True or value is False:
        proto.boolval = value
    elif isinstance(value, int):
        proto.int32val = value
    elif isinstance(value, list):
        if isinstance(value[0], str):
            proto.stringlist.list.extend(value)
        elif value[0] is True or value[0] is False:
            proto.boollist.list.extend(value)
        elif isinstance(value[0], int):
            proto.int32list.list.extend(value)


def _convertType(value, newtype):
    if "str" in newtype:
        return unicode(value)
    if "bool" in newtype:
        if isinstance(value, bool):
            return value
        return str(value.lower()) == "true"
    if "int32" in newtype:
        return int(value)


def _overriddenTypeUnpack(proto, value, valtype):
    if valtype == "str":
        proto.strval = _convertType(value, valtype)
    elif valtype == "bool":
        proto.boolval = _convertType(value, valtype)
    elif valtype == "int32":
        proto.int32val = _convertType(value, valtype)
    elif "list" in valtype:
        if valtype == "stringlist":
            outvalue = proto.stringlist
        elif valtype == "boollist":
            outvalue = proto.boollist
        elif valtype == "int32list":
            outvalue = proto.int32list
        if value is not None and len(value) > 0:
            if isinstance(value, list):
                convertable = value
            else:
                convertable = [value]
            outvalue.list.extend(
                map(lambda x: _convertType(x, valtype), convertable))
        else:
            outvalue.SetInParent()


class SysAdminClient(object):
    def __init__(self, ip, port, xid=None, timeout=None):
        self.ip = ip
        self.port = port
        self._connect(timeout)
        if xid is not None:
            self.xid = xid

    def _connect(self, timeout):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.settimeout(timeout)
        self.socket.connect((self.ip, self.port))

    def set(self, key, value, override_type=None):
        message = sysadminctl_pb2.Command()
        message.set.key = key
        message.id = rand_uint32()
        if hasattr(self, "xid"):
            message.xid = self.xid
        if override_type is None:
            _unpackIntoProto(message.set.value, value)
        else:
            _overriddenTypeUnpack(message.set.value, value, override_type)
        response = self._send(message)
        assert response.id == message.id
        return response

    def get(self, key):
        return self._genericKeyCommand("get", key)

    def erase(self, key):
        return self._genericKeyCommand("erase", key)

    def commit(self, opt=sysadminctl_pb2.DEFAULT):
        message = sysadminctl_pb2.Command()
        message.commit.config = opt
        message.id = rand_uint32()
        if hasattr(self, "xid"):
            message.xid = self.xid
        response = self._send(message)
        assert response.id == message.id
        return response

    def blame(self, key):
        return self._genericKeyCommand("blame", key)

    def drop(self):
        return self._genericEmptyCommand("drop")

    def firehooks(self):
        return self._genericEmptyCommand("firehooks")

    def reset(self):
        return self._genericEmptyCommand("reset")

    def rollback(self, id):
        message = sysadminctl_pb2.Command()
        message.rollback.id = id
        message.id = rand_uint32()
        response = self._send(message)
        assert response.id == message.id
        return response

    def dumphooks(self):
        return self._genericEmptyCommand("dump")

    def trigger(self, hook):
        message = sysadminctl_pb2.Command()
        message.trigger.hook = hook
        message.id = rand_uint32()
        response = self._send(message)
        assert response.id == message.id
        return response

    def inflight(self):
        return self._genericEmptyCommand('inflight')

    def _genericKeyCommand(self, command, key):
        message = sysadminctl_pb2.Command()
        setattr(getattr(message, command), "key", key)
        message.id = rand_uint32()
        if hasattr(self, "xid"):
            message.xid = self.xid
        response = self._send(message)
        assert response.id == message.id
        return response

    def _genericEmptyCommand(self, field):
        message = sysadminctl_pb2.Command()
        getattr(message, field).SetInParent()
        message.id = rand_uint32()
        if hasattr(self, "xid"):
            message.xid = self.xid
        response = self._send(message)
        assert response.id == message.id
        return response

    def _send(self, message):
        def full_recv(sock, recvlen):
            bytes_left = recvlen
            out_data = ""
            while bytes_left > 0:
                to_read = min(bytes_left, 4096)
                data = sock.recv(to_read)
                bytes_left -= len(data)
                out_data += data
            return out_data
        data = message.SerializeToString()
        length = struct.pack('I', len(data))
        self.socket.sendall(length)
        self.socket.sendall(data)
        recvlen = struct.unpack('I', self.socket.recv(4))[0]
        data = full_recv(self.socket, recvlen)
        return sysadminctl_pb2.Response.FromString(data)
