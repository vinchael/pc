import pandas as pd
import os
import sys
import re
import argparse
import numpy as np
from pandas import ExcelWriter
from collections import Counter

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
input_data = ['Interface', 'A, E, I, J', 12]

def createTextFile(df):
    writer = ExcelWriter('DataExtracted.xlsx')
    df.to_excel(writer, 'Sheet1', index=False)
    writer.save()
    writer.close()

def setupData(variant):
    df = pd.read_excel(variant,
                                sheet_name=input_data[0],
                                usecols=input_data[1],
                                skiprows=input_data[2])
<<<<<<< HEAD
    df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'SigName'})
=======
>>>>>>> 8ec29e2259e94e238dafe3f75ed0bb94c1ad8175

    #remove the [][] only and create type column copy
    df['dtype'] = df['型[配列サイズ]'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    newdf = df.drop(df[(df['dtype'] == "nan") | (
        df['dtype'] == "-") | (df['dtype'] == "")].index)

    newdf['dtype'] = newdf['dtype'].replace(['single', 'Single'], 'float32')

<<<<<<< HEAD
    newdf['CAN_LIN_RTE'] = newdf['SigName'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    newdf['CAN_LIN_RTE'] = newdf['CAN_LIN_RTE'].astype(str).str.replace(
        r'^(\d+)|([\u4e00-\u9fff]+)|([\u3040-\u309Fー]+)|([\u30A0-\u30FF]+)|(-)$', '', regex=True)
    newdf['CAN_LIN_RTE'] = newdf['CAN_LIN_RTE'].replace(
        ['パラ', 'nan', 'OFF', 'ON'], '')
    
    newdf['dtype_CAN_LIN_RTE'] = newdf['CAN_LIN_RTE']
    newdf.dtype_CAN_LIN_RTE[newdf['dtype_CAN_LIN_RTE'] != ''] = newdf['dtype']
    #newdf.dtype_CAN_LIN_RTE[newdf['dtype_CAN_LIN_RTE'].notnull()
    #                        ] = newdf['dtype']
    
=======
>>>>>>> 8ec29e2259e94e238dafe3f75ed0bb94c1ad8175
    #get the datatype text only and remove [][]
    newdf['arrayValue'] = newdf['型[配列サイズ]'].astype(str).str.replace(
        r'^\w+\d{0,2}[^\[]', '', regex=True)

    newdf['arrayValue2'] = newdf['arrayValue'].astype(str).str.replace(
        r'^\[\d*\]$', '', regex=True)

    #get variable names without []
    newdf['variable_names'] = newdf['SigName(MDL)'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
<<<<<<< HEAD
        
    
=======

>>>>>>> 8ec29e2259e94e238dafe3f75ed0bb94c1ad8175
    newdf['remCombine'] = newdf['dtype'] + ' ' + newdf['対象モデル'] + \
        ' ' + newdf['variable_names']

    newdf['counts'] = newdf.remCombine.map(newdf.remCombine.value_counts())
    newdf['counts'] = ('[' + newdf['counts'].astype(str) + ']')
    newdf['counts'] = newdf['counts'].replace('[1]', '')

    

    countsCond = newdf.counts == ''
    newdf.counts[countsCond] = newdf.arrayValue2[newdf.arrayValue2 != '']
    newdf.counts[countsCond] = newdf.arrayValue[newdf.arrayValue != '']

<<<<<<< HEAD
    newdf['counts_CAN_LIN_RTE'] = newdf['CAN_LIN_RTE']
    newdf.counts_CAN_LIN_RTE[newdf['counts_CAN_LIN_RTE'] != ''] = newdf['counts']

    newdf['colon'] = newdf['CAN_LIN_RTE']
    newdf.colon[newdf['colon']
                             != ''] = ';'

    newdf['DataType_CAN_LIN_RTE'] = newdf['dtype_CAN_LIN_RTE']+' ' + \
        newdf['CAN_LIN_RTE'] + \
        newdf['counts_CAN_LIN_RTE'].astype(str) + newdf['colon']
        
    newdf['DataType'] = newdf['dtype']+' ' + \
        newdf['variable_names'] + newdf['counts'].astype(str) + ';'
    
    dropdf = newdf.drop_duplicates(subset=['remCombine'], keep='last')
    
    result_CAN_LIN_RTE = dropdf[["対象モデル", "DataType_CAN_LIN_RTE"]]
    
    #finaldf = dropdf[["対象モデル", "DataType"]]
    
    createTextFile(newdf)
=======
    newdf['DataType'] = newdf['dtype']+' ' + newdf['variable_names'] + newdf['counts'].astype(str) +';'
    dropdf = newdf.drop_duplicates(subset=['remCombine'], keep='last')
    
    finaldf = dropdf[["対象モデル", "DataType"]]

    createTextFile(finaldf)
>>>>>>> 8ec29e2259e94e238dafe3f75ed0bb94c1ad8175

def main(argv):
    # Read arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--variant', dest="variant",
                        help='Variant: [Filename].xlsx')
    args = parser.parse_args()

    if args.variant:
        variant = args.variant
        print("Extracting data...")
        setupData(variant)
        print("Finished")
    else:
        print('Please input: python ExtractData.py -v [filename].xlsx')
    
if __name__ == "__main__":
    main(sys.argv[1:])
