from util import *
import sys
import numpy as np

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Interface', 'A,B,D,E,I,J,K', 12]

def  createData(df):
    # ------------------------------
    # Change column name for SigName(CAN/LIN/MDL)
    MDL_df = df.drop(df[(df['Status'] == "Not Valid")].index)
    
    # ------------------------------
    # Create column data type from SigName(MDL)
    # Remove unwanted strings, arrays and values
    MDL_df['MDL_DataType'] = MDL_df['型[配列サイズ]'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
    MDL_df = MDL_df.drop(MDL_df[(MDL_df['MDL_DataType'] == "nan") | (
        MDL_df['MDL_DataType'] == "-") | (MDL_df['MDL_DataType'] == "")].index)
    MDL_df['MDL_DataType'] = MDL_df['MDL_DataType'].replace(
        ['single', 'Single'], 'float32')

    # ------------------------------
    # Create column variable names from SigName(MDL)
    MDL_df['MDL_Variable'] = MDL_df['SigName(MDL)'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    # ------------------------------
    # Create 2d array and 1d array from Signame(MDL)
    MDL_df['MDL_Array2D1D'] = MDL_df['型[配列サイズ]'].astype(str).str.replace(
        r'^\w*\d{0,2}[^\[]|(\[\D+\]|\[\D+\]\[\D+\])', '', regex=True)
    MDL_df['MDL_Array2D'] = MDL_df['MDL_Array2D1D'].astype(str).str.replace(
        r'^\[\d*\]$', '', regex=True)

    MDL_df['MDL_ModVar'] = MDL_df['対象モデル'] + '_' + MDL_df['MDL_Variable']

    createExcelFile(MDL_df, 'ForFunctionVariable.xlsx')
    # ------------------------------
    # Combine, count and drop duplicate data from Signame(MDL)
    MDL_df['MDL_Combine'] = MDL_df['MDL_DataType'] + ' ' + MDL_df['対象モデル'] + \
        ' ' + MDL_df['MDL_Variable']  
    MDL_df['MDL_Count'] = MDL_df.MDL_Combine.map(
        MDL_df.MDL_Combine.value_counts())
    MDL_df['MDL_Count'] = ('[' + MDL_df['MDL_Count'].astype(str) + ']')
    MDL_df['MDL_Count'] = MDL_df['MDL_Count'].replace('[1]', '')
    countCond = MDL_df.MDL_Count == ''
    MDL_df.MDL_Count[countCond] = MDL_df.MDL_Array2D[MDL_df.MDL_Array2D != '']
    MDL_df.MDL_Count[countCond] = MDL_df.MDL_Array2D1D[MDL_df.MDL_Array2D1D != '']

    
    # ------------------------------
    # Create MDL_SigName to merge
    MDL_df['MDL_Count'] = MDL_df['MDL_Count'].replace([
        np.nan], '')
    MDL_df['MDL_Merge'] = MDL_df['MDL_DataType']+' ' + \
        MDL_df['対象モデル'] + '_'+ MDL_df['MDL_Variable'] + \
        MDL_df['MDL_Count'].astype(str) + ';'
    
    # ------------------------------
    # drop duplicate column in MDL_Combine
    # Create final df for MDL
    MDL_Res_df = MDL_df.drop_duplicates(subset=['MDL_Combine'], keep='last')
    MDL_Res_df = MDL_Res_df[["対象モデル", "MDL_Merge",
                             "Unnamed: 1", "入力元", "SigName(CAN/LIN/MDL)",
                             "MDL_ModVar"]]
    MDL_Res_df = MDL_Res_df.rename(columns={'MDL_Merge': 'DataType',
                                            '対象モデル': 'Model'})
    
    #print("Finish creating dataframe")
    createExcelFile(MDL_Res_df, 'ForMergingTextFile.xlsx')


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


