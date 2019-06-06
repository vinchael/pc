from Util import *
import sys
import os

map_valid_values = {'OFF', 'ON'}

iPAD_CAR_PARA = []
iEG_CAR_PARA  = []
iTM_CAR_PARA  = []
iLDP_CAR_PARA = []
iLKS_CAR_PARA = []
iAES_CAR_PARA = []

MAP_ID_LIST = {
    'iPAD_CAR_PARA' : iPAD_CAR_PARA,
    'iEG_CAR_PARA'  : iEG_CAR_PARA,
    'iTM_CAR_PARA'  : iTM_CAR_PARA,
    'iLDP_CAR_PARA' : iLDP_CAR_PARA,
    'iLKS_CAR_PARA' : iLKS_CAR_PARA,
    'iAES_CAR_PARA' : iAES_CAR_PARA
}

STRUCTURE_TYPE_NAME = {
    'mp_ss_BrakePadUMap'                 : 'CarPara_BrakePadURearMap_gbl',
    'mp_ss_egtrq_map'                    : 'CarPara_EgTrqCharConvert_gbl',
    'mp_ss_a_fmw_direct_r'               : 'CarPara_AFMWDirectR_Tbl_gbl',
    'mp_ss_rs_inertia_mass_map'          : 'CarPara_RsInertiaMass_gbl',
    'mp_ss_tja_str_deg_calc_map'         : 'CarPara_TjaStrDegCalc_Map_gbl',
    'mp_ss_str_deg_calc_map'             : 'CarPara_StrDegCalc_Map_gbl',
    'mp_ss_ff_map_p'                     : 'CarPara_TgtYawrToTrqFF_P_gbl',
    'mp_ss_LDPDrvovrInStart'             : 'CarPara_LDPDrvovrInStart_gbl',
    'mp_ss_p_actual_max_trq_by_egrev_tbl': 'CarPara_FMWActualMaxTrq_Tbl_gbl',
    'mp_ss_TrqRateCtrlStartInELK'        : 'CarPara_ELKTrqRateCtrlStartIn_gbl',
    'mp_ss_actual_trq_map'               : 'CarPara_ActualTrqFB_Tbl_gbl',
    'mp_ss_a_fmw_kyuu_direct_r'          : 'CarPara_AFMWSuddenDirectR_Tbl_gbl',
    'mp_ss_trcon_trq_ratio_map'          : 'CarPara_TrconTrqRatio_Map_gbl',
    'mp_ss_tja_k_sec_calc_map'           : 'CarPara_TjaKSecCalc_Map_gbl',
    'mp_ss_rho_calc_map'                 : 'CarPara_RhoCalc_Map_gbl',
    'mp_ss_ff_map_m'                     : 'CarPara_TgtYawrToTrqFF_M_gbl',
    'mp_ss_LDPDrvovrOutStart'            : 'CarPara_LDPDrvovrOutStart_gbl',
    'm_f_920RebuildEng'                  : 'f_CarPara_920RebuildEng_gbl',
    'mp_ss_TrqRateCtrlStartOutELK'       : 'CarPara_ELKTrqRateCtrlStartOut_gbl',
    'mp_ss_filt_trq_map'                 : 'CarPara_FilterTrqFB_Tbl_gbl',
    'mp_ss_atr_rev_trq_border'           : 'CarPara_AtrRevTrqBoder_Tbl_gbl',
    'mp_ss_rs_tm_tbl'                    : 'CarPara_RsTM_Tbl_gbl',
    'mp_ss_TJA_FF_StrAngRangeLimTbl'     : 'CarPara_TjaFFStrAngRangeLimTbl_gbl',
    'mp_ss_tgt_trq_fbc_map_p'            : 'CarPara_TgtTrqFBC_P_Map_gbl',
    'mp_ss_kiridashi_in'                 : 'CarPara_SldpTrqRateCutIn_gbl',
    'mp_ss_LDPDrvovrOutIncutUp'          : 'CarPara_LDPDrvovrOutIncutUp_gbl',
    'mp_ss_TrqRateFailbackInELK'         : 'CarPara_ELKTrqRateFailBackIn_gbl',
    'mp_ss_eng_rpm_lim_si_ss'            : 'CarPara_EngRrmLimBySIMode_SS_gbl',
    'mp_ss_atr_fwd_trq_border'           : 'CarPara_AtrFwdTrqBoder_Tbl_gbl',
    'mp_ss_tm_efficiency'                : 'CarPara_TMEfficiency_gbl',
    'mp_ss_TJA_TGT_StrAngRangeLimTbl'    : 'CarPara_TjaTgtStrAngRangeLimTbl_gbl',
    'mp_ss_tgt_trq_calc_map_p'           : 'CarPara_TgtTrqCalc_P_Map_gbl',
    'mp_ss_kiridashi_out'                : 'CarPara_SldpTrqRateCutOut_gbl',
    'mp_ss_LDPDrvovrOutIncutDown'        : 'CarPara_LDPDrvovrOutIncutDown_gbl',
    'mp_ss_TrqRateFailbackOutELK'        : 'CarPara_ELKTrqRateFailBackOut_gbl',
    'mp_ss_eng_rpm_lim_si_s'             : 'CarPara_EngRrmLimBySIMode_S_gbl',
    'mp_ss_fmw_fm_thresh'                : 'CarPara_FMWSuddStepThrs_Map_gbl',
    'mp_ss_tm_efficiency_rpm'            : 'CarPara_TMEfficiencyRpm_gbl',
    'mp_ss_tgt_trq_fbc_map_n'            : 'CarPara_TgtTrqFBC_N_Map_gbl',
    'mp_ss_kirimodoshi_in'               : 'CarPara_SldpTrqRateBackIn_gbl',
    'mp_ss_LDPDrvovrInEnd'               : 'CarPara_LDPDrvovrInEnd_gbl',
    'mp_ss_YawrFbPGainInELK'             : 'CarPara_ELKPGainIn_gbl',
    'mp_ss_eng_rpm_lim_si_i'             : 'CarPara_EngRrmLimBySIMode_I_gbl',
    'mp_ss_fmw_goffset'                  : 'CarPara_FMWGOffset_Tbl_gbl',
    'mp_ss_tgt_trq_calc_map_n'           : 'CarPara_TgtTrqCalc_N_Map_gbl',
    'mp_ss_kirimodoshi_out'              : 'CarPara_SldpTrqRateBackOut_gbl',
    'mp_ss_LDPDrvovrOutEnd'              : 'CarPara_LDPDrvovrOutEnd_gbl',
    'mp_ss_YawrFbPGainOutELK'            : 'CarPara_ELKPGainOut_gbl',
    'mp_ss_target_turbin_rpm_hp_I'       : 'CarPara_TgtTurbinRrmHP_I_gbl',
    'mp_ss_EgAclFBPNormalMap'            : 'CarPara_EgAfbPBase_Norm_R_Map_gbl',
    'mp_us_VsoMpsMin'                    : 'CarPara_VsoMpsMin_gbl',
    'mp_ss_KiriStGainVLat'               : 'CarPara_SldpCutStartGainVlat_gbl',
    'mp_ss_sas_ldp_yawr_big'             : 'CarPara_LdpYawrBig_gbl',
    'mp_ss_YawrFbPGainModoshiInELK'      : 'CarPara_ELKYawrFBPGainBackIn_gbl',
    'mp_ss_target_turbin_rpm_hp_S'       : 'CarPara_TgtTurbinRrmHP_S_gbl',
    'mp_ss_EgAclFBPSuddenMap'            : 'CarPara_EgAfbPBase_Sudd_R_Map_gbl',
    'mp_us_VsoMpsMax'                    : 'CarPara_VsoMpsMax_gbl',
    'mp_ss_KirimodoshiGainVLat'          : 'CarPara_SldpBackStartGainVlat_gbl',
    'mp_ss_sas_ldp_yawr_normal'          : 'CarPara_LdpYawrNormal_gbl',
    'mp_ss_YawrFbPGainModoshiOutELK'     : 'CarPara_ELKYawrFBPGainBackOut_gbl',
    'mp_ss_target_turbin_rpm_hp_SS'      : 'CarPara_TgtTurbinRrmHP_SS_gbl',
    'mp_ss_EgAclFBINormalMap'            : 'CarPara_EgAfbIBase_Norm_R_Map_gbl',
    'mp_us_SatEstRngHigh'                : 'CarPara_SatEstRngHigh_gbl',
    'mp_ss_tgt_yaw_angle_in'             : 'CarPara_SldpTgtYwaAnglIn_gbl',
    'mp_ss_sas_ldp_yawr_react'           : 'CarPara_LdpYawrReact_gbl',
    'mp_ss_YawrFbIGainInELK'             : 'CarPara_ELKIGainIn_gbl',
    'mp_ss_trcon_decel_ratio'            : 'CarPara_TrconDecelRatioCVT_gbl',
    'mp_ss_BrkDclFBPNormalMap'           : 'CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl',
    'mp_us_SatEstRngLow'                 : 'CarPara_SatEstRngLow_gbl',
    'mp_ss_tgt_yaw_angle_out'            : 'CarPara_SldpTgtYwaAnglOut_gbl',
    'mp_ss_sas_ldp_cancel_kyokuritsu'    : 'CarPara_LdpHorizGCancel_gbl',
    'mp_ss_YawrFbIGainOutELK'            : 'CarPara_ELKIGainOut_gbl',
    'mp_ss_trq_decel_ratio'              : 'CarPara_RatioRedTrqByATM_gbl',
    'mp_ss_BrkDclFBPSuddenMap'           : 'CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl',
    'mp_ss_yawrfb_pgain_in'              : 'CarPara_SldpYawrFBPGainIn_gbl',
    'mp_ss_sas_ldp_fukki_kyokuritsu'     : 'CarPara_LdpHorizGOn_gbl',
    'mp_ss_YawrFbDGainInELK'             : 'CarPara_ELKDGainIn_gbl',
    'mp_ss_trq_decel_ratio_4trq'         : 'CarPara_RatioRedTrqByATM4Trq_gbl',
    'mp_ss_BrkDclFBINormalMap'           : 'CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl',
    'mp_ss_yawrfb_pgain_out'             : 'CarPara_SldpYawrFBPGainOut_gbl',
    'mp_ss_YawrFbDGainOutELK'            : 'CarPara_ELKDGainOut_gbl',
    'mp_ss_trq_eng_minus_max'            : 'CarPara_TrqEngMinusMax_gbl',
    'mp_ss_BrkDclFBISuddenMap'           : 'CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl',
    'mp_ss_YawrFbPgainModoshiIn'         : 'CarPara_SldpYawrFBPGainBackIn_gbl',
    'mp_ss_ELKDrvovrInStart'             : 'CarPara_ELKDrvovrInStart_gbl',
    'mp_ss_trq_eng_accessory'            : 'CarPara_TrqEngAccessory_gbl',
    'mp_ss_EgAclFBPSlopeNormalTbl'       : 'CarPara_AfbPCoeff_Norm_Tbl_gbl',
    'mp_ss_YawrFbPgainModoshiOut'        : 'CarPara_SldpYawrFBPGainBackOut_gbl',
    'mp_ss_ELKDrvovrOutStart'            : 'CarPara_ELKDrvovrOutStart_gbl',
    'mp_ss_trq_ac_compressor'            : 'CarPara_TrqAcCompressor_gbl',
    'mp_ss_EgAclFBPSlopeSuddenTbl'       : 'CarPara_AfbPCoeff_Sudd_Tbl_gbl',
    'mp_ss_yawrfb_igain_in'              : 'CarPara_SldpYawrFBIGainIn_gbl',
    'mp_ss_ELKDrvovrOutIncutUp'          : 'CarPara_ELKDrvovrOutIncutUp_gbl',
    'mp_ss_trq_fcut'                     : 'CarPara_TrqFCut_gbl',
    'mp_ss_EgAclFBISlopeNormalTbl'       : 'CarPara_AfbICoeff_Norm_Tbl_gbl',
    'mp_ss_yawrfb_igain_out'             : 'CarPara_SldpYawrFBIGainOut_gbl',
    'mp_ss_ELKDrvovrOutIncutDown'        : 'CarPara_ELKDrvovrOutIncutDown_gbl',
    'mp_ss_trq_throff'                   : 'CarPara_ThrOffEgTrq_gbl',
    'mp_ss_BrkDclFBPSlopeNormalTbl'      : 'CarPara_BfbPCoeff_Norm_Tbl_gbl',
    'mp_ss_yawrfb_dgain_in'              : 'CarPara_SldpYawrFBDGainIn_gbl',
    'mp_ss_ELKDrvovrInEnd'               : 'CarPara_ELKDrvovrInEnd_gbl',
    'mp_ss_rpm_lim_fbinh'                : 'CarPara_RrmLimFBinh_gbl',
    'mp_ss_BrkDclFBPSlopeSuddenTbl'      : 'CarPara_BfbPCoeff_Sudd_Tbl_gbl',
    'mp_ss_yawrfb_dgain_out'             : 'CarPara_SldpYawrFBDGainOut_gbl',
    'mp_ss_ELKDrvovrOutEnd'              : 'CarPara_ELKDrvovrOutEnd_gbl',
    'mp_ss_target_a_max_koubai_teisoku'  : 'CarPara_TgtAMaxSlopeFixSp_gbl',
    'mp_ss_BrkDclFBISlopeNormalTbl'      : 'CarPara_BfbICoeff_Norm_Tbl_gbl',
    'mp_ss_ctrl_start_time_in'           : 'CarPara_SldpCtrlStartTimeIn_gbl',
    'mp_ss_target_a_max_koubai_tuibi'    : 'CarPara_TgtAMaxSlopeTracking_gbl',
    'mp_ss_BrkDclFBISlopeSuddenTbl'      : 'CarPara_BfbICoeff_Sudd_Tbl_gbl',
    'mp_ss_ctrl_start_time_out'          : 'CarPara_SldpCtrlStartTimeOut_gbl',
    'mp_ss_towing_limit'                 : 'CarPara_TowingLimit_gbl',
    'mp_ss_delta_a_norm'                 : 'CarPara_Delta_A_Norm_gbl',
    'mp_ss_tgt_yawr_ff_gain_in'          : 'CarPara_SldpTgtYawrFFGainIn_gbl',
    'mp_ss_engine_rpm_lim_by_urusai_map' : 'CarPara_EngRrmLimByNoisy_Map_gbl',
    'mp_ss_delta_a_kyuu'                 : 'CarPara_Delta_A_Sudd_gbl',
    'mp_ss_width_time_st'                : 'CarPara_SldpRWidthTimeST_gbl',
    'mp_ss_eg_cvt_rate'                  : 'CarPara_EgCVTRate_gbl',
    'mp_ss_delta_a_woseigyo'             : 'CarPara_Delta_A_Off_gbl',
    'mp_ss_grad_yawrfb_pgain_kudari'     : 'CarPara_SldpRGradYawrFBPGain_Dn_gbl',
    'mp_uc_start_gratio_thr'             : 'CarPara_StartGRatioThr_gbl',
    'mp_ss_kyufumi_b_filt'               : 'CarPara_SuddDepresBStatFilt_gbl',
    'mp_ss_grad_yawrfb_pgain_nobori'     : 'CarPara_SldpRGradYawrFBPGain_Up_gbl',
    'mp_ss_rpm_ratio_trpm_start'         : 'CarPara_RpmRatioTrpmStart_gbl',
    'mp_ss_kyufumi_f_filt'               : 'CarPara_SuddDepresFStatFilt_gbl',
    'mp_ss_grad_yawrfb_igain_kudari'     : 'CarPara_SldpRGradYawrFIPGain_Dn_gbl',
    'mp_ss_kasoku_gain_rpm'              : 'CarPara_TgtAGainRpm_gbl',
    'mp_ss_f_state_filt'                 : 'CarPara_FStatFilt_gbl',
    'mp_ss_grad_yawrfb_igain_nobori'     : 'CarPara_SldpRGradYawrFIPGain_Up_gbl',
    'mp_ss_throff_inhi_minclip'          : 'CarPara_ThrOffInhiMinClip_gbl',
    'mp_ss_atr_afb_i_rt'                 : 'CarPara_AtrAfbIRt0_gbl',
    'mp_ss_grad_yawrfb_dgain_kudari'     : 'CarPara_SldpRGradYawrFDPGain_Dn_gbl',
    'mp_ss_rs_ac_comp'                   : 'CarPara_RsAcComp_gbl',
    'mp_ss_atr_bfb_i_rt'                 : 'CarPara_AtrBfbIRt0_gbl',
    'mp_ss_grad_yawrfb_dgain_nobori'     : 'CarPara_SldpRGradYawrFDPGain_Up_gbl',
    'mp_ss_KMdGainTeisoku'               : 'CarPara_KMdGainFixSp_gbl',
    'mp_ss_a_apor_in'                    : 'CarPara_AAporInit_gbl',
    'mp_ss_width_kiridashi_kudari'       : 'CarPara_SldpRGradWidthCut_Dn_gbl',
    'mp_ss_KMdGainTuibi'                 : 'CarPara_KMdGainTracking_gbl',
    'mp_ss_a_apor_kyuu_ini'              : 'CarPara_AAporSuddInit_gbl',
    'mp_ss_width_kiridashi_nobori'       : 'CarPara_SldpRGradWidthCut_Up_gbl',
    'm_f_SpecEngTrqLim'                  : 'f_CarPara_SpecEngTrqLimit_gbl',
    'mp_ss_target_deg_m4_filt'           : 'CarPara_TgtDegM4Filt_gbl',
    'mp_ss_width_kirimodoshi_kudari'     : 'CarPara_SldpRGradWidthBack_Dn_gbl',
    'm_f_SpecEngTrqLimPreign'            : 'f_CarPara_SpecEngTrqLimPreign_gbl',
    'mp_ss_target_deg_filt_recv'         : 'CarPara_TgtDegFiltRecv_gbl',
    'mp_ss_width_kirimodoshi_nobori'     : 'CarPara_SldpRGradWidthBack_Up_gbl',
    'mp_s_aown_filt'                     : 'CarPara_AOwnFilt_gbl',
    'mp_ss_ay_lim'                       : 'CarPara_SldpAYLim_gbl',
    'mp_s_t_diff4a_lng'                  : 'CarPara_TDiff4ALong_gbl',
    'mp_ss_SldpAyLimFB'                  : 'CarPara_SldpAyLimFB_gbl',
    'mp_ss_TrqRateCtrlStartIn'           : 'CarPara_TrqRateCtrlStartIn_gbl',
    'mp_ss_TrqRateCtrlStartOut'          : 'CarPara_TrqRateCtrlStartOut_gbl',
    'mp_ss_TgtYawrVownC'                 : 'CarPara_TgtYawrVownC_gbl',
    'mp_uc_ThYawInvCnt'                  : 'CarPara_ThYawInvCnt_gbl',
    'mp_ss_DrvTrqAxis'                   : 'CarPara_SldpFricDrvTrqAxis_gbl',
    'mp_ss_FricDrvGainIn'                : 'CarPara_SldpFricDrvGainIn_gbl',
    'mp_ss_FricDrvGainOut'               : 'CarPara_SldpFricDrvGainOut_gbl',
    'mp_sl_FricDrvTrq'                   : 'CarPara_ThrFricDrvTrq_gbl',
    'mp_ss_RestartPosIn'                 : 'CarPara_SldpCtrlHorizRestartPosIn_gbl',
    'mp_ss_RestartPosOut'                : 'CarPara_SldpCtrlHorizRestartPosOut_gbl',
    'mp_ss_NotDepPosEndIn'               : 'CarPara_SldpCtrlHorizPosEndHis1In_gbl',
    'mp_ss_NotDepPosEndOut'              : 'CarPara_SldpCtrlHorizPosEndHis1Out_gbl',
    'mp_ss_DepPosEndIn'                  : 'CarPara_SldpCtrlHorizPosEndHis2In_gbl',
    'mp_ss_DepPosEndOut'                 : 'CarPara_SldpCtrlHorizPosEndHis2Out_gbl',
    'mp_ss_TgtTimeYawfbIn'               : 'CarPara_SldpTgtTimeYawFBIn_gbl',
    'mp_ss_TgtTimeYawfbOut'              : 'CarPara_SldpTgtTimeYawFBOut_gbl',
    'm_uc_slope_tp_ff_enb'               : 'f_CarPara_SlopeTpFFEnb_gbl',
    'mp_us_t_delay_PresUpAES'            : 'CarPara_t_delay_PresUpAES_gbl',
    'mp_ss_k_sec_calc_map'               : 'CarPara_KSecCalc_Map_gbl',
    'mp_us_jerk_max_PC_AOWNAES'          : 'CarPara_jerkAES_gbl',
    'mp_ss_LKS_FF_StrAngRangeLimTbl'     : 'CarPara_LKSFFStrAngRangeLimTbl_gbl',
    'mp_ss_LKS_TGT_StrAngRangeLimTbl'    : 'CarPara_LKSTgtStrAngRangeLimTbl_gbl',
    'mp_ss_turbo_trq_thr_4eleWGV'        : 'CarPara_TurboTrqThr_gbl',
    'mp_ss_corr_atm_trq_thr_4eleWGV'     : 'CarPara_CorrAtmTrqThr_gbl',
    'mp_ss_trq_decel_ratio_4trq_4eleWGV' : 'CarPara_RatioRedTrqByATM4Trq4WGV_gbl',
    'mp_ss_trq_decel_ratio_4eleWGV'      : 'CarPara_RatioRedTrqByATM4WGV_gbl',
    'mp_ss_LDP_PredictLength'            : 'CarPara_LDP_PredictLength_gbl',
    'mp_ss_LDP_V2L_Line_hensa_Gain'      : 'CarPara_LDP_V2L_Line_hensa_Gain_gbl',
    'mp_ss_LDP_V2V_Pgain'                : 'CarPara_LDP_V2V_Pgain_gbl',
    'mp_ss_LDP_V2L_lat_acc_Pgain'        : 'CarPara_LDP_V2L_lat_acc_Pgain_gbl',
    'mp_ss_LDP_V2L_lat_acc_Igain'        : 'CarPara_LDP_V2L_lat_acc_Igain_gbl',
    'mp_ss_LDP_V2L_lat_acc_YawPgain'     : 'CarPara_LDP_V2L_lat_acc_YawPgain_gbl',
    'mp_ss_LdpYawrBig'                   : 'CarPara_LdpYawrBig_gbl',
    'mp_ss_LDP_add_str_jark'             : 'CarPara_LDP_add_str_jark_gbl',
    'mp_ss_LdpYawrNormal'                : 'CarPara_LdpYawrNormal_gbl',
    'mp_ss_LDP_car_response_delay'       : 'CarPara_LDP_car_response_delay_gbl',
    'mp_ss_LdpYawrReact'                 : 'CarPara_LdpYawrReact_gbl',
    'mp_ss_LDP_cal_StrAngRateLim'        : 'CarPara_LDP_cal_StrAngRateLim_gbl',
    'mp_ss_LdpHorizGCancel'              : 'CarPara_LdpHorizGCancel_gbl',
    'mp_ss_LDP_Str_Ang_RateLim'          : 'CarPara_LDP_Str_Ang_RateLim_gbl',
    'mp_ss_LdpHorizGOn'                  : 'CarPara_LdpHorizGOn_gbl',
    'mp_ss_LDP_Rate_Lo_IN'               : 'CarPara_LDP_Rate_Lo_IN_gbl',
    'mp_ss_LDP_Rate_Lo_OUT'              : 'CarPara_LDP_Rate_Lo_OUT_gbl',
    'mp_ss_LDP_Rate_Hi'                  : 'CarPara_LDP_Rate_Hi_gbl_gbl',
    'mp_ss_LDP_lat_acc_std'              : 'CarPara_LDP_lat_acc_std_gbl',
    'mp_ss_RDP_lat_acc_std'              : 'CarPara_RDP_lat_acc_std_gbl',
    'mp_ss_RDP_add_str_jark'             : 'CarPara_RDP_add_str_jark_gbl',
    'mp_us_MirrorWidthAES'               : 'CarPara_MirrorWidthAES_gbl',
    'mp_ss_CtrlStrAngMaxAES'             : 'CarPara_CtrlStrAngMaxAES_gbl',
    'mp_ss_LatPosPgainAES'               : 'CarPara_LatPosPgainAES_gbl',
    'mp_ss_YawAngPgainAES'               : 'CarPara_YawAngPgainAES_gbl',
    'mp_ss_DrvOvrAES'                    : 'CarPara_DrvOvrAES_gbl',
    'mp_ss_LatPosIgainAES'               : 'CarPara_LatPosIgainAES_gbl',
    'mp_us_CtrlTTCAES'                   : 'CarPara_CtrlTTCAES_gbl',
}


