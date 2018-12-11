from Util import *


def create_type_code_for_hypen(df, sheet_name, dic):
    global type_code_for_hypen_declaration, type_code_for_hypen_definition

    type_code_for_hypen_declaration = []
    type_code_for_hypen_definition  = []

    maru_count = 0    

    for i in range(maru_count_start, df.shape[0]):
        map_maru_val = df.iat[i, map_maru_col]
        if map_maru_val == endofrow:
            continue
        if map_maru_val == maru:
            maru_count = maru_count + 1
            type_code_for_hypen_definition.append(
                str(df.iat[i,  + 2]) + ', '
            )


    type_code_for_hypen_declaration.append(
        extern + space + const + space + datatype_declaration[0] +
        space  + 'k_'  + sheet_name    + '_CAR_CODE_INDEX'       +
        '['    + str(maru_count)       + '];\n'
    )

    add_str_hypen = const + space + datatype_declaration[0]        + \
                    space + 'k_'   + sheet_name + '_CAR_CODE_INDEX' + \
                    '['   + str(maru_count)    + '] = \n{\n'       + tab

    type_code_for_hypen_definition.insert(0, add_str_hypen)

    type_code_for_hypen_definition.append(
        '\n};\n'
    )

def create_default_REQ_BRKPRS(ReqBrkPrsBaseF,
                              ReqBrkPrsBaseR,
                              brkTblCaliperFactSoft):
    global Req_Brk_declaration, Req_Brk_definition

    Req_Brk_declaration, Req_Brk_definition = [], []
    five_only = 0

    l_ChgReqBrkPrs, l_ChgReqBrkPrsNoClpr = 0, 0
    baseFact = 2 * ReqBrkPrsBaseF * ReqBrkPrsBaseR / (ReqBrkPrsBaseF + ReqBrkPrsBaseR)

    l_SetBrkPrs2ndArray = baseFact / 100

    Req_Brk_declaration.append(
        extern + ' ' +
        const  + ' ' +
        datatype_declaration[1] + ' ' +
        'default_REQ_BRKPRS[32];\n'
    )

    Req_Brk_definition.append(
        const + ' ' +
        datatype_declaration[1] +
        ' default_REQ_BRKPRS[32] = \n{\n' + tab + ' 0.F, '
    )

    Req_Brk_definition.append(
        str(l_SetBrkPrs2ndArray + brkTblCaliperFactSoft) + 'F, '
    )

    for i in range(3,32 + 1):
        l_ChgReqBrkPrsNoClpr = l_ChgReqBrkPrsNoClpr + baseFact
        l_ChgReqBrkPrs       = l_ChgReqBrkPrsNoClpr + brkTblCaliperFactSoft
        
        if five_only == 5:
            five_only = 0
            Req_Brk_definition.append(
                '\n' + tab
            )
        
        if five_only < 5:
            Req_Brk_definition.append(
                str(l_ChgReqBrkPrs) + 'F, '
            )
        
        five_only = five_only + 1

    Req_Brk_definition.append(
        '\n};\n'
    )

def default_str_declaration_func(appendto, structure_name, type):
    # default_xxx declaration
    appendto.append(
        extern + ' ' +
        const + ' ' +
        datatype_declaration[type] + ' ' +
        default_ +
        str(structure_name[3:]) + ';\n'
    )

def default_map_declaration_func(appendto, save_variable, maru_count, type):
    appendto.append(
        extern + ' ' +
        const + ' ' +
        datatype_declaration[type] + ' ' +
        str(save_variable) + '[' +
        str(maru_count) + '];\n'
    )

