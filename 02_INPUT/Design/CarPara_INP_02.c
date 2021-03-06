#ifndef CARPARA_INP_02
#define CARPARA_INP_02

#include "Rte_Type.h"
#include "CarPara_INP_02.h"
#include "CarPara_GLOBAL.h"
#include "MDL_Car_Param.h"
#include "MDL_VC_Param.h"
#include "RC_Const_Common_Tuning_Param.h"
#include "RC_Const_Select_Tuning_Param.h"
#include "AllFeatureLocalParams.h"
#include "stdio.h"

static boolean get_ALKSpecJudge(void);
static boolean is_equal(boolean *buf, uint16 size);
static void set_BrkLamp(uint8 add_BRK_LAMP);
static void set_TgtATrackingSp(uint8 TgtAMap_index);
static void set_TgtAConstSp(uint8 TgtAMap_index);
static void set_TgtASetSpAcc(uint8 TgtAMap_index);
static void set_TgtASetSpAccConst(uint8 TgtAMap_index);
static void set_TgtASetSpMsa(uint8 TgtAMap_index);
static void set_PCMarket(uint8 PCMarket_index);
static void set_FMWMaxGCont(uint8 FMWMaxGType_index);
static void set_FMWMaxGVown(uint8 FMWMaxGType_index);
static void set_FMWMaxGR(uint8 FMWMaxGType_index);
static void set_BrakePad_uMAP(const float32 *x_table, uint16 N, float32 x, uint16 *irx, float32 *fraction);
static void set_LookupTable_2D_Pre(uint16 *ReqBrkG_irx,
                               uint16 *ReqBrkG_minus1,
                               uint16 *ActualSpeedCorrect_irx,
                               uint16 *ActualSpeedCorrect_minus1,
                               uint16 *ReqBrkG_irx_out,
                               uint16 *ActualSpeedCorrect_irx_out);
static void set_LookupTable_2D_Logic(uint16  *ReqBrkG_irx_out,
                                     uint16  *ActualSpeedCorrect_irx_out,
                                     uint16  *ReqBrkG_irx,
                                     float32 *ReqBrkG_fraction,
                                     uint16  *ActualSpeedCorrect_irx,
                                     float32 *ActualSpeedCorrect_fraction,
                                     float32 table_uMap[7][6],
                                     float32 *out_indexValue,
                                     float32 *out1,
                                     float32 *out2,
                                     float32 *out3);
static void set_LookupTable_2D_Post(float32 *out_indexValue,
                                    float32 *out1,
                                    float32 *out2,
                                    float32 *out3,
                                    float32 *ReqBrkG_fraction,
                                    float32 *ActualSpeedCorrect_fraction,
                                    float32 *out_y);
static void set_LookupTable_1D_Pre(uint16  *ReqBrkG_irx,
                                   uint16  *ReqBrkG_minus1,
                                   uint16  *ReqBrkG_irx_out,
                                   boolean *ReqBrkG_irx_cond);
static void set_LookupTable_1D_Logic(uint16  *ReqBrkG_irx_out,
                                     boolean *ReqBrkG_irx_cond,
                                     uint16  *ReqBrkG_irx,
                                     float32 *ReqBrkG_fraction,
                                     float32 table_stopHoji[10],
                                     float32 *out);

static void set_CarPara_VariantPTType(uint8 iEG_CAR_PARA);     /* iEG_CAR_PARA    */
static void set_CarPara_VariantTMType(uint8 iTM_CAR_PARA);     /* iTM_CAR_PARA    */
static void set_CarPara_VariantForLKS(uint8 iLKS_CAR_PARA);    /* iLKS_CAR_PARA   */
static void set_CarPara_VariantForLDP(uint8 iLDP_CAR_PARA);    /* iLDP_CAR_PARA   */   
static void set_CarPara_VariantPadTypeF(uint8 iPAD_CAR_PARA_F);/* iPAD_CAR_PARA_F */    
static void set_CarPara_VariantPadTypeR(uint8 iPAD_CAR_PARA_R);/* iPAD_CAR_PARA_R */
static void set_CarPara_VariantForAES(uint8 iAES_CAR_PARA);    /* iAES_CAR_PARA   */

