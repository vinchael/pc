import argparse
import os
import pandas as pd
from pandas import ExcelWriter
from xlrd import open_workbook

OFF = 0
ON = 1

def createExcelFile(df, filename):
    writer = ExcelWriter(filename)
    df.to_excel(writer, 'Sheet1', index=False)
    writer.save()
    writer.close()

def readExcelFile(filename, inputData):
    df = pd.read_excel(filename,
                       sheet_name=inputData[0],
                       usecols=inputData[1],
                       skiprows=inputData[2])
    return(df)

def readArgParse():
    # Accepting input
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', dest = 'dest', help = 'Variant: -v [filename')
    args = parser.parse_args()

    if args.dest:
        print("Argument accepted")
        return args.dest
    else:
        return 0

def getCurrentDirectory(filename):
    base_path = os.path.dirname(os.path.realpath(__file__))
    currentfile = os.path.join(base_path, filename)
    
    print(currentfile)
