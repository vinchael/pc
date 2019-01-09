from Car_Para import *
import sys
import os

def start_process(df, dic):

    # creating header in file
    for i in Car_Para_FileName_H:
        create_header(i, "_H")
        pass

    for i in Car_Para_FileName_C:
        create_header(i, "_C")
        pass
    
    # addition of include
    write_1line(Car_Para_FileName_C[0], MDL_Car_Param_h)
    #write_1line(Car_Para_FileName_C[1], MDL_VC_Param_h)
    write_1line(Car_Para_FileName_H[0], Rte_Type)

    create_contents(df, dic)
    #create_contentsforSub(df['Sub'], dic)


    # creating footer in file
    for i in Car_Para_FileName_H:
        create_footer(i, "\n\n#endif\n\n")
        pass

    for i in Car_Para_FileName_C:
        create_footer(i, "\n\n#endif\n\n")
        pass

def main(argv):

    filename = readArgParse()
    if filename:
        df, dic = {}, {}
        count = 0
        xls = readExcelFile(filename)
        for sheet_name in xls.sheet_names:
            #print('Sheet name:', sheet_name, 'index:',count)
            df[sheet_name] = xls.parse(sheet_name)
            dic[count] = sheet_name
            count = count + 1
            df[sheet_name] = df[sheet_name].replace(
                r'\s+', np.nan, regex=True)
            df[sheet_name] = df[sheet_name].fillna(NULL)
        # --START--
        start_process(df, dic)

    else:
        print('Please input: python ' + sys.argv[0] + ' -h')


if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])
