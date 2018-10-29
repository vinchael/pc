columnHeader = {0: 'IN',
                1: 'OUT',
                2: 'Model',
                3: 'DataType',
                4: 'IN_OUT',
                5: 'Input_Source',
                6: 'CANSigName'}
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
    df = df.drop_duplicates(subset=['Model', 'DataType'])
    #print(df)
    # ------------------------------ NEW -----------------------------------#
    INStartSkipCount = 4
    INStartStr = r'(.*)Input Interfaces:(.*)'
    INStartNo = findLine(args.variant, INStartStr)
    INStartNo = INStartNo + INStartSkipCount
    
    INEndSkipCount = 1
    INEndStr = r'(.*)Output Interfaces:(.*)'
    INEndNo = findLine(args.variant, INEndStr)
    INEndNo = INEndNo - INEndSkipCount

    OUTStartSkipCount = 5
    OUTStartNo = INEndNo + OUTStartSkipCount
    OUTEndSkipCount = 3
    OUTEndStr = r'(.*)DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>(.*)'
    OUTEndNo = findLine(args.variant, OUTEndStr)
    OUTEndNo = OUTEndNo - OUTEndSkipCount
    
    INLines = []
    while INStartNo < INEndNo:
        INLines.append(concatLine(args.variant, INStartNo))
        INStartNo += 1
 
    INdf = pd.DataFrame(INLines,columns=['IN'])
    INdf['IN'] = INdf.IN.str.replace(r'(^.*:.*$)|([\r\n]+)', '')
    INdf = INdf.drop(INdf[(INdf['IN'] == "")].index)

    OUTLines = []
    while OUTStartNo < OUTEndNo:
        OUTLines.append(concatLine(args.variant, OUTStartNo))
        OUTStartNo += 1

    OUTdf = pd.DataFrame(OUTLines, columns=['OUT'])
    OUTdf['OUT'] = OUTdf.OUT.str.replace(r'(^.*:.*$)|([\r\n]+)', '')
    OUTdf = OUTdf.drop(OUTdf[(OUTdf['OUT'] == "")].index)

    INdf.reset_index(drop=True, inplace=True)
    OUTdf.reset_index(drop=True, inplace=True)
    newDf.reset_index(drop=True, inplace=True)

    df = pd.concat([INdf, OUTdf, newDf], ignore_index=True, axis=1)
    df = df.rename(lambda x: columnHeader[x], axis=1)

    df.IN = df.IN.map(lambda x: str(x)[20:])
    df.OUT = df.OUT.map(lambda x: str(x)[20:])

    #print(OUTdf)
   
    #FUNCTIONVARIABLE.py
    for i in Module.keys():
        for value in Module[i]:
            INdf_ = INdf[INdf['入力元'].str.contains(value)]
            if value == "CAN":
                INdf_['CAN_Func'] = 'Rte_Read_RP_' + INdf_['入力元'] + 'Rx_' + INdf_['Module'] + '_' + \
                    INdf_['MDL_Variable'] + INdf_['Array2D1D'] + '( '
                print(INdf_)
                #createExcelFile(INdf_, 'Sample.xlsx')
    #print(df)

    from util import *
import sys
import pandas as pd

#             Sheet name , Use cols , Skip rows
#             [0]        , [1]      , [2]
inputData = ['Sheet1', 'A:L', 0]

Module = {'CAN_VP': ['CAN', 'VP'],
          'Others': ['Others']}


def createData(arges, df):
    # ------------------------------
    # Data Manipulation

    INdf = df.drop(df[(df['Unnamed: 1'] == "OUT")].index)

    #functionList = []
    #createExcelFile(df, 'Sample.xlsx')
    for i in Module.keys():
        for value in Module[i]:
            INdf_ = INdf[INdf['入力元'].str.contains(value)]
            if value == "CAN":
                INdf_['CAN_Func'] = 'Rte_Read_RP_' + INdf_['入力元'] + 'Rx_' + INdf_['Module'] + '_' + \
                    INdf_['MDL_Variable'] + INdf_['Array2D1D'] + '( '
                print(INdf_)
                #createExcelFile(INdf_, 'Sample.xlsx')
    #print(df)


def filterData(args):
    df = readExcelFile(args.data, inputData)

    #Filter Data from ExtractedData
    df = df.rename(columns={'SigName(CAN/LIN/MDL)': 'CAN_SigName'})

    df['Module'] = df['対象モデル']
    df['対象モデル'] = df.対象モデル.astype(str).str.lower()
    charMe = args.variant[:-2].lower()
    df = df[df['対象モデル'].str.contains(charMe)]
    #print(newDf)

    df['Array2D1D'] = df['CAN_SigName'].astype(str).str.replace(
        r'.*[^(\d{2})]', '', regex=True)

    if len(df.head(1)) == 0:
        print("No Object Model for ", args.variant)
    else:
        #setDataVariable(args, newDf)
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


newDf = df[df['Model'].str.contains(charMe)]

df['Array2D1D'] = reg_replace(df, 'CAN_SigName', r'.*[^(\d{2})]', '')
df['MDL_Array2D1D'] = reg_replace(df, 'MDL_Array2D1D', r'[\[\]]', '')
df['MDL_Array2D1D'] = replace(df, 'MDL_Array2D1D', 'nan', '')


