from util import *
from StringUtils import *
import sys
import pandas as pd
import numpy as np

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Sheet1', 'A:L', OFF]
module = {'CAN_VP': ['CANRx_', 'IPCRx_'],
          'CTL': ['Ctl_'],
          'Others': ['Feat_']}
CAN_VP = { 'CAN', 'VP'}
CTL = {'EGI','TCU_CVT','TCU_Shift','VDC'}
NOT = {'RTE', 'FWBSW', 'NVMBSW', 'CONST', 'FMWF',
       'Ground', 'SWBSW', 'SSM', 'Input_Getway', 
       'RTE_Gateway', 'FMWR1'}

INDefault = "Rte_Read_RP_"
OUTDefault = "Rte_Write_PP_"

unwantedString = {'nan', '-', '', 'パラ', 'OFF', 'ON'}

State_col_index = 1
frModule_columm_index = 2
MDL_Array2D1D_col_index = 9
MDL_ModVar_col_index = 10
Module_col_index = 11
count_col_index = 13
CAN_Variable_col_index = 14
CAN_ModVar_col_index = 15
index_col_index = 16


def getModPosKey(df, row, col):
    index = df.iat[row, col]
    if index in CAN_VP:
        return 'CAN_VP'
    elif index in CTL:
        return 'CTL'
    else:
        return 'Others'

def createFuncName(df, row, posKey, posVal, varName, name, insideVarName ):
    
    Arr = df.iat[row, MDL_Array2D1D_col_index]
    Array2D1D = df.iat[row, count_col_index]
    index_ = df.iat[row, index_col_index]
    state = df.iat[row, State_col_index]
    string = name + module[posKey][posVal] + varName
    
    if state == "IN":
        if Arr == '':
            string = string + '( &' + insideVarName + ' );'
            return string
        else:
            if index_ > 1:
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
            if index_ > 1:
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
    modName = df.iat[0, Module_col_index]
    for row in range(df.shape[0]): # Starstr
        if df.iat[row, frModule_columm_index] in NOT:
            continue
        MDL_ModVar = df.iat[row, MDL_ModVar_col_index]
        CAN_Variable = df.iat[row, CAN_Variable_col_index]
        CAN_ModVar = df.iat[row, CAN_ModVar_col_index]
        state = df.iat[row, State_col_index]
        inputFrom = df.iat[row, frModule_columm_index]
        if state == "IN":
            # CAN and VP -------------------------------------------------------
            # INDefault    + posKey  + Module  + MDL_ModVar        + MDL_ModVar
            # Rte_Read_RP_ + CANRx_  + CUS_    + CustomChange_ACCL + (uint8 *data)

            # CTL and Feat -----------------------------------------------------
            # INDefaule    + posKey  + Module  + CAN_ModVar        + MDL_ModVar
            # Rte_Read_RP_ + Feat_   + FS_     + DetFailCode_0     + (uint8 *data)

            posKey = getModPosKey(df, row, frModule_columm_index)
            if posKey == 'CAN_VP':
                if MDL_ModVar != '':
                    if inputFrom == "CAN":  # CAN
                        string.append(createFuncName(
                            df, row, posKey, 0, MDL_ModVar, INDefault, MDL_ModVar))
                        moduleName.append(modName)
                    else:  # VP
                        string.append(createFuncName(
                            df, row, posKey, 1, MDL_ModVar, INDefault, MDL_ModVar))
                        moduleName.append(modName)
            elif posKey == 'CTL':
                if CAN_Variable != '':
                    string.append(createFuncName(
                        df, row, posKey, 0, CAN_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
            else:
                if CAN_Variable != '':
                    string.append(createFuncName(
                        df, row, posKey, 0, CAN_ModVar, INDefault, MDL_ModVar))
                    moduleName.append(modName)
        else:
            # CAN and VP -------------------------------------------------------
            # INDefault     + posKey  + Module  + MDL_ModVar            + MDL_ModVar
            # Rte_Write_PP_ + Feat_   + CUS_f_  + CustomCont_AUTO_DRIVE + (boolean data)
            posKey = getModPosKey(df, row, Module_col_index)
            if posKey == 'CAN_VP':
                if MDL_ModVar != '':
                    if inputFrom == "CAN":  # CAN
                        string.append(createFuncName(
                            df, row, posKey, 0, MDL_ModVar, OUTDefault, MDL_ModVar))
                        moduleName.append(modName)
                    else:  # VP
                        string.append(createFuncName(
                            df, row, posKey, 1, MDL_ModVar, OUTDefault, MDL_ModVar))
                        moduleName.append(modName)
            elif posKey == 'CTL':
                if CAN_Variable != '':
                    string.append(createFuncName(
                        df, row, posKey, 0, MDL_ModVar, OUTDefault, MDL_ModVar))
                    moduleName.append(modName)
            else:
                if MDL_ModVar != '':
                    string.append(createFuncName(
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
        
        df['入力元'] = reg_replace(df, '入力元', r'[\(\)]', '')
        for i in unwantedString:
            df['入力元'] = replace(df, '入力元', i, '')

        df['Array2D1D'] = reg_replace(df, 'CAN_SigName', r'.*[^(\d{2})]', '')
        df['MDL_Array2D1D'] = reg_replace(df, 'MDL_Array2D1D', r'[\[\]]', '')
        df['MDL_Array2D1D'] = replace(df, 'MDL_Array2D1D', 'nan', '')

        df['count'] = df.groupby(
            (df['MDL_Variable'] != df['MDL_Variable'].shift(1)).cumsum()).cumcount()+1
        df['count'] = df['count'] - 1
        df['count'] = df['count'].astype(str)

        df['CAN_Variable'] = reg_replace(
            df, 'CAN_SigName', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')
        df['CAN_Variable'] = reg_replace(
            df, 'CAN_Variable', r'^(\d+)|([\u4e00-\u9fff]+)|([\u3040-\u309Fー]+)|([\u30A0-\u30FF]+)|(-)$', '')

        for i in unwantedString:
            df['CAN_Variable'] = replace(df, 'CAN_Variable', i, '')
 
        df['CAN_ModVar'] = df['入力元'] + '_' + df['CAN_Variable']

        df['index'] = df.MDL_Variable.map(df.MDL_Variable.value_counts())

        createExcelFile(df, 'Sample.xlsx')
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
