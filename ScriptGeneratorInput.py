import re
import script_generator as a
from script_generator import *

reload(a)
from script_generator import *

events_file_path = "C:/Users/Velocitek/Documents/Gideon/Code/Style-Scripts/State-Machine-Script-Generator"
events_filename = "Events.txt"
events_file = open('%s/%s' %(events_file_path, events_filename) ,'r')
events_file_content = events_file.readlines()

c_file_path = "C:/Users/Velocitek/Documents/Gideon/Code"
c_filename = "script_generated"
year = '2015'
authors = ['Gideon Grossman', 'Alec Stewart']
states  = ['State_1', 'State_2', 'State_3']

# Creates a dictionary containing all transition events
# The keys are the source states
# Each value is an array of subarrays. Each subarray has two elements: [transition event, destination state]
events = {}
for state in states:
    events[state] = []
    for line in events_file_content:
        if line.startswith(state):
            split_line = re.split(', |\n', line)
            events[state].append([split_line[1], split_line[2]])

script_generator_object = script_generator(c_file_path, c_filename, year, authors, states, events)
script_generator_object.GenerateScript()

