# generateInput.py

import sys
import random
import configparser

def getEntries():
    entries = []
    
    conf = configparser.ConfigParser() 
    with open('prices.conf', 'r') as file:
        conf_string = '[section]\n' + file.read()
       
    conf.read_string(conf_string)   

    for key in conf['section']:
        if key != 'budget' and key != 'threshold':
            entries.append(key.upper())

    return entries

def createCommand():
    action = random.choice(actions)
    stock = random.choice(entries)
    
    increment = (max_shares - min_shares) / interval
    shares = min_shares + 5 * random.randint(0, increment)

    safe = random.choice(safety)

    return action + ' ' + stock + ' ' + repr(shares) + ' ' + safe + '\n'

actions = ['B', 'S']
entries = getEntries()

min_shares = 10
max_shares = 400
interval = 5

safety = ['S', 'U']
    
if len(sys.argv) != 2:
    print('Usage: python[version] generateInput.py (number of commands to generate)')
    sys.exit(1)

command_count = int(sys.argv[1])

with open('input.txt', 'w') as input_file:
    for i in range(command_count + 1):
        input_file.write(createCommand())
    
