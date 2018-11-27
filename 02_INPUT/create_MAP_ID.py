from Util import *
import sys
import os


CarVariant = {
    'idx_VariantPadTypeF',
    'idx_VariantPadTypeR',
    'idx_variant_pt',
    'idx_variant_tm',
    'idx_variant_for_ldp',
    'idx_variant_for_lks',
}
# tm
oneDiTM = {
    'RS_TM': '11',
    'RS_INERTIA': '6'
}
twoDiTM = {
    'TM_EFFI': ['20', '10'],
    'TRCON_TRQ': ['13', '11'],
    'TM_EFFICIENCY': ['20', '10'],
}

# iPAD
oneDiPAD = {
    'BrakePad_uMAP': '6',
}
twoDiPAD = {
    'BrakePad_uMAP': ['7', '6'],
}

# iLDP
oneDiLDP = {
    'STR_DEG': '31',
    'TGT_TRQ': '16',
    'SLDP_TGT': '10',
    'SLDP_AY': '8',
    'LDP_YokoG': '8',
    'LDP_Yawr': '8',
    'k_LDP': '8',
    'k_ELK': '8',
    'SLDP_R': '6',
    'SLDP_FRIC': '6',
}
twoDiLDP = {
    'STR_DEG': ['7', '31'],
    'TGT_TRQ': ['7', '16'],
    'RHO_CALC': ['7', '31'],
    'SLDP_TGT': ['6', '10'],
    'SLDP_CTRL': ['6', '10'],
    'SLDP_YAWRFB': ['6', '10'],
    'SLDP_FRIC': ['6', '5'],
    'SLDP_TRQ': ['6', '10'],
    'SLDP_AY': ['8', '10'],
    'SLDP_KIRIMODOSHI': ['8', '10'],
    'SLDP_KIRI': ['8', '10'],
    'SLDP_R': ['7', '6'],
    'SLDP_TGT': ['7', '6'],
    'TGT_YAWR': ['10', '7'],
    'ELK_YAWRFB': ['6', '10'],
    'ELK_TRQ': ['6', '10'],
}

# iLKS
oneDiLKS = {
    'FF_STR': '10',
    'TGT_STR': '10'
}
twoDiLKS = {
    'TJA_K': ['11', '16'],
    'TJA_STR': ['11', '16']
}

# PT
# KASOKU_MODE_GAIN_TUIBI_936 KASOKU_MODE_GAIN_TEISOKU_936
# k_eg_cvt_rate_936 TOWING_LIMIT_936 THR_OFF_EGTRQ_936
# ATM_RDTRQ_4TRQ_936
oneDPT = {
    'RPM_FBINH': '17',
    'TRQ_FCUT': '10',
    'TRQ_AC': '10',
    'TRQ_ENG': '10',
    'TGT_STR': '10',
    'FF_STR': '10',
    'ENGINE_RPM': '17',
    'ACTUAL_TRQ': '13',
    'FILTREQ_TRQ': '13',
    'bFB_I': '15',
    'bFB_P': '15',
    'aFB_I': '15',
    'aFB_P': '15',
    'ATR_FWD': '15',
    'ATR_REV': '15',
    'A_FMW': '15',
    'FMW_Goffset': '15',
    'FMW_FM': '24',
    #
    'KASOKU_MODE' : '10',
    'k_eg' : '1',
    'TOWING_LIMIT' : '1',
    'THR_OFF' : '1',
    'ATM_RDTRQ' : '17',
    'FMW_ACTUAL' : '24'
}
twoDPT = {
    'RS_AC': ['11', '13'],
    'KASOKU_GAIN': ['33', '17'],
    'RPM_RATIO': ['13', '7'],
    'ENGINE_RPM': ['8', '7'],
    'TARGET_A': ['7', '7'],
    'TRCON_DECEL': ['8', '7'],
    'TARGET_TURBIN': ['36', '17'],
    'EGTRQ_CHAR': ['17', '16'],
    'ATRBRK_DFB': ['13', '6'],
    'EG_AFB': ['13', '6'],
    'TJA_STR': ['11', '16'],
    'ATM_RDTRQ': ['1', '17'],
    'THR_OFF': ['1', '9'],
    'KASOKU_MODE': ['1', '10'],
}

create_data_variant = {   #sinle row,  double row,col
    'iPAD_F_CAR_PARA_' : [oneDiPAD,  twoDiPAD],
    'iPAD_R_CAR_PARA_' : [oneDiPAD,  twoDiPAD],
    'iEG_CAR_PARA_'    : [oneDPT,   twoDPT],
    'iTM_CAR_PARA_'    : [oneDiTM,  twoDiTM],
    'iLDP_CAR_PARA_'   : [oneDiLDP, twoDiLDP],
    'iLKS_CAR_PARA_'   : [oneDiLKS, twoDiLKS]
}


member_variable_name_tag = 'Member Variable Name'
parameter_name_tag = 'Parameter Name'
map_valid_values = {'OFF', 'ON'}

switching_variable = []
detailed_action = []


def get_detailed_action(df):
    for i in range(4, df.shape[0]):
        switching_variabl_list = df.iat[i, 2]
        if switching_variabl_list in CarVariant:
            detailed_action.append(str(df.iat[i, 6]))
            switching_variable.append(switching_variabl_list)

