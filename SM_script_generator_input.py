# Copyright (c) 2015 Velocitek Inc. All rights reserved.
# Author(s): Gideon Grossman
#
# Please follow the Velocitek C Style Guide when maintaining this code.
# http://www.velocitek.com/style-guide/c_style_guide.xml
#
# This code automatically generates .c and .h state machine files according to
# user input

import sm_script_generator as a
from sm_script_generator import *
reload(a)
from sm_script_generator import *

# Module variables -------------------------------------------------------------

# Enter the path where you'd like to create .c and .h files
# e.g. 'C:\Users\Velocitek\Documents\Gideon\Code\Style-Scripts
# \State-Machine-Script-Generator'
c_file_path = 'C:\Users\Velocitek\Documents\Gideon\Code\Style-Scripts\
\State-Machine-Script-Generator'

# Enter the name you'd like to call the .c and .h files
# e.g. if c_filename = "master", then the code will generate files
# named SMmaster.c and SMmaster.h
c_filename = 'master'

# Enter the year, author(s) and state machine states
year = '2015'   
#e.g. '2015'
authors = ['Gideon Grossman', 'Alec Stewart']   
#e.g. ['John Smith', 'Mohammed Gupta']
states  = ['State_1', 'State_2', 'State_3']     
#e.g. ['Sleep', 'Calibration', 'Normal_Operation']

# Enter the path and filename of the text file containing the transition events
events_file_path = 'C:/Users/Velocitek/Documents/Gideon/\
Code/Style-Scripts/State-Machine-Script-Generator'
# e.g. events_file_path = 'C:/Users/Velocitek/Documents/Gideon
# /Code/Style-Scripts/State-Machine-Script-Generator'
events_filename = 'Events_for_master.txt'
# e.g. events_filename = 'Events_for_master.txt'

# Enter the header files to include
includes = ['main.h', 'avr_compiler.h', 'shiftLcd.h', 'SMevents.h'] 
#e.g. includes = ['main.h', 'avr_compiler.h', 'shiftLcd.h', 'SMevents.h']

# Module Code ------------------------------------------------------------------
# Generates the .c and .h files with the parameters defined above 
# and in the events .txt file
script_generator_object = SM_script_generator(events_file_path,
                                            events_filename, 
                                            c_file_path, 
                                            c_filename, 
                                            year, 
                                            authors, 
                                            states, 
                                            includes)
script_generator_object.GenerateScript()
