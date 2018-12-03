from Util import *
import sys
import os

map_valid_values = {'OFF', 'ON'}

iPAD_CAR_PARA = []
iEG_CAR_PARA  = []
iTM_CAR_PARA  = []
iLDP_CAR_PARA = []
iLKS_CAR_PARA = []

MAP_ID_LIST = {
    'iPAD_CAR_PARA' : iPAD_CAR_PARA,
    'iEG_CAR_PARA'  : iEG_CAR_PARA,
    'iTM_CAR_PARA'  : iTM_CAR_PARA,
    'iLDP_CAR_PARA' : iLDP_CAR_PARA,
    'iLKS_CAR_PARA' : iLKS_CAR_PARA
}

STRUCTURE_TYPE_NAME = {
    'mp_ss_BrakePadUMap'                 : 'CarPara_BrakePadURearMap',
    'mp_ss_egtrq_map'                    : 'CarPara_EgTrqCharConvert',
    'mp_ss_a_fmw_direct_r'               : 'CarPara_AFMWDirectR_Tbl',
    'mp_ss_rs_inertia_mass_map'          : 'CarPara_RsInertiaMass',
    'mp_ss_tja_str_deg_calc_map'         : 'CarPara_TjaStrDegCalc_Map',
    'mp_ss_str_deg_calc_map'             : 'CarPara_StrDegCalc_Map',
    'mp_ss_ff_map_p'                     : 'CarPara_TgtYawrToTrqFF_P',
    'mp_ss_LDPDrvovrInStart'             : 'CarPara_LDPDrvovrInStart',
    'mp_ss_p_actual_max_trq_by_egrev_tbl': 'CarPara_FMWActualMaxTrq_Tbl',
    'mp_ss_TrqRateCtrlStartInELK'        : 'CarPara_ELKTrqRateCtrlStartIn',
    'mp_ss_actual_trq_map'               : 'CarPara_ActualTrqFB_Tbl',
    'mp_ss_a_fmw_kyuu_direct_r'          : 'CarPara_AFMWSuddenDirectR_Tbl',
    'mp_ss_trcon_trq_ratio_map'          : 'CarPara_TrconTrqRatio_Map',
    'mp_ss_tja_k_sec_calc_map'           : 'CarPara_TjaKSecCalc_Map',
    'mp_ss_rho_calc_map'                 : 'CarPara_RhoCalc_Map',
    'mp_ss_ff_map_m'                     : 'CarPara_TgtYawrToTrqFF_M',
    'mp_ss_LDPDrvovrOutStart'            : 'CarPara_LDPDrvovrOutStart',
    'm_f_920RebuildEng'                  : 'f_CarPara_920RebuildEng',
    'mp_ss_TrqRateCtrlStartOutELK'       : 'CarPara_ELKTrqRateCtrlStartOut',
    'mp_ss_filt_trq_map'                 : 'CarPara_FilterTrqFB_Tbl',
    'mp_ss_atr_rev_trq_border'           : 'CarPara_AtrRevTrqBoder_Tbl',
    'mp_ss_rs_tm_tbl'                    : 'CarPara_RsTM_Tbl',
    'mp_ss_TJA_FF_StrAngRangeLimTbl'     : 'CarPara_TjaFFStrAngRangeLimTbl',
    'mp_ss_tgt_trq_fbc_map_p'            : 'CarPara_TgtTrqFBC_P_Map',
    'mp_ss_kiridashi_in'                 : 'CarPara_SldpTrqRateCutIn',
    'mp_ss_LDPDrvovrOutIncutUp'          : 'CarPara_LDPDrvovrOutIncutUp',
    'mp_ss_TrqRateFailbackInELK'         : 'CarPara_ELKTrqRateFailBackIn',
    'mp_ss_eng_rpm_lim_si_ss'            : 'CarPara_EngRrmLimBySIMode_SS',
    'mp_ss_atr_fwd_trq_border'           : 'CarPara_AtrFwdTrqBoder_Tbl',
    'mp_ss_tm_efficiency'                : 'CarPara_TMEfficiency',
    'mp_ss_TJA_TGT_StrAngRangeLimTbl'    : 'CarPara_TjaTgtStrAngRangeLimTbl',
    'mp_ss_tgt_trq_calc_map_p'           : 'CarPara_TgtTrqCalc_P_Map',
    'mp_ss_kiridashi_out'                : 'CarPara_SldpTrqRateCutOut',
    'mp_ss_LDPDrvovrOutIncutDown'        : 'CarPara_LDPDrvovrOutIncutDown',
    'mp_ss_TrqRateFailbackOutELK'        : 'CarPara_ELKTrqRateFailBackOut',
    'mp_ss_eng_rpm_lim_si_s'             : 'CarPara_EngRrmLimBySIMode_S',
    'mp_ss_fmw_fm_thresh'                : 'CarPara_FMWSuddStepThrs_Map',
    'mp_ss_tm_efficiency_rpm'            : 'CarPara_TMEfficiencyRpm',
    'mp_ss_tgt_trq_fbc_map_n'            : 'CarPara_TgtTrqFBC_N_Map',
    'mp_ss_kirimodoshi_in'               : 'CarPara_SldpTrqRateBackIn',
    'mp_ss_LDPDrvovrInEnd'               : 'CarPara_LDPDrvovrInEnd',
    'mp_ss_YawrFbPGainInELK'             : 'CarPara_ELKPGainIn',
    'mp_ss_eng_rpm_lim_si_i'             : 'CarPara_EngRrmLimBySIMode_I',
    'mp_ss_fmw_goffset'                  : 'CarPara_FMWGOffset',
    'mp_ss_tgt_trq_calc_map_n'           : 'CarPara_TgtTrqCalc_N_Map',
    'mp_ss_kirimodoshi_out'              : 'CarPara_SldpTrqRateBackOut',
    'mp_ss_LDPDrvovrOutEnd'              : 'CarPara_LDPDrvovrOutEnd',
    'mp_ss_YawrFbPGainOutELK'            : 'CarPara_ELKPGainOut',
    'mp_ss_target_turbin_rpm_hp_I'       : 'CarPara_TgtTurbinRrmHP_I',
    'mp_ss_EgAclFBPNormalMap'            : 'CarPara_EgAfbPBase_Norm_R_Map',
    'mp_us_VsoMpsMin'                    : 'CarPara_VsoMpsMin',
    'mp_ss_KiriStGainVLat'               : 'CarPara_SldpCutStartGainVlat',
    'mp_ss_sas_ldp_yawr_big'             : 'CarPara_LdpYawrBig',
    'mp_ss_YawrFbPGainModoshiInELK'      : 'CarPara_ELKYawrFBPGainBackIn',
    'mp_ss_target_turbin_rpm_hp_S'       : 'CarPara_TgtTurbinRrmHP_S',
    'mp_ss_EgAclFBPSuddenMap'            : 'CarPara_EgAfbPBase_Sudd_R_Map',
    'mp_us_VsoMpsMax'                    : 'CarPara_VsoMpsMax',
    'mp_ss_KirimodoshiGainVLat'          : 'CarPara_SldpBackStartGainVlat',
    'mp_ss_sas_ldp_yawr_normal'          : 'CarPara_LdpYawrNormal',
    'mp_ss_YawrFbPGainModoshiOutELK'     : 'CarPara_ELKYawrFBPGainBackOut',
    'mp_ss_target_turbin_rpm_hp_SS'      : 'CarPara_TgtTurbinRrmHP_SS',
    'mp_ss_EgAclFBINormalMap'            : 'CarPara_EgAfbIBase_Norm_R_Map',
    'mp_us_SatEstRngHigh'                : 'CarPara_SatEstRngHigh',
    'mp_ss_tgt_yaw_angle_in'             : 'CarPara_SldpTgtYwaAnglIn',
    'mp_ss_sas_ldp_yawr_react'           : 'CarPara_LdpYawrReact',
    'mp_ss_YawrFbIGainInELK'             : 'CarPara_ELKIGainIn',
    'mp_ss_trcon_decel_ratio'            : 'CarPara_TrconDecelRatioCVT',
    'mp_ss_BrkDclFBPNormalMap'           : 'CarPara_AtrBrkDfbPBase_Norm_R_Map',
    'mp_us_SatEstRngLow'                 : 'CarPara_SatEstRngLow',
    'mp_ss_tgt_yaw_angle_out'            : 'CarPara_SldpTgtYwaAnglOut',
    'mp_ss_sas_ldp_cancel_kyokuritsu'    : 'CarPara_LdpHorizGCancel',
    'mp_ss_YawrFbIGainOutELK'            : 'CarPara_ELKIGainOut',
    'mp_ss_trq_decel_ratio'              : 'CarPara_RatioRedTrqByATM', 
    'mp_ss_BrkDclFBPSuddenMap'           : 'CarPara_AtrBrkDfbPBase_Sudd_R_Map',
    'mp_ss_yawrfb_pgain_in'              : 'CarPara_SldpYawrFBPGainIn',
    'mp_ss_sas_ldp_fukki_kyokuritsu'     : 'CarPara_LdpHorizGOn',
    'mp_ss_YawrFbDGainInELK'             : 'CarPara_ELKDGainIn',
    'mp_ss_trq_decel_ratio_4trq'         : 'CarPara_RatioRedTrqByATM4Trq',
    'mp_ss_BrkDclFBINormalMap'           : 'CarPara_AtrBrkDfbIBase_Norm_R_Map',
    'mp_ss_yawrfb_pgain_out'             : 'CarPara_SldpYawrFBPGainOut',
    'mp_ss_YawrFbDGainOutELK'            : 'CarPara_ELKDGainOut',
    'mp_ss_trq_eng_minus_max'            : 'CarPara_TrqEngMinusMax',
    'mp_ss_BrkDclFBISuddenMap'           : 'CarPara_AtrBrkDfbIBase_Sudd_R_Map',
    'mp_ss_YawrFbPgainModoshiIn'         : 'CarPara_SldpYawrFBPGainBackIn',
    'mp_ss_ELKDrvovrInStart'             : 'CarPara_ELKDrvovrInStart',
    'mp_ss_trq_eng_accessory'            : 'CarPara_TrqEngAccessory',
    'mp_ss_EgAclFBPSlopeNormalTbl'       : 'CarPara_AfbPCoeff_Norm_Tbl',
    'mp_ss_YawrFbPgainModoshiOut'        : 'CarPara_SldpYawrFBPGainBackOut',
    'mp_ss_ELKDrvovrOutStart'            : 'CarPara_ELKDrvovrOutStart',
    'mp_ss_trq_ac_compressor'            : 'CarPara_TrqAcCompressor',
    'mp_ss_EgAclFBPSlopeSuddenTbl'       : 'CarPara_AfbPCoeff_Sudd_Tbl',
    'mp_ss_yawrfb_igain_in'              : 'CarPara_SldpYawrFBIGainIn',
    'mp_ss_ELKDrvovrOutIncutUp'          : 'CarPara_ELKDrvovrOutIncutUp',
    'mp_ss_trq_fcut'                     : 'CarPara_TrqFCut',
    'mp_ss_EgAclFBISlopeNormalTbl'       : 'CarPara_AfbICoeff_Norm_Tbl',
    'mp_ss_yawrfb_igain_out'             : 'CarPara_SldpYawrFBIGainOut',
    'mp_ss_ELKDrvovrOutIncutDown'        : 'CarPara_ELKDrvovrOutIncutDown',
    'mp_ss_trq_throff'                   : 'CarPara_ThrOffEgTrq',
    'mp_ss_BrkDclFBPSlopeNormalTbl'      : 'CarPara_BfbPCoeff_Norm_Tbl',
    'mp_ss_yawrfb_dgain_in'              : 'CarPara_SldpYawrFBDGainIn',
    'mp_ss_ELKDrvovrInEnd'               : 'CarPara_ELKDrvovrInEnd',
    'mp_ss_rpm_lim_fbinh'                : 'CarPara_RrmLimFBinh',
    'mp_ss_BrkDclFBPSlopeSuddenTbl'      : 'CarPara_BfbPCoeff_Sudd_Tbl',
    'mp_ss_yawrfb_dgain_out'             : 'CarPara_SldpYawrFBDGainOut',
    'mp_ss_ELKDrvovrOutEnd'              : 'CarPara_ELKDrvovrOutEnd',
    'mp_ss_target_a_max_koubai_teisoku'  : 'CarPara_TgtAMaxSlopeFixSp',
    'mp_ss_BrkDclFBISlopeNormalTbl'      : 'CarPara_BfbICoeff_Norm_Tbl',
    'mp_ss_ctrl_start_time_in'           : 'CarPara_SldpCtrlStartTimeIn',
    'mp_ss_target_a_max_koubai_tuibi'    : 'CarPara_TgtAMaxSlopeTracking',
    'mp_ss_BrkDclFBISlopeSuddenTbl'      : 'CarPara_BfbICoeff_Sudd_Tbl',
    'mp_ss_ctrl_start_time_out'          : 'CarPara_SldpCtrlStartTimeOut',
    'mp_ss_towing_limit'                 : 'CarPara_TowingLimit',
    'mp_ss_delta_a_norm'                 : 'CarPara_Delta_A_Norm',
    'mp_ss_tgt_yawr_ff_gain_in'          : 'CarPara_SldpTgtYawrFFGainIn',
    'mp_ss_engine_rpm_lim_by_urusai_map' : 'CarPara_EngRrmLimByNoisy_Map',
    'mp_ss_delta_a_kyuu'                 : 'CarPara_Delta_A_Sudd',
    'mp_ss_width_time_st'                : 'CarPara_SldpRWidthTimeST',
    'mp_ss_eg_cvt_rate'                  : 'CarPara_EgCVTRate',
    'mp_ss_delta_a_woseigyo'             : 'CarPara_Delta_A_Off',
    'mp_ss_grad_yawrfb_pgain_kudari'     : 'CarPara_SldpRGradYawrFBPGain_Dn',
    'mp_uc_start_gratio_thr'             : 'CarPara_StartGRatioThr', 
    'mp_ss_kyufumi_b_filt'               : 'CarPara_SuddDepresBStatFilt',
    'mp_ss_grad_yawrfb_pgain_nobori'     : 'CarPara_SldpRGradYawrFBPGain_Up',
    'mp_ss_rpm_ratio_trpm_start'         : 'CarPara_RpmRatioTrpmStart',
    'mp_ss_kyufumi_f_filt'               : 'CarPara_SuddDepresFStatFilt',
    'mp_ss_grad_yawrfb_igain_kudari'     : 'CarPara_SldpRGradYawrFIPGain_Dn',
    'mp_ss_kasoku_gain_rpm'              : 'CarPara_TgtAGainRpm',
    'mp_ss_f_state_filt'                 : 'CarPara_FStatFilt',
    'mp_ss_grad_yawrfb_igain_nobori'     : 'CarPara_SldpRGradYawrFIPGain_Up',
    'mp_ss_throff_inhi_minclip'          : 'CarPara_ThrOffInhiMinClip',
    'mp_ss_atr_afb_i_rt'                 : 'CarPara_AtrAfbIRt0',
    'mp_ss_grad_yawrfb_dgain_kudari'     : 'CarPara_SldpRGradYawrFDPGain_Dn',
    'mp_ss_rs_ac_comp'                   : 'CarPara_RsAcComp',
    'mp_ss_atr_bfb_i_rt'                 : 'CarPara_AtrBfbIRt0',
    'mp_ss_grad_yawrfb_dgain_nobori'     : 'CarPara_SldpRGradYawrFDPGain_Up',
    'mp_ss_KMdGainTeisoku'               : 'CarPara_KMdGainFixSp', 
    'mp_ss_a_apor_in'                    : 'CarPara_AAporInit',
    'mp_ss_width_kiridashi_kudari'       : 'CarPara_SldpRGradWidthCut_Dn',
    'mp_ss_KMdGainTuibi'                 : 'CarPara_KMdGainTracking',
    'mp_ss_a_apor_kyuu_ini'              : 'CarPara_AAporSuddInit',
    'mp_ss_width_kiridashi_nobori'       : 'CarPara_SldpRGradWidthCut_Up',
    'm_f_SpecEngTrqLim'                  : 'f_CarPara_SpecEngTrqLimit',
    'mp_ss_target_deg_m4_filt'           : 'CarPara_TgtDegM4Filt',
    'mp_ss_width_kirimodoshi_kudari'     : 'CarPara_SldpRGradWidthBack_Dn',
    'm_f_SpecEngTrqLimPreign'            : 'f_CarPara_SpecEngTrqLimPreign',
    'mp_ss_target_deg_filt_recv'         : 'CarPara_TgtDegFiltRecv',
    'mp_ss_width_kirimodoshi_nobori'     : 'CarPara_SldpRGradWidthBack_Up', 
    'mp_s_aown_filt'                     : 'CarPara_AOwnFilt',
    'mp_ss_ay_lim'                       : 'CarPara_SldpAYLim',
    'mp_s_t_diff4a_lng'                  : 'CarPara_TDiff4ALong',
    'mp_ss_SldpAyLimFB'                  : 'CarPara_SldpAyLimFB',
    'mp_ss_TrqRateCtrlStartIn'           : 'CarPara_TrqRateCtrlStartIn',
    'mp_ss_TrqRateCtrlStartOut'          : 'CarPara_TrqRateCtrlStartOut',
    'mp_ss_TgtYawrVownC'                 : 'CarPara_TgtYawrVownC',
    'mp_uc_ThYawInvCnt'                  : 'CarPara_ThYawInvCnt',
    'mp_ss_DrvTrqAxis'                   : 'CarPara_SldpFricDrvTrqAxis',
    'mp_ss_FricDrvGainIn'                : 'CarPara_SldpFricDrvGainIn',
    'mp_ss_FricDrvGainOut'               : 'CarPara_SldpFricDrvGainOut',
    'mp_sl_FricDrvTrq'                   : 'CarPara_ThrFricDrvTrq',
    'mp_ss_RestartPosIn'                 : 'CarPara_SldpCtrlHorizRestartPosIn',
    'mp_ss_RestartPosOut'                : 'CarPara_SldpCtrlHorizRestartPosOut',
    'mp_ss_NotDepPosEndIn'               : 'CarPara_SldpCtrlHorizPosEndHis1In',
    'mp_ss_NotDepPosEndOut'              : 'CarPara_SldpCtrlHorizPosEndHis1Out',
    'mp_ss_DepPosEndIn'                  : 'CarPara_SldpCtrlHorizPosEndHis2In',
    'mp_ss_DepPosEndOut'                 : 'CarPara_SldpCtrlHorizPosEndHis2Out',
    'mp_ss_TgtTimeYawfbIn'               : 'CarPara_SldpTgtTimeYawFBIn',
    'mp_ss_TgtTimeYawfbOut'              : 'CarPara_SldpTgtTimeYawFBOut',
    'm_uc_slope_tp_ff_enb'               : 'f_CarPara_SlopeTpFFEnb',
}