void CarPara_INP_02(uint8 *CarCode)
{
    static boolean unit_delay      = 0;
    static uint16  CarCode_index   = 0;
    static boolean CarCode_unmatch = 1;
    uint16 car_code_value[16]       = {0};

    /* k_ CARPARA HEV */
    uint8 car_para_HEV = 0;
    static uint16 CarPara_HEV_index = 0;

    /* k_ CARPARA RAB */
    boolean car_para_RAB = 0;
    static uint16 f_CarPara_RAB_index = 0;

    /* CarPara_SpecTCM */
    static uint8 CarPara_TCM_index = 0;

    /* CarPara_Market_gbl */
    static uint8 CarPara_Market_index = 0;
    uint8 BrkLamp_index     = 0;
    uint8 TgtAMap_index     = 0;
    uint8 PCMarket_index    = 0;
    uint8 FMWMaxGType_index = 0;

    /* CarPara_PU_ENG */
    static uint16 CarPara_PU_index = 0;

    /* CarPara_SpecISS */
    static uint16 CarPara_ISS_index = 0;

    /* MtrCorrectTbl */
    uint8 mtr_correcttble = 0;

    /* Vel_coefficient */
    float32 ActualSpeedCorrectF = 0.F;
    float32 ActualSpeedCorrectR = 0.F;

    /* GetBrakePadU */
    /* Req_brk_press */
    uint8   row_BrakePad_uMAP_G = 0;
    uint16  row_G_minus1        = 0;
    uint8   row_BrakePad_uMAP_V = 0;
    uint16  row_V_minus1        = 0;
    uint16  ReqBrkG_F_irx;
    float32 ReqBrkG_F_fraction;
    uint16  ActualSpeedCorrect_F_irx;
    float32 ActualSpeedCorrect_F_fraction;
    uint16  ReqBrkG_F_out;
    uint16  ActualSpeedCorrect_F_out;
    float32 out_F_indexValue;
    float32 out1_F;
    float32 out2_F;
    float32 out3_F;
    float32 BrakePadUF;

    uint16  ReqBrkG_FR_irx;
    float32 ReqBrkG_FR_fraction;
    uint16  ActualSpeedCorrect_FR_irx;
    float32 ActualSpeedCorrect_FR_fraction;
    uint16  ReqBrkG_FR_out;
    uint16  ActualSpeedCorrect_FR_out;
    float32 out_FR_indexValue;
    float32 out1_FR;
    float32 out2_FR;
    float32 out3_FR;
    float32 BrakePadUR;

    /* GetBaseFact */
    float32 BrakePadUR_out;
    float32 switch_BrakePad;
    float32 ReqBrkPrsBase_out;
    float32 BaseFact;

    /* GetChgReqBrkPrsTbl */
    uint8   iterator;
    float32 ChgReqBrkprsNoClpr;
    uint8   row_ChgReqBrkPrsTbl = 0;
    static  float32 ChgReqBrkPrsTbl[32] = {0.F};

    /* CalcISSReqG */
    uint16  row_KOBAI_G  = 0;
    uint16  row_K_minus1 = 0;
    uint16  ReqBrkG_K_irx;
    float32 ReqBrkG_K_fraction;
    uint16  ReqBrkG_K_out;
    boolean ReqBrkG_K_out_cond;
    float32 out_ISSSlopeGradePrs;
    float32 STOPHOJI_ISS_SLOPE_MIN_temp[10];

    /* UncertaintyAxis */
    float32 uncertaintyAxisSelector[3]   = {0.F};
    boolean uncertaintyAxisLogicalOpt[2] = {0.F};
    float32 UncertaintyAxisMerge         = 0.F;

    /* NormalAxis */
    uint16  row_REQ_BRK_G                 = 0;
    uint16  row_REQ_BRK_G_minus1          = 0;
    static  float32 InvertedAxis_out[32]  = {0.F};
    static  float32 InvertedTable_out[32] = {0.F};
    uint16  InvertedAxis_irx;
    float32 InvertedAxis_fraction;
    uint16  Inverted_out;
    boolean Inverted_out_cond;

    uint16 row = 0;
    uint16 col = 0;

    if (unit_delay == OFF)
    {
        boolean CarPara_CarCodeValue[16] = {0};
        /* CarCodeInitJudg */
        row = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);
        for (uint16 i = 0; i < row; i++)
        {
            CarPara_CarCodeValue[i] = (boolean)(CarCode[i] == gk_uc_Car_Type_default[i]);
        }
        if (is_equal(CarPara_CarCodeValue, row))
        {
            CarCode_unmatch       = ON;
            f_CarPara_Init_gbl    = ON;
            f_CarPara_Decided_gbl = OFF;
            f_CarCodeUnmatch_gbl  = CarCode_unmatch;

            /* GetCarType */
            CarPara_Body_gbl            = (uint8)ZERO;
            CarPara_CarSystem_gbl       = (uint8)ZERO;
            CarPara_Market_gbl          = (uint8)ZERO;
            CarPara_Grade_gbl           = (uint8)ZERO;
            CarPara_PU_gbl              = (uint8)ZERO;
            CarPara_HEV_gbl             = (boolean)ZERO != 0.F;
            CarPara_ISS_gbl             = (boolean)ZERO != 0.F;
            CarPara_AWD_gbl             = (uint8)ZERO;
            CarPara_TCM_gbl             = (uint8)ZERO;
            CarPara_TireSize_gbl        = (uint8)ZERO;
            CarPara_BrkSize_gbl         = (uint8)ZERO;
            CarPara_MarketDetail_gbl    = (uint8)ZERO;
            f_CarPara_RAB_gbl           = (boolean)ZERO != 0.F;
            CarPara_YearMode_gbl        = (uint8)ZERO;
            f_CarPara_SpecATMEquip_gbl  = (boolean)ZERO != 0.F;
            CarPara_EBrkBooster_gbl     = (boolean)ZERO != 0.F;
            f_CarPara_EquipEPB_gbl      = (boolean)ZERO != 0.F;
            CarPara_AVH_gbl             = (boolean)ZERO != 0.F;
            CarPara_OEM_gbl             = (uint8)ZERO;
            CarPara_EquipMSA_gbl        = (uint8)ZERO;
            CarPara_TSR_gbl             = (boolean)ZERO != 0.F;
            CarPara_RadarF_gbl          = (boolean)ZERO != 0.F;
            CarPara_RadarR_gbl          = (boolean)ZERO != 0.F;
            f_CarPara_EquipELK_gbl      = (boolean)ZERO != 0.F;
            CarPara_StrVibration_gbl    = (boolean)ZERO != 0.F;
            CarPara_RoadEdge_gbl        = (boolean)ZERO != 0.F;
            CarPara_MapLocator_gbl      = (boolean)ZERO != 0.F;
            CarPara_SonarF_gbl          = (boolean)ZERO != 0.F;
            CarPara_DMS_gbl             = (boolean)ZERO != 0.F;
            CarPara_ALC_gbl             = (boolean)ZERO != 0.F;
            CarPara_NotHoldingALK_gbl   = (boolean)ZERO != 0.F;
            CarPara_AutoDepart_gbl      = (boolean)ZERO != 0.F;
            CarPara_iACC_gbl            = (boolean)ZERO != 0.F;
            CarPara_SlowBfCrv_gbl       = (boolean)ZERO != 0.F;

            /* version 2-0: */
            CarPara_LDPType_gbl         = (boolean)ZERO != 0.F;
            CarPara_LKSType_gbl         = (boolean)ZERO != 0.F;
            /* version 2-0: */

            CarPara_StrTouchSensor_gbl  = (boolean)ZERO != 0.F;
            CarPara_MonoCam_gbl         = (boolean)ZERO != 0.F;
            CarPara_LightCtrl_gbl       = (boolean)ZERO != 0.F;
            CarPara_HandlePos_gbl       = (uint8)ZERO;

            /* version 2-0: */
            CarPara_EPS_gbl             = (uint8)ZERO;
            GPF_gbl                     = (boolean)ZERO != 0.F;
            /* version 2-0: */

            /* version 2-10 */
            CarPara_PedAirbag_gbl       = (boolean)ZERO != 0.F;
            ftMVS_gbl                   = (boolean)ZERO != 0.F;
            /* version 2-10 */


            TCMType_gbl                        = (uint8)default_tcm_type;

            /* BrkLamp */
            set_BrkLamp((uint8)0);

            /* TgtAMap */
            set_TgtATrackingSp((uint8)0);
            set_TgtAConstSp((uint8)0);
            set_TgtASetSpAcc((uint8)0);
            set_TgtASetSpAccConst((uint8)0);
            set_TgtASetSpMsa((uint8)0);

            /* AccSp */
            CarPara_ACCSetSpInitMaxKph_gbl    = default_acc_set_spd_max;
            CarPara_ACCSetSpInitMinKph_gbl    = default_acc_set_spd_min;
            CarPara_ACCSetSpInitMaxMph_gbl    = default_acc_set_spd_max_mile;
            CarPara_ACCSetSpInitMinMph_gbl    = default_acc_set_spd_min_mile;
            CarPara_ACCCtrlSpInitMax_gbl      = default_acc_cont_spd_max;
            CarPara_ACCCtrlSpInitMin_gbl      = default_acc_cont_spd_min;

            /* CcSp    */
            CarPara_CCSetSpInitMaxKph_gbl     = default_cc_set_spd_max;
            CarPara_CCSetSpInitMinKph_gbl     = default_cc_set_spd_min;
            CarPara_CCSetSpInitMaxMph_gbl     = default_cc_set_spd_max_mile;
            CarPara_CCSetSpInitMinMph_gbl     = default_cc_set_spd_min_mile;
            CarPara_CCCtrlSpInitMax_gbl       = default_cc_cont_spd_max;
            CarPara_CCCtrlSpInitMin_gbl       = default_cc_cont_spd_min;

            /* Dist_warn    */
            CarPara_DistWarnSpMax_gbl         = default_dis_kh_spd_max;

            /* PCSpd    */
            CarPara_PCSpMax_gbl               = default_pc_spd_max;

            /* PCBa */
            CarPara_PCBAMaxVownOff2On_gbl     = default_pcba_max_on_by_vown;
            CarPara_PCBAMaxVownOn2Off_gbl     = default_pcba_max_off_by_vown;
            CarPara_PCBAMaxVrefOff2On_gbl     = default_pcba_max_on_by_vrel_pc;
            CarPara_PCBAMaxVrefOn2Off_gbl     = default_pcba_max_off_by_vrel_pc;

            /* FMWSp */
            CarPara_FMWSpMax_gbl              = default_fmw_spd_max;

            /* PCMarket */
            set_PCMarket((uint8)0);

            /* LdpSp */
            CarPara_LDP_VOffMax_gbl           = default_ldp_v_max_off;
            CarPara_LDP_VOffMin_gbl           = default_ldp_v_min_off;
            CarPara_LDP_VOnMax_gbl            = default_ldp_v_max_on;
            CarPara_LDP_VOnMin_gbl            = default_ldp_v_min_on;

            /* AlkLim */
            f_CarPara_ALK_LimitWarn_gbl       = (boolean)default_ALK_limit_kh;

            /* OneSideLane */
            CarPara_OnesideLaneMode_gbl       = (boolean)default_oneside_lane_mode;

            /* FMWMaxGType */
            set_FMWMaxGCont((uint8)0);
            set_FMWMaxGVown((uint8)0);
            set_FMWMaxGR((uint8)0);

            /* MarketInfo */
            CarPara_MarketInfo_gbl            = (uint8)default_MARKETInfo;

            /* OtherInfo */
            CarPara_SpecSignalType_gbl        = (uint8)default_spec_signal_type;
            CarPara_SpecFMWRType_gbl          = (uint8)default_spec_fmw_r_type;
            CarPara_SpecFMWRSpdLimDefault_gbl = (uint8)default_spec_fmw_r_spd_lim_default;

            /* EquipCustom */
            f_CarPara_SpecCustTypeRoad_gbl = (boolean)default_RoadTypeCustEquip;
            CarPara_RoadTypeDefault_gbl    = (uint8)default_RoadTypeDefault;

            /* version 2-0: */
            /* ALKCTJAVown */
            CarPara_LKSVownOffMax_gbl = default_LKSVownMaxOff;
            CarPara_LKSVownOffMin_gbl = default_LKSVownMinOff;
            CarPara_LKSVownOnMax_gbl  = default_LKSVownMaxOn;
            CarPara_LKSVownOnMin_gbl  = default_LKSVownMinOn;
            /* version 2-0: */

            /* RoadEdge */
            CarPara_RoadEdgeSpMaxOff_gbl = default_roadedge_v_max_off;
            CarPara_RoadEdgeSpMinOff_gbl = default_roadedge_v_min_off;
            CarPara_RoadEdgeSpMaxOn_gbl  = default_roadedge_v_max_on;
            CarPara_RoadEdgeSpMinOn_gbl  = default_roadedge_v_min_on;

            /* NotHoldingALK */
            CarPara_NotHoldingSpMaxOff_gbl = default_b2_v_max_off;
            CarPara_NotHoldingSpMinOff_gbl = default_b2_v_min_off;
            CarPara_NotHoldingSpMaxOn_gbl  = default_b2_v_max_on;
            CarPara_NotHoldingSpMinOn_gbl  = default_b2_v_min_on;

            /* ALC */
            CarPara_ALCSpMaxOff_gbl = default_alc_v_max_off;
            CarPara_ALCSpMinOff_gbl = default_alc_v_min_off;
            CarPara_ALCSpMaxOn_gbl  = default_alc_v_max_on;
            CarPara_ALCSpMinOn_gbl  = default_alc_v_min_on;

            /* CarVariant */
            CarPara_VariantPTType_gbl   = (uint8)default_variant_pt;
            CarPara_VariantTMType_gbl   = (uint8)default_variant_tm;
            CarPara_VariantForLKS_gbl   = (uint8)default_variant_for_lks;
            CarPara_VariantForLDP_gbl   = (uint8)default_variant_for_ldp;
            CarPara_VariantPadTypeF_gbl = (uint8)default_VariantPadTypeF;
            CarPara_VariantPadTypeR_gbl = (uint8)default_VariantPadTypeR;

            /* MovingTire */
            CarPara_MovingTire_R_gbl   = default_moving_tire_r;

            /* Vehicle_Weight */
            CarPara_Vehicle_Weight_gbl = default_vehicle_weight;

            /* RS_degree_C */
            CarPara_RS_0degree_C_gbl = default_rs_0degree_c;
            CarPara_RS_1degree_C_gbl = default_rs_1degree_c;
            CarPara_RS_2degree_C_gbl = default_rs_2degree_c;

            /* AownHosei_fact */
            CarPara_AownCorrectFact_gbl = default_aown_CorrectRate;

            /* VSO_Error */
            CarPara_VSOError_gbl = default_vso_error;

            /* Veh */
            CarPara_Vehicle_Width_gbl = default_vehicle_width;

            /* EngRev */
            CarPara_EngRevLimMax_gbl = default_eng_rev_min_lim;

            /* GearRatio */
            CarPara_GearRatioMax_gbl = default_gear_ratio_max;
            CarPara_GearRatioMin_gbl = default_gear_ratio_min;
            CarPara_FinalGear_gbl    = default_final_gear;

            /* CarSpec */
            CarPara_SpecLKSDefault_gbl   = (boolean)default_lks_default;
            CarPara_SpecALKSwMemory_gbl  = (uint8)default_SpecALKSwMemory;
            CarPara_VDCSwForPCSpec_gbl   = (uint8)default_vdc_offmode;
            CarPara_CCType_gbl           = (uint8)default_spec_cc_type;
            CarPara_CCSwType_gbl         = (uint8)default_spec_cc_sw_type;
            f_CarPara_SpecManualMode_gbl = (boolean)default_spec_manual_mode;
            CarPara_DistRWheel2Vdc_gbl   = default_distance_rw_2_vdc;

            /* DidSelect */
            CarPara_DidSelect_gbl = (uint8)default_did_select_tbl;
            

            /* MtrCorrectTbl */
            /* TODO check for the *value */
            col = sizeof(k_MtrCorrectTbl[0]) / sizeof(k_MtrCorrectTbl[0][0]);
            for (uint16 i = 0; i < col; i++)
            {
                CarPara_MeterCorrectionTbl_gbl[i] = k_MtrCorrectTbl[(uint8)default_add_mtr_correct][i];
            }
            
            /* SwLongPress */
            CarPara_SwLongPressThrTime_gbl  = default_nagaoshi_thr;

            /* Req_brk_press */
            CarPara_ReqBrakePressBase_F_gbl = default_ReqBrkPrsBaseF;
            CarPara_ReqBrakePressBase_R_gbl = default_ReqBrkPrsBaseR;
            CarPara_ReqBrakePressBase_C_gbl = default_ReqBrkPrsBaseCaliper;

            /* Vel_coefficient */
            CarPara_VelCoefficientFront_gbl = default_VelCoefficientF;
            CarPara_VelCoefficientRear_gbl  = default_VelCoefficientR;

            /* EquipCustom */
            f_CarPara_R79Spec_gbl           = (boolean)default_SpecR79;

            /* EpsCtrlTrqALKLim */
            CarPara_EpsCtrlTrqALKLim_gbl    = default_EpsCtrlTrqALKLim;

            /* CustomACC */
            CarPara_AcclCustType_gbl        = ON;
            
            /* CAMtoFtAxle */
            CarPara_CAMtoFtAxle_gbl         = default_CarPara_CAMtoFtAxle;
            CarPara_CAMtoRrAxle_gbl         = default_CarPara_CAMtoRrAxle;

            /* version 2-0: */
            CarPara_AES_gbl                 = (boolean)default_AES;
            /* version 2-0: */

            /* version 2-10 */
            CarPara_TJABlue_gbl            = (uint8)default_TJABlue;
            CarPara_SpecLDPSwMemory_gbl    = (uint8)default_SpecLDPSwMemory;
            CarPara_Variant_for_AES_gbl    = (uint8)default_variant_for_aes;
            /* version 2-10 */

            /* CarPara_SpecISS */
            f_CarPara_SpecISSEquip_gbl = OFF;
            CarPara_ISSRbgUprt_gbl          = ((float32)SINT16MIN) * 0.001F;
            CarPara_EgTrq_Max_gbl           = default_engtrq_max;

            row = sizeof(default_REQ_BRKPRS) / sizeof(default_REQ_BRKPRS[0]);
            for (uint16 i = 0; i < row; i++)
            {
                CarPara_ReqBrkPrsTbl_gbl[i] = default_REQ_BRKPRS[i];
            }
        }
        else
        {
            row = sizeof(k_CAR_CAR_CODE_INDEX) / sizeof(k_CAR_CAR_CODE_INDEX[0]);
            col = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);
            
            for (uint16 i = 0; i < row; i++)
            {
                for (uint16 j = 0; j < col; j++)
                {
                    CarPara_CarCodeValue[j] = (boolean)(CarCode[j] == k_CAR_CAR_CODE_INDEX[i][j]);
                }
                /* if found stop the loop  */
                if (is_equal(CarPara_CarCodeValue, col))
                {
                    CarCode_index    = i;
                    CarCode_unmatch  = OFF;
                    f_CarCodeUnmatch_gbl = CarCode_unmatch;
                    break;
                }
            }
            /* Set the array value to car_code_value from k_CAR_CAR_CODE_INDEX */
            row = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);
            for (uint16 i = 0; i < row; i++)
            {
                car_code_value[i] = k_CAR_CAR_CODE_INDEX[CarCode_index][i];
            }

            f_CarPara_Init_gbl    = OFF;
            f_CarPara_Decided_gbl = ON;

			/* GetCarType */
            CarPara_CarSystem_gbl       =  ((uint8)(car_code_value[ba_CarSystem        [0]] & ba_CarSystem     [1])) >> ba_CarSystem     [2]     ;
            CarPara_YearMode_gbl        =  ((uint8)(car_code_value[ba_YearMode         [0]] & ba_YearMode      [1])) >> ba_YearMode      [2]     ;
            CarPara_Market_gbl          =  ((uint8)(car_code_value[ba_Market           [0]] & ba_Market        [1])) >> ba_Market        [2]     ;
            CarPara_MarketDetail_gbl    =  ((uint8)(car_code_value[ba_MarketDetail     [0]] & ba_MarketDetail  [1])) >> ba_MarketDetail  [2]     ;
            CarPara_Grade_gbl           =  ((uint8)(car_code_value[ba_Grade            [0]] & ba_Grade         [1])) >> ba_Grade         [2]     ;
            f_CarPara_SpecATMEquip_gbl  = (((boolean)(car_code_value[ba_AtmPressSensor [0]] & ba_AtmPressSensor[1])) >> ba_AtmPressSensor[2])!= 0;
            CarPara_PU_gbl              =  ((uint8)(car_code_value[ba_PU               [0]] & ba_PU            [1])) >> ba_PU            [2]     ;
            CarPara_Body_gbl            =  ((uint8)(car_code_value[ba_Body             [0]] & ba_Body          [1])) >> ba_Body          [2]     ;
            CarPara_BrkSize_gbl         =  ((uint8)(car_code_value[ba_Brake            [0]] & ba_Brake         [1])) >> ba_Brake         [2]     ;
            CarPara_EBrkBooster_gbl     = (((boolean)(car_code_value[ba_EBrkBooster    [0]] & ba_EBrkBooster   [1])) >> ba_EBrkBooster   [2])!= 0;
            CarPara_TireSize_gbl        =  ((uint8)(car_code_value[ba_Tire             [0]] & ba_Tire          [1])) >> ba_Tire          [2]     ;
            CarPara_ISS_gbl             = (((boolean)(car_code_value[ba_ISS            [0]] & ba_ISS           [1])) >> ba_ISS           [2])!= 0;
            /* HEV */
            car_para_HEV               =  ((uint8)(car_code_value[ba_HEV              [0]] & ba_HEV           [1])) >> ba_HEV           [2]     ;
            row = sizeof(k_HEV_CAR_CODE_INDEX) / sizeof(k_HEV_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (car_para_HEV == k_HEV_CAR_CODE_INDEX[i])
                {
                    CarPara_HEV_index = i;
                    break;
                }
            }
            CarPara_HEV_gbl  = (boolean)k_HEV_CAR_CODE_INDEX[CarPara_HEV_index];
            /* End HEV */
            f_CarPara_EquipEPB_gbl      = (((boolean)(car_code_value[ba_EPB            [0]] & ba_EPB           [1])) >> ba_EPB           [2])!= 0;
            CarPara_AVH_gbl             = (((boolean)(car_code_value[ba_AVH            [0]] & ba_AVH           [1])) >> ba_AVH           [2])!= 0;
            CarPara_TCM_gbl             =  ((uint8)(car_code_value[ba_TCM              [0]] & ba_TCM           [1])) >> ba_TCM           [2]     ;
            CarPara_AWD_gbl             =  ((uint8)(car_code_value[ba_AWD              [0]] & ba_AWD           [1])) >> ba_AWD           [2]     ;
            /* RAB */
            car_para_RAB               =  (((boolean)(car_code_value[ba_RAB           [0]] & ba_RAB           [1])) >> ba_RAB           [2])!= 0;
            row = sizeof(k_RAB_SELECT_SPEC) / sizeof(k_RAB_SELECT_SPEC[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (car_para_RAB == k_RAB_SELECT_SPEC[i])
                {
                    f_CarPara_RAB_index = i;
                    break;
                }
            }
            f_CarPara_RAB_gbl  = (boolean)k_RAB_SELECT_SPEC[f_CarPara_RAB_index];
            

            CarPara_OEM_gbl             =  ((uint8)(car_code_value[ba_OEM              [0]] & ba_OEM           [1])) >> ba_OEM           [2]     ;
            CarPara_EquipMSA_gbl        =  ((uint8)(car_code_value[ba_MSA              [0]] & ba_MSA           [1])) >> ba_MSA           [2]     ;
            CarPara_TSR_gbl             =  ((boolean)(car_code_value[ba_TSR            [0]] & ba_TSR           [1])) >> ba_TSR           [2]     ;
            CarPara_RadarF_gbl          = (((boolean)(car_code_value[ba_RadarF         [0]] & ba_RadarF        [1])) >> ba_RadarF        [2])!= 0;
            CarPara_RadarR_gbl          = (((boolean)(car_code_value[ba_RadarR         [0]] & ba_RadarR        [1])) >> ba_RadarR        [2])!= 0;
            f_CarPara_EquipELK_gbl      = (((boolean)(car_code_value[ba_ELK            [0]] & ba_ELK           [1])) >> ba_ELK           [2])!= 0;
            CarPara_StrVibration_gbl    = (((boolean)(car_code_value[ba_StrVibration   [0]] & ba_StrVibration  [1])) >> ba_StrVibration  [2])!= 0;
            CarPara_RoadEdge_gbl        = (((boolean)(car_code_value[ba_RoadEdge       [0]] & ba_RoadEdge      [1])) >> ba_RoadEdge      [2])!= 0;
            CarPara_MapLocator_gbl      = (((boolean)(car_code_value[ba_MapLocator     [0]] & ba_MapLocator    [1])) >> ba_MapLocator    [2])!= 0;
            CarPara_SonarF_gbl          = (((boolean)(car_code_value[ba_SonarF         [0]] & ba_SonarF        [1])) >> ba_SonarF        [2])!= 0;
            CarPara_DMS_gbl             = (((boolean)(car_code_value[ba_DMS            [0]] & ba_DMS           [1])) >> ba_DMS           [2])!= 0;
            CarPara_ALC_gbl             = (((boolean)(car_code_value[ba_ALC            [0]] & ba_ALC           [1])) >> ba_ALC           [2])!= 0;
            CarPara_NotHoldingALK_gbl   = (((boolean)(car_code_value[ba_NotHoldingALK  [0]] & ba_NotHoldingALK [1])) >> ba_NotHoldingALK [2])!= 0;
            CarPara_AutoDepart_gbl      = (((boolean)(car_code_value[ba_AutoDepart     [0]] & ba_AutoDepart    [1])) >> ba_AutoDepart    [2])!= 0;
            CarPara_iACC_gbl            = (((boolean)(car_code_value[ba_iACC           [0]] & ba_iACC          [1])) >> ba_iACC          [2])!= 0;
            CarPara_SlowBfCrv_gbl       =  (((boolean)(car_code_value[ba_SlowBfCrv     [0]] & ba_SlowBfCrv     [1])) >> ba_SlowBfCrv     [2])!= 0;
            
            /* version 2-0: */
            CarPara_LDPType_gbl         = (((boolean)(car_code_value[ba_LDP            [0]] & ba_LDP           [1])) >> ba_LDP           [2])!= 0;
            CarPara_LKSType_gbl         = (((boolean)(car_code_value[ba_LKS            [0]] & ba_LKS           [1])) >> ba_LKS           [2])!= 0;
            /* version 2-0: */

            CarPara_StrTouchSensor_gbl  = (((boolean)(car_code_value[ba_StrTouchSensor [0]] & ba_StrTouchSensor[1])) >> ba_StrTouchSensor[2]) != 0;
            CarPara_MonoCam_gbl         = (((boolean)(car_code_value[ba_MonoCam        [0]] & ba_MonoCam       [1])) >> ba_MonoCam       [2]) != 0;
            CarPara_LightCtrl_gbl       = (((boolean)(car_code_value[ba_LightCtrl      [0]] & ba_LightCtrl     [1])) >> ba_LightCtrl     [2]) != 0;
            CarPara_HandlePos_gbl       =  ((uint8)(car_code_value[ba_HandlePos        [0]] & ba_HandlePos     [1])) >> ba_HandlePos     [2]     ;

            /* version 2-0: */
            CarPara_EPS_gbl             =  ((uint8)(car_code_value[ba_EPS              [0]] & ba_EPS           [1])) >> ba_EPS           [2]     ;
            GPF_gbl                     = (((boolean)(car_code_value[ba_GPF            [0]] & ba_GPF           [1])) >> ba_GPF           [2])!= 0;
            /* version 2-0: */

            /* version 2-10 */
            CarPara_PedAirbag_gbl       = (((boolean)(car_code_value[ba_PedAirbag      [0]] & ba_PedAirbag     [1])) >> ba_PedAirbag     [2])!= 0;
            ftMVS_gbl                   = (((boolean)(car_code_value[ba_ftMVS          [0]] & ba_ftMVS         [1])) >> ba_ftMVS         [2])!= 0;
            /* version 2-10 */

            /* CarPara_CarCode */
            /* CarVariant */
            CarPara_VariantPTType_gbl       = (uint8)k_CAR_SELECT_SPEC[idx_variant_pt]            [CarCode_index];
            CarPara_VariantTMType_gbl       = (uint8)k_CAR_SELECT_SPEC[idx_variant_tm]            [CarCode_index];
            CarPara_VariantForLKS_gbl       = (uint8)k_CAR_SELECT_SPEC[idx_variant_for_lks]       [CarCode_index];
            CarPara_VariantForLDP_gbl       = (uint8)k_CAR_SELECT_SPEC[idx_variant_for_ldp]       [CarCode_index];
            CarPara_VariantPadTypeF_gbl     = (uint8)k_CAR_SELECT_SPEC[idx_VariantPadTypeF]       [CarCode_index];
            CarPara_VariantPadTypeR_gbl     = (uint8)k_CAR_SELECT_SPEC[idx_VariantPadTypeR]       [CarCode_index];

            /* MovingTire */
            CarPara_MovingTire_R_gbl        = (float32)k_CAR_SELECT_SPEC[idx_moving_tire_r]       [CarCode_index];

            /* Vehicle_Weight */
            CarPara_Vehicle_Weight_gbl      = (float32)k_CAR_SELECT_SPEC[idx_vehicle_weight]      [CarCode_index];

            /* RS_degree_C  */
            CarPara_RS_0degree_C_gbl        = (float32)k_CAR_SELECT_SPEC[idx_rs_0degree_c]        [CarCode_index];
            CarPara_RS_1degree_C_gbl        = (float32)k_CAR_SELECT_SPEC[idx_rs_1degree_c]        [CarCode_index];
            CarPara_RS_2degree_C_gbl        = (float32)k_CAR_SELECT_SPEC[idx_rs_2degree_c]        [CarCode_index];

            /* AownHosei_fact */
            CarPara_AownCorrectFact_gbl     = (float32)k_CAR_SELECT_SPEC[idx_aown_CorrectRate]    [CarCode_index];

            /* VSO_Error */
            CarPara_VSOError_gbl            = (float32)k_CAR_SELECT_SPEC[idx_vso_error]           [CarCode_index];

            /* Veh */
            CarPara_Vehicle_Width_gbl       = (float32)k_CAR_SELECT_SPEC[idx_vehicle_width]       [CarCode_index];

            /* EngRev */
            CarPara_EngRevLimMax_gbl        = (float32)k_CAR_SELECT_SPEC[idx_eng_rev_min_lim]     [CarCode_index];

            /* GearRatio */
            CarPara_GearRatioMax_gbl        = (float32)k_CAR_SELECT_SPEC[idx_gear_ratio_max]      [CarCode_index];
            CarPara_GearRatioMin_gbl        = (float32)k_CAR_SELECT_SPEC[idx_gear_ratio_min]      [CarCode_index];
            CarPara_FinalGear_gbl           = (float32)k_CAR_SELECT_SPEC[idx_final_gear]          [CarCode_index];

            /* CarSpec */
            CarPara_SpecLKSDefault_gbl      = (boolean)k_CAR_SELECT_SPEC[idx_lks_default]         [CarCode_index];
            CarPara_SpecALKSwMemory_gbl     = (uint8)k_CAR_SELECT_SPEC[idx_SpecALKSwMemory]       [CarCode_index];
            CarPara_VDCSwForPCSpec_gbl      = (uint8)k_CAR_SELECT_SPEC[idx_vdc_offmode]           [CarCode_index];
            CarPara_CCType_gbl              = (uint8)k_CAR_SELECT_SPEC[idx_spec_cc_type]          [CarCode_index];
            CarPara_CCSwType_gbl            = (uint8)k_CAR_SELECT_SPEC[idx_spec_cc_sw_type]       [CarCode_index];
            f_CarPara_SpecManualMode_gbl    = (boolean)k_CAR_SELECT_SPEC[idx_spec_manual_mode]    [CarCode_index];
            CarPara_DistRWheel2Vdc_gbl      = (float32)k_CAR_SELECT_SPEC[idx_distance_rw_2_vdc]   [CarCode_index];

            /* DidSelect */
            CarPara_DidSelect_gbl           = (uint8)k_CAR_SELECT_SPEC[idx_did_select_tbl]        [CarCode_index];
            
            /* MtrCorrectTbl */
            mtr_correcttble                 = (uint8)k_CAR_SELECT_SPEC[idx_add_mtr_correct]       [CarCode_index];
            col = sizeof(k_MtrCorrectTbl[0]) / sizeof(k_MtrCorrectTbl[0][0]);
            for (uint16 i = 0; i < col; i++)
            {
                CarPara_MeterCorrectionTbl_gbl[i] = (float32)k_MtrCorrectTbl[mtr_correcttble][i];
            }
            
            /* SwLongPress */
            CarPara_SwLongPressThrTime_gbl  = (float32)k_CAR_SELECT_SPEC[idx_nagaoshi_thr]        [CarCode_index];

            /* Req_brk_press */
            CarPara_ReqBrakePressBase_F_gbl = (float32)k_CAR_SELECT_SPEC[idx_ReqBrkPrsBaseF]      [CarCode_index];
            CarPara_ReqBrakePressBase_R_gbl = (float32)k_CAR_SELECT_SPEC[idx_ReqBrkPrsBaseR]      [CarCode_index];
            CarPara_ReqBrakePressBase_C_gbl = (float32)k_CAR_SELECT_SPEC[idx_ReqBrkPrsBaseCaliper][CarCode_index];

            /* Vel_coefficient */
            CarPara_VelCoefficientFront_gbl = (float32)k_CAR_SELECT_SPEC[idx_VelCoefficientF]     [CarCode_index];
            CarPara_VelCoefficientRear_gbl  = (float32)k_CAR_SELECT_SPEC[idx_VelCoefficientR]     [CarCode_index];

            /* EquipCustom */
            f_CarPara_R79Spec_gbl           = (boolean)k_CAR_SELECT_SPEC[idx_SpecR79]             [CarCode_index];

            /* EpsCtrlTrqALKLim */
            CarPara_EpsCtrlTrqALKLim_gbl    = (float32)k_CAR_SELECT_SPEC[idx_EpsCtrlTrqALKLim]    [CarCode_index];

            /* CustomACC */
            if (k_SpecAcclCustDefault != i_CUSTOM_ACC_A_NO_EQUIP)
            {
                CarPara_AcclCustType_gbl    = ON;
            }
            else
            {
                CarPara_AcclCustType_gbl    = OFF;
            }
            /* CAMtoFtAxle */
            CarPara_CAMtoFtAxle_gbl         = (float32)k_CAR_SELECT_SPEC[idx_CarPara_CAMtoFtAxle] [CarCode_index];
            CarPara_CAMtoRrAxle_gbl         = (float32)k_CAR_SELECT_SPEC[idx_CarPara_CAMtoRrAxle] [CarCode_index];

            /* version 2-0: */
            CarPara_AES_gbl                 = (boolean)k_CAR_SELECT_SPEC[idx_AES]                 [CarCode_index];
            /* version 2-0: */

            /* version 2-10 */
            CarPara_TJABlue_gbl             = (boolean)k_CAR_SELECT_SPEC[idx_TJABlue]             [CarCode_index];
            CarPara_SpecLDPSwMemory_gbl     = (uint8)k_CAR_SELECT_SPEC[idx_SpecLDPSwMemory]       [CarCode_index];
            CarPara_Variant_for_AES_gbl     = (uint8)k_CAR_SELECT_SPEC[idx_variant_for_aes]       [CarCode_index];
            /* version 2-10 */

            
            /* CarPara_SpecTCM */
            row = sizeof(k_TCM_CAR_CODE_INDEX) / sizeof(k_TCM_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (CarPara_TCM_gbl == k_TCM_CAR_CODE_INDEX[i])
                {
                    CarPara_TCM_index = (uint8)i;
                    break;
                }
            }
            TCMType_gbl = (uint8)k_TCM_SELECT_SPEC[CarPara_TCM_index];

            /* CarPara_Market_gbl */
            row = sizeof(k_MARKET_CAR_CODE_INDEX) / sizeof(k_MARKET_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (CarPara_Market_gbl == k_MARKET_CAR_CODE_INDEX[i]) 
                {
                    CarPara_Market_index = (uint8)i;
                    break;
                }
            }
            BrkLamp_index                 = (uint8)k_MARKET_SELECT_SPEC[idx_BRK_LAMP_type]          [CarPara_Market_index];
            set_BrkLamp(BrkLamp_index);
            
            /* TgtAMap */
            TgtAMap_index                 = (uint8)k_MARKET_SELECT_SPEC[idx_TGT_A_type]             [CarPara_Market_index];
            set_TgtATrackingSp(TgtAMap_index);
            set_TgtAConstSp(TgtAMap_index);
            set_TgtASetSpAcc(TgtAMap_index);
            set_TgtASetSpAccConst(TgtAMap_index);
            set_TgtASetSpMsa(TgtAMap_index);
            
            /* AccSp */
            CarPara_ACCSetSpInitMaxKph_gbl    = k_MARKET_SELECT_SPEC[idx_acc_set_spd_max]           [CarPara_Market_index];
            CarPara_ACCSetSpInitMinKph_gbl    = k_MARKET_SELECT_SPEC[idx_acc_set_spd_min]           [CarPara_Market_index];
            CarPara_ACCSetSpInitMaxMph_gbl    = k_MARKET_SELECT_SPEC[idx_acc_set_spd_max_mile]      [CarPara_Market_index];
            CarPara_ACCSetSpInitMinMph_gbl    = k_MARKET_SELECT_SPEC[idx_acc_set_spd_min_mile]      [CarPara_Market_index];
            CarPara_ACCCtrlSpInitMax_gbl      = k_MARKET_SELECT_SPEC[idx_acc_cont_spd_max]          [CarPara_Market_index];
            CarPara_ACCCtrlSpInitMin_gbl      = k_MARKET_SELECT_SPEC[idx_acc_cont_spd_min]          [CarPara_Market_index];

            /* CcSp */
            CarPara_CCSetSpInitMaxKph_gbl     = k_MARKET_SELECT_SPEC[idx_cc_set_spd_max]            [CarPara_Market_index];
            CarPara_CCSetSpInitMinKph_gbl     = k_MARKET_SELECT_SPEC[idx_cc_set_spd_min]            [CarPara_Market_index];
            CarPara_CCSetSpInitMaxMph_gbl     = k_MARKET_SELECT_SPEC[idx_cc_set_spd_max_mile]       [CarPara_Market_index];
            CarPara_CCSetSpInitMinMph_gbl     = k_MARKET_SELECT_SPEC[idx_cc_set_spd_min_mile]       [CarPara_Market_index];
            CarPara_CCCtrlSpInitMax_gbl       = k_MARKET_SELECT_SPEC[idx_cc_cont_spd_max]           [CarPara_Market_index];
            CarPara_CCCtrlSpInitMin_gbl       = k_MARKET_SELECT_SPEC[idx_cc_cont_spd_min]           [CarPara_Market_index];

            /* Dist_warn */
            CarPara_DistWarnSpMax_gbl         = k_MARKET_SELECT_SPEC[idx_dis_kh_spd_max]            [CarPara_Market_index];

            /* PCSpd */
            CarPara_PCSpMax_gbl               = k_MARKET_SELECT_SPEC[idx_pc_spd_max]                [CarPara_Market_index];

            /* PCBa */
            CarPara_PCBAMaxVownOff2On_gbl     = k_MARKET_SELECT_SPEC[idx_pcba_max_on_by_vown]       [CarPara_Market_index];
            CarPara_PCBAMaxVownOn2Off_gbl     = k_MARKET_SELECT_SPEC[idx_pcba_max_off_by_vown]      [CarPara_Market_index];
            CarPara_PCBAMaxVrefOff2On_gbl     = k_MARKET_SELECT_SPEC[idx_pcba_max_on_by_vrel_pc]    [CarPara_Market_index];
            CarPara_PCBAMaxVrefOn2Off_gbl     = k_MARKET_SELECT_SPEC[idx_pcba_max_off_by_vrel_pc]   [CarPara_Market_index];

            /* FMWSp */
            CarPara_FMWSpMax_gbl              = k_MARKET_SELECT_SPEC[idx_fmw_spd_max]               [CarPara_Market_index];               

            /* PCMarket */
            PCMarket_index                    = (uint8)k_MARKET_SELECT_SPEC[idx_PC_A_type]          [CarPara_Market_index];
            set_PCMarket(PCMarket_index);

            /* LdpSp */
            CarPara_LDP_VOffMax_gbl           = k_MARKET_SELECT_SPEC[idx_ldp_v_max_off]             [CarPara_Market_index];
            CarPara_LDP_VOffMin_gbl           = k_MARKET_SELECT_SPEC[idx_ldp_v_min_off]             [CarPara_Market_index];
            CarPara_LDP_VOnMax_gbl            = k_MARKET_SELECT_SPEC[idx_ldp_v_max_on]              [CarPara_Market_index];
            CarPara_LDP_VOnMin_gbl            = k_MARKET_SELECT_SPEC[idx_ldp_v_min_on]              [CarPara_Market_index];
        
            /* AlkLim */
            f_CarPara_ALK_LimitWarn_gbl       = (boolean)k_MARKET_SELECT_SPEC[idx_ALK_limit_kh]     [CarPara_Market_index];

            /* OneSideLane */
            CarPara_OnesideLaneMode_gbl       = (boolean)k_MARKET_SELECT_SPEC[idx_oneside_lane_mode][CarPara_Market_index];

            /* FMWMaxGType */
            FMWMaxGType_index             = (uint8)k_MARKET_SELECT_SPEC[idx_FMWMaxGType]            [CarPara_Market_index];
            set_FMWMaxGCont(FMWMaxGType_index);
            set_FMWMaxGVown(FMWMaxGType_index);
            set_FMWMaxGR(FMWMaxGType_index);

            /* MarketInfo */
            CarPara_MarketInfo_gbl            = (uint8)k_MARKET_SELECT_SPEC[idx_MARKETInfo][CarPara_Market_index];

            /* OtherInfo */
            CarPara_SpecSignalType_gbl        = (uint8)k_MARKET_SELECT_SPEC[idx_spec_signal_type]   [CarPara_Market_index];
            CarPara_SpecFMWRType_gbl          = (uint8)k_MARKET_SELECT_SPEC[idx_spec_fmw_r_type]    [CarPara_Market_index];
            CarPara_SpecFMWRSpdLimDefault_gbl = (uint8)k_MARKET_SELECT_SPEC[idx_spec_fmw_r_spd_lim_default][CarPara_Market_index];

            /* EquipCustom */
            f_CarPara_SpecCustTypeRoad_gbl    = (boolean)k_MARKET_SELECT_SPEC[idx_RoadTypeCustEquip][CarPara_Market_index];
            CarPara_RoadTypeDefault_gbl       = (uint8)k_MARKET_SELECT_SPEC[idx_RoadTypeDefault]    [CarPara_Market_index];

            /* ALKCTJAVown */
            CarPara_LKSVownOffMax_gbl         = k_MARKET_SELECT_SPEC[idx_LKSVownMaxOff]             [CarPara_Market_index];
            CarPara_LKSVownOffMin_gbl         = k_MARKET_SELECT_SPEC[idx_LKSVownMinOff]             [CarPara_Market_index];
            CarPara_LKSVownOnMax_gbl          = k_MARKET_SELECT_SPEC[idx_LKSVownMaxOn]              [CarPara_Market_index];
            CarPara_LKSVownOnMin_gbl          = k_MARKET_SELECT_SPEC[idx_LKSVownMinOn]              [CarPara_Market_index];

            /* RoadEdge */
            CarPara_RoadEdgeSpMaxOff_gbl      = k_MARKET_SELECT_SPEC[(uint8)idx_roadedge_v_max_off] [CarPara_Market_index];
            CarPara_RoadEdgeSpMinOff_gbl      = k_MARKET_SELECT_SPEC[(uint8)idx_roadedge_v_min_off] [CarPara_Market_index];
            CarPara_RoadEdgeSpMaxOn_gbl       = k_MARKET_SELECT_SPEC[(uint8)idx_roadedge_v_max_on]  [CarPara_Market_index];
            CarPara_RoadEdgeSpMinOn_gbl       = k_MARKET_SELECT_SPEC[(uint8)idx_roadedge_v_min_on]  [CarPara_Market_index];

            /* NotHoldingALK */
            CarPara_NotHoldingSpMaxOff_gbl    = k_MARKET_SELECT_SPEC[(uint8)idx_b2_v_max_off]       [CarPara_Market_index];
            CarPara_NotHoldingSpMinOff_gbl    = k_MARKET_SELECT_SPEC[(uint8)idx_b2_v_min_off]       [CarPara_Market_index];
            CarPara_NotHoldingSpMaxOn_gbl     = k_MARKET_SELECT_SPEC[(uint8)idx_b2_v_max_on]        [CarPara_Market_index];
            CarPara_NotHoldingSpMinOn_gbl     = k_MARKET_SELECT_SPEC[(uint8)idx_b2_v_min_on]        [CarPara_Market_index];

            /* ALC */
            CarPara_ALCSpMaxOff_gbl           = k_MARKET_SELECT_SPEC[(uint8)idx_alc_v_max_off]      [CarPara_Market_index];
            CarPara_ALCSpMinOff_gbl           = k_MARKET_SELECT_SPEC[(uint8)idx_alc_v_min_off]      [CarPara_Market_index];
            CarPara_ALCSpMaxOn_gbl            = k_MARKET_SELECT_SPEC[(uint8)idx_alc_v_max_on]       [CarPara_Market_index];
            CarPara_ALCSpMinOn_gbl            = k_MARKET_SELECT_SPEC[(uint8)idx_alc_v_min_on]       [CarPara_Market_index];
            
            /* FMWR_CANJudge */
            if (CarPara_SpecFMWRType_gbl == iSPEC_FMW_R_NONE)
            {
                CarPara_SpecFMWRType_Can_gbl = OFF;
            }
            else
            {
                CarPara_SpecFMWRType_Can_gbl = ON;
            }
            
            /* CarPara_PU_ENG */
            row = sizeof(k_PT_CAR_CODE_INDEX) / sizeof(k_PT_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (CarPara_PU_gbl == k_PT_CAR_CODE_INDEX[i]) 
                {
                    CarPara_PU_index = i;
                    break;
                }
            }
            CarPara_EgTrq_Max_gbl = k_PT_SELECT_SPEC[CarPara_PU_index];

            /* CarPara_SpecISS */
            row = sizeof(k_ISS_CAR_CODE_INDEX) / sizeof(k_ISS_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (CarPara_ISS_gbl == (boolean)k_ISS_CAR_CODE_INDEX[i])
                {
                    CarPara_ISS_index = i;
                    break;
                }
            }
            f_CarPara_SpecISSEquip_gbl = (boolean)k_ISS_SELECT_SPEC[CarPara_ISS_index];

            if ((f_CarPara_SpecISSEquip_gbl == ON) && (f_CarPara_EquipEPB_gbl == ON))
            {
                CarPara_ISSRbgUprt_gbl = k_ISS_RBG_UPRT;
            }
            else
            {
                CarPara_ISSRbgUprt_gbl = ((float32)SINT16MIN) * 0.001F;
            }

            /* ALKSpecJudge */
            SASSpecNone_gbl = get_ALKSpecJudge();
            
            /* THIS PART IS AUTOMATICALLY CREATED USING Map_ID.py */
            /* DO NOT MODIFY */
            /* CarVariant */
            set_CarPara_VariantPTType(CarPara_VariantPTType_gbl);     /* iEG_CAR_PARA    */
            set_CarPara_VariantTMType(CarPara_VariantTMType_gbl);     /* iTM_CAR_PARA    */
            set_CarPara_VariantForLKS(CarPara_VariantForLKS_gbl);     /* iLKS_CAR_PARA   */
            set_CarPara_VariantForLDP(CarPara_VariantForLDP_gbl);     /* iLDP_CAR_PARA   */    
            set_CarPara_VariantPadTypeF(CarPara_VariantPadTypeF_gbl); /* iPAD_CAR_PARA_F */
            set_CarPara_VariantPadTypeR(CarPara_VariantPadTypeR_gbl); /* iPAD_CAR_PARA_R */
            set_CarPara_VariantForAES(CarPara_AES_gbl);               /* iAES_CAR_PARA_R */
            /* ------------------------------------------------- */

            /* CorrectActualSpeed */
            /* Vel_coefficient   TODO why mulitply the value to ZERO? */
            ActualSpeedCorrectF = ((float32)ZERO * CarPara_VelCoefficientFront_gbl) / 100.F;
            ActualSpeedCorrectR = ((float32)ZERO * CarPara_VelCoefficientRear_gbl)  / 100.F;

            /* Req_brk_press */
            /* GetBrakePadU for Front */
            row_BrakePad_uMAP_G = sizeof(BrakePad_uMAP_G) / sizeof(BrakePad_uMAP_G[0]);
            row_BrakePad_uMAP_V = sizeof(BrakePad_uMAP_V) / sizeof(BrakePad_uMAP_V[0]);
            row_G_minus1 = row_BrakePad_uMAP_G - (uint16)1;
            row_V_minus1 = row_BrakePad_uMAP_V - (uint16)1;
            set_BrakePad_uMAP(BrakePad_uMAP_G, (uint16)row_BrakePad_uMAP_G, (float32)ZERO      , &ReqBrkG_F_irx           , &ReqBrkG_F_fraction);
            set_BrakePad_uMAP(BrakePad_uMAP_V, (uint16)row_BrakePad_uMAP_V, ActualSpeedCorrectF, &ActualSpeedCorrect_F_irx, &ActualSpeedCorrect_F_fraction);
            
            /* PreLookUptable PRE -LOGIC -POST */
            set_LookupTable_2D_Pre(&ReqBrkG_F_irx,
                                   &row_G_minus1,
                                   &ActualSpeedCorrect_F_irx,
                                   &row_V_minus1,
                                   &ReqBrkG_F_out,
                                   &ActualSpeedCorrect_F_out);
            /* LOGIC */
            set_LookupTable_2D_Logic(&ReqBrkG_F_out,
                                     &ActualSpeedCorrect_F_out,
                                     &ReqBrkG_F_irx,
                                     &ReqBrkG_F_fraction,
                                     &ActualSpeedCorrect_F_irx,
                                     &ActualSpeedCorrect_F_fraction,
                                     CarPara_BrakePadUFrontMap_gbl,
                                     &out_F_indexValue,
                                     &out1_F,
                                     &out2_F,
                                     &out3_F);

            /* POST */
            set_LookupTable_2D_Post(&out_F_indexValue,
                                    &out1_F,
                                    &out2_F,
                                    &out3_F,
                                    &ReqBrkG_F_fraction,
                                    &ActualSpeedCorrect_F_fraction,
                                    &BrakePadUF);

            /* Rear */
            set_BrakePad_uMAP(BrakePad_uMAP_G, (uint16)row_BrakePad_uMAP_G, (float32)ZERO      , &ReqBrkG_FR_irx           , &ReqBrkG_FR_fraction);
            set_BrakePad_uMAP(BrakePad_uMAP_V, (uint16)row_BrakePad_uMAP_V, ActualSpeedCorrectR, &ActualSpeedCorrect_FR_irx, &ActualSpeedCorrect_FR_fraction);

            /* PreLookUptable */
            set_LookupTable_2D_Pre(&ReqBrkG_FR_irx,
                                   &row_G_minus1,
                                   &ActualSpeedCorrect_FR_irx,
                                   &row_V_minus1,
                                   &ReqBrkG_FR_out,
                                   &ActualSpeedCorrect_FR_out);
            /* LOGIC */
            set_LookupTable_2D_Logic(&ReqBrkG_FR_out,
                                     &ActualSpeedCorrect_FR_out,
                                     &ReqBrkG_FR_irx,
                                     &ReqBrkG_FR_fraction,
                                     &ActualSpeedCorrect_FR_irx,
                                     &ActualSpeedCorrect_FR_fraction,
                                     CarPara_BrakePadURearMap_gbl,
                                     &out_FR_indexValue,
                                     &out1_FR,
                                     &out2_FR,
                                     &out3_FR);

            /* POST */
            set_LookupTable_2D_Post(&out_FR_indexValue,
                                    &out1_FR,
                                    &out2_FR,
                                    &out3_FR,
                                    &ReqBrkG_FR_fraction,
                                    &ActualSpeedCorrect_FR_fraction,
                                    &BrakePadUR);

            /* GetBaseFact */
            BrakePadUR_out = (CarPara_ReqBrakePressBase_F_gbl * (BrakePadUR * 0.01F)) +
                             (CarPara_ReqBrakePressBase_R_gbl * (BrakePadUF * 0.01F));
            
            if(BrakePadUR_out <= ZERO)
            {
                switch_BrakePad = ZERO;
            }
            else
            {
                ReqBrkPrsBase_out = 2.F * CarPara_ReqBrakePressBase_F_gbl * CarPara_ReqBrakePressBase_R_gbl;
                if (BrakePadUR_out != 0.F)
                {
                    switch_BrakePad = ReqBrkPrsBase_out / BrakePadUR_out;
                }
                else
                {
                    if (ReqBrkPrsBase_out < 0.F)
                    {
                        switch_BrakePad = -3.402823466e+38F;
                    }
                    else{
                        switch_BrakePad = 3.402823466e+38F;
                    }
                }
            }
            if (switch_BrakePad < i01_CHG_REQ_BRKPRS)
            {
                BaseFact = switch_BrakePad;
            }
            else
            {
                BaseFact = i01_CHG_REQ_BRKPRS;
            }

            /* GetChgReqBrkPrsTbl */
            iterator = 2;
            ChgReqBrkprsNoClpr = 0.F;
            ChgReqBrkPrsTbl[0] = 0.F;

            ChgReqBrkPrsTbl[1] = CarPara_ReqBrakePressBase_C_gbl + (BaseFact / 100.F);

            row_ChgReqBrkPrsTbl = sizeof(ChgReqBrkPrsTbl) / sizeof(ChgReqBrkPrsTbl[0]);
            while (iterator < row_ChgReqBrkPrsTbl)
            {
                ChgReqBrkprsNoClpr        = ChgReqBrkprsNoClpr + BaseFact;
                ChgReqBrkPrsTbl[iterator] = ChgReqBrkprsNoClpr + CarPara_ReqBrakePressBase_C_gbl;
                iterator = iterator + 1;
            }
            for (uint8 i = 0; i < row_ChgReqBrkPrsTbl; i++)
            {
                CarPara_ReqBrkPrsTbl_gbl[i] = ChgReqBrkPrsTbl[i];
            }

            /* CalcISSReqG */
            row_KOBAI_G = sizeof(KOBAI_G) / sizeof(KOBAI_G[0]);
            row_K_minus1 = row_KOBAI_G - (uint16)1;
            set_BrakePad_uMAP(KOBAI_G, (uint16)row_KOBAI_G, (float32)ZERO, &ReqBrkG_K_irx, &ReqBrkG_K_fraction);
            set_LookupTable_1D_Pre(&ReqBrkG_K_irx , &row_K_minus1        , &ReqBrkG_K_out           , &ReqBrkG_K_out_cond);
            /* temp table to remove const in STOPHOJI_ISS_SLOPE_MIN */
            row = sizeof(STOPHOJI_ISS_SLOPE_MIN_temp) / sizeof(STOPHOJI_ISS_SLOPE_MIN_temp[0]);
            for (uint16 i = 0; i < row; i++)
            {
                STOPHOJI_ISS_SLOPE_MIN_temp[i] = STOPHOJI_ISS_SLOPE_MIN[i];
            }
            set_LookupTable_1D_Logic(&ReqBrkG_K_out,
                                     &ReqBrkG_K_out_cond,
                                     &ReqBrkG_K_irx,
                                     &ReqBrkG_K_fraction,
                                     STOPHOJI_ISS_SLOPE_MIN_temp,
                                     &out_ISSSlopeGradePrs);

            /* UncertaintyAxis */
            uncertaintyAxisSelector[0] = ChgReqBrkPrsTbl[(uint8)ONE_W   - 1];
            uncertaintyAxisSelector[1] = ChgReqBrkPrsTbl[(uint8)TWO_W   - 1];
            uncertaintyAxisSelector[2] = ChgReqBrkPrsTbl[(uint8)THREE_W - 1];

            uncertaintyAxisLogicalOpt[0] = ((uncertaintyAxisSelector[0] < uncertaintyAxisSelector[1]) &&
                                            (uncertaintyAxisSelector[1] < uncertaintyAxisSelector[2]));

            uncertaintyAxisLogicalOpt[1] = uncertaintyAxisLogicalOpt[0] || ((uncertaintyAxisSelector[0] > uncertaintyAxisSelector[1]) &&
                                                                            (uncertaintyAxisSelector[1] > uncertaintyAxisSelector[2]));

            if (!(uncertaintyAxisLogicalOpt[1]))
            {
                UncertaintyAxisMerge = REQ_BRK_G[(uint8)ONE_W - 1];
            }

            if ((uncertaintyAxisLogicalOpt[1]))
            {
                /* NormalAxis */
                row_REQ_BRK_G = sizeof(REQ_BRK_G) / sizeof(REQ_BRK_G[0]);
                if (uncertaintyAxisLogicalOpt[0])
                {
                    for (uint8 i = 0; i < row_REQ_BRK_G; i++)
                    {
                        InvertedTable_out[i] = REQ_BRK_G[i];
                    }
                }
                else
                {
                    for (uint8 i = 0; i < row_REQ_BRK_G; i++)
                    {
                        InvertedTable_out[i] = REQ_BRK_G[(row_REQ_BRK_G - 1) - i];
                    }
                }
                
                if (uncertaintyAxisLogicalOpt[0])
                {
                    for (uint8 i = 0; i < row_ChgReqBrkPrsTbl; i++)
                    {
                        InvertedAxis_out[i] = ChgReqBrkPrsTbl[i];
                    }
                }
                else
                {
                    for (uint8 i = 0; i < row_ChgReqBrkPrsTbl; i++)
                    {
                        InvertedAxis_out[i] = ChgReqBrkPrsTbl[(row_ChgReqBrkPrsTbl - 1) - i];
                    }
                }

                set_BrakePad_uMAP((const float32 *)InvertedAxis_out,
                                  (uint16)row_REQ_BRK_G,
                                  out_ISSSlopeGradePrs,
                                  &InvertedAxis_irx,
                                  &InvertedAxis_fraction);
                row_REQ_BRK_G_minus1 = row_REQ_BRK_G - (uint16)1;
                set_LookupTable_1D_Pre(&InvertedAxis_irx, &row_REQ_BRK_G_minus1, &Inverted_out, &Inverted_out_cond);
                set_LookupTable_1D_Logic(&Inverted_out,
                                         &Inverted_out_cond,
                                         &InvertedAxis_irx,
                                         &InvertedAxis_fraction,
                                         InvertedTable_out,
                                         &UncertaintyAxisMerge);

                CarPara_StpHldIssBase_gbl = UncertaintyAxisMerge;
            }
        }
 //       unit_delay = f_CarPara_Decided_gbl;
    }

    /* version 2-10: */
    static uint16 CUS_CarCode_index = 0;
    static uint8  CUS_CarPara_Market_index = 0;

    uint8 CUS_CarPara_Market = 0;
    uint16 CUS_car_code_value[16] = {0};

    if (f_CarCodeUpdate_gbl == ON)
    {
        boolean CUS_CarPara_CarCodeValue[16] = {0};
        /* CarCodeInitJudg */
        row = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);
        for (uint16 i = 0; i < row; i++)
        {
            CUS_CarPara_CarCodeValue[i] = (boolean)(CarCode[i] == gk_uc_Car_Type_default[i]);
        }
        if (is_equal(CUS_CarPara_CarCodeValue, row))
        {

            /* DEFAULT VALUE FOR CUS */
            CarPara_f_ALKMainSW_Variant_gbl           = ON;
            CarPara_f_SPEC_LKS_TYPE_Variant_gbl       = (boolean)ZERO != 0.F;
            CarPara_f_SpecLKSDefault_Variant_gbl      = (boolean)default_lks_default;
            CarPara_SpecFMWRSpdLimDefault_Variant_gbl = (uint8)default_spec_fmw_r_spd_lim_default;
            CarPara_SpecFMWRType_Variant_gbl          = (uint8)default_spec_fmw_r_type;
            CarPara_f_SpecCustomACCLType_Variant_gbl  = ON;
            CarPara_SpecAcclCustomDefault_Variant_gbl = k_SpecAcclCustDefault;
            CarPara_SpecCustomTypeRoad_Variant_gbl    = (boolean)default_RoadTypeCustEquip;
            CarPara_RoadTypeDefault_Variant_gbl       = (uint8)default_RoadTypeDefault;
            CarPara_f_SpecCustomTypeELK_Variant_gbl   = (boolean)ZERO != 0.F;
            CarPara_f_ELKSettingSEL_Variant_gbl       = OFF;
            CarPara_f_EquipELK_Variant_gbl            = (boolean)ZERO != 0.F;
            CarPara_f_SPEC_LDP_TYPE_Variant_gbl       = (boolean)ZERO != 0.F;
            CarPara_LDP_Default_Select_Variant_gbl    = (uint8)default_SpecLDPSwMemory;
            CarPara_f_Spec_STR_VIB_Variant_gbl        = (boolean)ZERO;
            CarPara_f_Spec_CURV_TOLLG_Variant_gbl     = (boolean)ZERO;
            CarPara_f_Spec_TSR_Variant_gbl            = (boolean)ZERO;
            CarPara_Spec_SIGNALSIGN_Variant_gbl       = (uint8)default_spec_signal_type;

            CarPara_f_VehCode_Update_gbl = OFF;
        }
        else
        {
            row = sizeof(k_CAR_CAR_CODE_INDEX) / sizeof(k_CAR_CAR_CODE_INDEX[0]);
            col = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);

            for (uint16 i = 0; i < row; i++)
            {
                for (uint16 j = 0; j < col; j++)
                {
                    CUS_CarPara_CarCodeValue[j] = (boolean)(CarCode[j] == (uint8)k_CAR_CAR_CODE_INDEX[i][j]);
                }
                /* if found stop the loop  */
                if (is_equal(CUS_CarPara_CarCodeValue, col))
                {
                    CUS_CarCode_index = i;
                    break;
                }
            }

            /* Set the array value to car_code_value from k_CAR_CAR_CODE_INDEX */
            row = sizeof(k_CAR_CAR_CODE_INDEX[0]) / sizeof(k_CAR_CAR_CODE_INDEX[0][0]);
            for (uint16 i = 0; i < row; i++)
            {
                CUS_car_code_value[i] = k_CAR_CAR_CODE_INDEX[CUS_CarCode_index][i];
            }

            CarPara_f_ALKMainSW_Variant_gbl           = ON;
            CarPara_f_SPEC_LKS_TYPE_Variant_gbl       = (((boolean)(CUS_car_code_value[ba_LKS   [0]] & ba_LKS   [1])) >> ba_LKS           [2])!= 0;
            CUS_CarPara_Market                        = ((uint8)   (CUS_car_code_value[ba_Market[0]] & ba_Market[1])) >> ba_Market        [2]     ;
            
            CarPara_f_SpecLKSDefault_Variant_gbl      = (boolean)k_CAR_SELECT_SPEC[idx_lks_default][CUS_CarCode_index];
            
            /* CarPara_Market_gbl */
            row = sizeof(k_MARKET_CAR_CODE_INDEX) / sizeof(k_MARKET_CAR_CODE_INDEX[0]);
            for (uint16 i = 0; i < row; i++)
            {
                if (CUS_CarPara_Market == k_MARKET_CAR_CODE_INDEX[i])
                {
                    CUS_CarPara_Market_index = (uint8)i;
                    break;
                }
            }

            CarPara_SpecFMWRSpdLimDefault_Variant_gbl = (uint8)k_MARKET_SELECT_SPEC[idx_spec_fmw_r_spd_lim_default][CUS_CarPara_Market_index];
            CarPara_SpecFMWRType_Variant_gbl          = (uint8)k_MARKET_SELECT_SPEC[idx_spec_fmw_r_type]           [CUS_CarPara_Market_index];

            /* CustomACC */
            if (k_SpecAcclCustDefault != (float32)i_CUSTOM_ACC_A_NO_EQUIP)
            {
                CarPara_f_SpecCustomACCLType_Variant_gbl = ON;
            }
            else
            {
                CarPara_f_SpecCustomACCLType_Variant_gbl = OFF;
            }

            CarPara_SpecAcclCustomDefault_Variant_gbl = k_SpecAcclCustDefault;
            CarPara_SpecCustomTypeRoad_Variant_gbl    = (boolean)k_MARKET_SELECT_SPEC[idx_RoadTypeCustEquip]       [CUS_CarPara_Market_index];
            CarPara_RoadTypeDefault_Variant_gbl       = (uint8)k_MARKET_SELECT_SPEC[idx_RoadTypeDefault]           [CUS_CarPara_Market_index];
            CarPara_f_SpecCustomTypeELK_Variant_gbl   = (((boolean)(CUS_car_code_value[ba_ELK      [0]] & ba_ELK           [1])) >> ba_ELK           [2])!= 0;
            CarPara_f_ELKSettingSEL_Variant_gbl       = OFF;
            CarPara_f_EquipELK_Variant_gbl            = (((boolean)(CUS_car_code_value[ba_ELK      [0]] & ba_ELK           [1])) >> ba_ELK           [2])!= 0;
            CarPara_f_SPEC_LDP_TYPE_Variant_gbl       = (((boolean)(CUS_car_code_value[ba_LDP      [0]] & ba_LDP           [1])) >> ba_LDP           [2])!= 0;
            CarPara_LDP_Default_Select_Variant_gbl    = (uint8)k_CAR_SELECT_SPEC[idx_SpecLDPSwMemory][CUS_CarCode_index];
            CarPara_f_Spec_STR_VIB_Variant_gbl        = ((uint8)(CUS_car_code_value[ba_StrVibration[0]] & ba_StrVibration  [1])) >> ba_StrVibration  [2]     ;
            CarPara_f_Spec_CURV_TOLLG_Variant_gbl     = ((uint8)(CUS_car_code_value[ba_SlowBfCrv   [0]] & ba_SlowBfCrv     [1])) >> ba_SlowBfCrv     [2]     ;
            CarPara_f_Spec_TSR_Variant_gbl            = ((uint8)(CUS_car_code_value[ba_TSR         [0]] & ba_TSR           [1])) >> ba_TSR           [2]     ;
            CarPara_Spec_SIGNALSIGN_Variant_gbl       = (uint8)k_MARKET_SELECT_SPEC[idx_spec_signal_type]           [CUS_CarPara_Market_index];


            CarPara_f_VehCode_Update_gbl = ON;
        }
    }
    else
    {
        CarPara_f_VehCode_Update_gbl = OFF;
    }
}

