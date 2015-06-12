#from enum import Enum
import re


def CapitalizeFirstLettersRemoveUnderscores(word_to_capitalize):
    new_word = word_to_capitalize[0].upper()
    index = 1
    while (index < len(word_to_capitalize)):
        letter = word_to_capitalize[index]
        if word_to_capitalize[index - 1] == '_':
            new_word = new_word + letter.upper()
        elif letter != '_':
            new_word = new_word + (letter)
        #elif letter == '_':
        #    x = 4
        index += 1
    return new_word
    
def PrintPrivateDuringFunctions(h, states):
    h.write('// Private During Functions ' + '-' * 52 + '\n')
    for state in states:
        h.write('static void During' + CapitalizeFirstLettersRemoveUnderscores(state) + '(uint8_t event)\n')
        h.write('{\n\n  if (event == EV_ENTRY)\n  {\n\n  }\n')
        h.write('  else if (event == EV_EXIT)\n  {\n\n  }\n')
        h.write('  else\n  {\n\n  }\n}\n\n')
        
class script_generator:
    
    # Creates a dictionary containing all transition events
    # The keys are the source states
    # Each value is an array of subarrays. Each subarray has two elements: [transition event, destination state]
    def CreateDictionaryOfTransitionEvents(self):
        events_file = open('%s/%s' %(self.events_path, self.events_filename) ,'r')
        events_file_content = events_file.readlines()

        self.events = {}
        for state in self.states:
            self.events[state] = []
            for line in events_file_content:
                if line.startswith(state):
                    split_line = re.split(', |\n', line)
                    self.events[state].append([split_line[1], split_line[2]])
        events_file.close()
        return self.events

    def __init__(self, events_path, events_filename, c_path, c_filename, year, authors, states, includes):
        self.events_path = events_path
        self.events_filename = events_filename
        self.path = c_path
        self.filename = c_filename
        self.year = year
        self.authors = authors
        self.states = states
        self.events = self.CreateDictionaryOfTransitionEvents()
        self.includes = includes
        
    
    def PrintTransitionStatements(self, state, implementation_file):
        if self.events[state]:
            for event in self.events[state]:
                implementation_file.write('          case EV_' + event[0].upper() + ' :\n')
                implementation_file.write('            nextState = ' + event[1].upper() + ';\n            break;\n\n')
        else:
            implementation_file.write('        }\n')
        
    def CreateHeaderFile(self):
        f = open('%s/SM%s.h' %(self.path, self.filename) ,'w')
        capitalized_states = []
        for state in self.states:
            capitalized_states.append(state.upper())
        boilerplate_comments = '// Copyright (c) '+self.year+' Velocitek Inc. All rights reserved.\n\
    //\n\
    // Please follow the Velocitek C Style Guide when maintaining this code.\n\
    // http://www.velocitek.com/style-guide/c_style_guide.xml\n\n'
        
        header_guard = '#ifndef SM%s_H\n#define SM%s_H\n\n' %(self.filename.upper(), self.filename.upper())
        
        enum_type_definition = 'typedef enum\n{\n'
        for state in capitalized_states:
            enum_type_definition = enum_type_definition + '%s_STATE,\n' %(state)
        enum_type_definition = enum_type_definition + '}\n'
        
        capitalized_filename_no_underscores = CapitalizeFirstLettersRemoveUnderscores(self.filename)
        f.write(boilerplate_comments)
        f.write(header_guard)
        f.write('#include "events.h"\n\n')
        f.write(enum_type_definition)
        f.write(capitalized_filename_no_underscores + 'States;\n\n')
        f.write('uint8_t Run' + capitalized_filename_no_underscores + 'SM(uint8_t current_event);\n')
        f.write('void Start' + capitalized_filename_no_underscores + 'SM(void);\n')
        f.write(capitalized_filename_no_underscores + 'States Query' + capitalized_filename_no_underscores + 'SM(void);\n\n')
        f.write('#endif')
        f.close()
    
    def CreateImplementationFile(self):
        g = open('%s/SM%s.c' %(self.path, self.filename), 'w')
        
        authors_listed = ''
        index =  0
        while (index < len(self.authors) - 1):
            authors_listed = authors_listed + self.authors[index] + ', '
            index += 1
        authors_listed = authors_listed + self.authors[len(self.authors) - 1]

    
        boilerplate_comments = '// Copyright (c) ' + self.year + ' Velocitek Inc. All rights reserved.\n\
// Author(s): ' + authors_listed + '\n\
//\n\
// Hierarchical State Machine Template by Ed Carryer, Stanford University\n\
//\n\
// Please follow the Velocitek C Style Guide when maintaining this code.\n\
// http://www.velocitek.com/style-guide/c_style_guide.xml\n\n'

        g.write(boilerplate_comments)
        g.write('// Include Files ' + '-'*63 + '\n\n')
        for include in self.includes:
            g.write('#include \"' + include + '\"\n')
        g.write('\n')
        g.write('#include "SM' + self.filename + '.h"\n\n')
        
        g.write('// Module Constants ' + '-' * 60 + '\n\n\n')
        
        g.write('// Module Functions ' + '-' * 60 + '\n')
        for state in self.states:
            g.write('static void During'+CapitalizeFirstLettersRemoveUnderscores(state) + '(uint8_t event);\n')
            
        g.write('\n// Module Variables' + '-'*60 + '\n')
        g.write('static ' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'States current_' + self.filename + '_state_ = ' + self.states[0].upper() + '_STATE;' + '\n\n')
        
        g.write('// Module Code ' + '-'*65 + '\n')
        g.write('uint8_t Run' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'SM(uint8_t current_event)\n')
        g.write('{\n  ' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'States next_state = current_' + self.filename + '_state_;\n\n')
        g.write('  switch (current_' + self.filename + '_state_)\n  {\n')
        for state in self.states:
            g.write('    case ' + state.upper() + '_STATE' + ' :\n')
            g.write('      During' + CapitalizeFirstLettersRemoveUnderscores(state) + '(current_event);\n\n\
      if (current_event != EV_NO_EVENT)\n\
      {\n        switch (current_event)\n        {\n')
      
            self.PrintTransitionStatements(state, g)

            g.write('          default:\n            break;\n        }\n      }\n\n      break;\n\n')
       
        g.write('  }\n\n')
        g.write('  if (next_state != current_' + self.filename + '_state_)\n  {\n')
        g.write('    switch (current_' + self.filename + '_state_)\n    {\n')
        for state in self.states:
            g.write('      case ' + state.upper() + '_STATE' + ':\n        During' + CapitalizeFirstLettersRemoveUnderscores(state) + '(EV_EXIT);\n        break;\n\n')
        g.write('      default:\n        break;\n    }\n\n')
        g.write('    current_' + self.filename + '_state_ = next_state;\n\n')
        g.write('    switch (current_' + self.filename + '_state_)\n    {\n')
        for state in self.states:
            g.write('      case ' + state.upper() + '_STATE' + ':\n        During' + CapitalizeFirstLettersRemoveUnderscores(state) + '(EV_ENTRY);\n        break;\n\n')
        g.write('      default:\n        break;\n    }\n  }\n\n')
        g.write('  return (current_event);\n}\n\n')
        g.write('void Start' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'SM(void)\n{\n')
        g.write('  current_' + self.filename + '_state_ = ' + self.states[0].upper() + '_STATE' + ';\n')
        g.write('  Run' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'SM(EV_ENTRY);\n}\n\n')
        g.write(CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'States Query' + CapitalizeFirstLettersRemoveUnderscores(self.filename) + 'SM(void)\n')
        g.write('{\n  return current_' + self.filename + '_state_;\n}\n\n\n')
        PrintPrivateDuringFunctions(g, self.states)
        g.close()
        return

    def GenerateScript(self):
        self.CreateHeaderFile()
        self.CreateImplementationFile()
        return
