#!/usr/bin/env python
# script to retry finished (partially failed) jobs

from __future__ import print_function
import datetime
import pandatools.Client
import re
import os

# debug
one_only = False
dosubmit = True

# define what should be retried
user = os.getenv('RUCIO_ACCOUNT')
campaign = 'schannel_App_MemTk'
# version = r'v29lj_MC16.*\.v0'
# version = r'v29lj_MC16.*\.v.*'
# version = r'v29lj_MC16d\.v1'
version = r'v29lj_MC16[ade]_syst.v11'
use_status = False
statuses = ['finished']  # , 'exhausted', 'aborted']
use_failed = True
# statuses = ['finished', 'exhausted']  # , 'aborted']
# earliest_date = datetime.datetime(2019, 11, 5)
earliest_date = datetime.datetime(2020, 1, 25)
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
    finished_dict = campaign_dict
    print('found {} jobs for search "{}"'.format(len(finished_dict), search_string))
    if use_status:
        finished_dict = {k: v for k, v in finished_dict.items()
                         if v['status'] == status}
        print('found {} jobs in state "{}"'.format(len(finished_dict), status))
    if use_failed:
        finished_dict = {k: v for k, v in finished_dict.items()
                         if v['statistics'].find('failed') != -1}
        print('found {} jobs with failed subjobs'.format(len(finished_dict)))

    # retry tasks (keys are panda job IDs, tasks are from the jediTaskID keyword)
    for JobID in finished_dict.keys():
        task = finished_dict[JobID]['jediTaskID']
        print('Retrying task with ID {} and name {}'.format(
            task, finished_dict[JobID]['taskName']))
        if dosubmit:
            pandatools.Client.retryTask(task, verbose=True, properErrorCode=True)
        if one_only:
            break

# other
# has_failed = {k: v for k, v in campaign_dict.items() if v['statistics'].find('failed') != -1}