def create_car_type_code(df, dic, maru_count):
    h = Car_Para_FileName_H[0]
    c = Car_Para_FileName_C[0]
    type_code_declaration, type_code_definition = [], []


    type_code_declaration.append(
        extern                  + ' ' +
        const                   + ' ' +
        datatype_declaration[0] + ' ' +
        k_CAR_CAR_CODE_INDEX    + '[' +
        str(maru_count)         + '][10];\n'
    )


    type_code_definition.append(
        const + ' ' +
        datatype_declaration[0] + ' '       +
        k_CAR_CAR_CODE_INDEX    + '['       +
        str(maru_count)         + '][10] =' +
        '\n{\n'                 + tab       + 
        '{\n'                   + double_tab
    )
    for m in range(maru_count_start, df.shape[0]):
        map_maru_val = df.iat[m, map_maru_col]
        if map_maru_val == maru:
            for i in range(type_code_col, ten_bytes):
                type_code_value = df.iat[m, i]
                type_code_value = type_code_value.replace('$', '')
                type_code_dec = int(type_code_value, 16)

                type_code_definition.append(
                    str(type_code_dec) + ', '
                )

            type_code_definition.append(
                tab    + '/*'   +
                tab    + str(m - maru_count_start) +
                tab    + '*/'   +
                ('\n' + tab + '}\n};\n' if m == (df.shape[0] - 2)
                 else '\n' + tab + '},\n' + tab + '{\n' + double_tab)
                )

    write_list(h, type_code_declaration)
    write_list(c, type_code_definition)

