import os
import shutil
import socket
import subprocess
import time
import unittest

from sysadmin import FetchAllValues
from sysadmin import LazySysAdmin
from sysadmin import SysAdminClient
from sysadmin import SysAdminMigrator
from sysadmin import UnpackFromProto
from sysadmin.generated import sysadminctl_pb2


class SysAdminRunner(object):
    def Start(self, configFile):
        self.pid = subprocess.Popen(["bin/sysadmin",
                                     configFile])
        time.sleep(2)

    def Stop(self):
        self.pid.kill()


class SysAdminFixture(unittest.TestCase):
    def setUp(self, configFile="test/configs/config.yaml"):
        self.sysadmin = SysAdminRunner()
        self.sysadmin.Start(configFile)

    def tearDown(self):
        print "Stopping processes"
        self.sysadmin.Stop()
        print "Killing backend storage"
        shutil.rmtree("test/tmp")
        shutil.rmtree("test/historyTmp")

    def assertRenderedTemplateMatches(self, correct, renderedPath):
        with open(renderedPath, "r") as f:
            self.assertEqual(correct, f.read())

    def assertArgsDumpedProperly(self, correct):
        with open("argdumper", "r") as f:
            correctdict = set(correct.split(" "))
            dumpeddict = set(f.read().strip().split(" "))
            self.assertEqual(correctdict, dumpeddict)

    def assertFileExistsTimeout(self, file, timeout=10):
        dt = 0
        while dt < timeout:
            if os.path.exists(file):
                break
            dt += 1
            time.sleep(1)
        self.assertTrue(os.path.exists(file))

    def assertFileDoesNotExistTimeout(self, file, timeout=10):
        dt = 0
        while dt < timeout:
            if os.path.exists(file):
                break
            dt += 1
            time.sleep(1)
        self.assertFalse(os.path.exists(file))


class NoHooksSysAdminTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_start_stop(self):
        time.sleep(2)

    def test_basic_sets(self):
        # one in-flight
        client = SysAdminClient("127.0.0.1", 4000)
        resp = client.set("wireless.wl0.ssid", "cheese")
        self.assertEqual("wireless.wl0.ssid", resp.get.kvs[0].key)
        self.assertEqual("cheese", UnpackFromProto(resp.get.kvs[0].value))
        resp = client.get("wireless.wl0.ssid")
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, resp.status)
        client.commit()
        resp = client.get("wireless.wl0.ssid")
        self.assertEqual("cheese", UnpackFromProto(resp.get.kvs[0].value))

        # multiple in-flight, one erasing the other
        client.set("wireless.wl0.psk", "password")
        client.set("wireless.wl0.channel", 40)
        client.set("wireless.wl0.channel", 60)
        client.set("wireless.wl0.enabled", True)
        client.set("wireless.wl0.frequencies", [1, 2, 3])
        client.commit()
        self.assertEqual(60, UnpackFromProto(client.get("wireless.wl0.channel").get.kvs[0].value))
        self.assertEqual("password", UnpackFromProto(client.get("wireless.wl0.psk").get.kvs[0].value))
        self.assertEqual(True, UnpackFromProto(client.get("wireless.wl0.enabled").get.kvs[0].value))
        self.assertEqual([1, 2, 3], UnpackFromProto(client.get("wireless.wl0.frequencies").get.kvs[0].value))

    def test_basic_drop(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("wireless.wl0.psk", "password")
        client.set("wireless.wl0.channel", 40)
        client.drop()
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("wireless.wl0.psk").status)
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("wireless.wl0.channel").status)

    def test_type_checking(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("wireless.wl0.psk", "password")
        resp = client.set("wireless.wl0.psk", 5)
        self.assertEqual(sysadminctl_pb2.SUCCESS, resp.status)
        client.commit()
        resp = client.set("wireless.wl0.psk", "password")
        self.assertEqual(sysadminctl_pb2.TYPE_MISMATCH, resp.status)
        client.commit()
        self.assertEqual(5, UnpackFromProto(client.get("wireless.wl0.psk").get.kvs[0].value))

    def test_erase(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("wireless.wl0.psk", "password")
        client.set("wireless.wl0.channel", 40)
        client.set("wireless.wl0.enabled", True)
        client.set("wireless.wl0.frequencies", [1, 2, 3])
        client.commit()
        self.assertEqual("password", UnpackFromProto(client.get("wireless.wl0.psk").get.kvs[0].value))
        client.erase("wireless.wl0.psk")
        self.assertEqual("password", UnpackFromProto(client.get("wireless.wl0.psk").get.kvs[0].value))
        client.erase("not.a.key")
        client.commit()
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("wireless.wl0.psk").status)


class TemplateSysAdminTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self, "test/configs/hooksConfig.yaml")

    def tearDown(self):
        SysAdminFixture.tearDown(self)
        if os.path.exists("test.conf"):
            os.remove("test.conf")
        if os.path.exists("argdumper"):
            os.remove("argdumper")

    def test_basic_templating(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.lease", 1440)
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("network.dhcp.breakfast", "waffles")
        # fulfilled for the service which isn't tested here
        client.set("my.service.restarter", True)
        client.commit()
        self.assertFileExistsTimeout("test.conf")
        self.assertRenderedTemplateMatches(
"""dhcp-range=192.168.1.1,192.168.1.100,1440
dhcp-authoritative
interfaces=eth0,eth1,
breakfast=waffles
""", "test.conf")


class ServiceSysAdminTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self, "test/configs/hooksConfig.yaml")

    def tearDown(self):
        SysAdminFixture.tearDown(self)
        if os.path.exists("test.conf"):
            os.remove("test.conf")
        if os.path.exists("argdumper"):
            os.remove("argdumper")

    def test_basic_service(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        # the following are fulfilled for the template that isn't tested here
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit(sysadminctl_pb2.NO_HOOKS)
        self.assertFileDoesNotExistTimeout("argdumper")
        client.set("network.dhcp.endip", "192.168.1.99")
        client.commit(sysadminctl_pb2.TEMPLATE_ONLY)
        self.assertFileDoesNotExistTimeout("argdumper")
        client.set("network.dhcp.endip", "192.168.1.98")
        client.commit(sysadminctl_pb2.DEFAULT)
        self.assertFileExistsTimeout("argdumper")
        self.assertArgsDumpedProperly("network.dhcp.endip")  # noqa


class FireAllHooksTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self, "test/configs/hooksConfig.yaml")

    def tearDown(self):
        SysAdminFixture.tearDown(self)
        if os.path.exists("test.conf"):
            os.remove("test.conf")
        if os.path.exists("argdumper"):
            os.remove("argdumper")

    def test_fire_hooks(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit()
        self.assertFileExistsTimeout("test.conf")
        self.assertFileExistsTimeout("argdumper")
        # remove the files dumped by the commit hook firing
        os.remove("test.conf")
        os.remove("argdumper")
        client.firehooks()
        self.assertFileExistsTimeout("test.conf")
        self.assertFileExistsTimeout("argdumper")
        self.assertRenderedTemplateMatches(
"""dhcp-range=192.168.1.1,192.168.1.100,1440
dhcp-authoritative
interfaces=eth0,eth1,
breakfast=waffles\n""", "test.conf")
        self.assertArgsDumpedProperly("network.dhcp.startip network.dhcp.endip network.dhcp.interfaces my.service.restarter network.dhcp.breakfast network.dhcp.lease")  # noqa

    def test_fire_hooks_timeout(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit()
        client.socket.settimeout(0.001)
        self.assertRaises(socket.timeout, client.firehooks)


class FetchingTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_fetching(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit()
        output = FetchAllValues(["network.dhcp.startip", "network.dhcp.lease",
                                 "network.dhcp.interfaces"])
        self.assertEqual(output, {"network": {"dhcp": {"startip": "192.168.1.1",
                                                       "lease": 1440,
                                                       "interfaces": [u"eth0", u"eth1"]}}})


class GetAllKeysTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_get_all_keys(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.breakfast", "waffles")
        client.set("network.lease", 1440)
        client.commit()
        resp = set(map(lambda x: x.key, client.get("network.dhcp.*").get.kvs))
        self.assertEqual(set(["network.dhcp.startip", "network.dhcp.endip",
                              "network.dhcp.interfaces"]),
                         resp)
        resp = map(lambda x: UnpackFromProto(x.value), client.get("network.dhcp.*").get.kvs)
        self.assertEqual(3, len(resp))
        self.assertTrue("192.168.1.1" in resp)
        self.assertTrue("192.168.1.100" in resp)
        self.assertTrue(["eth0", "eth1"] in resp)
        resp = set(map(lambda x: x.key, client.get("network.*").get.kvs))
        self.assertEqual(set(["network.dhcp.startip", "network.dhcp.endip",
                              "network.dhcp.interfaces", "network.breakfast",
                              "network.lease"]),
                         resp)
        resp = map(lambda x: UnpackFromProto(x.value), client.get("network.*").get.kvs)
        self.assertEqual(5, len(resp))
        self.assertTrue("192.168.1.1" in resp)
        self.assertTrue("192.168.1.100" in resp)
        self.assertTrue(["eth0", "eth1"] in resp)
        self.assertTrue("waffles" in resp)
        self.assertTrue(1440 in resp)


class MigrationTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_new_key_migration(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.breakfast", "waffles")
        client.set("network.lease", 1440)
        client.commit()
        m = SysAdminMigrator(client)
        m.new_keys({"new.key": "value"})
        client.commit()
        self.assertEqual("value", UnpackFromProto(client.get("new.key").get.kvs[0].value))
        m.change_values({"new.key": "4"})
        client.commit()
        self.assertEqual("4", UnpackFromProto(client.get("new.key").get.kvs[0].value))
        m.change_types({"new.key": "int32"})
        client.commit()
        self.assertEqual(4, UnpackFromProto(client.get("new.key").get.kvs[0].value))
        m.rename_keys([{"new.key": "renamed.key"}])
        client.commit()
        self.assertEqual(4, UnpackFromProto(client.get("renamed.key").get.kvs[0].value))
        m.remove_keys(["new.key"])
        client.commit()
        self.assertEqual(4, UnpackFromProto(client.get("renamed.key").get.kvs[0].value))

    def test_change_types(self):
        client = SysAdminClient("127.0.0.1", 4000)
        m = SysAdminMigrator(client)
        m.new_keys({"gonna.be.list": "value"})
        client.commit()
        m.change_types({"gonna.be.list": "stringlist"})
        client.commit()
        self.assertEqual(["value"], UnpackFromProto(client.get("gonna.be.list").get.kvs[0].value))
        m.change_types({"gonna.be.list": "str"})
        client.commit()
        self.assertEqual("value", UnpackFromProto(client.get("gonna.be.list").get.kvs[0].value))
        m.new_keys({"gonna.be.intlist": "4"})
        client.commit()
        m.change_types({"gonna.be.intlist": "int32list"})
        client.commit()
        self.assertEqual([4], UnpackFromProto(client.get("gonna.be.intlist").get.kvs[0].value))
        m.change_types({"gonna.be.intlist": "str"})
        client.commit()
        self.assertEqual("4", UnpackFromProto(client.get("gonna.be.intlist").get.kvs[0].value))
        m.new_keys({"gonna.be.boollist": "true"})
        client.commit()
        m.change_types({"gonna.be.boollist": "boollist"})
        client.commit()
        self.assertEqual([True], UnpackFromProto(client.get("gonna.be.boollist").get.kvs[0].value))
        m.change_types({"gonna.be.boollist": "str"})
        client.commit()
        self.assertEqual("True", UnpackFromProto(client.get("gonna.be.boollist").get.kvs[0].value))
        m.new_keys({"long.list": ["stringlist", "4", "5", "6"]})
        client.commit()
        m.change_types({"long.list": "int32list"})
        client.commit()
        self.assertEqual([4, 5, 6], UnpackFromProto(client.get("long.list").get.kvs[0].value))
        m.change_types({"long.list": "int32list"})
        client.commit()
        self.assertEqual([4, 5, 6], UnpackFromProto(client.get("long.list").get.kvs[0].value))
        m.change_types({"long.list": "stringlist"})
        client.commit()
        self.assertEqual(["4", "5", "6"], UnpackFromProto(client.get("long.list").get.kvs[0].value))


class ClientIdTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_overridden_xid(self):
        # This client gets the regular sysadmin assigned id
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        # This client has a overridden id 0
        otherclient = SysAdminClient("127.0.0.1", 4000, 0)
        # This client can't commit the original clients changes
        otherclient.commit()
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("network.dhcp.startip").status)
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("network.dhcp.endip").status)
        client.commit()
        self.assertEqual("192.168.1.1", UnpackFromProto(client.get("network.dhcp.startip").get.kvs[0].value))
        self.assertEqual("192.168.1.100", UnpackFromProto(client.get("network.dhcp.endip").get.kvs[0].value))


class RollbackTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_basic_rollback(self):
        # This client gets the regular sysadmin assigned id
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        self.assertEqual(1, client.commit().commit.commit_id)
        client.set("network.dhcp.startip", "10.0.0.1")
        client.set("network.dhcp.endip", "10.0.0.100")
        self.assertEqual(2, client.commit().commit.commit_id)
        # Can't rollback the first commit
        self.assertEqual(sysadminctl_pb2.FAILED_ROLLBACK, client.rollback(1).status)
        self.assertEqual(sysadminctl_pb2.SUCCESS, client.rollback(2).status)
        self.assertEqual("192.168.1.1", UnpackFromProto(client.get("network.dhcp.startip").get.kvs[0].value))
        self.assertEqual(sysadminctl_pb2.FAILED_ROLLBACK, client.rollback(100).status)

    def test_new_key_rollback(self):
        #make a commit with a new key and value, ensure that a rollback can remove the key
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("cheese", 42)
        self.assertEqual(1, client.commit().commit.commit_id)
        self.assertEqual(42, UnpackFromProto(client.get("cheese").get.kvs[0].value))
        self.assertEqual(sysadminctl_pb2.SUCCESS, client.rollback(1).status)
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, client.get("cheese").status)


class ResetTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_new_key_migration(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.breakfast", "eggs")
        client.set("network.lease", 1440)
        client.set("default.network.breakfast", "waffles")
        client.set("default.network.lease", 1440)
        client.commit()

        client.reset()
        resp = client.get("network.dhcp.startip")
        self.assertEqual(sysadminctl_pb2.KEY_NOT_FOUND, resp.status)
        self.assertEqual("waffles", UnpackFromProto(client.get("network.breakfast").get.kvs[0].value))


class DumpHooksTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self, "test/configs/hooksConfig.yaml")

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_dump_hooks(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit()
        resp = client.dumphooks()
        self.assertEqual(sysadminctl_pb2.SUCCESS, resp.status)
        self.assertEqual("test.conf", resp.dump.templatehooks[0])
        self.assertEqual("testService", resp.dump.servicehooks[0])


class TriggerTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self, "test/configs/hooksConfig.yaml")

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_trigger(self):
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.dhcp.breakfast", "waffles")
        client.set("network.dhcp.lease", 1440)
        client.commit()
        self.assertEqual(sysadminctl_pb2.SUCCESS, client.trigger("test.conf").status)
        self.assertEqual(sysadminctl_pb2.SUCCESS, client.trigger("testService").status)
        self.assertEqual(sysadminctl_pb2.HOOK_NOT_FOUND, client.trigger("nonExist").status)


class ClientWrapper(SysAdminClient):

    def __init__(self):
        super(ClientWrapper, self).__init__("127.0.0.1", 4000)
        self.call_counts = {
            "get": 0,
        }

    def get(self, key):
        self.call_counts["get"] = self.call_counts["get"] + 1
        return super(ClientWrapper, self).get(key)


class LazySysAdminTest(SysAdminFixture):
    def setUp(self):
        SysAdminFixture.setUp(self)
        client = SysAdminClient("127.0.0.1", 4000)
        client.set("network.dhcp.startip", "192.168.1.1")
        client.set("network.dhcp.endip", "192.168.1.100")
        client.set("network.dhcp.interfaces", ["eth0", "eth1"])
        client.set("my.service.restarter", True)
        client.set("network.breakfast", "eggs")
        client.set("network.lease", 1440)
        client.set("default.network.breakfast", "waffles")
        client.set("default.network.lease", 1440)
        client.set("loops.one", 1)
        client.set("loops.two", 2)
        client.set("loops.three", 3)

        client.commit()

    def tearDown(self):
        SysAdminFixture.tearDown(self)

    def test_iteration(self):
        s = LazySysAdmin(ClientWrapper())
        # This is an interesting thing to test because the act of iteration should
        # save all the namespaces values in the lazy dictionary, and not require
        # further fetches from sysadmin to access the sub values
        for _ in s["network"]:
            pass
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        s["network"]
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        self.assertEqual("192.168.1.1", s["network"]["dhcp"]["startip"])
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        self.assertEqual("192.168.1.1", s["network"]["dhcp"]["startip"])
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        self.assertEqual("192.168.1.100", s["network"]["dhcp"]["endip"])
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        self.assertEqual(["eth0", "eth1"], s["network"]["dhcp"]["interfaces"])
        self.assertEqual(2, s.sysadmin.call_counts["get"])
        for _ in s["network"]["dhcp"]:
            pass
        self.assertEqual(3, s.sysadmin.call_counts["get"])

    def test_iteritems(self):
        s = LazySysAdmin(ClientWrapper())
        loop = s["loops"]
        i = 0
        for key, value in loop.iteritems():
            i += 1
            if key == "one":
                self.assertEqual(value, 1)
            elif key == "two":
                self.assertEqual(value, 2)
            else:
                self.assertEqual(value, 3)

        self.assertEqual(i, 3)
        self.assertEqual(2, s.sysadmin.call_counts["get"])

        for key, value in s.iteritems():
            self.assertEqual(s[key], value)

    def test_list_iteration(self):
        s = LazySysAdmin(ClientWrapper())
        for iface in s["network"]["dhcp"]["interfaces"]:
            # just confirming I'm getting a nice string value out of this iteration
            self.assertTrue("eth" in iface)

    def test_looping(self):
        s = LazySysAdmin(ClientWrapper())
        counter = 0
        for key in s:
            counter += 1
        self.assertEqual(11, counter)
        all_values = [
            "192.168.1.1",
            "192.168.1.100",
            ["eth0", "eth1"],
        ]
        for key in s["network"]["dhcp"]:
            value = s["network"]["dhcp"][key]
            all_values.remove(value)
        self.assertEqual(0, len(all_values))
        for key in s["network"]:
            if key == "dhcp":
                self.assertTrue(s["network"].is_namespace(key))
                for dk in s["network"][key]:
                    self.assertTrue(s["network"][key].is_value(dk))
            else:
                self.assertTrue(s["network"].is_value(key))

    def test_identifiers(self):
        s = LazySysAdmin(ClientWrapper())
        self.assertTrue(s["network"].is_namespace("dhcp"))
        self.assertFalse(s["network"].is_value("dhcp"))
        self.assertTrue(s["network"]["dhcp"].is_value("startip"))
        self.assertFalse(s["network"]["dhcp"].is_namespace("startip"))

    def test_get_default(self):
        s = LazySysAdmin(ClientWrapper())
        self.assertEqual("192.168.1.1", s["network"]["dhcp"].get("startip", ""))
        self.assertEqual("192.168.1.100", s["network"]["dhcp"].get("endip", ""))
        self.assertEqual(["eth0", "eth1"], s["network"]["dhcp"].get("interfaces", ""))
        self.assertFalse(s["network"]["dhcp"].get("notreal", False))
        self.assertTrue(s["network"].get("dhcp", True))

    def test_prefetch(self):
        s = LazySysAdmin(ClientWrapper())
        s["network"].prefetch()
        call_count = s.sysadmin.call_counts["get"]
        self.assertEqual("192.168.1.1", s["network"]["dhcp"]["startip"])
        self.assertEqual(call_count, s.sysadmin.call_counts["get"])
        self.assertEqual("192.168.1.1", s["network"]["dhcp"]["startip"])
        self.assertEqual(call_count, s.sysadmin.call_counts["get"])
        self.assertEqual("192.168.1.100", s["network"]["dhcp"]["endip"])
        self.assertEqual(True, s["my"]["service"]["restarter"])
        self.assertLess(call_count, s.sysadmin.call_counts["get"])
        s.prefetch()
        call_count = s.sysadmin.call_counts["get"]
        self.assertEqual("waffles", s["default"]["network"]["breakfast"])
        self.assertEqual(call_count, s.sysadmin.call_counts["get"])
