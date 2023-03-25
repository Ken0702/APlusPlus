#!/usr/bin/env python
# script to retry finished (partially failed) jobs

from __future__ import print_function
import pandatools.Client

# debug
one_only = False
dosubmit = True
printoutfile = 'tasks_to_abort.txt'
tasks = {}

with open(printoutfile) as pof:
    for line in pof:
        if not line or line.startswith('#') or line.isspace():
            continue
        words = line.split()
        tasks[words[0]] = words[1]

for task in sorted(tasks.keys()):
    print('Killing task with ID {} and name {}'.format(
        task, tasks[task]))
    if dosubmit:
        pandatools.Client.killTask(task, verbose=True)
    if one_only:
        break
