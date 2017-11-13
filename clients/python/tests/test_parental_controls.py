import json
import unittest

import pytz
from sysadmin import helpers


class TestHelpers(unittest.TestCase):

    def test_adjust_with_timezone(self):
        tz = pytz.timezone('Asia/Jakarta')  # no DST +7:00 TZ
        new_hour, new_minute, day_offset = helpers.adjust_with_timezone(0, tz)
        self.assertEqual(new_hour, 17)
        self.assertEqual(new_minute, 0)
        self.assertEqual(day_offset, -1)

    def test_adjust_with_timezone_utc(self):
        tz = pytz.timezone('UTC')
        new_hour, new_minute, day_offset = helpers.adjust_with_timezone(0, tz)
        self.assertEqual(new_hour, 0)
        self.assertEqual(new_minute, 0)
        self.assertEqual(day_offset, 0)

    def test_parse_parental_control_rule(self):
        rule = {
            'start_time': 0,
            'end_time': 240,
            'day': 0,
            'mac_address': 'ABCDEF123456',
        }
        (mac, start, end, day) = \
            helpers.parse_parental_control_rule(json.dumps(rule), 'UTC')
        self.assertEqual(mac, 'ABCDEF123456')
        self.assertEqual(start, '00:00')
        self.assertEqual(end, '04:00')
        self.assertEqual(day, 'Sun')

    def test_parse_parental_control_rule_underflow(self):
        rule = {
            'start_time': 0,
            'end_time': 240,
            'day': 0,
            'mac_address': 'ABCDEF123456',
        }
        (mac, start, end, day) = helpers.parse_parental_control_rule(
            json.dumps(rule),
            'Asia/Jakarta'  # no DST +7:00 TZ
        )
        self.assertEqual(mac, 'ABCDEF123456')
        self.assertEqual(start, '17:00')
        self.assertEqual(end, '21:00')
        self.assertEqual(day, 'Sat')

    def test_parse_parental_control_rule_partial_underflow(self):
        rule = {
            'start_time': 0,
            'end_time': 20 * 60,
            'day': 0,
            'mac_address': 'ABCDEF123456',
        }
        (mac, start, end, day) = helpers.parse_parental_control_rule(
            json.dumps(rule),
            'Asia/Jakarta'  # no DST +7:00 TZ
        )
        self.assertEqual(mac, 'ABCDEF123456')
        self.assertEqual(start, '17:00')
        self.assertEqual(end, '13:00')
        self.assertEqual(day, 'Sat')

    def test_parse_parental_control_rule_overflow(self):
        rule = {
            'start_time': 21 * 60,
            'end_time': 23 * 60,
            'day': 0,
            'mac_address': 'ABCDEF123456',
        }
        (mac, start, end, day) = helpers.parse_parental_control_rule(
            json.dumps(rule),
            'America/Eirunepe'  # no DST +5:00 TZ
        )
        self.assertEqual(mac, 'ABCDEF123456')
        self.assertEqual(start, '02:00')
        self.assertEqual(end, '04:00')
        self.assertEqual(day, 'Mon')

    def test_parse_parental_control_rule_partial_overflow(self):
        rule = {
            'start_time': 16 * 60,
            'end_time': 23 * 60,
            'day': 0,
            'mac_address': 'ABCDEF123456',
        }
        (mac, start, end, day) = helpers.parse_parental_control_rule(
            json.dumps(rule),
            'America/Eirunepe'  # no DST +5:00 TZ
        )
        self.assertEqual(mac, 'ABCDEF123456')
        self.assertEqual(start, '21:00')
        self.assertEqual(end, '04:00')
        self.assertEqual(day, 'Sun')
