# C file declaration 
ifndef     = '#ifndef'
define     = '#define'
endif      = '#endif'
extern     = 'extern'
const      = 'const'
uint8      = 'uint8'
float32    = 'float32'
space      = ' '
tab        = '    '
NULL       = 'NULL'
default_   = 'default'
double_tab = tab + tab
triple_tab = double_tab + tab
four_tab   = triple_tab + tab
five_tab   = four_tab + tab
boolean    = 'boolean'

OFF = 0
ON = 1

# for Sub Sheet name only
endofrow = 'ここから下は書込み禁止'

# header/footer file 
Car_Para_FileName_H = ['MDL_Car_Param.h']
Car_Para_FileName_C = ['MDL_Car_Param.c']

# for Sub sheet only

# include header file on C file
MDL_Car_Param_h = '#include "MDL_Car_Param.h"\n\n'
MDL_VC_Param_h  = '#include "MDL_VC_Param.h"\n\n'
Rte_Type        = '#include "Rte_Type.h"\n\n'

# default array for declaration
datatype_declaration = [uint8, float32, boolean]

car_type  = 'CAR_TYPE'
market    = 'MARKET'
map_id    = 'MAP_ID'
tcm       = 'TCM'
sub_sheet = 'Sub'

exception_sheet = {'CAR_TYPE', 'MARKET', 'SUB'}
exception_string = {'-', '', NULL, 'xxx'}

# Sheet declaration
is_eng_ON = 0

if is_eng_ON:
    contents  = 'Contents'  
    structure = 'Structure'
else:
    contents  = '内容'
    structure = '構造体'

has_contents = {"CAR_TYPE"}

sheets_not_include = {"MAP_ID", "Sub"}
maru = '○'
map_maru_col = 0
cont_struct_row = 0
maru_count_start = 4

# type code values
type_code_col = 4
ten_bytes = 20
k_CAR_CAR_CODE_INDEX = 'k_CAR_CAR_CODE_INDEX'

# market idx_xxx datatype where to start/end 
market_float32_start = 'idx_roadedge_v_max_off'
market_uint8_start = 'idx_RoadTypeCustEquip'

# map_ID for MAP_ID sheet
map_id_cnt_col   = 5
map_id_param_col = 2


# MAP_ID declaration definition
""" excel file starts with row 3 index is 0 """
""" start,  maximum count, datatype """
iPAD_status  = [22 ,  16, uint8  ]  # 'ON' , iPAD
iEG_status   = [39 ,  33, float32]  # 'ON' , iEG
iTM_status   = [117, 111, uint8  ]  # 'ON' , iTM
iLKS_status  = [151, 145, uint8  ]  # 'ON' , iLKS
iLDP_status  = [171, 165, uint8  ]  # 'ON' , iLDP
AES_status   = [249, 243, uint8  ]  # 'ON' , AES


mp_ss_status = {0: ['ON', iPAD_status],
                1: ['ON', iEG_status ],
                2: ['ON', iTM_status ],
                3: ['ON', iLKS_status],
                4: ['ON', iLDP_status],
                5: ['ON', AES_status ],
                }

# CAR_TYPE sheet

state_ = {'OFF' : '0',
          'ON'  : '1'
         }

""" version 2-10 """
iPAD__CAR_PARA_ = {
    'FA300': '0',
    'FA430': '1',
    'FA431': '2',
    'FH400': '3',
    'GA5504H': '4',
    'J636': '5',
    'FN360': '6',
    'FN410': '7',
    'NUM': '8'
}
iEG_CAR_PARA_ = {
    '936_US': '0',
    '936_EU': '1',
    '936_DOM': '2',
    '925_US': '3',
    '925_EU': '4',
    '925_DOM': '5',
    '925_OTHER': '6',
    '920DIB_US': '7',
    '920DIB_EU': '8',
    '920DIB_DOM': '9',
    '920_US': '10',
    '920_US_XUV': '11',
    '920_DOM': '12',
    '920_DOM_XUV': '13',
    '920_EU': '14',
    '920_EU_SAI': '15',
    '920_EU_XUV': '16',
    '916DIB_EU': '17',
    '916DIB_DOM': '18',
    '916_DOM': '19',
    '916_DOM_XUV': '20',
    '916_EU_XUV': '21',
    '520B_EU': '22',
    '924DIB_US': '23',
    '920_CHINA': '24',
    '920_CHINA_B': '25',
    '924DIB_DOM': '26',
    '924DIB_EU': '27',
    '918DST_DOM': '28',
    '918DST_DOM_B': '29',
    'NUM': '30'
}
iTM_CAR_PARA_ = {
    'CVT_C20': '0',
    'CVT_C20_B': '1',
    'CVT_C20_C': '2',
    'CVT_C20_D': '3',
    'CVT_C20_E': '4',
    'CVT_C20_F': '5',
    'CVT_C20_916': '6',
    'CVT_C20_916_B': '7',
    'CVT_C20_916_C': '8',
    'CVT_C23': '9',
    'CVT_C23_B': '10',
    'CVT_C26': '11',
    'CVT_C26_B': '12',
    'CVT_C26_C': '13',
    'HTCVT_C31': '14',
    'HTCVT_C31_B': '15',
    'HTCVT_C31_C': '16',
    'HTCVT_C37': '17',
    'HTCVT_C37_B': '18',
    'HTCVT_C37_C': '19',
    'HTCVT_C52': '20',
    'HTCVT_C52_B': '21',
    '5AT': '22',
    'CVT_C20_CHINA': '23',
    'CVT_C20_CHINA_B': '24',
    'NUM': '25'
}
iLKS_CAR_PARA_ = {
    'OTHERS': '0',
    'V_S': '1',
    'V_W': '2',
    'V_Swide': '3',
    'V_S_US': '4',
    'V_Swide_US': '5',
    'G_S': '6',
    'G_W': '7',
    'G_X': '8',
    'B_S': '9',
    'B_O': '10',
    'NUM': '11'
}
iLDP_CAR_PARA_ = {
    'OTHERS': '0',
    'STD': '1',
    'OBK': '2',
    'OBK_WILD': '3',
    'NUM': '4'
}
iAES_CAR_PARA_ = {
    'OTHERS': '0',
    'STD': '1',
    'STD_iB': '2',
    'OBK': '3',
    'OBK_iB': '4',
    'OBK_WILD': '5',
    'OBK_WILD_iB': '6',
    'NUM': '7'
}



