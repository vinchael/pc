from util import *
from StringUtils import *
import pandas as pd
import os
import re
import sys
import argparse
import numpy as np
import linecache


#             Sheet name , Use cols
#             [0]        , [1]
inputData0 = ['Sheet1', 'A:F', OFF]
inputData1 = ['Sheet1', 'A:B', OFF]

dataHandler = {"0": [inputData0],
               "1": [inputData1]}

def filterData(args):
    df = readExcelFile(args.data, dataHandler[args.count][0])
    charMe = args.variant[:-2].lower()

    df['Module'] = df.Module.astype(str).str.lower()
    newDf = df[df['Module'] == charMe]
        
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