static void set_LookupTable_1D_Pre(uint16  *ReqBrkG_irx,
                                   uint16  *ReqBrkG_minus1,
                                   uint16  *ReqBrkG_irx_out,
                                   boolean *ReqBrkG_irx_cond)
{
    *ReqBrkG_irx_cond = *ReqBrkG_irx >= *ReqBrkG_minus1;

    if (*ReqBrkG_irx_cond)
    {
        *ReqBrkG_irx_out = *ReqBrkG_minus1;
    }
    else
    {
        *ReqBrkG_irx_out = (uint16)(*ReqBrkG_irx + 1);
    }
}
static void set_LookupTable_1D_Logic(uint16  *ReqBrkG_irx_out,
                                     boolean *ReqBrkG_irx_cond,
                                     uint16  *ReqBrkG_irx,
                                     float32 *ReqBrkG_fraction,
                                     float32 table_stopHoji[10],
                                     float32 *out)
{
    if (*ReqBrkG_irx_cond)
    {
        *out = table_stopHoji[*ReqBrkG_irx_out];
    }
    else
    {
        *out = ((table_stopHoji[*ReqBrkG_irx_out] - table_stopHoji[*ReqBrkG_irx]) * *ReqBrkG_fraction) +
                        table_stopHoji[*ReqBrkG_irx];
    }
}

