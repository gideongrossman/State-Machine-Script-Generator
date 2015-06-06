def CapitalizeFirstLettersRemoveUnderscores(word_to_capitalize):
    new_word = word_to_capitalize[0].upper()
    index = 1
    while (index < len(word_to_capitalize)):
        letter = word_to_capitalize[index]
        if word_to_capitalize[index - 1] == '_':
            new_word = new_word + letter.upper()
        elif letter != '_':
            new_word = new_word + (letter)
        elif letter == '_':
            x = 4
        index += 1
    return new_word
    

        
def CreateHeaderFile(path, filename, year, states):
    f = open('%s/sm_%s.h' %(path, filename) ,'w')
    capitalized_states = []
    for state in states:
        capitalized_states.append(state.upper())
    boilerplate_comments = '// Copyright (c) '+year+' Velocitek Inc. All rights reserved.\n\
//\n\
// Please follow the Velocitek C Style Guide when maintaining this code.\n\
// http://www.velocitek.com/style-guide/c_style_guide.xml\n\n'
    
    header_guard = '#ifndef SM_%s_H\n#define SM_%s_H\n\n' %(filename.upper(), filename.upper())
    
    enum_type_definition = 'typedef enum\n{\n  %s,\n  %s,\n  %s,\n}\n' %(capitalized_states[0],capitalized_states[1],capitalized_states[2]) 
    
    capitalized_filename_no_underscores = CapitalizeFirstLettersRemoveUnderscores(filename)
    f.write(boilerplate_comments)
    f.write(header_guard)
    f.write('#include "events.h"\n\n')
    f.write(enum_type_definition)
    f.write(capitalized_filename_no_underscores + 'States;\n\n')
    f.write('Events Run' + capitalized_filename_no_underscores + 'SM(Events current_event);\n')
    f.write('void Start' + capitalized_filename_no_underscores + 'SM(void);\n')
    f.write(capitalized_filename_no_underscores + 'States Query' + capitalized_filename_no_underscores + 'SM(void);\n\n')
    f.write('#endif')
    f.close()
    
def CreateImplementationFile(path, filename, year, authors, states):
    g = open('%s/sm_%s.c' %(path, filename), 'w')
    
    authors_listed = ''
    index =  0
    while (index < len(authors) - 1):
        authors_listed = authors_listed + authors[index] + ', '
        index += 1
    authors_listed = authors_listed + authors[len(authors) - 1]

    
    boilerplate_comments = '// Copyright (c) ' + year + ' Velocitek Inc. All rights reserved.\n\
// Author(s): ' + authors_listed + '\n\
//\n\
// Hierarchical State Machine Template by Ed Carryer, Stanford University\n\
//\n\
// Please follow the Velocitek C Style Guide when maintaining this code.\n\
// http://www.velocitek.com/style-guide/c_style_guide.xml\n\n'

    
    g.write(boilerplate_comments)
    g.write('// Include Files ' + '-'*63 + '\n\n')
    g.write('#include "events.h"\n#include "sm_' + filename + '_state.h"\n\n')
    g.write('// Module Constants ' + '-' * 60 + '\n\n\n')
    g.write('// Module Functions ' + '-' * 60 + '\n')
    g.write('static void During')
    g.close()
    return


path = "C:/Users/Velocitek/Documents/Gideon/Code"
filename = "script_generated"
year = '2015'
authors = ['Gideon Grossman', 'Alec Stewart']
states  = ['State_1', 'State_2', 'State_3']
CreateHeaderFile(path, filename, year, states)
CreateImplementationFile(path, filename, year, authors, states)
