import script_generator_for_master as a
from script_generator_for_master import *

reload(a)
from script_generator_for_master import *

# Enter the path where you'd like to create .c and .h files
c_file_path = "C:\Users\Velocitek\Documents\Gideon\Code\Style-Scripts\State-Machine-Script-Generator"

# Enter the name you'd like to call the .c and .h files
c_filename = "master"


year = '2015'   #e.g. '2015'
authors = ['Gideon Grossman', 'Alec Stewart']   #e.g. ['John Smith', 'Mohamed Gupta']
states  = ['State_1', 'State_2', 'State_3']     #e.g. ['Sleep', 'Calibration', 'Normal_Operation']

# Enter the path and filename of the text file containing the transition events
events_file_path = "C:/Users/Velocitek/Documents/Gideon/Code/Style-Scripts/State-Machine-Script-Generator"
events_filename = "Events_for_master.txt"

# Header files to include
includes = ['main.h', 'avr_compiler.h', 'shiftLcd.h', 'events.h']

# Generate the .c and .h files with the parameters defined above and in the events .txt file
script_generator_object = script_generator(events_file_path, events_filename, c_file_path, c_filename, year, authors, states, includes)
script_generator_object.GenerateScript()