static void set_LookupTable_2D_Pre(uint16 *ReqBrkG_irx,
                                   uint16 *ReqBrkG_minus1,
                                   uint16 *ActualSpeedCorrect_irx,
                                   uint16 *ActualSpeedCorrect_minus1,
                                   uint16 *ReqBrkG_irx_out,
                                   uint16 *ActualSpeedCorrect_irx_out)
{
    if (*ActualSpeedCorrect_irx >= *ActualSpeedCorrect_minus1)
    {
        *ActualSpeedCorrect_irx_out = *ActualSpeedCorrect_minus1;
    }
    else
    {
        *ActualSpeedCorrect_irx_out = (uint16)(*ActualSpeedCorrect_irx + 1);
    }

    if (*ReqBrkG_irx >= *ReqBrkG_minus1)
    {
        *ReqBrkG_irx_out = *ReqBrkG_minus1;
    }
    else
    {
        *ReqBrkG_irx_out = (uint16)(*ReqBrkG_irx + 1);
    }
}

static void set_LookupTable_2D_Logic(uint16  *ReqBrkG_irx_out,
                                     uint16  *ActualSpeedCorrect_irx_out,
                                     uint16  *ReqBrkG_irx,
                                     float32 *ReqBrkG_fraction,
                                     uint16  *ActualSpeedCorrect_irx,
                                     float32 *ActualSpeedCorrect_fraction,
                                     float32 table_uMap[7][6],
                                     float32 *out_indexValue,
                                     float32 *out1,
                                     float32 *out2,
                                     float32 *out3)
{
    *out2 = ((table_uMap[*ReqBrkG_irx_out][*ActualSpeedCorrect_irx] - table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx]) *
             *ReqBrkG_fraction) +
            table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx];

    *out3 = (((((table_uMap[*ReqBrkG_irx_out][*ActualSpeedCorrect_irx_out] -
                 table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx_out]) *
                *ReqBrkG_fraction) +
               table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx_out]) -
              *out2) *
             *ActualSpeedCorrect_fraction) +
            *out2;

    *out1 = ((table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx_out] - table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx]) *
             *ActualSpeedCorrect_fraction) +
            table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx];

    *out_indexValue = table_uMap[*ReqBrkG_irx][*ActualSpeedCorrect_irx];
}

