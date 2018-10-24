import pandas as pd
import os
import re
import sys
import argparse
import numpy as np
import linecache
from util import *

#             Sheet name , Use cols
#             [0]        , [1]
input_data = ['Sheet1', 'A:B']


def findLine(filename, str):
    lineNumber = 0
    with open(filename, 'r') as f:
            for line in f.readlines():
                nLine = line.strip()
                if bool(re.match(str, nLine)):
                    break
                else:
                    lineNumber = lineNumber + 1
    f.close()
    return lineNumber + 1

def setDataVariable(args, newDf):
    str = r'(.*)DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>(.*)'
    lineNumber = findLine(args.variant, str)
    lineNumber = lineNumber + 3
    #get datatype column only
    function_Name = newDf["Function_Name"]

    #if lineNumber has value
    if lineNumber:
        for datas in range(function_Name.shape[0]):
            replaceLine(args.variant, lineNumber, function_Name.iat[datas])
            lineNumber = lineNumber + 1
            #print(function_Name.iat[datas])

    print("Finish creating function for " + args.variant)


def replaceLine(filename, line_num, text):
    INLines = open(filename, 'r').readlines()
    INLines[line_num] = '  '+ text + '\n\n'
    out = open(filename, 'w')
    out.writelines(INLines)
    out.close()


def filterData(args):
    df = pd.read_excel(args.data,
                       sheet_name=input_data[0],
                       usecols=input_data[1])

    #Filter Data from ExtractedData
    df['Module'] = df.Module.astype(str).str.lower()
    charMe = args.variant[:-2].lower()
    newDf = df[df['Module'].str.contains(charMe)]

    #print(newDf)

    if len(newDf.head(1)) == 0:
        #print("No Object Module for ", args.variant)
        pass
    else:
        setDataVariable(args, newDf)


def main(argv):
    # Read arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', dest="variant", help='[filename].c')
    parser.add_argument('-d', dest="data", help='[Filename].xlsx')
    args = parser.parse_args()

    filterData(args)


if len(sys.argv) > 2:
    if __name__ == "__main__":
        main(sys.argv[1:])
else:
    print(
        "Please input arguments: python MergingTextFile.py -v [filename].c -d [filename].xlsx")
