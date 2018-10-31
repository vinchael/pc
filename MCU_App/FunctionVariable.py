from util import *
from StringUtils import *
import sys
import pandas as pd
import numpy as np

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Sheet1', 'A:M', OFF]
module = {'CAN_VP': ['CANRx_', 'IPCRx_'],
          'CTL': ['Ctl_'],
          'Others': ['Feat_']}
CAN_VP = { 'CAN', 'VP'}
CTL = {'EGI','TCU_CVT','TCU_Shift','VDC'}
NOT = {'RTE', 'FWBSW', 'NVMBSW', 'CONST', 'FMWF',
       'Ground', 'SWBSW', 'SSM', 'Input_Getway', 
       'RTE_Gateway', 'FMWR1'}

unwantedString = {'nan', '-', '', 'パラ', 'OFF', 'ON'}

State_col_ind1 = 1
frModule_columm_ind2 = 2
MDL_ModVar_col_ind10 = 10
MDL_Count_col_ind12 = 12
Module_col_ind13 = 13
CAN_Variable_col_ind14 = 14
CAN_ModVar_col_ind15 = 15

def getModPosKey(df, row, col):
    index = df.iat[row, col]
    if index in CAN_VP:
        return 'CAN_VP'
    elif index in CTL:
        return 'CTL'
    else:
        return 'Others'

def createFuncName(df, row, posKey, posVal, varName, name, insideVarName ):
    
    Arr = df.iat[row, MDL_Count_col_ind12]
    state = df.iat[row, State_col_ind1]
    string = name + module[posKey][posVal] + varName
    
    if state == "IN":
        if Arr == 'nan':
            string = string + '( &' + insideVarName + ' );'
            return string
        else:
            string = string + '_Arr' + Arr + '( ' + insideVarName + ' );'
            return string
    else:
        if Arr == 'nan':
            string = string + '( ' + insideVarName + ' );'
            return string
        else:
            string = string + '_Arr' + Arr + '( ' + insideVarName + ' );'
            return string

def createData(args, df):
    INDefault = "Rte_Read_RP_"
    OUTDefault = "Rte_Write_PP_"
    # ------------------------------
    # Data Manipulation
    #createExcelFile(df, 'dfdf.xlsx')
    #print('Creating function call for ' + args.variant)
    string = []
    moduleName = []
    modName = df.iat[0, Module_col_ind13]
    for row in range(df.shape[0]): # Starstr
        if df.iat[row, frModule_columm_ind2] in NOT:
            continue
        MDL_ModVar = df.iat[row, MDL_ModVar_col_ind10]
        CAN_Variable = df.iat[row, CAN_Variable_col_ind14]
        CAN_ModVar = df.iat[row, CAN_ModVar_col_ind15]
        state = df.iat[row, State_col_ind1]
        inputFrom = df.iat[row, frModule_columm_ind2]
        if state == "IN":
            # CAN and VP -------------------------------------------------------
            # INDefault    + posKey  + Module  + MDL_ModVar        + MDL_ModVar
            # Rte_Read_RP_ + CANRx_  + CUS_    + CustomChange_ACCL + (uint8 *data)

            # CTL and Feat -----------------------------------------------------
            # INDefaule    + posKey  + Module  + CAN_ModVar        + MDL_ModVar
            # Rte_Read_RP_ + Feat_   + FS_     + DetFailCode_0     + (uint8 *data)

            posKey = getModPosKey(df, row, frModule_columm_ind2)
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
            posKey = getModPosKey(df, row, Module_col_ind13)
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
    Module = 'Module'
    CAN_Variable = 'CAN_Variable'
    CAN_SigName = 'CAN_SigName'
    CAN_ModVar = 'CAN_ModVar'
    MDL_Count = 'MDL_Count'

    df = readExcelFile(args.data, inputData)
    df[Module] = df['対象モデル']
    df['対象モデル'] = df.対象モデル.astype(str).str.lower()
    charMe = args.variant[:-2].lower()
    df = df[df['対象モデル'].str.contains(charMe)]
    #print(newDf)

    if len(df.head(1)) == 0:
        print("No Object Model for ", args.variant)
    else:
    
        #Filter Data from ExtractedData
        df = df.rename(columns={'SigName(CAN/LIN/MDL)': CAN_SigName})
        
        df['入力元'] = reg_replace(df, '入力元', r'[\(\)]', '')
        for i in unwantedString:
            df['入力元'] = replace(df, '入力元', i, '')

        df[CAN_Variable] = reg_replace(
            df, CAN_SigName, r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')
        df[CAN_Variable] = reg_replace(
            df, CAN_Variable, r'^(\d+)|([\u4e00-\u9fff]+)|([\u3040-\u309Fー]+)|([\u30A0-\u30FF]+)|(-)$', '')

        for i in unwantedString:
            df[CAN_Variable] = replace(df, CAN_Variable, i, '')
 
        df[CAN_ModVar] = df['入力元'] + '_' + df[CAN_Variable]

        df[MDL_Count] = reg_replace(df, MDL_Count, r'[\[\]]', '')
        df[MDL_Count] = df[MDL_Count].astype(str)
        

        #createExcelFile(df, 'Sample.xlsx')
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
        "Please input arguments: python FunctionVariable.py -v [filename].c -d [filename].xlsx")