static void set_LookupTable_2D_Post(float32 *out_indexValue,
                                    float32 *out1,
                                    float32 *out2,
                                    float32 *out3,
                                    float32 *ReqBrkG_fraction,
                                    float32 *ActualSpeedCorrect_fraction,
                                    float32 *out_y)
{
    if (*ActualSpeedCorrect_fraction <= 0.F)
    {
        if (*ReqBrkG_fraction <= 0.F)
        {
            *out_y = *out_indexValue;
        }
        else
        {
            *out_y = *out2;
        }
    }
    else
    {
        if (*ReqBrkG_fraction <= 0.F)
        {
            *out_y = *out1;
        }
        else
        {
            *out_y = *out3;
        }
    }
}

static void set_BrakePad_uMAP(const float32 *x_table, uint16 N, float32 x, uint16 *irx, float32 *fraction)
{
    /* SLLocal: Default storage class for local variables | Width: 16 */
    uint16 brakeuMap_cnt;

    /* Saturation. */
    if (x <= *x_table)
    {
        *irx = 0;
        *fraction = 0.F;
    }
    else
    {
        if (x >= x_table[N - 1])
        {
            *irx = N - 1;
            *fraction = 0.F;
        }
        else
        {
            brakeuMap_cnt = 0;

            /* Linear search, start low. */
            x_table++;
            while (x >= *(x_table++))
            {
                brakeuMap_cnt++;
            }
            x_table -= 2;
            *irx = brakeuMap_cnt;
            *fraction = (x - x_table[0]) / (x_table[1] - x_table[0]);
        }
    }
}