def create_contents(df, dic):
    h = Car_Para_FileName_H[0]
    c = Car_Para_FileName_C[0]
    # sheet value
    key_count = len(dic.keys())

    structure_name_holder = ""
    default_REQ_BRKPRS_F = 0
    default_REQ_BRKPRS_R = 0
    default_REQ_BRKPRS_C = 0

    for i in range(4, key_count):
        # sheet_name is equal to Sheet name
        sheet_name = dic[i]
        save_variable, market_intfloat_flag = '', 0
        contents_flag, structure_flag, svn_flag = 0, 0, 0
        counter, ten_only, is_hypen = 0, 0, OFF

        contents_declaration   , contents_definition    = [], []
        structure_declaration  , structure_definition   = [], []
        default_str_declaration, default_str_definition = [], []
        map_declaration        , map_definition         = [], []
        map_id_declaration     , map_id_definition      = [], []
        
        # read column data
        # print('Sheet name:', sheet_name, "Total column count:", df[sheet_name].shape[1] - 1)
        for j in range(0,df[sheet_name].shape[1]):
            # default_xxx maru flag to check if it has default value
            # maru_flag set to 0 whenever changing column
            default_maru_flag = 1
            # if sheet name is equal to MAP_ID skip
            if (sheet_name == map_id) and (sheet_name == sub_sheet):
                continue
            # if 'Structure' text was found
            if (df[sheet_name].iat[cont_struct_row, j] == structure) and \
                (str(df[sheet_name].iat[cont_struct_row + 1, j]) not in exception_string):
                contents_flag  = 0
                structure_flag = 1
                structure_name_holder = df[sheet_name].iat[cont_struct_row + 1, j]
            if (df[sheet_name].iat[cont_struct_row, j] == structure) and \
                    (str(df[sheet_name].iat[cont_struct_row + 1, j]) == '-'):
                """ FOR k_***_CAR_CODE_INDEX """
                is_hypen = ON
                create_type_code_for_hypen(df[sheet_name], sheet_name, dic)
            # if it has contents 
            if (df[sheet_name].iat[cont_struct_row, j] == contents) and (sheet_name in has_contents):
                contents_flag = 1

            if contents_flag:
                contents_value = df[sheet_name].iat[cont_struct_row + 1, j]
                if contents_value in exception_string:
                    pass
                else:
                    if sheet_name == car_type:
                        contents_declaration.append(
                            extern                  + ' ' +
                            const                   + ' ' +
                            datatype_declaration[0] + ' ' +
                            str(contents_value)     + '[3];\n'
                        )

                        # creating decimal value from bit bx_x-x
                        bit_zero_list = list('00000000')  # 1byte
                        bit_shift_val_Arr = []
                        dec_shift_val_Arr = []
                        
                        bit_var_name     = df[sheet_name].iat[cont_struct_row + 1, j]
                        bit_str_position = df[sheet_name].iat[cont_struct_row + 2, j]

                        bit_loc = getchar(bit_str_position, 1)
                        bit_L   = getchar(bit_str_position, 3)
                        bit_R   = getchar(bit_str_position, 5)

                        if bit_var_name in exception_string:
                            pass
                        elif bit_str_position in exception_string:
                            pass
                        else:
                            if bit_R != '':
                                for x in range(int(bit_R), int(bit_L) + 1):
                                    bit_zero_list[x] = '1'
                            else:
                                bit_zero_list[int(bit_L)] = '1'

                            bit_holder = ''.join(bit_zero_list)

                            # Append result           invert the result
                            bit_shift_val_Arr = bit_holder[::-1]
                            dec_shift_val_Arr = bin_to_dec(bit_shift_val_Arr)
                            contents_definition.append(
                                const                   + ' '                    +
                                datatype_declaration[0] + ' '                    +
                                str(bit_var_name)       + '[3] =\n{\n'           +
                                tab                     + str(bit_loc)           +
                                ', '                    + str(dec_shift_val_Arr) +
                                ', ' + (str(bit_L) if str(bit_R) is '' else str(bit_R)) +
                                '\n};\n'
                            )
            
            if svn_flag:
                structure_name = df[sheet_name].iat[cont_struct_row + 1, j]
                # checking changes in CAR_TYPE structures
                if ((structure_name not in CAR_TYPE_checking) and (sheet_name == car_type)) and ( structure_name not in exception_string):
                    print( "Sheet name:", sheet_name, "Structure added:", structure_name)

                if (structure_name == market_float32_start) and (sheet_name == market):
                    market_intfloat_flag = 1
                if (structure_name == market_uint8_start)   and (sheet_name == market):
                    market_intfloat_flag = 0

                if structure_name in exception_string:
                    pass
                else:
                    if sheet_name == market:
                        if market_intfloat_flag:
                            structure_declaration.append(
                                extern + ' ' +
                                const + ' ' +
                                datatype_declaration[1] + ' ' +
                                str(structure_name) + ';\n'
                            )
                            structure_definition.append(
                                const + ' ' +
                                datatype_declaration[1] + ' ' +
                                str(structure_name) + ' = ' +
                                str(counter) + '.F;\n'
                            )
                        else:
                            structure_declaration.append(
                                extern + ' ' +
                                const + ' ' +
                                datatype_declaration[0] + ' ' +
                                str(structure_name) + ';\n'
                            )
                            structure_definition.append(
                                const + ' ' +
                                datatype_declaration[0] + ' ' +
                                str(structure_name) + ' = ' +
                                str(counter) + ';\n'
                            )
                            # default definition
                    else:
                        structure_declaration.append(
                            extern + ' ' +
                            const + ' ' +
                            datatype_declaration[0] + ' ' +
                            str(structure_name) + ';\n'
                        )
                        structure_definition.append(
                            const + ' ' +
                            datatype_declaration[0] + ' ' +
                            str(structure_name) + ' = ' +
                            str(counter) + ';\n'
                        )
                    counter = counter + 1
                    
                # for map definition
                if structure_name in exception_string:
                    pass
                else:
                    for m in range(maru_count_start, df[sheet_name].shape[0]):
                        map_maru_val = df[sheet_name].iat[m, map_maru_col]
                        if map_maru_val == maru:
                            if ten_only == 10:
                                ten_only = 0
                                map_definition.append(
                                    tab + '/*  ' +
                                    '[' + str(counter - 1) + ']' +
                                    '  */\n' + double_tab
                                )
                            if ten_only < 10:
                                map_value = df[sheet_name].iat[m, j]
                                # CAR_TYPE processing
                                if sheet_name == car_type:
                                    if structure_name in k_CAR_SELECT_SPEC:
                                        if str(map_value) in k_CAR_SELECT_SPEC[structure_name][0]:
                                            append_F_pF(map_definition, str(
                                                k_CAR_SELECT_SPEC[structure_name][0].get(map_value)))
                                            if default_maru_flag:
                                                default_str_declaration_func(default_str_declaration, structure_name, 1)
                                                default_maru_definition(
                                                    default_str_definition, structure_name, str(
                                                        k_CAR_SELECT_SPEC[structure_name][0].get(map_value)), 1)
                                        else:
                                            print(
                                                "Not in k_CAR_SELECT_SPEC", structure_name, str(map_value))
                                    elif structure_name in mmtom:
                                        map_value = float(map_value) * mm2m
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                    elif structure_name in cmtom:
                                        map_value = map_value * cm2m
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                    elif structure_name in NmmtoNm:
                                        map_value = map_value * Nmm2Nm
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                    elif structure_name in mstos:
                                        map_value = map_value * ms2s
                                        append_F_pF(map_definition,
                                                    str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)                                   
                                    else:
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                            if structure_name == 'idx_ReqBrkPrsBaseF':
                                                default_REQ_BRKPRS_F = map_value
                                            if structure_name == 'idx_ReqBrkPrsBaseR':
                                                default_REQ_BRKPRS_R = default_REQ_BRKPRS_F #to change please confirm
                                            if structure_name == 'idx_ReqBrkPrsBaseCaliper':
                                                default_REQ_BRKPRS_C = map_value
                                                create_default_REQ_BRKPRS(
                                                    default_REQ_BRKPRS_F,
                                                    default_REQ_BRKPRS_R,
                                                    default_REQ_BRKPRS_C)
                                # MARKET processing
                                elif sheet_name == market:
                                    if structure_name in mile_htom_s:
                                        map_value = map_value * mileOffset * kmh2ms
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(default_str_declaration, structure_name , 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                    elif structure_name in k_MARKET_SELECT_SPEC:
                                        if str(map_value) in k_MARKET_SELECT_SPEC[structure_name][0]:
                                            append_F_pF(
                                                map_definition, k_MARKET_SELECT_SPEC[structure_name][0].get(map_value))
                                            if default_maru_flag:
                                                default_str_declaration_func(
                                                    default_str_declaration, structure_name, 1)
                                                default_maru_definition(
                                                    default_str_definition, structure_name, k_MARKET_SELECT_SPEC[structure_name][0].get(map_value), 1)
                                        else:
                                            print(
                                                "Not in k_MARKET_SELECT_SPEC", structure_name, str(map_value))
                                    else:
                                        map_value = map_value * kmh2ms
                                        append_F_pF(map_definition, str(map_value))
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 1)
                                            default_maru_definition(
                                                default_str_definition, structure_name, str(map_value), 1)
                                elif sheet_name in datatype_boolean:
                                    map_definition.append(
                                        (str(OFF) + ', ') if str(map_value) == 'OFF' else (str(ON) + ', '))
                                    if default_maru_flag:
                                        default_str_declaration_func(
                                            default_str_declaration, structure_name, 2)
                                        default_maru_def_ub(
                                            default_str_definition, structure_name, (str(OFF)) if str(map_value) == 'OFF' else (str(ON)), 2)
                                elif sheet_name in datatype_uint8:
                                    if str(map_value) in i_TYPE_:
                                        map_definition.append(
                                            str(i_TYPE_[map_value][0]) + ', ')
                                        if default_maru_flag:
                                            default_str_declaration_func(
                                                default_str_declaration, structure_name, 0)
                                            default_maru_def_ub(
                                                default_str_definition, structure_name, i_TYPE_[map_value][0], 0)
                                    else:
                                        print("Sheet : ", sheet_name, "values : ", map_value)
                                else:
                                    append_F_pF(map_definition, str(map_value))
                                    if default_maru_flag:
                                        default_str_declaration_func(
                                            default_str_declaration, structure_name, 1)
                                        default_maru_definition(
                                            default_str_definition, structure_name, str(map_value), 1)
                            ten_only = ten_only + 1
                            default_maru_flag = 0
                    # after end of map[][]
                    if sheet_name in exception_sheet:
                        map_definition.append(
                            ('\n' + tab + '}\n' if j == (df[sheet_name].shape[1] - 1)
                                else '\n' + tab + '},\n' + tab + '{\n' + double_tab)
                        )

            if structure_flag:
                    save_variable = df[sheet_name].iat[cont_struct_row + 1, j]
                    if save_variable in exception_string:
                        svn_flag = 0
                    else:
                        svn_flag = 1
                    structure_flag = 0

        if svn_flag:
            # read row data
            maru_count = 0
            col = 0
            dec_count = 0
            dec_list  = []
            for i in range(maru_count_start, df[sheet_name].shape[0]):
                if df[sheet_name].iat[i, col] == maru:
                    maru_count = maru_count + 1
                    dec_list.append(str(dec_count))
                dec_count = dec_count + 1
            if sheet_name not in exception_sheet:
                if sheet_name in datatype_boolean:
                    default_map_declaration_func(
                        map_declaration, save_variable, maru_count, 2)
                elif sheet_name in datatype_uint8:
                    default_map_declaration_func(
                        map_declaration, save_variable, maru_count, 0)
                else:
                    default_map_declaration_func(
                        map_declaration, save_variable, maru_count, 1)
            else:
                map_declaration.append(
                    extern + ' ' +
                    const + ' ' +
                    datatype_declaration[1] + ' ' +
                    str(save_variable) + '[' +
                    str(counter) + '][' +
                    str(maru_count) + '];\n'
                )
        
        # map_id creation
        if sheet_name == map_id:
            for i in mp_ss_status:
                value = [0, 0, 0, 0]
                ind, map_id_counter = 0, 0
                if mp_ss_status[i][0] == 'ON':
                    for j in mp_ss_status[i][1]:
                        value[ind] = j
                        ind = ind + 1
                    for k in range(value[0],df[sheet_name].shape[0] - 1):
                        map_id_values = df[sheet_name].iat[k, map_id_param_col]
                        if map_id_values == NULL:
                            break
                        map_id_declaration.append(
                            extern             + ' ' +
                            const              + ' ' +
                            value[2]           + ' ' +
                            str(map_id_values) +
                            ';\n'
                        )
                        map_id_definition.append(
                            const                                   + ' ' +
                            value[2]                                + ' ' +
                            str(map_id_values)                            +
                            ' = '                   + str(map_id_counter) +
                            ('' if value[2] == uint8 else '.F')       + ';\n' 
                        )
                        map_id_counter = map_id_counter + 1
                    map_id_declaration.append(
                        extern                                       + ' ' +
                        const                                        + ' ' +
                        value[2]                                     + ' ' +
                        str(df[sheet_name].iat[value[1], map_id_cnt_col]) +
                        ';\n'
                    )
                    map_id_definition.append(
                        const                                        + ' ' +
                        value[2]                                     + ' ' +
                        str(df[sheet_name].iat[value[1], map_id_cnt_col]) +
                        ' = ' + str(map_id_counter) +
                        ('' if value[2] == uint8 else '.F')          + ';\n'
                    )
        
        # addition to map definition 
        if svn_flag:
            if sheet_name in datatype_boolean:
                map_definition_add = str(const) + ' ' + datatype_declaration[2] + ' ' + str(save_variable) + \
                '[' + str(maru_count) + '] = ' + '\n{' + str(tab) + '\n' + tab
            elif sheet_name in datatype_uint8:
                map_definition_add = str(const) + ' ' + datatype_declaration[0] + ' ' + str(save_variable) + \
                    '[' + str(maru_count) + '] = ' + '\n{' + str(tab) + '\n' + tab
            else:
                if sheet_name in exception_sheet:
                    map_definition_add = str(const) + ' ' + datatype_declaration[1] + ' ' + str(save_variable) + "[" \
                        + str(counter) + '][' + str(maru_count) + \
                        '] = ' + '\n{\n' + str(tab) + '{\n' + double_tab
                else:
                    map_definition_add = str(const) + ' ' + datatype_declaration[1] + ' ' + str(save_variable) + \
                        '[' + str(maru_count) + \
                        '] = ' + '\n{' + str(tab) + '\n' + tab
                
            map_definition.insert(0, map_definition_add)
            # closing brackets for map [][]
            if sheet_name in exception_sheet:
                map_definition.append("};\n")
            else:
                map_definition.append("\n};\n")

        # h file
        write_1line(h, "/* --- " + sheet_name + " --- */\n" )
        write_list(h, contents_declaration)
        write_list(h, structure_declaration)
        write_list(h, default_str_declaration)
        write_list(h, map_declaration)
        write_list(h, map_id_declaration)



        # c file
        write_1line(c, "/* --- " + sheet_name + " --- */\n")
        write_list(c, contents_definition)
        write_list(c, structure_definition)
        write_list(c, default_str_definition)
        write_list(c, map_definition)
        write_list(c, map_id_definition)
        

        if svn_flag:
            if sheet_name == car_type:
                create_car_type_code(df[sheet_name], dic, maru_count)
            else:
                create_type_code(df, sheet_name, dic, maru_count, dec_count, dec_list, structure_name_holder)

        if sheet_name == car_type:
            write_list(h, Req_Brk_declaration)
            write_list(c, Req_Brk_definition)


        if is_hypen:
            write_list(h, type_code_for_hypen_declaration)
            write_list(c, type_code_for_hypen_definition)

