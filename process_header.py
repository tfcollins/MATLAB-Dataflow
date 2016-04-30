#!/usr/bin/python
import sys
sys.path = ["../"] + sys.path
import CppHeaderParser

# Automatic code generate block files
# 1. Get input and output variable names from .m code
# 2. Get C++ names from headers

hroot = 'codegen/exe/RX/'
mroot = 'mfiles/'
functions =['CheckCRC','GenCRC']

class FunctionTemplate:
    def __init__(self,name):
        self.Outputs = []
        self.Inputs = []
        self.Name = name

def get_inputs(txt):
    # EX: function differencesWithSelf = PairWiseDifferences(P)
    fn = 'function'
    loc = txt.find(fn)+len(fn)
    end = txt.find(')')+1
    signature = txt[loc:end]

    if not signature.find('()')>-1:
        # Inputs
        if signature.find('=')>-1: # there are Outputs
            ins = signature.split("=")[1]
        else: # No Outputs
            ins = signature
        # Get string inbetween parantheses
        ins = ins[ins.find('(')+1:ins.find(')')]
        ins = ins.split(',') # split arguments
        for i in range(len(ins)):
            ins[i] = ins[i].strip() # remove brackets
        return ins
    else:
        return []


def get_outputs(txt):
    # EX: function differencesWithSelf = PairWiseDifferences(P)
    fn = 'function'
    loc = txt.find(fn)+len(fn)
    end = txt.find(')')+1
    signature = txt[loc:end]

    if signature.find('=')>-1:
        # Outputs
        outs = signature.split("=")[0]
        outs = outs.replace('[','').replace(']','') # remove brackets
        outs = outs.split(',') # remove brackets
        for o in range(len(outs)):
            outs[o] = outs[o].strip() # remove brackets
        return outs
    else:
        return []

def Get_IN_OUT_Variables(func):

    f = open(mroot+func+'.m')
    txt = f.read()
    outs = get_outputs(txt)
    ins = get_inputs(txt)

    sig = {'in':ins,'out':outs}
    return sig

def Get_Cpp_Names(funcname,sigs):
    # Get C++ Names
    try:
        cppHeader = CppHeaderParser.CppHeader(hroot+funcname+'.h')
    except CppHeaderParser.CppParseError as e:
        print(e)
        sys.exit(1)

    template = FunctionTemplate(funcname)
    #print("\nFree functions are:")
    index = 0
    for func in cppHeader.functions:
        # Pick out function of interest that matches MATLAB function
        if funcname==func["name"]:
            # Evaluate function parameters
            if len(func['parameters'])>0:
                # Pick out parameters that match matlab
                for p in func['parameters']:
                    # Inputs
                    if p['name'] in sigs['out']:
                        sig = {'name':p['name'], 'type':p['type']+'['+p['array_size']+']'}
                        template.Outputs.append(sig)
                    # Outputs
                    elif p['name'] in sigs['in']:
                        sig = {'name':p['name'], 'type':p['type']+'['+p['array_size']+']'}
                        template.Inputs.append(sig)
            #else:
            #    print "No params"

            index = index + 1

    return template

def WriteBlockCode(funcTemplate):

    # Tab character
    tab = '    '

    # Block function signature
    CppSignature = "OUTPUTS "+funcTemplate.Name+"BLK(INPUTS input, int *flag)\n{\n"
    instr = CppSignature # add

    # Write inputs
    if len(funcTemplate.Inputs)>0:
        instr = instr+tab+"// Get input(s)\n"
        for i in range(len(funcTemplate.Inputs)):
            instr = instr+tab+funcTemplate.Inputs[i]['type'][:funcTemplate.Inputs[i]['type'].find('[')]+' *'+funcTemplate.Inputs[i]['name']+' = ('+funcTemplate.Inputs[i]['type'][:funcTemplate.Inputs[i]['type'].find('[')]+'*) input['+str(i)+'];\n'
    else:
        instr = instr+tab+"// No Inputs\n"
    instr = instr + '\n'

    # Form outputs
    if len(funcTemplate.Outputs)>0:
        instr = instr+tab+"// Setup output(s)\n"
        for i in range(len(funcTemplate.Outputs)):
            instr = instr+tab+funcTemplate.Outputs[i]['type'][:funcTemplate.Outputs[i]['type'].find('[')]+' *'+funcTemplate.Outputs[i]['name']+' = new '+funcTemplate.Outputs[i]['type']+';\n'
    else:
        instr = instr+tab+"// No outputs\n"

    # Write actual function call
    actualCall = '// MATLAB Function Call\n'
    Sigs = []
    for i in funcTemplate.Inputs:
        Sigs.append(i['name'])
    for o in funcTemplate.Outputs:
        Sigs.append(o['name'])
    Sigs = ','.join(Sigs)
    MLSignature = funcTemplate.Name+"("+Sigs+");\n\n"
    instr = instr + '\n' + tab + actualCall + tab + MLSignature

    # Cleanup
    if len(funcTemplate.Inputs)>0:
        instr = instr+tab+"// Cleanup\n"
        deletes = []
        for i in range(len(funcTemplate.Inputs)):
            deletes.append(funcTemplate.Inputs[i]['name'])
        deletes = ','.join(deletes)
        deletes = 'delete ' + deletes + ';\n\n'
        instr = instr + tab + deletes

    # Write outputs pass
    if len(funcTemplate.Outputs)>0:
        instr = instr+tab+"// Send Out\n" + tab + '*flag = 1;\n'
        outs = []
        for i in range(len(funcTemplate.Outputs)):
            outs.append( funcTemplate.Outputs[i]['name'] )
        outs = ','.join(outs)
    else:
        instr = instr+tab+"// Send Nothing\n"
        outs = 'none'
    outpass = tab + 'return SendOut(' + outs + ');\n'
    instr = instr + outpass

    # End function
    instr = instr + '\n}\n'

    # Done
    return instr

def ProcessFunctions(functions):
    BC = []
    # Form all functions
    for func in functions:
        # Get function signatures from MATLAB
        sigs = Get_IN_OUT_Variables(func)
        # Get signature from CPP Headers
        template = Get_Cpp_Names(func,sigs)
        # Write CPP block header code
        BC.append( WriteBlockCode(template) )

    # Form header
    separator = '\n/////////////////////////////////////////////////\n'
    BlocksHeader = separator.join(BC)
    header = '// MATLAB Datatype support functions (May not always be included with generated code, if not they are not required here)\n//#include "RX_emxutil.h"\n'
    footer = '//[EOF]'
    BlocksHeader = header + separator + BlocksHeader + footer
    return BlocksHeader

# MAIN
functions = sys.argv[1:]
BlocksHeader = ProcessFunctions(functions)
print BlocksHeader
#print '----------------------------------'
