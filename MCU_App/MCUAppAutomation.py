from util import *
import sys
import numpy as np


#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Interface', 'A, E, I, J', 12]
inputData2 =['Sheet1', 'A:B', 0]

def  createData(df):

    # ------------------------------
    # Change column name for SigName(CAN/LIN/MDL)
    CAN_df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'CAN_SigName'})

    # ------------------------------
    # Create column data type from SigName(MDL)
    # Remove unwanted strings, arrays and values
    MDL_df = df
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
        MDL_df['MDL_Variable'] + MDL_df['MDL_Count'].astype(str) + ';'

    # ------------------------------
    # drop duplicate column in MDL_Combine
    # Create final df for MDL
    MDL_Res_df = MDL_df.drop_duplicates(subset=['MDL_Combine'], keep='last')
    MDL_Res_df = MDL_Res_df[["対象モデル", "MDL_Merge"]]
    MDL_Res_df = MDL_Res_df.rename(columns={'MDL_Merge': 'DataType',
                                    '対象モデル': 'Model'})

    # ------------------------------
    # CAN SIGNAME
    # ------------------------------

    # ------------------------------
    # Create column data type from SigName(CAN)
    # Remove unwanted strings, arrays and values
    CAN_df['CAN_DataType'] = CAN_df['型[配列サイズ]'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
    CAN_df = CAN_df.drop(CAN_df[(CAN_df['CAN_DataType'] == "nan") | (
        CAN_df['CAN_DataType'] == "-") | (CAN_df['CAN_DataType'] == "")].index)
    CAN_df['CAN_DataType'] = CAN_df['CAN_DataType'].replace(
        ['single', 'Single'], 'float32')
        
    # ------------------------------
    # Create column variable names from CAN_SigName
    # Remove unwanted strings, arrays and values
    # Remove unwanted strings, arrays and values
    CAN_df['CAN_Variable'] = CAN_df['CAN_SigName'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
    CAN_df['CAN_Variable'] = CAN_df['CAN_Variable'].astype(str).str.replace(
        r'^(\d+)|([\u4e00-\u9fff]+)|([\u3040-\u309Fー]+)|([\u30A0-\u30FF]+)|(-)$', '', regex=True)
    CAN_df['CAN_Variable'] = CAN_df['CAN_Variable'].replace(
        ['パラ', 'nan', 'OFF', 'ON'], '')
    CAN_df = CAN_df.drop(CAN_df[(CAN_df['CAN_Variable'] == "")].index)

    # ------------------------------
    # Create 2d array and 1d array from Signame(CAN)
    CAN_df['CAN_Array2D1D'] = CAN_df['型[配列サイズ]'].astype(str).str.replace(
        r'^\w*\d{0,2}[^\[]|(\[\D+\]|\[\D+\]\[\D+\])', '', regex=True)
    CAN_df['CAN_Array2D'] = CAN_df['CAN_Array2D1D'].astype(str).str.replace(
        r'^\[\d*\]$', '', regex=True)
    
    # ------------------------------
    # Combine, count and drop duplicate data from Signame(CAN)
    CAN_df['CAN_Combine'] = CAN_df['CAN_DataType'] + ' ' + CAN_df['対象モデル'] + \
        ' ' + CAN_df['CAN_Variable']
    CAN_df['CAN_Count'] = CAN_df.CAN_Combine.map(
        CAN_df.CAN_Combine.value_counts())
    CAN_df['CAN_Count'] = ('[' + CAN_df['CAN_Count'].astype(str) + ']')
    CAN_df['CAN_Count'] = CAN_df['CAN_Count'].replace('[1]', '')
    countCond = CAN_df.CAN_Count == ''
    CAN_df.CAN_Count[countCond] = CAN_df.CAN_Array2D[CAN_df.CAN_Array2D != '']
    CAN_df.CAN_Count[countCond] = CAN_df.CAN_Array2D1D[CAN_df.CAN_Array2D1D != '']

    # ------------------------------
    # Create CAN_SigName to merge
    CAN_df['CAN_Count'] = CAN_df['CAN_Count'].replace([
        np.nan], '')
    CAN_df['CAN_Merge'] = CAN_df['CAN_DataType']+' ' + \
        CAN_df['CAN_Variable'] + CAN_df['CAN_Count'].astype(str) + ';'

    # ------------------------------
    # drop duplicate column in CAN_Combine
    # Create final df for MDL
    CAN_Res_df = CAN_df.drop_duplicates(subset=['CAN_Combine'], keep='last')
    CAN_Res_df = CAN_Res_df[["対象モデル", "CAN_Merge"]]
    CAN_Res_df = CAN_Res_df.rename(columns={'CAN_Merge': 'DataType',
                                            '対象モデル': 'Model'})
    
    # ------------------------------
    # Combine two data frame
    df = [MDL_Res_df, CAN_Res_df]
    df = pd.concat(df, sort=False, ignore_index=True)
    df = df.drop_duplicates(subset=['Model','DataType'])
    #print(df)

    print("Finish creating dataframe")
    createExcelFile(df, 'DataExtracted.xlsx')

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





