import pandas as pd
import os
import re
import sys
import argparse
import numpy as np

#             Sheet name , Use cols
#             [0]        , [1]
input_data = ['Sheet1', 'A:B']

def setDataVariable(args, newDf):
    counter = 1
    with open(args.variant, 'r') as f:
            for line in f.readlines():
                nLine = line.strip()
                if bool(re.match(r'(.*){3}DO NOT CHANGE THIS COMMENT!           << Start of include(.*)', nLine)):
                    counter = counter + 2
                    break
                else:
                    counter = counter + 1

    #get datatype column only
    dataTypedf = newDf["DataType"]
    
    #if counter has value
    if counter:
        for datas in range(dataTypedf.shape[0]):
            replaceLine(args.variant, counter, dataTypedf.iat[datas])
            counter = counter + 1
            #print(dataTypedf.iat[datas])

    print("Finish creating global variable for " + args.variant)

def replaceLine(filename, line_num, text):
    lines = open(filename, 'r').readlines()
    lines[line_num] = text + '\n\n'
    out = open(filename, 'w')
    out.writelines(lines)
    out.close()

def filterData(args):
    df = pd.read_excel(args.data,
                            sheet_name=input_data[0],
                            usecols=input_data[1])

    #Filter Data from ExtractedData
    df['Model'] = df.Model.astype(str).str.lower()
    charMe = args.variant[:-2].lower()
    newDf = df[df['Model'].str.contains(charMe)]
    
    #print(newDf)

    if len(newDf.head(1)) == 0:
        print("No Object Model for: ", args.variant[:-2])
    else:
        #print("Creating " + args.variant + " global variable")
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
    print("Please input arguments: python MergingTextFile.py -v [filename].c -d [filename].xlsx")
