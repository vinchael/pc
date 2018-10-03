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
    df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'CAN_SigName'})

    # ------------------------------
    # Create column data type from SigName(MDL)
    # Remove unwanted strings, arrays and values
    df['MDL_DataType'] = df['型[配列サイズ]'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
    df = df.drop(df[(df['MDL_DataType'] == "nan") | (
        df['MDL_DataType'] == "-") | (df['MDL_DataType'] == "")].index)
    df['MDL_DataType'] = df['MDL_DataType'].replace(
        ['single', 'Single'], 'float32')

    # ------------------------------
    # Create column variable names from SigName(MDL)
    df['MDL_Variable'] = df['SigName(MDL)'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    # ------------------------------
    # Create 2d array and 1d array from Signame(MDL)
    df['MDL_Array2D1D'] = df['型[配列サイズ]'].astype(str).str.replace(
        r'^\w*\d{0,2}[^\[]|(\[\D+\]|\[\D+\]\[\D+\])', '', regex=True)
    df['MDL_Array2D'] = df['MDL_Array2D1D'].astype(str).str.replace(
        r'^\[\d*\]$', '', regex=True)

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
    df['MDL_Count'] = df['MDL_Count'].replace([
        np.nan], '')
    df['MDL_Merge'] = df['MDL_DataType']+' ' + \
        df['MDL_Variable'] + df['MDL_Count'].astype(str) + ';'

    # ------------------------------
    # drop duplicate column in MDL_Combine
    # Create final df for MDL
    df = df.drop_duplicates(subset=['MDL_Combine'], keep='last')
    MDL_df = df[["対象モデル", "MDL_Merge"]]
    MDL_df = MDL_df.rename(columns={'MDL_Merge': 'DataType',
                                    '対象モデル': 'Model'})

    # ------------------------------
    # CAN SIGNAME
    # ------------------------------
    # Create column variable names from CAN_SigName
    # Remove unwanted strings, arrays and values
    df['CAN_Variable'] = df['CAN_SigName'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)
    df['CAN_Variable'] = df['CAN_Variable'].astype(str).str.replace(
        r'^(\d+)|([\u4e00-\u9fff]+)|([\u3040-\u309Fー]+)|([\u30A0-\u30FF]+)|(-)$', '', regex=True)
    df['CAN_Variable'] = df['CAN_Variable'].replace(
        ['パラ', 'nan', 'OFF', 'ON'], '')
    
    # ------------------------------
    # Create 2d array and 1d array from Signame(CAN)
    df['CAN_Array2D1D'] = df['CAN_Variable']
    df.CAN_Array2D1D[df['CAN_Array2D1D'] != ''] = df['MDL_Count']

    # ------------------------------
    # Create column datat type from CAN_SigName
    df['CAN_DataType'] = df['CAN_Variable']
    df.CAN_DataType[df['CAN_DataType'] != ''] = df['MDL_DataType']

    # ------------------------------
    # Combine and count duplicate data from CAN_SigName
    # Create Model for CAN_SigName
    df['CAN_Model'] = df['CAN_Variable']
    df.CAN_Model[df['CAN_Model']!= ''] = df['対象モデル']
    df['CAN_Combine'] = df['CAN_DataType'] + ' ' + df['CAN_Model'] + \
        ' ' + df['CAN_Variable'].astype(str)
    df['CAN_Count'] = df.CAN_Combine.map(df.CAN_Combine.value_counts())
    df['CAN_Count'] = ('[' + df['CAN_Count'].astype(str) + ']')
    df['CAN_Count'] = df['CAN_Count'].replace(['[1]','[1286]'], '')
    countCond = df.CAN_Count == ''
    df.CAN_Count[countCond] = df.CAN_Array2D1D[df.CAN_Array2D1D != '']

    # ------------------------------
    # Create ; column for CAN_SigName
    df['CAN_Colon'] = df['CAN_Variable']
    df.CAN_Colon[df['CAN_Colon'] != ''] = ';'

    # ------------------------------
    # Create CAN_SigName to merge
    df['CAN_Count'] = df['CAN_Count'].replace([np.nan], '')
    df['CAN_Merge'] = df['CAN_DataType']+' ' + \
        df['CAN_Variable'] + \
        df['CAN_Count'].astype(str) + df['CAN_Colon']
    
    # ------------------------------
    # Create final df for CAN
    # drop duplicate column in CAN_Combine
    # Change column name to DataType
    CAN_df = df[["CAN_Model", "CAN_Merge"]]
    CAN_df = CAN_df.rename(columns={'CAN_Merge': 'DataType',
                                    'CAN_Model': 'Model'})
    
    # ------------------------------
    # Combine two data frame
    df = [MDL_df, CAN_df]
    df = pd.concat(df, sort=False, ignore_index=True)
    df = df.dropna(subset=['DataType'])
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





