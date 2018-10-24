import pandas as pd
import re

def reg_replace( df, from_column, regex, str_replace):
    return df[from_column].astype(str).replace(regex, str_replace, regex=True)

def drop(df, from_column, str_drop):
    return df.drop(df[(df[from_column] == str_drop)].index)

def replace(df, from_column, str_before, str_replace):
    return df[from_column].replace(str_before, str_replace)

def replaceLine(filename, line_num, text, space):
    INLines = open(filename, 'r').readlines()
    INLines[line_num] = space +text + '\n\n'
    out = open(filename, 'w')
    out.writelines(INLines)
    out.close()

def setDataVariable(filename, df, string, column_name, skip_count, spaces):
    lineNumber = findLine(filename, string)
    lineNumber = lineNumber + skip_count
    function_Name = df[column_name]

    if lineNumber:
        for datas in range(function_Name.shape[0]):
            replaceLine(filename, lineNumber,
                        function_Name.iat[datas], spaces)
            lineNumber = lineNumber + 1

def findLine(filename, string):
    lineNumber = 0
    with open(filename, 'r') as f:
            for line in f.readlines():
                nLine = line.strip()
                if bool(re.match(string, nLine)):
                    break
                else:
                    lineNumber = lineNumber + 1
    f.close()
    return lineNumber + 1