def create_contentsforSub(df, dic):
    row_count, column_count, last_val = 0, 0, 0
    save_row_count, save_column_count = [], []
    k_reserve = ''
    k_declaration        , k_definition        = [], []
    k_default_declaration, k_default_definition = [], []

    # shape[1] means column
    for i in range(0, df.shape[1]):
        k_value = df.iat[0,i]
        if k_value in exception_string:
            continue
        if 'k_' in k_value:
            k_reserve = k_value
            # shape[0] means row
            for j in range(2, df.shape[0]):
                row_value = df.iat[j,i]
                if row_value == endofrow:
                    break
                row_count = row_count + 1
            save_row_count.append(str(row_count))
            row_count = 0
        if k_reserve == k_value:
            column_count = 0
        else:
            if last_val > column_count:
                save_column_count.append(last_val)
            column_count = column_count + 1
            last_val = column_count
            
            k_declaration.append(
                extern + ' ' +
                const  + ' ' +
                datatype_declaration[1] + ' ' +
                str(k_reserve) + '_' +
                str(k_value)
            )
    save_column_count.append(column_count)

    
    print(save_column_count, save_row_count)
    for i in save_column_count:
        for j in range(int(i)):
            pass

def create_type_code(df, sheet_name, dic, maru_count, dec_count, dec_list, structure_name_holder):
    h = Car_Para_FileName_H[0]
    c = Car_Para_FileName_C[0]

    ten_only = 0
    type_code_declaration, type_code_definition = [], []

    structure_name = structure_name_holder[:-11]
    structure_name = structure_name + "CAR_CODE_INDEX"
    type_code_declaration.append(
        extern + ' ' +
        const + ' ' +
        datatype_declaration[0] + ' ' +
        structure_name + '[' +
        str(maru_count) + '];\n'
    )

    type_code_definition.append(
        const + ' ' +
        datatype_declaration[0] + ' ' +
        structure_name + '[' +
        str(maru_count) + '] = \n{\n' +
        tab
    )

    for i in dec_list:
        if ten_only == 10:
            type_code_definition.append('\n' + tab )
        if ten_only < 10:
            type_code_definition.append(
                str(i) + ', '
            )
    type_code_definition.append('\n};')
    

    write_list(h, type_code_declaration)
    write_list(c, type_code_definition)