static boolean get_ALKSpecJudge(void)
{
    uint8 result[4] = {0};
    uint16 row = 0;
    row = sizeof(result) / sizeof(result[0]);
    if (CarPara_LDPType_gbl == ON)
    {
        result[0] = iALK_SPEC_ALKB;
    }
    else
    {
        result[0] = iALK_SPEC_NONE;
    }

    if (CarPara_LKSType_gbl == ON)
    {
        result[1] = iALK_SPEC_ALKC;
        result[3] = iALK_SPEC_TJA;
    }
    else
    {
        result[1] = iALK_SPEC_NONE;
        result[3] = iALK_SPEC_NONE;
    }

    if (f_CarPara_EquipELK_gbl == ON)
    {
        result[2] = iALK_SPEC_ELK;
    }
    else
    {
        result[2] = iALK_SPEC_NONE;
    }

    for(uint16 i = 0; i < row; i++)
    {
        ALKSpec_gbl = ALKSpec_gbl | result[i];
    }


    if (ALKSpec_gbl == iALK_SPEC_NONE)
    {
        return ON;
    }
    else
    {
        return OFF;
    }
	
}
static boolean is_equal(boolean *buf, uint16 size)
{
    for (uint16 i = 0; i < size; i++)
    {
        if (buf[i] != 0)
            continue;
        else
            return 0;
    }
    return (boolean)1;
}

static void set_BrkLamp(uint8 add_BRK_LAMP)
{
    uint16 row = 0;
    row = sizeof(BRK_LAMP_AOWN_OFFON_OTHERS) / sizeof(BRK_LAMP_AOWN_OFFON_OTHERS[0]);

    const float32 *ptroff2on;
    const float32 *ptron2off;
    switch (add_BRK_LAMP)
    {
        case (uint8)1:
        {
            ptroff2on = &BRK_LAMP_AOWN_OFFON_CHINA[0];
            ptron2off = &BRK_LAMP_AOWN_ONOFF_CHINA[0];
            break;
        }
        default:
        {
            ptroff2on = &BRK_LAMP_AOWN_OFFON_OTHERS[0];
            ptron2off = &BRK_LAMP_AOWN_ONOFF_OTHERS[0];
            break;
        }
    }

    for (uint16 i = 0; i < row; i++)
    {
        CarPara_AddBrkLampOff2On_gbl[i] = *(ptroff2on + i);
        CarPara_AddBrkLampOn2Off_gbl[i] = *(ptron2off + i);
    }
}

static void set_TgtATrackingSp(uint8 TgtAMap_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(TARGET_A_TUIBI_MAX_DOM) / sizeof(TARGET_A_TUIBI_MAX_DOM[0]);
    col = sizeof(TARGET_A_TUIBI_MAX_DOM[0]) / sizeof(TARGET_A_TUIBI_MAX_DOM[0][0]);

    if ((TgtAMap_index >= (uint8)1) && (TgtAMap_index <= (uint8)4))
    {
        ptr = &TARGET_A_TUIBI_MAX_FRGN[0][0];
    }
    else
    {
        ptr = &TARGET_A_TUIBI_MAX_DOM[0][0];
    }

    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_TgtATrackingSp_gbl[i][j] = *(ptr + i * col + j);
        }
    }
    
}

static void set_TgtAConstSp(uint8 TgtAMap_index)
{
    const float32 *ptr;
    uint16 row = sizeof(TARGET_A_TEISOKU_DOM) / sizeof(TARGET_A_TEISOKU_DOM[0]);

    if ((TgtAMap_index >= (uint8)1) && (TgtAMap_index <= (uint8)4))
    {
        ptr = &TARGET_A_TEISOKU_FRGN[0];
    }
    else
    {
        ptr = &TARGET_A_TEISOKU_DOM[0];
    }

    for (uint16 i = 0; i < row; i++)
    {
        CarPara_TgtAConstSp_gbl[i] = *(ptr + i);
    }

}

