import os
import sys
import pandas as pd
import numpy as np
import argparse as ap
from pandas import ExcelWriter

# -------------------------------------------------------------------------

parser = ap.ArgumentParser(description='Eyesight Reader')
parser.add_argument('-v', '--variant', dest="variant",
                    help='Variant (default: GC7)')
args = parser.parse_args()


# Variant name :     Sheet name , Use cols , Skip rows
#                    [0]        , [1]      , [2]
input_data = {'GC7': ['G○7_国内', 'B, E:AJ', 2],
              'RE7': ['R○7_国内', 'B, E:AJ', 2]}


class SheetClass(object):
    def __init__(self, file_name, argv):
        # Get the file_name
        if file_name:
            self.xml_file = file_name
        else:
            base_path = os.path.dirname(os.path.realpath(__file__))
            self.xml_file = os.path.join(base_path, "EyeSight_Ver.4_ SSM.xls")
        print('File name: ' + self.xml_file)

        # Get the sheet_name
        if args.variant:
            self.variant = args.variant
        else:
            self.variant = 'GC7'
        print('Sheet name: ' + input_data[self.variant][0])

        # Setting the header
        self.df = pd.read_excel(self.xml_file,
                                sheet_name=input_data[self.variant][0],
                                usecols=input_data[self.variant][1],
                                skiprows=input_data[self.variant][2])

    def filterData(self):
        # The column 0 convert from hex to decimal
        self.df['Unnamed: 0'] = self.df['Unnamed: 0'].astype(str).dropna()\
                                    .apply(lambda x: int(x, 16))

        createdString = []
        print('Creating...')
        for row in range(self.df.shape[0]):
            for col in range(1, 33):
                columnHeader = format(col, '02x').upper()
                if self.df.at[row, columnHeader] == 1:
                    # Get all the necessary values
                    createdString.append(format(self.df.at[row,
                                                'Unnamed: 0'] + col,
                                                '02x').upper())
                elif self.df.at[row, columnHeader] == 0:
                    pass
                else:
                    print("Not 0/1 value, check index row[{}] Col[{}] "
                          .format(row, columnHeader))
        # print(createdString)
        self.newDf = pd.DataFrame(createdString,
                                  columns=['Data_' +
                                           input_data[self.variant][0]])

    def saveData(self):
        writer = ExcelWriter('EyeSight_Ver_' +
                             input_data[self.variant][0] + '.xlsx')
        self.newDf.to_excel(writer, input_data[self.variant][0], index=False)
        writer.save()
        writer.close()

if __name__ == "__main__":
    print("Start")
    # Open file and parse contents
    xmlFile = "EyeSight_Ver.4_ SSM.xls"
    EyeSightClass = SheetClass(xmlFile, sys.argv[1:])
    EyeSightClass.filterData()
    EyeSightClass.saveData()
    print("End")
