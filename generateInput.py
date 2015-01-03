# generateInput.py

import sys
import re

def getKeyValuePair(line):
    ''' Gets key-value pair from line '''
    

def getEntries():
    entries = []
    
    with open("prices.conf", "r") as conf:
        lines = conf.readlines();
    
        for line in lines:
            dividedLine = re.split("=", line)

            if len(dividedLine) >= 1:
                entries.append(dividedLine[0])
        
    return entries




