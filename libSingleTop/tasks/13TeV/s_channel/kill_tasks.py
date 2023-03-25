#!/usr/bin/env python
# script to retry finished (partially failed) jobs

from __future__ import print_function
import datetime
import pandatools.Client
import re
import os

# debug
one_only = False
dosubmit = False
printout = True
printoutfile = 'tasks_to_abort.txt'

# define what should be retried
user = os.getenv('RUCIO_ACCOUNT')
campaign = 'schannel_App_MemTk'
# version = r'v29lj_MC16.*\.v0'
# version = r'v29lj_MC16.*\.v.*'
version = r'v29lj_MC16d\.v1'
# statuses = ['finished']  # , 'exhausted', 'aborted']
# statuses = ['finished', 'exhausted']  # , 'aborted']
statuses = ['running', 'throttled']
earliest_date = datetime.datetime(2019, 11, 5)
search_string = r'user\.{}\.{}\..*_{}/?$'.format(user, campaign, version)

# allowed users
users = ['cappelt', 'gjakel', 'kkreul', 'lohse', 'natlay', 'prieck', 'pseema', 'skaphle', 'smergelm']
user_full_names = ['Christian Appelt', 'Gunnar Jaekel', 'Ken Matthias Kreul',
                   'Thomas Lohse', 'Naim Bora Atlay', 'Patrick Rieck',
                   'Pienpen Seema-Mergelmeyer', 'Stephan Kaphle',
                   'Sebastian Mergelmeyer']
full_name = dict(zip(users, user_full_names))

# get tasks for user
# somehow the syntax changed in v29??
print('Getting tasks since {} for user {}...'.format(earliest_date, user))
state, jediTask_dict = pandatools.Client.\
    getJobIDsJediTasksInTimeRange(earliest_date, dn=full_name[user])

if state != 0:
    print('Something went wrong; check grid proxy')
    exit(1)

# filter for campaign/version
campaign_dict = {k: v for k, v in jediTask_dict.items()
                 if re.findall(search_string, v['taskName'])}

for status in statuses:
    # filter for finished tasks
    finished_dict = {k: v for k, v in campaign_dict.items()
                     if v['status'] == status}

    print('found {} jobs in state "{}"'.format(len(finished_dict), status))

    # retry tasks (keys are panda job IDs, tasks are from the jediTaskID keyword)
    for JobID in sorted(finished_dict.keys()):
        task = finished_dict[JobID]['jediTaskID']
        print('Killing task with ID {} and name {}'.format(
            task, finished_dict[JobID]['taskName']))
        if printout:
            with open(printoutfile, 'a') as pof:
                pof.write('{} {}\n'.format(task, finished_dict[JobID]['taskName']))
        if dosubmit:
            pandatools.Client.killTask(task, verbose=True)
        if one_only:
            break
