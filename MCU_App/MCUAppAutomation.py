from util import *
import sys
import numpy as np


#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Interface', 'A, E, I, J', 12]
inputData2 =['Sheet1', 'A:B', 0]

def  createData(df):
    df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'SigName'})

    #remove the [][] only and create type column copy
    df['dtype'] = df['型[配列サイズ]'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    newdf = df.drop(df[(df['dtype'] == "nan") | (
        df['dtype'] == "-") | (df['dtype'] == "")].index)

    newdf['dtype'] = newdf['dtype'].replace(['single', 'Single'], 'float32')

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
    
    #get the datatype text only and remove [][]
    newdf['arrayValue'] = newdf['型[配列サイズ]'].astype(str).str.replace(
        r'^\w+\d{0,2}[^\[]', '', regex=True)

    newdf['arrayValue2'] = newdf['arrayValue'].astype(str).str.replace(
        r'^\[\d*\]$', '', regex=True)

    #get variable names without []
    newdf['variable_names'] = newdf['SigName(MDL)'].astype(str).str.replace(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', regex=True)

    newdf['remCombine'] = newdf['dtype'] + ' ' + newdf['対象モデル'] + \
        ' ' + newdf['variable_names']

    newdf['counts'] = newdf.remCombine.map(newdf.remCombine.value_counts())
    newdf['counts'] = ('[' + newdf['counts'].astype(str) + ']')
    newdf['counts'] = newdf['counts'].replace('[1]', '')

    countsCond = newdf.counts == ''
    newdf.counts[countsCond] = newdf.arrayValue2[newdf.arrayValue2 != '']
    newdf.counts[countsCond] = newdf.arrayValue[newdf.arrayValue != '']

    newdf['counts_CAN_LIN_RTE'] = newdf['CAN_LIN_RTE']
    newdf.counts_CAN_LIN_RTE[newdf['counts_CAN_LIN_RTE']
                             != ''] = newdf['counts']

    newdf['colon'] = newdf['CAN_LIN_RTE']
    newdf.colon[newdf['colon']
                != ''] = ';'

    newdf['CAN_LIN_RTE'] = newdf['CAN_LIN_RTE'].replace([
        ''], np.nan)

    newdf['counts_CAN_LIN_RTE'] = newdf['counts_CAN_LIN_RTE'].replace([
        np.nan], '')

    newdf['DataType_CAN_LIN_RTE'] = newdf['dtype_CAN_LIN_RTE']+' ' + \
        newdf['CAN_LIN_RTE'] + \
        newdf['counts_CAN_LIN_RTE'].astype(str) + newdf['colon']
    
    newdf['counts'] = newdf['counts'].replace([
        np.nan], '')

    newdf['DataType'] = newdf['dtype']+' ' + \
        newdf['variable_names'] + newdf['counts'].astype(str) + ';'

    #CreateExcelFile(newdf, 'DataExtracted.xlsx')
    
    dropdf = newdf.drop_duplicates(subset=['remCombine'], keep='last')        

    finaldf1 = dropdf[["対象モデル", "DataType"]]
    
    finaldf2 = dropdf[["対象モデル", "DataType_CAN_LIN_RTE"]]
    
    finaldf2 = finaldf2.rename(columns={'DataType_CAN_LIN_RTE': 'DataType'})

    df_concat = [finaldf1, finaldf2]
    result = pd.concat(df_concat, sort=False, ignore_index = True)
    
    
    toExtractdf = result.dropna(subset=['DataType'])

    thisIsFinaldf = toExtractdf.drop_duplicates(subset=['DataType'], keep='last')
    #print(toExtractdf)
    
    createExcelFile(thisIsFinaldf, 'DataExtracted.xlsx')
    print("Finish creating dataframe")
    
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





