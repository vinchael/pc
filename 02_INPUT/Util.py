from Common_Car import *
from Common_VC import *
from StringManipulation import *
import argparse
import os
import pandas as pd
from pandas import ExcelWriter
import numpy as np

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

k_CAR_SELECT_SPEC = {
                    '0'                        : [''],
                    '1'                        : [''],
                    '2'                        : [''],
                    '3'                        : [''],
                    '4'                        : [''],
                    '5'                        : [''],
                    'idx_VariantPadTypeF'      : [iPAD_F_CAR_PARA_],
                    'idx_VariantPadTypeR'      : [iPAD_F_CAR_PARA_],
                    '8'                        : [''],
                    '9'                        : [''],
                    '10'                       : [''],
                    '11'                       : [''],
                    'idx_moving_tire_r'        : [''],
                    '13'                       : [''],
                    'idx_vehicle_weight'       : [''],
                    'idx_rs_0degree_c'         : [''],
                    'idx_rs_1degree_c'         : [''],
                    'idx_rs_2degree_c'         : [''],
                    '18'                       : [''], 
                    'idx_add_mtr_correct'      : [iMTR_CORRECT_TBL],
                    '20'                       : [''],
                    'idx_SpecR79'              : [state_]          ,
                    'idx_did_select_tbl'       : [iDID_SELECT_TBL_],
                    '23'                       : [''],
                    'idx_vehicle_width'        : [''],
                    'idx_spec_manual_mode'     : [state_]          ,
                    '26'                       : [''],
                    '27'                       : [''],
                    'idx_eng_rev_min_lim'      : [''],
                    'idx_gear_ratio_max'       : [''],
                    'idx_gear_ratio_min'       : [''],
                    'idx_final_gear'           : [''],
                    'idx_distance_rw_2_vdc'    : [''],
                    'idx_variant_pt'           : [iEG_CAR_PARA_]   ,
                    'idx_variant_tm'           : [iTM_CAR_PARA_]   ,
                    'idx_variant_for_ldp'      : [iLDP_CAR_PARA]   ,
                    'idx_EpsCtrlTrqALKLim'     : [''],
                    'idx_SpecALKSwMemory'      : [iSPEC_ALK_MEM_]  ,
                    'idx_lks_default'          : [state_],
                    'idx_variant_for_lks'      : [iLKS_CAR_PARA_]  ,
                    'idx_CarPara_CAMtoFtAxle'  : [''],
                    'idx_CarPara_CAMtoRrAxle'  : [''],
                    'idx_spec_cc_type'         : [iSPEC_]          ,
                    'idx_spec_cc_sw_type'      : [iSPEC_]          ,
                    'idx_nagaoshi_thr'         : [''],
                    'idx_vdc_offmode'          : [iSPEC_VDCSW_]    ,
                    'idx_ReqBrkPrsBaseF'       : [''],
                    'idx_ReqBrkPrsBaseR'       : [''],
                    '48'                       : [''],
                    '49'                       : [''],
                    '50'                       : [''],
                    '51'                       : [''],
                    'idx_ReqBrkPrsBaseCaliper' : [''],
                    'idx_VelCoefficientF'      : [''],
                    'idx_VelCoefficientR'      : [''],
                    'idx_aown_CorrectRate'     : [''],
                    'idx_vso_error'            : ['']
                    }

k_MARKET_SELECT_SPEC = {'idx_acc_set_spd_max'            : [''],
                        'idx_acc_set_spd_min'            : [''],
                        'idx_acc_set_spd_max_mile'       : [''],
                        'idx_acc_set_spd_min_mile'       : [''],
                        'idx_acc_cont_spd_max'           : [''],
                        'idx_acc_cont_spd_min'           : [''],
                        'idx_cc_set_spd_max'             : [''],
                        'idx_cc_set_spd_min'             : [''],
                        'idx_cc_set_spd_max_mile'        : [''],
                        'idx_cc_set_spd_min_mile'        : [''],
                        'idx_cc_cont_spd_max'            : [''],
                        'idx_cc_cont_spd_min'            : [''],
                        'idx_dis_kh_spd_max'             : [''],
                        'idx_pc_spd_max'                 : [''],
                        'idx_pcba_max_on_by_vown'        : [''],
                        'idx_pcba_max_off_by_vown'       : [''],
                        'idx_pcba_max_on_by_vrel_pc'     : [''],
                        'idx_pcba_max_off_by_vrel_pc'    : [''],
                        'idx_fmw_spd_max'                : [''],
                        'idx_BRK_LAMP_type'              : [iBRK_LAMP]   ,
                        'idx_TGT_A_type'                 : [iTGT_A]      ,
                        'idx_FMWMaxGType'                : [iFMW]        ,
                        'idx_PC_A_type'                  : [iPC_A]       ,
                        'idx_ldp_v_max_off'              : [''],
                        'idx_ldp_v_min_off'              : [''],
                        'idx_ldp_v_max_on'               : [''],
                        'idx_ldp_v_min_on'               : [''],
                        'idx_roadedge_v_max_off'         : [''],
                        'idx_roadedge_v_min_off'         : [''],
                        'idx_roadedge_v_max_on'          : [''],
                        'idx_roadedge_v_min_on'          : [''],
                        'idx_b2_v_max_off'               : [''],
                        'idx_b2_v_min_off'               : [''],
                        'idx_b2_v_max_on'                : [''],
                        'idx_b2_v_min_on'                : [''],
                        'idx_alc_v_max_off'              : [''],
                        'idx_alc_v_min_off'              : [''],
                        'idx_alc_v_max_on'               : [''],
                        'idx_alc_v_min_on'               : [''],
                        'idx_RoadTypeCustEquip'          : [state_]       ,
                        'idx_RoadTypeDefault'            : [state_]       ,
                        'idx_ALK_limit_kh'               : [state_]       ,
                        'idx_oneside_lane_mode'          : [state_]       ,
                        'idx_MARKETInfo'                 : [iMARKET],
                        'idx_spec_signal_type'           : [iSPEC_SIGNAL_],
                        'idx_spec_fmw_r_type'            : [iSPEC_FMW_R_] ,
                        'idx_spec_fmw_r_spd_lim_default' : [iSPEC_FMW_R_SPD_],
                        'idx_ALKCTJAVownMaxOff'          : [''],
                        'idx_ALKCTJAVownMinOff'          : [''],
                        'idx_ALKCTJAVownMaxOn'           : [''],
                        'idx_ALKCTJAVownMinOn'           : ['']
                        }
