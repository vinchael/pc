from util import *
from StringUtils import *
import sys
import pandas as pd
import numpy as np

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Sheet1', 'A:L', 0]
module = {'CAN_VP': ['CANRx_', 'IPCRx_'],
          'CTL': ['Ctl_'],
          'Others': ['Feat_']}
CAN_VP = { 'CAN', 'VP'}
CTL = {'EGI','TCU_CVT','TCU_Shift','VDC'}

INDefault = "Rte_Read_RP_"
OUTDefault = "Rte_Write_PP_"

unwantedString = {'nan', '-', ''}

def getModPosKey(df, row, col):
    index = df.iat[row, col]
    if index in CAN_VP:
        return 'CAN_VP'
    elif index in CTL:
        return 'CTL'
    else:
        return 'Others'

def createVarName(df, row, posKey, posVal, varName, name, insideVarName ):
    Arr = df.iat[row, 9]
    Array2D1D = df.iat[row, 13]
    state = df.iat[row, 1]
    string = name + module[posKey][posVal] + varName
    
    if state == "IN":
        if Arr == '':
            string = string + '( &' + insideVarName + ' );'
            return string
        else:
            if Arr == Array2D1D:
                string = string + "_" + Array2D1D + \
                    '( &' + insideVarName + '[' + Array2D1D + '] );'
                return string
            else:
                string = string + '_Arr' + Arr + '( ' + insideVarName + ' );'
                return string
    else:
        if Arr == '':
            string = string + '( ' + insideVarName + ' );'
            return string
        else:
            if Arr == Array2D1D:
                string = string + "_" + Array2D1D + \
                    '( ' + insideVarName + '[' + Array2D1D + '] );'
                return string
            else:
                string = string + '_Arr' + Arr + '( ' + insideVarName + ' );'
                return string

def createData(args, df):
    # ------------------------------
    # Data Manipulation
    #createExcelFile(df, 'dfdf.xlsx')
    #print('Creating function call for ' + args.variant)
    string = []
    moduleName = []
    modName = df.iat[0, 12]
    for row in range(df.shape[0]): # Starstr
        MDL_ModVar = df.iat[row, 11]
        CAN_ModVar = df.iat[row, 16]
        state = df.iat[row, 1]
        inputFrom = df.iat[row, 2]
    
        if state == "IN":  # IN
            posKey = getModPosKey(df, row, 2)
            if posKey == 'CAN_VP':
                if inputFrom == "CAN":  # CAN
                    string.append(createVarName(
                        df, row, posKey, 0, MDL_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
                else:  # VP
                    string.append(createVarName(
                        df, row, posKey, 1, MDL_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
            elif posKey == 'CTL':
                if CAN_ModVar != '_':
                    string.append(createVarName(
                        df, row, posKey, 0, CAN_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
            else:
                if CAN_ModVar != '_':
                    string.append(createVarName(
                        df, row, posKey, 0, CAN_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
        else: # OUT
            posKey = getModPosKey(df, row, 12)
            if posKey == 'CAN_VP':
                if inputFrom == "CAN":  # CAN
                    string.append(createVarName(
                        df, row, posKey, 0, MDL_ModVar, OUTDefault, MDL_ModVar))
                    moduleName.append(modName)
                else:  # VP
                    string.append(createVarName(
                        df, row, posKey, 1, MDL_ModVar, OUTDefault, MDL_ModVar))
                    moduleName.append(modName)
            elif posKey == 'CTL':
                if CAN_ModVar != '_':
                    string.append(createVarName(
                        df, row, posKey, 0, MDL_ModVar, OUTDefault, MDL_ModVar))
                    moduleName.append(modName)
            else:
                if CAN_ModVar != '_':
                    string.append(createVarName(
                        df, row, posKey, 0, MDL_ModVar, OUTDefault, MDL_ModVar))
                    moduleName.append(modName)
    
    df = pd.DataFrame({'Module': moduleName, 'Function_Name': string})
    df = df.sort_values(by=['Function_Name'], ascending=True)
    df = df.drop_duplicates(subset=['Function_Name'], keep='last')

    createExcelFile(df, 'ForMCUAppMethod.xlsx')
    

def filterData(args):
    df = readExcelFile(args.data, inputData)
    df['Module'] = df['対象モデル']
    df['対象モデル'] = df.対象モデル.astype(str).str.lower()
    charMe = args.variant[:-2].lower()
    df = df[df['対象モデル'].str.contains(charMe)]
    #print(newDf)

    if len(df.head(1)) == 0:
        print("No Object Model for ", args.variant)
    else:
    
        #Filter Data from ExtractedData
        df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'CAN_SigName'})

        for i in unwantedString:
            df['入力元'] = replace(df, '入力元', i, '')

        df['Array2D1D'] = reg_replace(df, 'CAN_SigName', r'.*[^(\d{2})]', '')
        df['MDL_Array2D1D'] = reg_replace(df, 'MDL_Array2D1D', r'[\[\]]', '')
        df['MDL_Array2D1D'] = replace(df, 'MDL_Array2D1D', 'nan', '')

        df['Index'] = df.apply(lambda x: x['MDL_Array2D1D']
                            == x['Array2D1D'], axis=1)

        df['CAN_Variable'] = reg_replace(
            df, 'CAN_SigName', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')

        for i in unwantedString:
            df['CAN_Variable'] = replace(df, 'CAN_Variable', i, '')

        df['CAN_ModVar'] = df['入力元'] + '_' + df['CAN_Variable']
        createData(args, df)

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