def get_repetition_count_row(result, parameter_name_value):
    global row_counter_map_id, counter_map_id, col_counter_map_id

    counter_map_id = 0
    row_counter_map_id = 0
    col_counter_map_id = 0
    row_counter_flag = ON

    for i in range(0, result.shape[0]):
        initial_value = result.iat[0, 1]
        if initial_value == result.iat[i, 1]:
            if counter_map_id == 1:
                row_counter_flag = OFF
            counter_map_id = counter_map_id + 1
            for j in range(2, result.shape[1]):
                column_value_umpersand = result.iat[i, j]
                if re.search("&", str(column_value_umpersand)) or \
                        column_value_umpersand in map_valid_values:
                    col_counter_map_id = col_counter_map_id + 1

        if row_counter_flag:
            row_counter_map_id = row_counter_map_id + 1

    #print(counter_map_id, row_counter_map_id, col_counter_map_id)

def is_valid(result, row, col):
    tofind = str(result.iat[row, col])
    if re.search("&", str(tofind)) or \
        str(tofind) in map_valid_values:
            return 1
    else:
        return 0

def display_map_id(result, row, col, cur_value, create_data, parameter_name_value):
    if cur_value == 1:
        remove_braces = result.iat[row, col]
        #print(remove_braces, '1')
        remove_braces = remove_braces[1:-5]
        holder_name = re.match("^(.*?_.*?)_", str(remove_braces)).group(1)
        create_data.append(
            'for (uint8 i = 0; i < ' + str(create_data_variant[parameter_name_value][0].get(holder_name)[0]) +
            '; i++)\n' + triple_tab + '{\n' + four_tab +
            str(holder_name) + '[i] = ' + str(remove_braces) +
            '[i];\n' + triple_tab + '}\n'
        )
        
    elif cur_value == 2:
        remove_braces = result.iat[row, col]
        #print(remove_braces, '2')
        remove_braces = remove_braces[1:-10]
        holder_name = re.match("^(.*?_.*?)_", str(remove_braces)).group(1)
        create_data.append(
            'for (uint8 i = 0; i < ' + str(create_data_variant[parameter_name_value][1].get(holder_name)[0]) +
            '; i++)\n' + triple_tab + '{\n' + four_tab +
            'for (uint8 j = 0; j < ' + str(create_data_variant[parameter_name_value][1].get(holder_name)[1]) +
            '; i++)\n' + four_tab + '{\n' + five_tab +
            str(holder_name) + '[i][j] = ' + str(remove_braces) +
            '[i][j];\n' + four_tab + '}\n' + triple_tab + '}\n'
        )
        
    else:
        remove_braces = result.iat[row, col]
        if remove_braces in map_valid_values:
            create_data.append(
                str(remove_braces) + '_' + ' = ' + str(remove_braces) + ';\n'
            )
        else:
            remove_braces = str(remove_braces).replace('&', '')
            holder_name = remove_braces[:6]
            create_data.append(
                str(holder_name) + ' = ' + str(remove_braces) + ';\n'
            )
        
    return(create_data)


def create_static_variable(result, row, col):
    pass

def creating_data(result, parameter_name_value):
    create_data = []
    result = result.reset_index()
    get_repetition_count_row(result, parameter_name_value)
    # change NULL to xxx
    #result = df.replace(np.nan, '', regex=True)

    print(counter_map_id, row_counter_map_id, col_counter_map_id)
    #print(result)
    #createExcelFile(result, 'Today.xlsx')

    cur_value = ''
    for col in range(2,result.shape[1]):
        row_counter = 0
        for row in range(0, result.shape[0]):
            if row_counter == row_counter_map_id:
                row_counter = 0

            if row_counter == 0 and is_valid(result, row, col):
                #print(row,col)
                create_data.append(
                    tab + 'switch (' + parameter_name_value +
                    ')\n' + tab + '{\n' + double_tab
                )
                # create static variable
                create_static_variable(result, row, col)
            
            if is_valid(result,row,col) and row_counter != 0:
                cur_value = result.iat[row,col]
                cur_value = str(cur_value).count('[')
                #print(cur_value, row_counter)
                create_data.append(
                    'case (uint8)' + str(row_counter) + ':\n' + double_tab +
                    '{\n' + triple_tab
                )
                create_data = display_map_id(
                    result, row, col, cur_value, create_data, parameter_name_value)
                create_data.append(
                    triple_tab + 'break;\n' + double_tab + '}\n' + double_tab
                )
                
            row_counter = row_counter + 1

            if row_counter == row_counter_map_id:
                if is_valid(result, row, col):
                    cur_value = result.iat[row - (row_counter_map_id - 1), col]
                    cur_value = str(cur_value).count('[')
                    create_data.append(
                        'default:\n' + double_tab + '{\n' + triple_tab
                    )
                    create_data = display_map_id(
                        result, row - (row_counter_map_id - 1), col, cur_value, create_data, parameter_name_value)
                    create_data.append(
                        triple_tab + 'break;\n' + double_tab + '}\n'
                    )
    
    write_list('Sample.c', create_data)

def start_process(df, dic):
    get_detailed_action(df['LIST'])
    data_conversion_list = pd.DataFrame(
        {'Switching_variable': switching_variable, 'Detailed_actioin': detailed_action})

    combined = pd.concat([data_conversion_list, df['MAP_ID']], ignore_index=True, axis=1)
    combined = combined.fillna(NULL)
    #print(combined)
    for i in range(0,5):
        parameter_name_value = combined.iat[i, 1]
        if parameter_name_value in exception_string:
            break
        else:
            parameter_name_value2 = parameter_name_value
            if re.search("iPAD", str(parameter_name_value)):
                parameter_name_value = 'iPAD_'
            else:
                pass
            
            # get all the variant and remove NULL column values
            result = combined[combined[4].str.contains(
                parameter_name_value , case=False)]
            result = result.loc[:, (result != NULL).any(axis=0)]

            creating_data(result, parameter_name_value2)

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
        # --START--
        #start_process(df, dic)
        creating_data(df['Sheet2'], 'iTM_CAR_PARA_')

if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])
