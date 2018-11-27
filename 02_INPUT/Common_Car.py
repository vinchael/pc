# C file declaration 
ifndef     = '#ifndef'
define     = '#define'
endif      = '#endif'
extern     = 'extern'
const      = 'const'
uint8      = 'uint8'
float32    = 'float32'
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
contents = 'Contents'  # 内容   Contents
structure = 'Structure'    # 構造体 Structure

has_contents = {"CAR_TYPE"}

sheets_not_include = {"MAP_ID", "Sub"}
maru = '○'
map_maru_col = 0
cont_struct_row = 0
maru_count_start = 4

# type code values
type_code_col = 3
ten_bytes = 13
k_CAR_CAR_CODE_INDEX = 'k_CAR_CAR_CODE_INDEX'

# market idx_xxx datatype where to start/end 
market_float32_start = 'idx_roadedge_v_max_off'
market_uint8_start = 'idx_RoadTypeCustEquip'

# map_ID for MAP_ID sheet
map_id_cnt_col   = 5
map_id_param_col = 2


# MAP_ID declaration definition
# ieg, itm, ilks, ildp               start,  total index, datatype
iPAD_CAR_PARA_NUM                   = [22 ,  16, uint8  ]  # 'ON' , iPAD
mp_ss_egtrq_map                     = [37 ,  31, float32]  # 'OFF', iEG
mp_ss_a_fmw_direct_r                = [74 ,  68, float32]  # 'ON' , iEG
mp_ss_rs_inertia_mass_map           = [111, 105, uint8  ]  # 'ON' , iTM
mp_ss_tja_str_deg_calc_map          = [142, 136, uint8  ]  # 'ON' , iLKS
mp_ss_str_deg_calc_map              = [166, 160, float32]  # 'OFF', iLDP
mp_ss_ff_map_p                      = [217, 211, float32]  # 'OFF', iLDP
mp_ss_LDPDrvovrInStart              = [268, 262, float32]  # 'OFF', iLDP
mp_ss_p_actual_max_trq_by_egrev_tbl = [319, 313, float32]  # 'OFF', IEG
mp_ss_TrqRateCtrlStartInELK         = [356, 350, uint8  ]  # 'ON' , iLDP

mp_ss_status = {0: ['ON' , iPAD_CAR_PARA_NUM                    ],
                1: ['OFF', mp_ss_egtrq_map                      ],
                2: ['ON' , mp_ss_a_fmw_direct_r                 ],
                3: ['ON' , mp_ss_rs_inertia_mass_map            ],
                4: ['ON' , mp_ss_tja_str_deg_calc_map           ],
                5: ['OFF', mp_ss_str_deg_calc_map               ],
                6: ['OFF', mp_ss_ff_map_p                       ],
                7: ['OFF', mp_ss_LDPDrvovrInStart               ],
                8: ['OFF', mp_ss_p_actual_max_trq_by_egrev_tbl  ],
                9: ['ON' , mp_ss_TrqRateCtrlStartInELK          ]
                }

# CAR_TYPE sheet
iPAD__CAR_PARA_ = {
    'GA9005': '0',
    'FN410' : '1',
    'GA5504': '2',
    'FH400' : '3',
    'FN360' : '4',
    'FA300' : '5',
    'NUM'   : '6'
}

state_ = {'OFF' : '0',
          'ON'  : '1'
         }

iEG_CAR_PARA_ = {'936_US'      : '0' ,
                 '936_EU'      : '1' ,
                 '936_DOM'     : '2' ,
                 '925_US'      : '3' ,
                 '925_EU'      : '4' ,
                 '925_DOM'     : '5' ,
                 '920DIB_US'   : '6' ,
                 '920DIB_EU'   : '7' ,
                 '920DIB_DOM'  : '8' ,
                 '920_US'      : '9' ,
                 '920_US_XUV'  : '10',
                 '920_DOM'     : '11',
                 '920_DOM_XUV' : '12',
                 '920_EU'      : '13',
                 '920_EU_SAI'  : '14',
                 '920_EU_XUV'  : '15',
                 '916DIB_EU'   : '16',
                 '916DIB_DOM'  : '17',
                 '916_DOM'     : '18',
                 '916_DOM_XUV' : '19',
                 '916_EU_XUV'  : '20',
                 '520B_EU'     : '21',
                 '924DIB_US'   : '22',
                 '920_CHINA'   : '23',
                 '920_CHINA_B' : '24',
                 '924DIB_DOM'  : '25',
                 '924DIB_EU'   : '26',
                 '918DST_DOM'  : '27',
                 'NUM'         : '28'
                }

