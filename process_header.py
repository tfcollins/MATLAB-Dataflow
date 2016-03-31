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

def get_signature(txt):
    # EX: function differencesWithSelf = PairWiseDifferences(P)
    fn = 'function'
    loc = txt.find(fn)+len(fn)
    end = txt.find(')')+1
    signature = txt[loc:end]
    print 'Sig:'+signature

    if signature.find('=')>-1:
        # Outputs
        outs = signature.split("=")[0]
        print outs
        outs = outs.replace('[','').replace(']','') # remove brackets
        outs = outs.split(',') # remove brackets
        for o in range(len(outs)):
            outs[o] = outs[o].strip() # remove brackets

        print "Outputs:"
        print outs
        return outs
    else:
        print "No outputs"
        return []

def Get_IN_OUT_Variables(func):

    f = open(mroot+func+'.m')
    txt = f.read()
    return get_signature(txt)

def Get_Cpp_Names(funcname,mnames):
    # Get C++ Names
    try:
        cppHeader = CppHeaderParser.CppHeader(hroot+funcname+'.h')
    except CppHeaderParser.CppParseError as e:
        print(e)
        sys.exit(1)

    print("\nFree functions are:")
    index = 0
    for func in cppHeader.functions:
        #print(" %s"%func)
        if funcname==func["name"]:
            print(" %s"%func["name"])
            if len(func['parameters'])>0:
                for p in func['parameters']:
                    print("--%s"%p['raw_type'])
                    print "--mname: "+str(mnames)
                    # print("--%s"%p['raw_type'])
            else:
                print "No params"

            index = index + 1

    # Process headers for function prototypes

# MAIN
for func in functions:
    outs = Get_IN_OUT_Variables(func)
    Get_Cpp_Names(func,outs)
    print '----------------------------------'
