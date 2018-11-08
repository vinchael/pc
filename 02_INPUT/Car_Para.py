from Util import *


def create_contents(df, dic):
    h = Car_Para_FileName_H[0]
    c = Car_Para_FileName_C[0]
    # sheet value
    key_count = len(dic.keys())

    for i in range(4, key_count):
        # sheet_value is equal to Sheet name
        sheet_value = dic[i]
        save_variable, market_intfloat_flag = '', 0
        contents_flag, structure_flag, svn_flag = 0, 0, 0
        counter, ten_only = 0, 0

        contents_declaration ,  contents_definition = [], []
        structure_declaration, structure_definition = [], []
        map_declaration      , map_definition       = [], []
        map_id_declaration   , map_id_definition    = [], []
        
        # read column data
        print('Sheet name:', sheet_value, "Total column count:", df[sheet_value].shape[1] - 1)
        for j in range(0,df[sheet_value].shape[1]):
            # if sheet name is equal to MAP_ID skip
            if sheet_value == map_id:
                continue
            # if 'Structure' text was found
            if (df[sheet_value].iat[cont_struct_row, j] == structure) and \
                (str(df[sheet_value].iat[cont_struct_row + 1, j]) not in exception_string):
                contents_flag  = 0
                structure_flag = 1
            # if it has contents 
            if (df[sheet_value].iat[cont_struct_row, j] == contents) and (sheet_value in has_contents):
                contents_flag = 1

            if contents_flag:
                contents_value = df[sheet_value].iat[cont_struct_row + 1, j]
                if contents_value in exception_string:
                    pass
                else:
                    if sheet_value == car_type:
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
                        
                        bit_var_name     = df[sheet_value].iat[cont_struct_row + 1, j]
                        bit_str_position = df[sheet_value].iat[cont_struct_row + 2, j]

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
                structure_value = df[sheet_value].iat[cont_struct_row + 1, j]
                if (structure_value == market_float32_start) and (sheet_value == market):
                    market_intfloat_flag = 1
                if (structure_value == market_uint8_start)   and (sheet_value == market):
                    market_intfloat_flag = 0

                if structure_value in exception_string:
                    pass
                else:
                    if sheet_value == market:
                        if market_intfloat_flag:
                            structure_declaration.append(
                                extern + ' ' +
                                const + ' ' +
                                datatype_declaration[1] + ' ' +
                                str(structure_value) + ';\n'
                            )
                            structure_definition.append(
                                const + ' ' +
                                datatype_declaration[1] + ' ' +
                                str(structure_value) + ' = ' +
                                str(counter) + '.F;\n'
                            )
                        else:
                            structure_declaration.append(
                                extern + ' ' +
                                const + ' ' +
                                datatype_declaration[0] + ' ' +
                                str(structure_value) + ';\n'
                            )
                            structure_definition.append(
                                const + ' ' +
                                datatype_declaration[0] + ' ' +
                                str(structure_value) + ' = ' +
                                str(counter) + ';\n'
                            )
                    else:
                        structure_declaration.append(
                            extern + ' ' +
                            const + ' ' +
                            datatype_declaration[0] + ' ' +
                            str(structure_value) + ';\n'
                        )
                        structure_definition.append(
                            const + ' ' +
                            datatype_declaration[0] + ' ' +
                            str(structure_value) + ' = ' +
                            str(counter) + ';\n'
                        )
                    counter = counter + 1
                    
                # for map definition
                if structure_value in exception_string:
                    pass
                else:
                    for m in range(maru_count_start, df[sheet_value].shape[0]):
                        map_maru_val = df[sheet_value].iat[m, map_maru_col]
                        if map_maru_val == maru:
                            if ten_only == 10:
                                ten_only = 0
                                map_definition.append(
                                    tab + '/*  ' +
                                    '[' + str(counter - 1) + ']' +
                                    '  */\n' + double_tab
                                )
                            if ten_only < 10:
                                map_value = df[sheet_value].iat[m, j]
                                # CAR_TYPE processing
                                if (structure_value in k_CAR_SELECT_SPEC) and (sheet_value == car_type):
                                    if str(map_value) in k_CAR_SELECT_SPEC[structure_value][0]:
                                        map_definition.append(
                                            str(k_CAR_SELECT_SPEC[structure_value][0].get(
                                                map_value)) + '.F, '
                                        )
                                    else:
                                        if structure_value in dot_atbeginning:
                                            map_value = str(
                                                map_value).replace('.', '')
                                            map_definition.append('0.' +
                                                                  str(map_value) +
                                                                  'F, '
                                                                  )
                                        elif structure_value in first_number:
                                            no_dot = str(
                                                map_value).replace('.', '')
                                            map_value = no_dot[:1] + \
                                                '.' + no_dot[1:]
                                            map_definition.append(str(map_value) +
                                                                  'F, '
                                                                  )
                                        elif structure_value in remo_zero:
                                            no_dot = str(
                                                map_value).replace('.', '')
                                            map_value = no_dot[2:]
                                            map_value = no_dot[:1] + \
                                                '.' + no_dot[1:]
                                            map_definition.append(str(map_value) +
                                                                  'F, '
                                                                  )
                                        else:
                                            if '.' in str(map_value):
                                                map_definition.append(
                                                    str(map_value) + 'F, '
                                                )
                                            else:
                                                map_definition.append(
                                                    str(map_value) + '.F, '
                                                )
                                elif (structure_value not in k_CAR_SELECT_SPEC) and (sheet_value == car_type):
                                    print("Sheet name:", sheet_value,"idx_xxx:", structure_value)
                                # MARKET processing
                                elif (structure_value in k_MARKET_SELECT_SPEC) and (sheet_value == market):
                                    if str(map_value) in k_MARKET_SELECT_SPEC[structure_value][0]:
                                        map_definition.append(
                                            str(k_MARKET_SELECT_SPEC[structure_value][0].get(
                                                map_value)) + '.F, ')
                                    else:
                                        if '.' in str(map_value):
                                            map_definition.append(
                                                str(map_value) + 'F, '
                                            )
                                        else:
                                            map_definition.append(
                                                str(map_value) + '.F, '
                                            )
                                elif (structure_value not in k_MARKET_SELECT_SPEC) and (sheet_value == market):
                                    print("Sheet name:", sheet_value, "idx_xxx :", structure_value)
                                elif sheet_value in datatype_boolean:
                                    map_definition.append(
                                        (str(OFF) + ', ') if str(map_value) == 'OFF' else (str(ON) + ', '))
                                elif (sheet_value in datatype_uint8) and (sheet_value == tcm):
                                    if str(map_value) in i_TYPE_:
                                        map_definition.append(
                                            str(i_TYPE_[map_value][0]) + ', ')
                                    else:
                                        print("Sheet : ", sheet_value, "values : ", map_value)
                                # addition of elif for remaining sheet

                                # else defaul value
                                else:
                                    if '.' in str(map_value):
                                        map_definition.append(
                                            str(map_value) + 'F, '
                                        )
                                    else:
                                        map_definition.append(
                                            str(map_value) + '.F, '
                                        )
                            ten_only = ten_only + 1

                    # after end of map[][]
                    map_definition.append(
                        ('\n' + tab + '}\n' if j == (df[sheet_value].shape[1] - 1)
                            else '\n' + tab + '},\n' + tab + '{\n' + double_tab)
                    )

            if structure_flag:
                    save_variable = df[sheet_value].iat[cont_struct_row + 1, j]
                    if save_variable in exception_string:
                        svn_flag = 0
                    else:
                        svn_flag = 1
                    structure_flag = 0

        # read row data
        maru_count = 0
        col = 0
        for i in range(maru_count_start, df[sheet_value].shape[0]):
            if df[sheet_value].iat[i, col] == maru:
                maru_count = maru_count + 1
        if svn_flag:
            if sheet_value not in exception_sheet:
                map_declaration.append(
                    extern + ' ' +
                    const + ' ' +
                    datatype_declaration[1] + ' ' +
                    str(save_variable) + '[' +
                    str(maru_count) + '];\n'
                )
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
        if sheet_value == map_id:
            for i in mp_ss_status:
                value = [0, 0, 0, 0]
                ind, map_id_counter = 0, 0
                if mp_ss_status[i][0] == 'ON':
                    for j in mp_ss_status[i][1]:
                        value[ind] = j
                        ind = ind + 1
                    for k in range(value[0],df[sheet_value].shape[0] - 1):
                        map_id_values = df[sheet_value].iat[k, map_id_param_col]
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
                        str(df[sheet_value].iat[value[1], map_id_cnt_col]) +
                        ';\n'
                    )
                    map_id_definition.append(
                        const                                        + ' ' +
                        value[2]                                     + ' ' +
                        str(df[sheet_value].iat[value[1], map_id_cnt_col]) +
                        ' = ' + str(map_id_counter) +
                        ('' if value[2] == uint8 else '.F')          + ';\n'
                    )
        
        # addition to map definition 
        if svn_flag:
            if sheet_value in datatype_boolean:
                map_definition_add = str(const) + ' ' + datatype_declaration[2] + ' ' + str(save_variable) + "[" \
                + str(counter) + '][' + str(maru_count) + \
                      '] = ' + '\n{\n' + str(tab) + '{\n' + double_tab
            elif sheet_value in datatype_uint8:
                map_definition_add = str(const) + ' ' + datatype_declaration[0] + ' ' + str(save_variable) + "[" \
                + str(counter) + '][' + str(maru_count) + \
                      '] = ' + '\n{\n' + str(tab) + '{\n' + double_tab
            else:
                map_definition_add = str(const) + ' ' + datatype_declaration[1] + ' ' + str(save_variable) + "[" \
                    + str(counter) + '][' + str(maru_count) + \
                    '] = ' + '\n{\n' + str(tab) + '{\n' + double_tab
                
            map_definition.insert(0, map_definition_add)
            # closing brackets for map [][]
            map_definition.append("}\n")

        # h file
        write_1line(h, "/* --- " + sheet_value + " --- */\n" )
        write_list(h, contents_declaration)
        write_list(h, structure_declaration)
        write_list(h, map_declaration)
        write_list(h, map_id_declaration)


        # c file
        write_1line(c, "/* --- " + sheet_value + " --- */\n")
        write_list(c, contents_definition)
        write_list(c, structure_definition)
        write_list(c, map_id_definition)
        write_list(c, map_definition)