iTM_CAR_PARA_ = {'CVT_C20'        : '0' ,
                'CVT_C20_B'       : '1' ,
                'CVT_C20_C'       : '2' ,
                'CVT_C20_D'       : '3' ,
                'CVT_C20_E'       : '4' ,
                'CVT_C20_F'       : '5' ,
                'CVT_C20_916'     : '6' ,
                'CVT_C20_916_B'   : '7' ,
                'CVT_C20_916_C'   : '8' ,
                'CVT_C26'         : '9' ,
                'CVT_C26_B'       : '10',
                'CVT_C26_C'       : '11',
                'HTCVT_C31'       : '12',
                'HTCVT_C31_B'     : '13',
                'HTCVT_C31_C'     : '14',
                'HTCVT_C37'       : '15',
                'HTCVT_C37_B'     : '16',
                'HTCVT_C52'       : '17',
                'HTCVT_C52_B'     : '18',
                '5AT'             : '19',
                'CVT_C20_CHINA'   : '20',
                'CVT_C20_CHINA_B' : '21',
                'CVT_NUM'         : '22'
                }


iLDP_CAR_PARA = {'OTHERS'     : '0' ,
                 'G_S17'      : '1' ,
                 'G_W17'      : '2' ,
                 'G_X17'      : '3' ,
                 'B_S17'      : '4' ,
                 'B_S18'      : '5' ,
                 'B_O17'      : '6' ,
                 'B_O18'      : '7' ,
                 'V_S18W'     : '8' ,
                 'V_W17'      : '9' ,
                 'V_W18'      : '10',
                 'B_S17_US'   : '11',
                 'B_S18_US'   : '12',
                 'B_O17_US'   : '13',
                 'B_O18_US'   : '14',
                 'V_S18_US'   : '15',
                 'S_O17'      : '16',
                 'S_O18'      : '17',
                 'S_O17_US'   : '18',
                 'S_O18_US'   : '19',
                 'V_S18_EU'   : '20',
                 'V_W17_EU'   : '21',
                 'V_W18_EU'   : '22',
                 'GH_S16'     : '23',
                 'GH_S17'     : '24',
                 'GH_S18'     : '25',
                 'GH_W16'     : '26',
                 'GH_W17'     : '27',
                 'GH_W18'     : '28',
                 'GH_X17'     : '29',
                 'GH_X178'    : '30',
                 'GH_S167_US' : '31',
                 'GH_S168_US' : '32',
                 'GH_W167_US' : '33',
                 'GH_W168_US' : '34',
                 'GH_X178_US' : '35',
                 'GH_X17_EU'  : '36',
                 'GH_X178_EU' : '37',
                 'W_O20_US'   : '38',
                 'SE_O178'    : '39',
                 'SE_O178_US' : '40',
                 'SE_O178_EU' : '41',
                 'NUM'        : '42'
                 }


iLKS_CAR_PARA_ = {'OTHERS'     : '0' ,
                  'V_S'        : '1' ,
                  'V_W'        : '2' ,
                  'V_Swide'    : '3' ,
                  'V_S_US'     : '4' ,
                  'V_Swide_US' : '5' ,
                  'G_S'        : '6' ,
                  'G_W'        : '7' ,
                  'G_X'        : '8' ,
                  'B_S'        : '9' ,
                  'B_O'        : '10',
                  'NUM'        : '11'
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
                     'idx_ReqBrkPrsBaseF',
                     'idx_ReqBrkPrsBaseR',
                     'idx_ReqBrkPrsBaseCaliper',
                     'idx_VelCoefficientF',
                     'idx_VelCoefficientR',
                     'idx_aown_CorrectRate',
                     'idx_vso_error'
                     }
