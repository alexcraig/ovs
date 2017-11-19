#!/usr/bin/env python
import os
import sys
import signal
from subprocess import call

ps_out = os.popen('ps -e | grep ovs')
for line in ps_out:
    print line,
    line_split = line.strip().split(' ')
    try:
        os.kill(int(line_split[0]), signal.SIGTERM)
    except Exception as e:
        print str(e.strerror)