# default is float for MAPPING Definition
datatype_boolean = {'AUTO_BRK', 'ISS', 'HEV'}
datatype_uint8 = {'TCM'}

# TCM parameter 
i_TYPE_ = {'5AT' : ['1'],
           'CVT' : ['0']
          }

ms2s       = 1 / 1000  # 1ms -> 1s
kmh2ms     = 1 / 3.6   # km/h->m/s
spdLsbMax  = 1 / 256   # LSB: 2 ^ 8->1
mileOffset = 1.609     # mile->meter
mm2m       = 1 / 1000  # mm -> m
cm2m       = 1 / 100   # cm -> m
Nmm2Nm     = 1000      # N/mm→N/m
_2BYTES    = 65535 * 1 / 256

spd_max = {'MAX' : _2BYTES,
           'MIN' : '0'
           }

# check always if there is changes in here
# MARKET
mile_htom_s = {'idx_acc_set_spd_max_mile',
               'idx_acc_set_spd_min_mile',
               'idx_cc_set_spd_max_mile' ,
               'idx_cc_set_spd_min_mile'
               }
mmtom = {'idx_moving_tire_r',
         'idx_vehicle_width',
         'idx_CarPara_CAMtoFtAxle',
         'idx_CarPara_CAMtoRrAxle',
        }
cmtom = {'idx_distance_rw_2_vdc'}
NmmtoNm = {'idx_EpsCtrlTrqALKLim'}
mstos = {'idx_nagaoshi_thr'}


# changes cheking added idx_xxx in CAR_TYPE
CAR_TYPE_checking = {'idx_VariantPadTypeF',
                     'idx_VariantPadTypeR',
                     'idx_moving_tire_r',
                     'idx_vehicle_weight',
                     'idx_rs_0degree_c',
                     'idx_rs_1degree_c',
                     'idx_rs_2degree_c',
                     'idx_add_mtr_correct',
                     'idx_SpecR79',
                     'idx_did_select_tbl',
                     'idx_vehicle_width',
                     'idx_spec_manual_mode',
                     'idx_eng_rev_min_lim',
                     'idx_gear_ratio_max',
                     'idx_gear_ratio_min',
                     'idx_final_gear',
                     'idx_distance_rw_2_vdc',
                     'idx_variant_pt',
                     'idx_variant_tm',
                     'idx_variant_for_ldp',
                     'idx_EpsCtrlTrqALKLim',
                     'idx_SpecALKSwMemory',
                     'idx_lks_default',
                     'idx_variant_for_lks',
                     'idx_CarPara_CAMtoFtAxle',
                     'idx_CarPara_CAMtoRrAxle',
                     'idx_spec_cc_type',
                     'idx_spec_cc_sw_type',
                     'idx_nagaoshi_thr',
                     'idx_vdc_offmode',
                    #  'idx_AES',
                     'idx_ReqBrkPrsBaseF',
                     'idx_ReqBrkPrsBaseR',
                     'idx_ReqBrkPrsBaseCaliper',
                     'idx_VelCoefficientF',
                     'idx_VelCoefficientR',
                     'idx_aown_CorrectRate',
                     'idx_vso_error'
                    }

CAR_TYPE_contents_checking = {'ba_CarSystem',
                              'ba_YearMode',
                              'ba_Market',
                              'ba_MarketDetail',
                              'ba_Grade',
                              'ba_AtmPressSensor',
                              'ba_PU',
                              'ba_Body',
                              'ba_OEM',
                              'ba_AWD',
                              'ba_RAB',
                              'ba_Brake',
                              'ba_Tire',
                              'ba_TCM',
                              'ba_EBrkBooster',
                              'ba_ISS',
                              'ba_HEV',
                              'ba_EPB',
                              'ba_AVH',
                              'ba_MSA',
                              'ba_TSR',
                              'ba_RadarF',
                              'ba_RadarR',
                              'ba_ELK',
                              'ba_StrVibration',
                              'ba_RoadEdge',
                              'ba_MapLocator',
                              'ba_SonarF',
                              'ba_DMS',
                              'ba_ALC',
                              'ba_NotHoldingALK',
                              'ba_AutoDepart',
                              'ba_iACC',
                              'ba_SlowBfCrv',
                              'ba_ALKB',
                              'ba_ALKC',
                              'ba_StrTouchSensor',
                              'ba_MonoCam',
                              'ba_LightCtrl',
                              'ba_HandlePos',
                            }