def create_param_dimension(string, df):
    global row_paramDim, col_paramDim

    # for [][] count
    cur_value_cnt = str(string).count('[')
    if cur_value_cnt == 0:
        return 0
   
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

    return 1

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
            'for (uint8 i = 0U; i < ' + str(row_paramDim)             + 'U' +
            '; i++)\n' + triple_tab   + '{\n'    + four_tab           +
            str(right_holder_name)    + '[i] = ' + str(remove_braces) +
            '[i];\n' + triple_tab     + '}\n'
        )
    elif cur_value_cnt == 2:
        remove_braces = re.sub(
            r'\[(.*){1,3}\]\[(.*){1,3}\]|\[(.*){1,3}\]', '', result)
        remove_braces = remove_braces[1:]
        brakets = re.search(r'\[(.*?)\]', result).group(1)
        
        if int(brakets) != 0:
            create_data.append(
                'for (uint8 i = 0U; i < ' + str(col_paramDim) + 'U' +
                '; i++)\n' + triple_tab   + '{\n' + four_tab  +
                str(right_holder_name)    + 
                '[i] = '   + str(remove_braces)   +
                '['        + str(brakets) + ']'   +
                '[i];\n'   + triple_tab   + '}\n'
            )
        else:
            """ Please check for the change [*][] 
                first index array might change this condition"""
            if str(right_holder_name) == STRUCTURE_TYPE_NAME['mp_ss_eng_rpm_lim_si_i']:
                create_data.append(
                    'for (uint8 i = 0U; i < ' + str(col_paramDim) + 'U' +
                    '; i++)\n' + triple_tab  + '{\n' + four_tab  +
                    str(right_holder_name)   +
                    '[i] = ' + str(remove_braces)    +
                    '[0]'    + '[i];\n' + triple_tab + '}\n'
                )
            else:
                create_data.append(
                    'for (uint8 i = 0U; i < ' + str(row_paramDim) + 'U' +
                    '; i++)\n' + triple_tab  + '{\n'       + four_tab +
                    'for (uint8 j = 0U; j < ' + str(col_paramDim) + 'U' +
                    '; j++)\n' + four_tab    + '{\n'       + five_tab +
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
    dimen_filename = 'MCUAppAllParamDims_20190328.xlsx'
    df_dimen = create_dataframe_dimen(dimen_filename)
    
    #print(df_dimen)

    if is_eng_ON:
        member_variable_name = 'Member Variable Name'
    else:
        member_variable_name = 'メンバ変数名'
        
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
                        # print('->', df.iat[row, col])
                        param_dim = create_param_dimension(df.iat[row + 2, col], df_dimen)
                        if param_dim !=0:
                            str_row_paramDim = '[' + str(row_paramDim) + ']'
                            str_col_paramDim = '[' + str(col_paramDim) + '];'
                        else:
                            str_row_paramDim = ''
                            str_col_paramDim = ';'
                        
                        value_name.append(
                            'float32 ' + STRUCTURE_TYPE_NAME[df.iat[row, col]] + \
                            str_row_paramDim + str_col_paramDim +
                            '                        /* ' + df.iat[row, col] + ' */' +
                            '\n'
                        )
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
                    """ print('row:' + str(row + 2) + tab+'col:' + str(col)) """
                
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
                                triple_tab + 'break;\n' +
                                double_tab + '}\n' + tab + '}\n'
                            )
                   
    for i in MAP_ID_LIST:
        write_list('Map_ID.c', MAP_ID_LIST[i])
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
            if sheet_name != 'MAP_ID':
                continue
            #print('Sheet name:', sheet_name, 'index:',count)
            df[sheet_name] = xls.parse(sheet_name)
            dic[count] = sheet_name
            count = count + 1
        # --START--
        start_process(df, dic)


if __name__ == "__main__":
    #print(__name__)
    main(sys.argv[1:])
