#!/bin/env python
# encoding: utf-8

import os


ip2int = lambda ip: sum([int(f) * (256**(3-i)) for i, f in enumerate(ip.strip().split('.'))])


class IP2Location(object):

    def __init__(self, dict_path, callback=None):
        if not os.path.exists(dict_path):
            raise ValueError("The dict_path must exist")

        self._ip = []

        with open(dict_path) as f:
            for line in f:
                if callback:
                    start, end, country = callback(line)
                    self._ip.append((ip2int(start), ip2int(end), country))
                else:
                    fields = line.strip().split('|')
                    start, end, country = fields[0], fields[1], fields[2]
                    self._ip.append((ip2int(start), ip2int(end), country))

    def get_country(self, ip):
        ip_int = ip2int(ip)
        floor = 0
        ceil = len(self._ip) - 1
        mid = 0
        while floor <= ceil:
            mid = (floor + ceil) / 2
            if self._ip[mid][0] <= ip_int <= self._ip[mid][1]:
                return self._ip[mid][2]
            if ip_int > self._ip[mid][0]:
                floor = mid + 1
            else:
                ceil = mid - 1

    def cleanup(self):
        self._ip = []


if __name__ == '__main__':
    ip = IP2Location('../dict/czip.txt')
    print ip.get_country("180.214.232.50")
    raw_input("xxx")

