from util import *
from StringUtils import *
import sys
import numpy as np

def createData(df):
    unwantedString = {'nan', '-', '', np.nan}
    replaceString = {'single', 'Single'}
    # Column header
    Module_col_ind0 = 0
    MDL_DataType_col_ind7 = 7
    MDL_Variable_col_ind8 = 8
    MDL_Count_col_ind12 = 12

    MDL_Count = 'MDL_Count'
    Combine_VarArr = 'Combine_VarArr'
    MDL_Variable = 'MDL_Variable'
    MDL_ModVar = 'MDL_ModVar'
    MDL_Array2D1D = 'MDL_Array2D1D'
    MDL_DataType = 'MDL_DataType'
    # ------------------------------
    # Change column name for SigName(CAN/LIN/MDL)
    df = drop(df, 'Status', "Not Valid")

    # ------------------------------
    # Create column data type from SigName(MDL)
    # Remove unwanted strings, arrays and values
    df[MDL_DataType] = reg_replace(
        df, '型[配列サイズ]', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')

    for i in unwantedString:
        df = drop(df, MDL_DataType, i)
    for i in replaceString:
        df[MDL_DataType] = replace(df, MDL_DataType, i, 'float32')

    # ------------------------------
    # Create column variable names from SigName(MDL)
    df[MDL_Variable] = reg_replace(
        df, 'SigName(MDL)', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')
    df[MDL_Variable] = reg_replace(
        df, MDL_Variable, r'\((.*){1,3}\)', '')
    
    # ------------------------------
    # Create 2d array from Signame(MDL)
    df[MDL_Array2D1D] = reg_replace(
        df, '型[配列サイズ]', r'^\w*\d{0,2}[^\[]|(\[\D+\]|\[\D+\]\[\D+\])', '')
        
    # ------------------------------
    # For FunctionVariable.xlsx input file
    df[MDL_ModVar] = df['対象モデル'] + '_' + df[MDL_Variable]
    
    # ------------------------------
    # Combine and drop duplicate variable name
    df[Combine_VarArr] = df['対象モデル'] + \
        df[MDL_Variable] + df[MDL_Array2D1D]
    df = df.drop_duplicates(subset=[Combine_VarArr], keep='last')
    
    # ------------------------------
    # Combine, count and drop duplicate data from Signame(MDL)
    df[MDL_Count] = df.MDL_ModVar.map(df.MDL_ModVar.value_counts())
    df = df.drop_duplicates(subset=[MDL_ModVar], keep='last')
    df[MDL_Count] = ('[' + df[MDL_Count].astype(str) + ']')
    countCond = df.MDL_Count == '[1]'
    df.MDL_Count[countCond] = df.MDL_Array2D1D[df.MDL_Array2D1D != '']
    for i in unwantedString:
        df[MDL_Count] = replace(df, MDL_Count, i, '')

    createExcelFile(df, 'ForFunctionVariable.xlsx')
    string = []
    moduleName = []
    for row in range(df.shape[0]):
        Module = df.iat[row, Module_col_ind0]
        MDL_DataType = df.iat[row, MDL_DataType_col_ind7]
        MDL_Variable = df.iat[row, MDL_Variable_col_ind8]
        MDL_Count = df.iat[row, MDL_Count_col_ind12]
        if MDL_Count == '':
            string.append(createVarName(
                Module, MDL_DataType, MDL_Variable, '', False))
            moduleName.append(Module)
        else:
            string.append(createVarName(Module, MDL_DataType,
                                        MDL_Variable, MDL_Count, True))
            moduleName.append(Module)
    df = pd.DataFrame({'Module': moduleName, 'DataType': string})
    createExcelFile(df, 'ForMergingTextFile.xlsx')

def createVarName( module, datatype, varName, index, boolean ):

    # datatype +  module + varName               +   index  ;
    # float32  +  CUS_   + varCurveDecInfoRadius + [ index ];
    string = datatype + ' ' + module + '_' + varName
    if boolean:
        return string + index + ';'
    else:
        return string + ';'

def main(argv):
    #             Sheet name , Use cols , Skip rows
    #             [0]        , [1]      , [2]
    inputData = ['Interface', 'A,B,D,E,I,J,K', 12]

    filename = readArgParse()
    if filename:
        df = readExcelFile(filename, inputData)
        createData(df)
    else:
        print('Please input: python ' + sys.argv[0] + ' -h')


if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])
