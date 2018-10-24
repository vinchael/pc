from util import *
from StringUtils import *
import sys
import numpy as np

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Interface', 'A,B,D,E,I,J,K', 12]

unwantedString = {'nan', '-', ''}
replaceString = {'single', 'Single'}

def  createData(df):
    # ------------------------------
    # Change column name for SigName(CAN/LIN/MDL)
    df = drop(df, 'Status', "Not Valid")

    # ------------------------------
    # Create column data type from SigName(MDL)
    # Remove unwanted strings, arrays and values
    df['MDL_DataType'] = reg_replace(
        df, '型[配列サイズ]', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')

    for i in unwantedString:
        df = drop(df, 'MDL_DataType', i)
    for i in replaceString:
        df['MDL_DataType'] = replace(df, 'MDL_DataType', i, 'float32')

    # ------------------------------
    # Create column variable names from SigName(MDL)
    df['MDL_Variable'] = reg_replace(
        df, 'SigName(MDL)', r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '')

    # ------------------------------
    # Create 2d array from Signame(MDL)
    df['MDL_Array2D1D'] = reg_replace(
        df, '型[配列サイズ]', r'^\w*\d{0,2}[^\[]|(\[\D+\]|\[\D+\]\[\D+\])', '')

    # ------------------------------
    # Create 1d array from Signame(MDL)
    df['MDL_Array2D'] = reg_replace(df, 'MDL_Array2D1D', r'^\[\d*\]$', '')

    # ------------------------------
    # For FunctionVariable.xlsx input file
    df['MDL_ModVar'] = df['対象モデル'] + '_' + df['MDL_Variable']
    createExcelFile(df, 'ForFunctionVariable.xlsx')

    # ------------------------------
    # Combine, count and drop duplicate data from Signame(MDL)
    df['MDL_Combine'] = df['MDL_DataType'] + ' ' + df['対象モデル'] + \
        ' ' + df['MDL_Variable']
    df['MDL_Count'] = df.MDL_Combine.map(df.MDL_Combine.value_counts())
    df['MDL_Count'] = ('[' + df['MDL_Count'].astype(str) + ']')
    df['MDL_Count'] = df['MDL_Count'].replace('[1]', '')
    countCond = df.MDL_Count == ''
    df.MDL_Count[countCond] = df.MDL_Array2D[df.MDL_Array2D != '']
    df.MDL_Count[countCond] = df.MDL_Array2D1D[df.MDL_Array2D1D != '']

    # ------------------------------
    # Create MDL_SigName to merge
    df['MDL_Count'] = replace( df, 'MDL_Count', np.nan, '' )
    df['MDL_Merge'] = df['MDL_DataType']+' ' + \
    df['対象モデル'] + '_' + df['MDL_Variable'] + \
    df['MDL_Count'].astype(str) + ';'

    # ------------------------------
    # drop duplicate column in MDL_Combine
    # Create final df for MDL
    df = df.drop_duplicates(subset=['MDL_Combine'], keep='last')
    df = df[["対象モデル", "MDL_Merge"]]
    df = df.rename(columns={'MDL_Merge': 'DataType', '対象モデル': 'Model'})

    #print("Finish creating dataframe")
    createExcelFile(df, 'ForMergingTextFile.xlsx')


def main(argv):
    filename = readArgParse()
    if filename:
        df = readExcelFile(filename, inputData)
        createData(df)
    else:
        print('Please input: python ' + sys.argv[0] + ' -h')

if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])


