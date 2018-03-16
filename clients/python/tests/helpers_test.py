import unittest

import pytz
from sysadmin_helpers import helpers


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
