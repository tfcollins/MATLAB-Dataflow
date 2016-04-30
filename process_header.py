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

class FunctionTemplate:#(object):

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
    # print 'Sig:'+signature

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

        # print "Inputs:"
        # print ins
        return ins
    else:
        # print "No inputs"
        return []


def get_outputs(txt):
    # EX: function differencesWithSelf = PairWiseDifferences(P)
    fn = 'function'
    loc = txt.find(fn)+len(fn)
    end = txt.find(')')+1
    signature = txt[loc:end]
    # print 'Sig:'+signature

    if signature.find('=')>-1:
        # Outputs
        outs = signature.split("=")[0]
        #print outs
        outs = outs.replace('[','').replace(']','') # remove brackets
        outs = outs.split(',') # remove brackets
        for o in range(len(outs)):
            outs[o] = outs[o].strip() # remove brackets

        # print "Outputs:"
        # print outs
        return outs
    else:
        # print "No outputs"
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
    print("\nFree functions are:")
    index = 0
    for func in cppHeader.functions:
        # Pick out function of interest that matches MATLAB function
        if funcname==func["name"]:
            # Evaluate function parameters
            if len(func['parameters'])>0:
                # Pick out parameters that match matlab
                for p in func['parameters']:
                    if p['name'] in sigs['out']:
                        sig = {'name':p['name'], 'type':p['type']+'['+p['array_size']+']'}
                        template.Outputs.append(sig)

                    elif p['name'] in sigs['in']:
                        sig = {'name':p['name'], 'type':p['type']+'['+p['array_size']+']'}
                        template.Inputs.append(sig)
            else:
                print "No params"

            index = index + 1

    return template
    # Process headers for function prototypes

def WriteBlockCode(funcTemplate):

    tab = '    '

    CppSignature = "OUTPUTS "+funcTemplate.Name+"BLK(INPUTS input, int *flag)\n{\n"

    instr = CppSignature
    # Write inputs
    if len(funcTemplate.Inputs)>0:
        instr = instr+tab+"// Get input(s)\n"
        for i in range(len(funcTemplate.Inputs)):
            instr = instr+tab+funcTemplate.Inputs[i]['type']+' *in'+str(i)+' = ('+funcTemplate.Inputs[i]['type']+') input['+str(i)+']'
    else:
        instr = instr+tab+"// No Inputs\n"

    # Write actual function call
    actualCall = '// MATLAB Function Call\n'
    variables = ','.join(','.join(funcTemplate.Inputs['name']),','.join(funcTemplate.Outputs['name']))
    MLSignature = funcTemplate.Name+"("+variables+")"
    instr = instr + MLSignature

    # Write outputs



    instr = instr + '\n}\n'
    print instr

# MAIN
for func in functions:
    sigs = Get_IN_OUT_Variables(func)
    template = Get_Cpp_Names(func,sigs)
    print template.Name
    print 'Outputs'
    print template.Outputs
    print 'Inputs'
    print template.Inputs
    WriteBlockCode(template)
    print '----------------------------------'
