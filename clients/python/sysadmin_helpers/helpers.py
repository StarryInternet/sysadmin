import datetime

import pytz

DAYS_OF_THE_WEEK = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']


def day_id_to_iptables_name(n):
    return DAYS_OF_THE_WEEK[n]


def time_to_hours_minutes(time):
    hours = time // 60
    minutes = time % 60
    return (hours, minutes)


def minutes_to_time(hours, minutes):
    return "%02d:%02d" % (hours, minutes)


def adjust_with_timezone(time, tzinfo):
    hour, minute = time_to_hours_minutes(time)
    d = datetime.datetime.now()
    d = d.replace(
        hour=hour,
        minute=minute,
        second=0,
        microsecond=0,
    )
    start_day = d.day
    d = tzinfo.localize(d)
    d = d.astimezone(pytz.timezone('UTC'))
    new_day = d.day
    day_offset = new_day - start_day
    # handle cases where the day wraps
    if day_offset > 1:
        day_offset = -1
    elif day_offset < -1:
        day_offset = 1
    return (d.hour, d.minute, day_offset)
