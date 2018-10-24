import pandas as pd
import os
import re
import sys
import argparse
import numpy as np
import linecache
from util import *
from StringUtils import *

#             Sheet name , Use cols
#             [0]        , [1]
inputData0 = ['Sheet1', 'A:F', 0]
inputData1 = ['Sheet1', 'A:B', 0]

def filterData(args):
    charMe = args.variant[:-2].lower()
    if args.count == "0":
        df = readExcelFile(args.data, inputData0)
        df['Model'] = df.Model.astype(str).str.lower()
        newDf = df[df['Model'].str.contains(charMe)]
    else:
        df = readExcelFile(args.data, inputData1)
        df['Module'] = df.Module.astype(str).str.lower()
        newDf = df[df['Module'].str.contains(charMe)]
    

    if len(newDf.head(1)) == 0:
        print("No Object Module for ", args.variant)
    else:
        if args.count == "0":
            string = r'(.*)DO NOT CHANGE THIS COMMENT!           << Start of include(.*)'
            column_name = 'DataType'
            skip_count = 2
            spaces = ''
            setDataVariable(args.variant, newDf, string,
                            column_name, skip_count, spaces)
            print("Finish creating global variable for " + args.variant)
        else:
            string = r'(.*)DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>(.*)'
            column_name = 'Function_Name'
            skip_count = 3
            spaces = '  '
            setDataVariable(args.variant, newDf, string,
                            column_name, skip_count, spaces)
            print("Finish creating function for " + args.variant)

def main(argv):
    # Read arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', dest="variant", help='[filename].c')
    parser.add_argument('-d', dest="data", help='[Filename].xlsx')
    parser.add_argument('-c', dest="count", help='[count]')
    args = parser.parse_args()
    
    filterData(args)

if len(sys.argv) > 2:
    if __name__ == "__main__":
        main(sys.argv[1:])
else:
    print("Please input arguments: python MergingTextFile.py -v [filename].c -d [filename].xlsx")