def create_param_dimension(string, df):
    global row_paramDim, col_paramDim

    # for [][] count
    cur_value_cnt = str(string).count('[')
    if cur_value_cnt == 0:
        return
   
    remove_braces = re.sub(
        r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', string)
    remove_braces = remove_braces[1:]


    df_dimen = df[df.Parameter == remove_braces]
    #print(df_dimen, remove_braces)

    if len(df_dimen.head(1)) == 0:
        row_paramDim = 0
        col_paramDim = 0
        print("No Object Model for ", remove_braces)
    else:
        row_paramDim = df_dimen['Dim1'].item()
        col_paramDim = df_dimen['Dim2'].item()

    row_paramDim = int(row_paramDim)
    col_paramDim = int(col_paramDim)

    

def is_valid(result, row, col):
    tofind = str(result.iat[row, col])
    if re.search("&", str(tofind)) or \
            str(tofind) in map_valid_values:
            return 1
    else:
        return 0

def is_match( match ):
    if match is not None:
        cur_value = match.group(1)
        return cur_value
    else:
        return 'xxx'

def display_map_id(result, create_data, right_holder_name, cur_value_cnt):
    if right_holder_name in STRUCTURE_TYPE_NAME:
        right_holder_name = STRUCTURE_TYPE_NAME[right_holder_name]

    if cur_value_cnt == 1:
        remove_braces = re.sub(
            r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', result)
        remove_braces = remove_braces[1:]
        create_data.append(
            'for (uint8 i = 0; i < ' + str(row_paramDim)             +
            '; i++)\n' + triple_tab  + '{\n'    + four_tab           +
            str(right_holder_name)   + '[i] = ' + str(remove_braces) +
            '[i];\n' + triple_tab    + '}\n'
        )
    elif cur_value_cnt == 2:
        remove_braces = re.sub(
            r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', result)
        remove_braces = remove_braces[1:]
        brakets = re.search(r'\[(.*?)\]', result).group(1)
        
        if int(brakets) != 0:
            create_data.append(
                'for (uint8 i = 0; i < ' + str(col_paramDim) +
                '; i++)\n' + triple_tab   + '{\n' + four_tab  +
                str(right_holder_name)    + 
                '[i] = '   + str(remove_braces)   +
                '['        + str(brakets) + ']'   +
                '[i];\n'   + triple_tab   + '}\n'
            )
        else:
            if str(right_holder_name) == STRUCTURE_TYPE_NAME['mp_ss_eng_rpm_lim_si_i']:
                create_data.append(
                    'for (uint8 i = 0; i < ' + str(col_paramDim) +
                    '; i++)\n' + triple_tab  + '{\n' + four_tab  +
                    str(right_holder_name)   +
                    '[i] = ' + str(remove_braces)    +
                    '[0]'    + '[i];\n' + triple_tab + '}\n'
                )
            else:
                create_data.append(
                    'for (uint8 i = 0; i < ' + str(row_paramDim)      +
                    '; i++)\n' + triple_tab  + '{\n'       + four_tab +
                    'for (uint8 j = 0; j < ' + str(col_paramDim)      +
                    '; i++)\n' + four_tab    + '{\n'       + five_tab +
                    str(right_holder_name)   + '[i][j] = ' + str(remove_braces) +
                    '[i][j];\n' + four_tab   + '}\n'       + triple_tab + '}\n'
                )
    else:
        if result in map_valid_values:
            create_data.append(
                str(right_holder_name) + 
                ' = ' + str(result) + ';\n'
            )
        else:
            remove_braces = str(result).replace('&', '')
            create_data.append(
                str(right_holder_name) +
                ' = ' + str(remove_braces) + ';\n'
            )

def start_process(df, dic):
    df = df['MAP_ID'].fillna('xxx')
    #df = df['Sheet3'].fillna('xxx')

    # create dataframe for all dimension
    dimen_filename = 'MCUAppAllParamDims.xlsx'
    df_dimen = create_dataframe_dimen(dimen_filename)
    
    #print(df_dimen)

    member_variable_name = 'Member Variable Name'
    found_flag, default_found, not_valid_cnt = 0, 0, 0
    value_name = []
    right_holder_name = ''
    
    for col in range(3, df.shape[1]):  # df.shape[1]
        for row in range(0, df.shape[0]):
            if member_variable_name == str(df.iat[row,2]):
                default_found = row + 2
                right_holder_name = df.iat[row,col]
                found_flag = 1
                not_valid_cnt = 0

                # for structure type name
                if df.iat[row,col] not in exception_string:
                    if df.iat[row, col] in STRUCTURE_TYPE_NAME:
                        value_name.append(
                            df.iat[row, col] + ' = ' +
                            STRUCTURE_TYPE_NAME[df.iat[row, col]] +  '\n'
                        )
                        create_param_dimension(df.iat[row + 2, col], df_dimen)
                    else:
                        print('Not int STRUCTURE_TYPE_NAME', df.iat[row, col])
                # check for the list value
                cur_value = df.iat[row + 2, 2]
                match = re.match("^(.*?_.*?_.*?)_", str(cur_value))
                cur_value = is_match(match)
                # -----------

                # reserve for R and F CAR PARA
                if cur_value == 'iPAD_F_CAR' or cur_value == 'iPAD_R_CAR':
                    cur_value = 'iPAD_CAR_PARA'
                    
                case_counter = 1
                if cur_value not in exception_string and \
                    is_valid(df, row + 2, col):
                    MAP_ID_LIST[cur_value].append(
                        tab + 'switch (' + cur_value +
                        ')\n' + tab + '{\n' + double_tab
                    )
                
            if found_flag:
                cur_value = df.iat[row, 2]
                match = re.match("^(.*?_.*?_.*?)_", str(cur_value))
                cur_value = is_match(match)

                # reserve for R and F CAR PARA
                if cur_value == 'iPAD_F_CAR' or cur_value == 'iPAD_R_CAR':
                    cur_value = 'iPAD_CAR_PARA'
                
                # for [][] count
                cur_value_cnt = df.iat[row, col]
                cur_value_cnt = str(cur_value_cnt).count('[')
                
                if is_valid(df, row, col) and row != default_found and \
                cur_value not in exception_string:
                    
                    # create case display
                    MAP_ID_LIST[cur_value].append(
                        'case (uint8)' + str(case_counter) + ':\n' + double_tab +
                        '{\n' + triple_tab
                    )
                    # for loop display
                    display_map_id(
                        df.iat[row,col],
                        MAP_ID_LIST[cur_value],
                        right_holder_name,
                        cur_value_cnt)
                    # break display
                    MAP_ID_LIST[cur_value].append(
                        triple_tab + 'break;\n' + double_tab + '}\n' + double_tab
                    )
                    #print(df.iat[row, col], MAP_ID_LIST[cur_value], right_holder_name, cur_value_cnt)
                    case_counter = case_counter + 1
                    #pass
                elif not is_valid(df, row, col):
                    not_valid_cnt = not_valid_cnt + 1
                
                if not_valid_cnt == 3:
                    if is_valid(df, default_found, col):
                        # for [][] count
                        cur_value_cnt = df.iat[default_found, col]
                        cur_value_cnt = str(cur_value_cnt).count('[')

                        # check for the list value
                        cur_value = df.iat[default_found, 2]
                        match = re.match("^(.*?_.*?_.*?)_", str(cur_value))
                        cur_value = is_match(match)
                        # -----------

                        # reserve for R and F CAR PARA
                        if cur_value == 'iPAD_F_CAR' or cur_value == 'iPAD_R_CAR':
                            cur_value = 'iPAD_CAR_PARA'

                        if cur_value not in exception_string:
                            #print('default', df.iat[default_found, 2])
                            MAP_ID_LIST[cur_value].append(
                                'default:\n' + double_tab + '{\n' + triple_tab
                            )
                            # for loop display
                            display_map_id(
                                df.iat[default_found, col],
                                MAP_ID_LIST[cur_value],
                                right_holder_name,
                                cur_value_cnt)
                            # create closing braces
                            MAP_ID_LIST[cur_value].append(
                                double_tab + '}\n' + tab + '}\n'
                            )
                   
    for i in MAP_ID_LIST:
        write_list('CarParaMap.c', MAP_ID_LIST[i])
        pass
    #write_list('CarParaMapValue.c', value_name)
    #write_list('Map_ID_Dim.c', paraDim)


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
        start_process(df, dic)


if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])
