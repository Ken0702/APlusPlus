#!/usr/bin/env python
# script to retry finished (partially failed) jobs

from __future__ import print_function
import datetime
import pandatools.Client
import re

#
# start change here
#
user = 'skaphle'
# version = 'v29_a_nominal.v0_nominal'
# version = 'v25_d.v2'
version = 'v29lj_MC16a.v0'
one_only = False
status = 'finished'
# status = 'exhausted'
dosubmit = True
#
# end change here
#

# general settings
earliest_date = datetime.datetime(2019, 11, 5)
campaign = 'schannel_App_MemTk'
rversion = version.replace('.', r'\.')
users = ['cappelt', 'gjakel', 'kkreul', 'lohse', 'natlay', 'prieck', 'pseema', 'skaphle', 'smergelm']
user_full_names = ['Christian Appelt', 'Gunnar Jaekel', 'Ken Matthias Kreul',
                   'Thomas Lohse', 'Naim Bora Atlay', 'Patrick Rieck',
                   'Pienpen Seema-Mergelmeyer', 'Stephan Kaphle',
                   'Sebastian Mergelmeyer']
full_name = dict(zip(users, user_full_names))

# get tasks for user
# somehow the syntax changed in v29??
print('Getting tasks since {} for user {}...'.format(earliest_date, user))
#state, jediTask_list, jediTask_dict = pandatools.Client.\
#    getJobIDsJediTasksInTimeRange(earliest_date, dn=full_name[user])
state, jediTask_dict = pandatools.Client.\
    getJobIDsJediTasksInTimeRange(earliest_date, dn=full_name[user])

if state != 0:
    print('Something went wrong; check grid proxy')
    exit(1)

# filter for campaign/version
campaign_dict = {k: v for k, v in jediTask_dict.items()
                 if re.findall(r'user\.{}\.{}\..*_{}/?$'.format(user, campaign, rversion), v['taskName'])}

# filter for finished tasks
finished_dict = {k: v for k, v in campaign_dict.items()
                 if v['status'] == status}

print('found {} jobs in state "{}"'.format(len(finished_dict), status))

# retry tasks (keys are panda job IDs, tasks are from the jediTaskID keyword)
for JobID in finished_dict.keys():
    task = finished_dict[JobID]['jediTaskID']
    print('Retrying task with ID {} and name {}'.format(
        task, finished_dict[JobID]['taskName']))
    if dosubmit:
        pandatools.Client.retryTask(task, verbose=True, properErrorCode=True)
    if one_only:
        break
