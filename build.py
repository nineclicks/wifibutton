#!/usr/bin/env python
import os
import re
import subprocess
import shlex

def run(command):
    cmd = shlex.split(command)
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, bufsize=1)
    for line in iter(p.stdout.readline, b''):
        print line,
    p.stdout.close()
    p.wait()

with open('wifibutton.ino', 'r') as myfile:
    data=myfile.read()
matches = re.findall('::(.*?)::', data)

for m in matches:
    with open(m, 'r') as myfile:
	include = myfile.read().replace('\n', '\\n\\\n').replace('"','\\"')
    data=data.replace('::' + m + '::', include)

if not os.path.isdir('temp'):
    os.makedirs('temp')
file = open('temp/temp.ino', 'w')
file.write(data)
file.close()

run('arduino --upload --preserve-temp-files temp/temp.ino')