static void set_TgtASetSpAcc(uint8 TgtAMap_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(TARGET_A_FROM_SETSPD_DOM) / sizeof(TARGET_A_FROM_SETSPD_DOM[0]);
    col = sizeof(TARGET_A_FROM_SETSPD_DOM[0]) / sizeof(TARGET_A_FROM_SETSPD_DOM[0][0]);

    switch (TgtAMap_index)
    {
        case (uint8)1 : 
        {
            ptr = &TARGET_A_FROM_SETSPD_FRGN[0][0];
            break;
        }
        case (uint8)2:
        {
            ptr = &TARGET_A_FROM_SETSPD_US[0][0];
            break;
        }
        case (uint8)3:
        {
            ptr = &TARGET_A_FROM_SETSPD_AU[0][0];
            break;
        }
        case (uint8)4:
        {
            ptr = &TARGET_A_FROM_SETSPD_EU[0][0];
            break;
        }
        default : {
            ptr = &TARGET_A_FROM_SETSPD_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_TgtASetSpAcc_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_TgtASetSpAccConst(uint8 TgtAMap_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(TARGET_A_FROM_SETSPD_DOM) / sizeof(TARGET_A_FROM_SETSPD_DOM[0]);
    col = sizeof(TARGET_A_FROM_SETSPD_DOM[0]) / sizeof(TARGET_A_FROM_SETSPD_DOM[0][0]);

    switch (TgtAMap_index)
    {
        case (uint8)1:
        {
            ptr = &TARGET_A_FROM_SETSPD_FRGN[0][0];
            break;
        }
        case (uint8)2:
        {
            ptr = &TARGET_A_FROM_SETSPD_US[0][0];
            break;
        }
        case (uint8)3:
        {
            ptr = &TARGET_A_FROM_SETSPD_AU[0][0];
            break;
        }
        case (uint8)4:
        {
            ptr = &TARGET_A_FROM_SETSPD_EU[0][0];
            break;
        }
        default:
        {
            ptr = &TARGET_A_FROM_SETSPD_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_TgtASetSpAccConst_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_TgtASetSpMsa(uint8 TgtAMap_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(TARGET_A_FROM_MSASET_DOM) / sizeof(TARGET_A_FROM_MSASET_DOM[0]);
    col = sizeof(TARGET_A_FROM_MSASET_DOM[0]) / sizeof(TARGET_A_FROM_MSASET_DOM[0][0]);

    switch (TgtAMap_index)
    {
        case (uint8)1:
        {
            ptr = &TARGET_A_FROM_MSASET_FRGN[0][0];
            break;
        }
        case (uint8)2:
        {
            ptr = &TARGET_A_FROM_MSASET_US[0][0];
            break;
        }
        case (uint8)3:
        {
            ptr = &TARGET_A_FROM_MSASET_AU[0][0];
            break;
        }
        case (uint8)4:
        {
            ptr = &TARGET_A_FROM_MSASET_EU[0][0];
            break;
        }
        default:
        {
            ptr = &TARGET_A_FROM_MSASET_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_TgtASetSpMsa_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_PCMarket(uint8 PCMarket_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(FHI_PC_G_FOR_DOM) / sizeof(FHI_PC_G_FOR_DOM[0]);
    col = sizeof(FHI_PC_G_FOR_DOM[0]) / sizeof(FHI_PC_G_FOR_DOM[0][0]);

    switch (PCMarket_index)
    {
        case (uint8)1:
        {
            ptr = &FHI_PC_G_FOR_OVERSEA[0][0];
            break;
        }
        default:
        {
            ptr = &FHI_PC_G_FOR_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_PCMarketSpec_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_FMWMaxGCont(uint8 FMWMaxGType_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(FMW_MAX_G_FOR_CONT_DOM) / sizeof(FMW_MAX_G_FOR_CONT_DOM[0]);
    col = sizeof(FMW_MAX_G_FOR_CONT_DOM[0]) / sizeof(FMW_MAX_G_FOR_CONT_DOM[0][0]);

    switch (FMWMaxGType_index)
    {
        case (uint8)1:
        {
            ptr = &FMW_MAX_G_FOR_CONT[0][0];
            break;
        }
        default:
        {
            ptr = &FMW_MAX_G_FOR_CONT_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_FMWMaxGCont_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_FMWMaxGVown(uint8 FMWMaxGType_index)
{
    const float32 *ptr;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(FMW_MAX_G_VOWN_DOM) / sizeof(FMW_MAX_G_VOWN_DOM[0]);
    col = sizeof(FMW_MAX_G_VOWN_DOM[0]) / sizeof(FMW_MAX_G_VOWN_DOM[0][0]);

    switch (FMWMaxGType_index)
        {
        case (uint8)1:
        {
            ptr = &FMW_MAX_G_VOWN[0][0];
            break;
        }
        default:
        {
            ptr = &FMW_MAX_G_VOWN_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_FMWMaxGVown_gbl[i][j] = *(ptr + i * col + j);
        }
    }
}

static void set_FMWMaxGR(uint8 FMWMaxGType_index)
{
    const float32 *ptrRap;
    const float32 *ptrPed;
    const float32 *ptrCyc;
    uint16 row = 0;
    uint16 col = 0;
    row = sizeof(FMW_MAX_G_RAP_FOR_CONT_DOM) / sizeof(FMW_MAX_G_RAP_FOR_CONT_DOM[0]);
    col = sizeof(FMW_MAX_G_RAP_FOR_CONT_DOM[0]) / sizeof(FMW_MAX_G_RAP_FOR_CONT_DOM[0][0]);

    switch (FMWMaxGType_index)
    {
        case (uint8)1:
        {
            ptrRap = &FMW_MAX_G_RAP_FOR_CONT[0][0];
            ptrPed = &FMW_MAX_G_RAP_PED_FOR_CONT[0][0];
            ptrCyc = &FMW_MAX_G_RAP_CYC_FOR_CONT[0][0];
            break;
        }
        default:
        {
            ptrRap = &FMW_MAX_G_RAP_FOR_CONT_DOM[0][0];
            ptrPed = &FMW_MAX_G_RAP_PED_FOR_CONT_DOM[0][0];
            ptrCyc = &FMW_MAX_G_RAP_CYC_FOR_CONT_DOM[0][0];
            break;
        }
    }
    for (uint16 i = 0; i < row; i++)
    {
        for (uint16 j = 0; j < col; j++)
        {
            CarPara_FMWMaxGRap_gbl[i][j]    = *(ptrRap + i * col + j);
            CarPara_FMWMaxGRapPed_gbl[i][j] = *(ptrPed + i * col + j);
            CarPara_FMWMaxGRapCyc_gbl[i][j] = *(ptrCyc + i * col + j);
        }
    }
}

/* iEG_CAR_PARA */
static void set_CarPara_VariantPTType(uint8 iEG_CAR_PARA)
{
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_936_EU[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_936_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_925_EU[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_US[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_US[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_EU[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_EU_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_EU[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916_EU[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_520B_EU[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_916DIB_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_EgTrqCharConvert_gbl[i][j] = EGTRQ_CHAR_936_US[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWDirectR_Tbl_gbl[i] = A_FMW_DIRECT_R_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_920DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 24; i++)
            {
                CarPara_FMWActualMaxTrq_Tbl_gbl[i] = FMW_ACTUAL_MAX_TRQ_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_936_EU[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_936_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_925_US[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_925_EU[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_925_US[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920DIB_US[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920DIB_EU[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_US[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_US[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_EU[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_EU_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_EU[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916DIB_EU[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916_EU[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_520B_EU[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_ActualTrqFB_Tbl_gbl[i] = ACTUAL_TRQ_FB_936_US[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AFMWSuddenDirectR_Tbl_gbl[i] = A_FMW_KYUU_DIRECT_R_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)2:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)3:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)4:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)5:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)6:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)7:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)8:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)9:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)10:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)11:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)12:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)13:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)14:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)15:
        {
            f_CarPara_920RebuildEng_gbl = ON;
            break;
        }
        case (uint8)16:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)17:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)18:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)19:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)20:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)21:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)22:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)23:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)24:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)25:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)26:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)27:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)28:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        case (uint8)29:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
            break;
        }
        default:
        {
            f_CarPara_920RebuildEng_gbl = OFF;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_936_EU[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_936_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_925_US[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_925_EU[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_925_US[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920DIB_US[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920DIB_EU[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_US[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_US[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_EU[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_EU_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_EU[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916DIB_EU[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916_EU[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_520B_EU[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_924DIB_US[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_916DIB_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                CarPara_FilterTrqFB_Tbl_gbl[i] = FILTREQ_TRQ_FB_936_US[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrRevTrqBoder_Tbl_gbl[i] = ATR_REV_TRQ_BORDER_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[ 2 ][i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[ 2 ][i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_US[ 2 ][i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US[ 2 ][i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US_XUV[ 2 ][i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM_XUV[ 2 ][i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_B[ 2 ][i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_XUV[ 2 ][i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM_XUV[ 2 ][i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_EU_XUV[ 2 ][i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_520B_EU[ 2 ][i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 2 ][i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA[ 2 ][i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA_B[ 2 ][i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 2 ][i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 2 ][i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 2 ][i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 2 ][i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_SS_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_US[ 2 ][i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AtrFwdTrqBoder_Tbl_gbl[i] = ATR_FWD_TRQ_BORDER_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[ 1 ][i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[ 1 ][i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_US[ 1 ][i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US[ 1 ][i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US_XUV[ 1 ][i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM_XUV[ 1 ][i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_B[ 1 ][i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_XUV[ 1 ][i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM_XUV[ 1 ][i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_EU_XUV[ 1 ][i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_520B_EU[ 1 ][i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 1 ][i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA[ 1 ][i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA_B[ 1 ][i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 1 ][i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[ 1 ][i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 1 ][i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[ 1 ][i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_S_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_US[ 1 ][i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 21; i++)
            {
                CarPara_FMWSuddStepThrs_Map_gbl[i] = FMW_FM_THRESH_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_EU[0][i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_DOM[0][i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[0][i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_EU[0][i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_DOM[0][i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_925_US[0][i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_US[0][i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_EU[0][i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920DIB_DOM[0][i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US[0][i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_US_XUV[0][i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM[0][i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_DOM_XUV[0][i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU[0][i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_B[0][i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_EU_XUV[0][i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_EU[0][i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[0][i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM[0][i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_DOM_XUV[0][i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916_EU_XUV[0][i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_520B_EU[0][i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[0][i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA[0][i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_920_CHINA_B[0][i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[0][i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_924DIB_US[0][i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[0][i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_916DIB_DOM[0][i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_EngRrmLimBySIMode_I_gbl[i] = ENGINE_RPM_LIM_BY_SIMODE_936_US[0][i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_FMWGOffset_Tbl_gbl[i] = FMW_Goffset_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_EU[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_EU[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_520B_EU[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_I_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_US[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Norm_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_EU[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_EU[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_520B_EU[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_S_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_US[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbPBase_Sudd_R_Map_gbl[i][j] = EG_AFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_EU[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_EU[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_925_US[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_US_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_EU[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_DOM_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916_EU_XUV[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_520B_EU[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_924DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_916DIB_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 36; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtTurbinRrmHP_SS_gbl[i][j] = TARGET_TURBIN_RPM_HP_936_US[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_EgAfbIBase_Norm_R_Map_gbl[i][j] = EG_AFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TrconDecelRatioCVT_gbl[i][j] = TRCON_DECEL_RATIO_CVT_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_NORM_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM_gbl[i][j] = ATM_RDTRQ_4RPM_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbPBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_P_BASE_R_KYUU_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq_gbl[i][j] = ATM_RDTRQ_4TRQ_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Norm_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_NORM_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngMinusMax_gbl[i] = TRQ_ENG_MINUS_MAX936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_920_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_916_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_924DIB_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_918DST_DOM[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_AtrBrkDfbIBase_Sudd_R_Map_gbl[i][j] = ATRBRK_DFB_I_BASE_R_KYUU_925_DOM[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqEngAccessory_gbl[i] = TRQ_ENG_ACCESSORY_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Norm_Tbl_gbl[i] = aFB_P_COEFF_NORM_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqAcCompressor_gbl[i] = TRQ_AC_COMPRESSOR[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbPCoeff_Sudd_Tbl_gbl[i] = aFB_P_COEFF_KYUU_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_TrqFCut_gbl[i] = TRQ_FCUT_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_AfbICoeff_Norm_Tbl_gbl[i] = aFB_I_COEFF_NORM_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_920_CHINA_B[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 9; j++)
                {
                    CarPara_ThrOffEgTrq_gbl[i][j] = THR_OFF_EGTRQ_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Norm_Tbl_gbl[i] = bFB_P_COEFF_NORM_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920_CHINA[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_920_CHINA_B[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                CarPara_RrmLimFBinh_gbl[i] = RPM_FBINH_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbPCoeff_Sudd_Tbl_gbl[i] = bFB_P_COEFF_KYUU_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920B[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920B[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920B[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916B[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916B[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_924B[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_924B[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_924B[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916B[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_916B[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeFixSp_gbl[i][j] = TARGET_A_MAX_KOUBAI_TEISOKU_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Norm_Tbl_gbl[i] = bFB_I_COEFF_NORM_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920B[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920B[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920B[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916B[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916B[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_924B[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_924B[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_924B[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916B[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_916B[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_TgtAMaxSlopeTracking_gbl[i][j] = TARGET_A_MAX_KOUBAI_TUIBI_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_920DIB_DOM[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_920_DOM[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_916DIB_DOM[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_916_DOM[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_924DIB_DOM[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_918DST_DOM[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 15; i++)
            {
                CarPara_BfbICoeff_Sudd_Tbl_gbl[i] = bFB_I_COEFF_KYUU_925_DOM[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_920[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 3; i++)
            {
                CarPara_TowingLimit_gbl[i] = TOWING_LIMIT_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_918DST;
            break;
        }
        default:
        {
            CarPara_Delta_A_Norm_gbl = k_DELTA_A_NORM_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_EngRrmLimByNoisy_Map_gbl[i][j] = ENGINE_RPM_LIM_BY_URUSAI_MAP_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_918DST;
            break;
        }
        default:
        {
            CarPara_Delta_A_Sudd_gbl = k_DELTA_A_KYUU_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_936[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_936[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_925[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_925[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_925[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_925[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_520B[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_920[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_916DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                CarPara_EgCVTRate_gbl[i] = k_eg_cvt_rate_936[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_918DST;
            break;
        }
        default:
        {
            CarPara_Delta_A_Off_gbl = k_DELTA_A_WOSEIGYO_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_936;
            break;
        }
        case (uint8)2:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_936;
            break;
        }
        case (uint8)3:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920DIB;
            break;
        }
        case (uint8)8:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)11:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)12:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)15:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)16:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)17:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916;
            break;
        }
        case (uint8)22:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_520B;
            break;
        }
        case (uint8)23:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_924DIB;
            break;
        }
        case (uint8)24:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)25:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_920;
            break;
        }
        case (uint8)26:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916DIB;
            break;
        }
        case (uint8)29:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_916DIB;
            break;
        }
        default:
        {
            CarPara_StartGRatioThr_gbl = k_start_totalgearratio_thr_936;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_918DST;
            break;
        }
        default:
        {
            CarPara_SuddDepresBStatFilt_gbl = k_KYUFUMI_B_STATE_FILT_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 7; j++)
                {
                    CarPara_RpmRatioTrpmStart_gbl[i][j] = RPM_RATIO_TRPM_START_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_918DST;
            break;
        }
        default:
        {
            CarPara_SuddDepresFStatFilt_gbl = k_KYUFUMI_F_STATE_FILT_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 33; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_TgtAGainRpm_gbl[i][j] = KASOKU_GAIN_RPM_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_918DST;
            break;
        }
        default:
        {
            CarPara_FStatFilt_gbl = k_F_STATE_FILT_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip936;
            break;
        }
        case (uint8)2:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip936;
            break;
        }
        case (uint8)3:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip925;
            break;
        }
        case (uint8)4:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip925;
            break;
        }
        case (uint8)5:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip925;
            break;
        }
        case (uint8)6:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip925;
            break;
        }
        case (uint8)7:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920DIB;
            break;
        }
        case (uint8)8:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)11:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)12:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)13:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)14:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)15:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)16:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)17:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916;
            break;
        }
        case (uint8)20:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916;
            break;
        }
        case (uint8)21:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916;
            break;
        }
        case (uint8)22:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip520B;
            break;
        }
        case (uint8)23:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip924DIB;
            break;
        }
        case (uint8)24:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)25:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip920;
            break;
        }
        case (uint8)26:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916DIB;
            break;
        }
        case (uint8)29:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip916DIB;
            break;
        }
        default:
        {
            CarPara_ThrOffInhiMinClip_gbl = k_throff_inhi_minclip936;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_918DST;
            break;
        }
        default:
        {
            CarPara_AtrAfbIRt0_gbl = k_ATR_aFB_I_rt0_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920_B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 13; j++)
                {
                    CarPara_RsAcComp_gbl[i][j] = RS_AC_COMP_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_918DST;
            break;
        }
        default:
        {
            CarPara_AtrBfbIRt0_gbl = k_ATR_bFB_I_rt0_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainFixSp_gbl[i][j] = KASOKU_MODE_GAIN_TEISOKU_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_918DST;
            break;
        }
        default:
        {
            CarPara_AAporInit_gbl = k_A_APOR_INI_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_936[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_936[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_925[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_520B[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_920[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_916DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 4; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_KMdGainTracking_gbl[i][j] = KASOKU_MODE_GAIN_TUIBI_936[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_918DST;
            break;
        }
        default:
        {
            CarPara_AAporSuddInit_gbl = k_A_APOR_KYUU_INI_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)2:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)3:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)4:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)5:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)6:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)7:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)8:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)9:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)10:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)11:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)12:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)13:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)14:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)15:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)16:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)17:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)18:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)19:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)20:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)21:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)22:
        {
            f_CarPara_SpecEngTrqLimit_gbl = ON;
            break;
        }
        case (uint8)23:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)24:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)25:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)26:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)27:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)28:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        case (uint8)29:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
            break;
        }
        default:
        {
            f_CarPara_SpecEngTrqLimit_gbl = OFF;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_918DST;
            break;
        }
        default:
        {
            CarPara_TgtDegM4Filt_gbl = k_TARGET_DEG_M4_FILT_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)2:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)3:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)4:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)5:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)6:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)7:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)8:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)9:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)10:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)11:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)12:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)13:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)14:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)15:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)16:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)17:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)18:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)19:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)20:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)21:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)22:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)23:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)24:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)25:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
            break;
        }
        case (uint8)26:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)27:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)28:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        case (uint8)29:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = ON;
            break;
        }
        default:
        {
            f_CarPara_SpecEngTrqLimPreign_gbl = OFF;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_918DST;
            break;
        }
        default:
        {
            CarPara_TgtDegFiltRecv_gbl = k_TARGET_DEG_FILT_RECV_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4WGV_gbl[i][j] = ATM_RDTRQ_4RPM_4eleWGV_NONE[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_918DST;
            break;
        }
        default:
        {
            CarPara_AOwnFilt_gbl = k_AOWN_FILT_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 17; i++)
            {
                for (uint8 j = 0; j < 2; j++)
                {
                    CarPara_RatioRedTrqByATM4Trq4WGV_gbl[i][j] = ATM_RDTRQ_4TRQ_4eleWGV_NONE[i][j];
                }
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)2:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)3:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)4:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)5:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)6:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)7:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)8:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_920DIB;
            break;
        }
        case (uint8)9:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_920DIB;
            break;
        }
        case (uint8)10:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)11:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)12:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_920;
            break;
        }
        case (uint8)13:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_920;
            break;
        }
        case (uint8)14:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)15:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)16:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)17:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_916DIB;
            break;
        }
        case (uint8)18:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_916DIB;
            break;
        }
        case (uint8)19:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_916;
            break;
        }
        case (uint8)20:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_916;
            break;
        }
        case (uint8)21:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)22:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)23:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)24:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)25:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
            break;
        }
        case (uint8)26:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_924DIB;
            break;
        }
        case (uint8)27:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_924DIB;
            break;
        }
        case (uint8)28:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_918DST;
            break;
        }
        case (uint8)29:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_918DST;
            break;
        }
        default:
        {
            CarPara_TDiff4ALong_gbl = k_T_DIFF4A_LNG_925;
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 16; i++)
            {
                CarPara_TurboTrqThr_gbl[i] = TWGVNATRQTHRE_924DIB[i];
            }
        }
    }
    switch (iEG_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)25:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)26:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)27:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)28:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        case (uint8)29:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 9; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CorrAtmTrqThr_gbl[i][j] = MWGVNATRQPATM_924DIB[i][j];
                }
            }
        }
    }
}

/* iTM_CAR_PARA */
static void set_CarPara_VariantTMType(uint8 iTM_CAR_PARA)
{
    switch (iTM_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_B[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_C[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_D[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_E[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_916[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_916_B[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_916_C[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_925[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_925[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT_925[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C31[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C31_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C31_B[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C37[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C37_B[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C37_B[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C52[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_HTCVT_C52_B[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_5AT[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 6; i++)
            {
                CarPara_RsInertiaMass_gbl[i] = RS_INERTIA_MASS_CVT[i];
            }
        }
    }
    switch (iTM_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_D[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_E[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_D[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_916[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_916_B[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20_916_C[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C26[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C26_B[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C26_C[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C31[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C31[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C31_B[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C37[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C37[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C37[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C52[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C52[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C31[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_CHINA_B[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 13; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TrconTrqRatio_Map_gbl[i][j] = TRCON_TRQ_RATIO_CVT_C20[i][j];
                }
            }
        }
    }
    switch (iTM_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT_B[i];
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_5AT[i];
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_RsTM_Tbl_gbl[i] = RS_TM_CVT[i];
            }
        }
    }
    switch (iTM_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_D[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_E[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_D[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_916[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_916_B[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_916_C[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_916[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_916_B[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_925[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_925_B[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_925_C[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_520B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_936[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_HT_936[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFICIENCY_AT[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT_CHINA_B[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiency_gbl[i][j] = TM_EFFI_4TRQ_CFT[i][j];
                }
            }
        }
    }
    switch (iTM_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_D[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_E[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_D[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_916[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_916_B[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_916_C[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_916[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_916_B[i][j];
                }
            }
            break;
        }
        case (uint8)11:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_925[i][j];
                }
            }
            break;
        }
        case (uint8)12:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_925_B[i][j];
                }
            }
            break;
        }
        case (uint8)13:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_925_C[i][j];
                }
            }
            break;
        }
        case (uint8)14:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)15:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_520B[i][j];
                }
            }
            break;
        }
        case (uint8)16:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_DIB_US[i][j];
                }
            }
            break;
        }
        case (uint8)17:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)18:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)19:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_DIB[i][j];
                }
            }
            break;
        }
        case (uint8)20:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_936[i][j];
                }
            }
            break;
        }
        case (uint8)21:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_HT_936[i][j];
                }
            }
            break;
        }
        case (uint8)22:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFICIENCY_AT[i][j];
                }
            }
            break;
        }
        case (uint8)23:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_CHINA[i][j];
                }
            }
            break;
        }
        case (uint8)24:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT_CHINA_B[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 20; i++)
            {
                for (uint8 j = 0; j < 11; j++)
                {
                    CarPara_TMEfficiencyRpm_gbl[i][j] = TM_EFFI_4RPM_CFT[i][j];
                }
            }
        }
    }
}

/* iLKS_CAR_PARA */
static void set_CarPara_VariantForLKS(uint8 iLKS_CAR_PARA)
{
   switch (iLKS_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD_US[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD_US[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_OBK[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_StrDegCalc_Map_gbl[i][j] = STR_DEG_CALC_MAP_STD[i][j];
                }
            }
        }
    }
    switch (iLKS_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD_US[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD_US[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_OBK[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_KSecCalc_Map_gbl[i][j] = K_SEC_CALC_MAP_STD[i][j];
                }
            }
        }
    }
    switch (iLKS_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD_US[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD_US[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_OBK[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_OBK[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSFFStrAngRangeLimTbl_gbl[i] = FF_STR_ANG_RANGE_LIM_STD[i];
            }
        }
    }
    switch (iLKS_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD_US[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD_US[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)8:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_OBK[i];
            }
            break;
        }
        case (uint8)9:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
            break;
        }
        case (uint8)10:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_OBK[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LKSTgtStrAngRangeLimTbl_gbl[i] = TGT_STR_ANG_RANGE_LIM_STD[i];
            }
        }
    }
}

/* iLDP_CAR_PARA */
static void set_CarPara_VariantForLDP(uint8 iLDP_CAR_PARA)
{
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_LDP_PredictLength_gbl[i] = LDP_PREDICTLENGTH_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_LDP_PredictLength_gbl[i] = LDP_PREDICTLENGTH_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_LDP_PredictLength_gbl[i] = LDP_PREDICTLENGTH_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                CarPara_LDP_PredictLength_gbl[i] = LDP_PREDICTLENGTH_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInStart_gbl[i] = LDPDrvovrInStart_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInStart_gbl[i] = LDPDrvovrInStart_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInStart_gbl[i] = LDPDrvovrInStart_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInStart_gbl[i] = LDPDrvovrInStart_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInStart_gbl[i] = ELKDrvovrInStart_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInStart_gbl[i] = ELKDrvovrInStart_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInStart_gbl[i] = ELKDrvovrInStart_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInStart_gbl[i] = ELKDrvovrInStart_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_LDP_V2L_Line_hensa_Gain_gbl = LDP_V2L_LINE_HENSA_GAIN_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_LDP_V2L_Line_hensa_Gain_gbl = LDP_V2L_LINE_HENSA_GAIN_OBK;
            break;
        }
        case (uint8)3:
        {
            CarPara_LDP_V2L_Line_hensa_Gain_gbl = LDP_V2L_LINE_HENSA_GAIN_OBK_WILD;
            break;
        }
        default:
        {
            CarPara_LDP_V2L_Line_hensa_Gain_gbl = LDP_V2L_LINE_HENSA_GAIN_STD;
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutStart_gbl[i] = LDPDrvovrOutStart_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutStart_gbl[i] = LDPDrvovrOutStart_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutStart_gbl[i] = LDPDrvovrOutStart_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutStart_gbl[i] = LDPDrvovrOutStart_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutStart_gbl[i] = ELKDrvovrOutStart_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutStart_gbl[i] = ELKDrvovrOutStart_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutStart_gbl[i] = ELKDrvovrOutStart_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutStart_gbl[i] = ELKDrvovrOutStart_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDP_V2V_Pgain_gbl[i] = LDP_V2V_PGAIN_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDP_V2V_Pgain_gbl[i] = LDP_V2V_PGAIN_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDP_V2V_Pgain_gbl[i] = LDP_V2V_PGAIN_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDP_V2V_Pgain_gbl[i] = LDP_V2V_PGAIN_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutUp_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_UP_NORMAL[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutUp_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_UP_NORMAL[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutUp_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_UP_NORMAL[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutUp_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_UP_NORMAL[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutUp_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_UP_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutUp_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_UP_STD[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutUp_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_UP_OBK[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutUp_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_UP_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Pgain_gbl[i] = LDP_V2L_LAT_ACC_PGAIN_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Pgain_gbl[i] = LDP_V2L_LAT_ACC_PGAIN_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Pgain_gbl[i] = LDP_V2L_LAT_ACC_PGAIN_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Pgain_gbl[i] = LDP_V2L_LAT_ACC_PGAIN_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutDown_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_DOWN_NORMAL[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutDown_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_DOWN_NORMAL[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutDown_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_DOWN_NORMAL[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutIncutDown_gbl[i] = k_LDP_DRVOVR_OUT_INCUT_DOWN_NORMAL[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutDown_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_DOWN_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutDown_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_DOWN_STD[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutDown_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_DOWN_OBK[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutIncutDown_gbl[i] = k_ELK_DRVOVR_OUT_INCUT_DOWN_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Igain_gbl[i] = LDP_V2L_LAT_ACC_IGAIN_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Igain_gbl[i] = LDP_V2L_LAT_ACC_IGAIN_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Igain_gbl[i] = LDP_V2L_LAT_ACC_IGAIN_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_Igain_gbl[i] = LDP_V2L_LAT_ACC_IGAIN_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInEnd_gbl[i] = LDPDrvovrInEnd_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInEnd_gbl[i] = LDPDrvovrInEnd_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInEnd_gbl[i] = LDPDrvovrInEnd_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrInEnd_gbl[i] = LDPDrvovrInEnd_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInEnd_gbl[i] = ELKDrvovrInEnd_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInEnd_gbl[i] = ELKDrvovrInEnd_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInEnd_gbl[i] = ELKDrvovrInEnd_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrInEnd_gbl[i] = ELKDrvovrInEnd_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_YawPgain_gbl[i] = LDP_V2L_LAT_ACC_YAWPGAIN_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_YawPgain_gbl[i] = LDP_V2L_LAT_ACC_YAWPGAIN_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_YawPgain_gbl[i] = LDP_V2L_LAT_ACC_YAWPGAIN_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 10; i++)
            {
                CarPara_LDP_V2L_lat_acc_YawPgain_gbl[i] = LDP_V2L_LAT_ACC_YAWPGAIN_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutEnd_gbl[i] = LDPDrvovrOutEnd_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutEnd_gbl[i] = LDPDrvovrOutEnd_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutEnd_gbl[i] = LDPDrvovrOutEnd_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LDPDrvovrOutEnd_gbl[i] = LDPDrvovrOutEnd_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutEnd_gbl[i] = ELKDrvovrOutEnd_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutEnd_gbl[i] = ELKDrvovrOutEnd_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutEnd_gbl[i] = ELKDrvovrOutEnd_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_ELKDrvovrOutEnd_gbl[i] = ELKDrvovrOutEnd_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_lat_acc_std_gbl[i][j] = LDP_LAT_ACC_STD_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_lat_acc_std_gbl[i][j] = LDP_LAT_ACC_STD_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_lat_acc_std_gbl[i][j] = LDP_LAT_ACC_STD_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_lat_acc_std_gbl[i][j] = LDP_LAT_ACC_STD_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrBig_gbl[i] = LdpYawrBig_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrBig_gbl[i] = LdpYawrBig_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrBig_gbl[i] = LdpYawrBig_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrBig_gbl[i] = LdpYawrBig_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_add_str_jark_gbl[i][j] = LDP_ADD_STR_JARK_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_add_str_jark_gbl[i][j] = LDP_ADD_STR_JARK_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_add_str_jark_gbl[i][j] = LDP_ADD_STR_JARK_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_LDP_add_str_jark_gbl[i][j] = LDP_ADD_STR_JARK_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrNormal_gbl[i] = LdpYawrNormal_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrNormal_gbl[i] = LdpYawrNormal_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrNormal_gbl[i] = LdpYawrNormal_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrNormal_gbl[i] = LdpYawrNormal_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_LDP_car_response_delay_gbl = LDP_CAR_RESPONSE_DELAY_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_LDP_car_response_delay_gbl = LDP_CAR_RESPONSE_DELAY_OBK;
            break;
        }
        case (uint8)3:
        {
            CarPara_LDP_car_response_delay_gbl = LDP_CAR_RESPONSE_DELAY_OBK_WILD;
            break;
        }
        default:
        {
            CarPara_LDP_car_response_delay_gbl = LDP_CAR_RESPONSE_DELAY_STD;
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrReact_gbl[i] = LdpYawrReact_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrReact_gbl[i] = LdpYawrReact_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrReact_gbl[i] = LdpYawrReact_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpYawrReact_gbl[i] = LdpYawrReact_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 12; i++)
            {
                CarPara_LDP_cal_StrAngRateLim_gbl[i] = LDP_CAL_STRANGRATELIM_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 12; i++)
            {
                CarPara_LDP_cal_StrAngRateLim_gbl[i] = LDP_CAL_STRANGRATELIM_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 12; i++)
            {
                CarPara_LDP_cal_StrAngRateLim_gbl[i] = LDP_CAL_STRANGRATELIM_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 12; i++)
            {
                CarPara_LDP_cal_StrAngRateLim_gbl[i] = LDP_CAL_STRANGRATELIM_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGCancel_gbl[i] = LdpHorizGCancel_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGCancel_gbl[i] = LdpHorizGCancel_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGCancel_gbl[i] = LdpHorizGCancel_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGCancel_gbl[i] = LdpHorizGCancel_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_LDP_Str_Ang_RateLim_gbl = LDP_STR_ANG_RATELIM_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_LDP_Str_Ang_RateLim_gbl = LDP_STR_ANG_RATELIM_OBK;
            break;
        }
        case (uint8)3:
        {
            CarPara_LDP_Str_Ang_RateLim_gbl = LDP_STR_ANG_RATELIM_OBK_WILD;
            break;
        }
        default:
        {
            CarPara_LDP_Str_Ang_RateLim_gbl = LDP_STR_ANG_RATELIM_STD;
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGOn_gbl[i] = LdpHorizGOn_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGOn_gbl[i] = LdpHorizGOn_OBK[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGOn_gbl[i] = LdpHorizGOn_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_LdpHorizGOn_gbl[i] = LdpHorizGOn_STD[i];
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_IN_gbl[i][j] = LDP_RATE_LO_IN_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_IN_gbl[i][j] = LDP_RATE_LO_IN_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_IN_gbl[i][j] = LDP_RATE_LO_IN_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_IN_gbl[i][j] = LDP_RATE_LO_IN_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_OUT_gbl[i][j] = LDP_RATE_LO_OUT_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_OUT_gbl[i][j] = LDP_RATE_LO_OUT_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_OUT_gbl[i][j] = LDP_RATE_LO_OUT_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Lo_OUT_gbl[i][j] = LDP_RATE_LO_OUT_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Hi_gbl_gbl[i][j] = LDP_RATE_HI_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Hi_gbl_gbl[i][j] = LDP_RATE_HI_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Hi_gbl_gbl[i][j] = LDP_RATE_HI_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 16; j++)
                {
                    CarPara_LDP_Rate_Hi_gbl_gbl[i][j] = LDP_RATE_HI_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_lat_acc_std_gbl[i][j] = RDP_LAT_ACC_STD_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_lat_acc_std_gbl[i][j] = RDP_LAT_ACC_STD_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_lat_acc_std_gbl[i][j] = RDP_LAT_ACC_STD_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_lat_acc_std_gbl[i][j] = RDP_LAT_ACC_STD_STD[i][j];
                }
            }
        }
    }
    switch (iLDP_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_add_str_jark_gbl[i][j] = RDP_ADD_STR_JARK_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_add_str_jark_gbl[i][j] = RDP_ADD_STR_JARK_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_add_str_jark_gbl[i][j] = RDP_ADD_STR_JARK_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 11; i++)
            {
                for (uint8 j = 0; j < 17; j++)
                {
                    CarPara_RDP_add_str_jark_gbl[i][j] = RDP_ADD_STR_JARK_STD[i][j];
                }
            }
        }
    }
}  

/* iPAD_CAR_PARA_F */  
static void set_CarPara_VariantPadTypeF(uint8 iPAD_CAR_PARA_F)
{
    switch (iPAD_CAR_PARA_F)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FA430[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FA430[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FH400[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_GA5504H[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_J636[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FN360[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FN410[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadUFrontMap_gbl[i][j] = BrakePad_uMAP_FA300[i][j];
                }
            }
        }
    }
}

/* iPAD_CAR_PARA_R */  
static void set_CarPara_VariantPadTypeR(uint8 iPAD_CAR_PARA_R)
{
    switch (iPAD_CAR_PARA_R)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FA430[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FA430[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FH400[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_GA5504H[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_J636[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FN360[i][j];
                }
            }
            break;
        }
        case (uint8)7:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FN410[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 7; i++)
            {
                for (uint8 j = 0; j < 6; j++)
                {
                    CarPara_BrakePadURearMap_gbl[i][j] = BrakePad_uMAP_FA300[i][j];
                }
            }
        }
    }
}

/* iAES_CAR_PARA */
void set_CarPara_VariantForAES(uint8 iAES_CAR_PARA)
{
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_STD_iB;
            break;
        }
        case (uint8)3:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_OBK;
            break;
        }
        case (uint8)4:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_OBK_iB;
            break;
        }
        case (uint8)5:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_OBK_WILD;
            break;
        }
        case (uint8)6:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_OBK_WILD_iB;
            break;
        }
        default:
        {
            CarPara_t_delay_PresUpAES_gbl = t_delay_PresUpAES_STD;
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_STD_iB;
            break;
        }
        case (uint8)3:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_OBK;
            break;
        }
        case (uint8)4:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_OBK_iB;
            break;
        }
        case (uint8)5:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_OBK_WILD;
            break;
        }
        case (uint8)6:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_OBK_WILD_iB;
            break;
        }
        default:
        {
            CarPara_jerkAES_gbl = jerk_max_PC_AOWNAES_STD;
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_STD;
            break;
        }
        case (uint8)2:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_STD;
            break;
        }
        case (uint8)3:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_OBK;
            break;
        }
        case (uint8)4:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_OBK;
            break;
        }
        case (uint8)5:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_OBK_WILD;
            break;
        }
        case (uint8)6:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_OBK_WILD;
            break;
        }
        default:
        {
            CarPara_MirrorWidthAES_gbl = MirrorWidthAES_STD;
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlStrAngMaxAES_gbl[i][j] = CtrlStrAngMaxAES_STD[i][j];
                }
            }
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_STD[i];
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_STD[i];
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_OBK[i];
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_OBK[i];
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_OBK_WILD[i];
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_OBK_WILD[i];
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                CarPara_DrvOvrAES_gbl[i] = DrvOvrAES_STD[i];
            }
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosPgainAES_gbl[i][j] = LatPosPgainAES_STD[i][j];
                }
            }
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_LatPosIgainAES_gbl[i][j] = LatPosIgainAES_STD[i][j];
                }
            }
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_YawAngPgainAES_gbl[i][j] = YawAngPgainAES_STD[i][j];
                }
            }
        }
    }
    switch (iAES_CAR_PARA)
    {
        case (uint8)1:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_STD[i][j];
                }
            }
            break;
        }
        case (uint8)2:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_STD_iB[i][j];
                }
            }
            break;
        }
        case (uint8)3:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_OBK[i][j];
                }
            }
            break;
        }
        case (uint8)4:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_OBK_iB[i][j];
                }
            }
            break;
        }
        case (uint8)5:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_OBK_WILD[i][j];
                }
            }
            break;
        }
        case (uint8)6:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_OBK_WILD_iB[i][j];
                }
            }
            break;
        }
        default:
        {
            for (uint8 i = 0; i < 8; i++)
            {
                for (uint8 j = 0; j < 10; j++)
                {
                    CarPara_CtrlTTCAES_gbl[i][j] = CtrlTTCAES_STD[i][j];
                }
            }
        }
    }
}

#endif /* CARPARA_INP_02 */
