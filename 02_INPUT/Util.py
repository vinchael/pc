from Common_Car import *
from Common_VC import *
from StringManipulation import *
import argparse
import os
import pandas as pd
from pandas import ExcelWriter
import numpy as np
import re

def createExcelFile(df, filename):
    writer = ExcelWriter(filename)
    df.to_excel(writer, 'Sheet1', index=False)
    writer.save()
    writer.close()
    
def readExcelFile(filename):
    xls = pd.ExcelFile(filename)
    return(xls)

def readArgParse():
    # Accepting input
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', dest = 'dest', help = 'Variant: -v [filename')
    args = parser.parse_args()

    if args.dest:
        print("Argument accepted")
        return args.dest
    else:
        return 0

def getCurrentDirectory(filename):
    base_path = os.path.dirname(os.path.realpath(__file__))
    currentfile = os.path.join(base_path, filename)
    
    print(currentfile)

def createIfNoFileName(file_path):
    if os.path.exists(file_path) == False:
        created_file = open(file_path, 'w')
        created_file.close()

def write_1line(file_name, string):
    with open(file_name, 'a') as f:
        f.write(string)
    f.close

def write_list(file_name, string_list):
    with open(file_name, 'a') as f:
        for l in string_list:
            f.write(l)
    f.close

def create_header(file_name, ex_name):  # 'MDL_Car_Param.h'
    header_name = file_name[:-2].upper()
    string_list = [ifndef + ' ' + header_name + ex_name + '\n',
                   define + ' ' + header_name + ex_name + '\n',
                   '\n\n']
    # Create if file does not exists
    
    createIfNoFileName(file_name)
    print("Creating Header file for : ", file_name)
    write_list(file_name, string_list)

def create_footer(file_name, string):
    write_1line(file_name, string)

def append_F_pF(appendto, map_value):
    if '.' in str(map_value):
        return appendto.append(str(map_value) + 'F, ')
    else:
        return appendto.append(str(map_value) + '.F, ')

def default_maru_definition(appendto, structure_name, computed_value):
    appendto.append(
        const + ' ' +
        datatype_declaration[1] + ' ' +
        default_ +
        str(structure_name[3:]) + ' = ' +
        str(computed_value) +
        ('F;\n' if '.' in str(computed_value) else '.F;\n')
    )

def default_maru_def_ub(appendto, structure_name, computed_value):
    appendto.append(
        const + ' ' +
        datatype_declaration[0] + ' ' +
        default_ +
        str(structure_name[3:]) + ' = ' +
        str(computed_value) +
        ';\n'
    )

k_CAR_SELECT_SPEC = {'idx_VariantPadTypeF'      : [iPAD__CAR_PARA_] ,
                     'idx_VariantPadTypeR'      : [iPAD__CAR_PARA_] ,
                     'idx_add_mtr_correct'      : [iMTR_CORRECT_TBL],
                     'idx_SpecR79'              : [state_]          ,
                     'idx_did_select_tbl'       : [iDID_SELECT_TBL_],
                     'idx_spec_manual_mode'     : [state_]          ,
                     'idx_variant_pt'           : [iEG_CAR_PARA_]   ,
                     'idx_variant_tm'           : [iTM_CAR_PARA_]   ,
                     'idx_variant_for_ldp'      : [iLDP_CAR_PARA]   ,
                     'idx_SpecALKSwMemory'      : [iSPEC_ALK_MEM_]  ,
                     'idx_lks_default'          : [state_],
                     'idx_variant_for_lks'      : [iLKS_CAR_PARA_]  ,
                     'idx_spec_cc_type'         : [iSPEC_]          ,
                     #'idx_spec_cc_sw_type'      : [iSPEC_]          , TODO
                     'idx_vdc_offmode'          : [iSPEC_VDCSW_]
                    }

k_MARKET_SELECT_SPEC = {'idx_dis_kh_spd_max'             : [spd_max]         ,
                        'idx_BRK_LAMP_type'              : [iBRK_LAMP]       ,
                        'idx_TGT_A_type'                 : [iTGT_A]          ,
                        'idx_FMWMaxGType'                : [iFMW]            ,
                        'idx_PC_A_type'                  : [iPC_A]           ,
                        'idx_RoadTypeCustEquip'          : [state_]          ,
                        'idx_RoadTypeDefault'            : [state_]          ,
                        'idx_ALK_limit_kh'               : [state_]          ,
                        'idx_oneside_lane_mode'          : [state_]          ,
                        'idx_MARKETInfo'                 : [iMARKET]         ,
                        'idx_spec_signal_type'           : [iSPEC_SIGNAL_]   ,
                        'idx_spec_fmw_r_type'            : [iSPEC_FMW_R_]    ,
                        'idx_spec_fmw_r_spd_lim_default' : [iSPEC_FMW_R_SPD_]
                        }
