iATTYPE_5AT  = 3
iATTYPE_CVT1 = 1
iATTYPE_CVT2 = 7
k_TCMType_CAN_TBL1 = [iATTYPE_5AT, iATTYPE_CVT1]
k_TCMType_CAN_TBL2 = [iATTYPE_5AT, iATTYPE_CVT2]

iCARTYPE_INVALID = 255
iCARTYPE_NOCHECK = 254


iAWD_ACT4 = 0
iAWD_VTD  = 1
iAWD_2WD  = 2
k_AWD_CAN_TBL = [
                 [iCARTYPE_INVALID, iAWD_ACT4, iAWD_VTD],
                 [iCARTYPE_INVALID, iAWD_ACT4, iAWD_VTD],
                 [iAWD_2WD        , iAWD_ACT4, iAWD_VTD],
                 [iCARTYPE_INVALID, iAWD_ACT4, iCARTYPE_INVALID]
                 ]
# CAR TYPE map definition
iDID_SELECT_TBL_ = {'DOM' : '0',
                    'NULL': '1',
                    'NA'  : '1',
                    'EU0' : '2',
                    'EU1' : '3',
                    'EU2' : '4',
                    'EU3' : '5',
                    'EU4' : '6',
                    'EU5' : '7',
                    'EU6' : '8',
                    'TBL' : '9'
                    }

iMTR_CORRECT_TBL = {'_DOM_0' : '0' ,
                    '_DOM_1' : '1' ,
                    '_DOM_2' : '2' ,
                    '_CANADA': '3' ,
                    '_EU'    : '4' ,
                    '_EU_SP' : '5' ,
                    '_KA'    : '6' ,
                    '_KA_SP' : '7' ,
                    '_US'    : '8' ,
                    '_EK'    : '9' ,
                    '_EK_SP' : '10',
                    '_TBL'   : '11'
                    }

iSPEC_ = {'CC'      : '0',
          'ACC'     : '1',
          'CCSW_0'  : '0',
          'CCSW_1'  : '1',
          'CCSW_2'  : '2',
          'CCSW_3'  : '3',
          'CCSW_4'  : '4',
          'CCSW_5'  : '5',
          'CCSW_6'  : '6',
          'CCSW_7'  : '7',
          'CCSW_8'  : '8',
          'CCSW_9'  : '9',
          'CCSW_10' : '10',
          'CCSW_NUM': '11'
}

iSPEC_ALK_MEM_ = {'LAST_MEM'    : '0',
                  'DEFAULT_ON'  : '1',
                  'DEFAULT_OFF' : '2'
                 }
iSPEC_VDCSW_ = {'3MODE' : '0',
                '2MODE' : '1'
                }

# MARKET map Definitions
iBRK_LAMP = {'_OTHERS' : '0',
             '_CHINA'  : '1',
             '_NUM'    : '2'
             }

iTGT_A = {'_DOM'          : '0',
          '_FRGN'         : '1',
          '_USCAN'        : '2',
          '_AUS'          : '3',
          '_EU'           : '4',
          '_MOD_TYPE_NUM' : '5'
          }

iFMW = {'_DOM'  : '0',
        '_FRGN' : '1',
        '_NUM'  : '2'
        }

iPC_A = {'_DOM'          : '0',
         '_OVERSEA'      : '1',
         '_MOD_TYPE_NUM' : '2'
         }

iMARKET = {'_NONE'        : '0',
           '_DOM'         : '1',
           '_US'          : '2',
           '_CANADA'      : '3',
           '_EU_GEN_L_04' : '4',
           '_EU_GEN_L_05' : '5',
           '_EU_GEN_L_06' : '6',
           '_EU_GEN_R_07' : '7',
           '_EU_GEN_R_08' : '8',
           '_AUS'         : '9',
           '_CHINA'       : '10',
           '_KOREA'       : '11',
           '_EU_GEN_R_12' : '12',
           '_MAX'         : '13'
           }

iSPEC_SIGNAL_ = {'NONE' : '0',
                 'BUZ'  : '1'
                 }

iSPEC_FMW_R_ = {'NONE'      : '0',
                'NO_SEN'    : '1',
                'SEN_EQUIP' : '2',
                }

iSPEC_FMW_R_SPD_ = {'NONE' : '0',
                    'LO'   : '1',
                    'MID'  : '2',
                    'HI'   : '3'
                    }

