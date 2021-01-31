#pragma once

//namespace KEY {
//	enum E_KEY { KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LBUTTON, KEY_RBUTTON, KEY_Q, KEY_E, KEY_W, KEY_A, KEY_S, KEY_D, KEY_LSHIFT, KEY_P, KEY_G, KEY_SPACE, KEY_R, KEY_TAB, KEY_M, KEY_F, KEY_Z, KEY_X, KEY_END };
//}

namespace OBJ {
	enum E_DIRECTION { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN, DIR_END };
	enum E_COLLIDED { COL_LEFT, COL_RIGHT, COL_TOP, COL_BOTTOM, COL_END };
	enum E_TYPE { TYPE_PLAYER, TYPE_ENEMY, TYPE_ITEM, TYPE_PLAYER_BULLET, TYPE_PLAYER_BOMB, TYPE_ENEMY_BULLET, TYPE_END };
	enum E_MODEL_TYPE { MODEL_TYPE_ELLIPSE, MODEL_TYPE_RECTANGLE, MODEL_TYPE_LINE, MODEL_TYPE_END };
}

namespace TILE {
	enum E_TYPE { TYPE_NORMAL, TYPE_BLOCKING, TYPE_OK, TYPE_NO, TYPE_END }; 
}

namespace COMMANDER {
	enum E_COMMAND { COMMAND_NOTHING, COMMAND_GATHERING, COMMAND_SATURATION }; 
}

namespace WINDMILL {
	// 플레이어의 것, 자신 소유의 것, 다른 플레이어의 것, 점령되지 않은 것 // 것 == 제분소
	enum E_OWN_TYPE { TYPE_PLAYER, TYPE_OWN, TYPE_OTHER, TYPE_UNOCCUPIED, TYPE_RANDOM, TYPE_END };
	enum E_STATE { STATE_UNOCCUPIED, STATE_BUILDING, STATE_OCCUPIED, STATE_DESTROYED, STATE_END };
}

namespace PATH {
	enum E_PURPOSE { PURPOSE_WANDERING, PURPOSE_GATHERING, PURPOSE_ACTIVE, PURPOSE_END };
}

namespace UNIT{
	enum E_TYPE { TYPE_PIG, TYPE_SQUIRREL, TYPE_LIZARD, TYPE_MOLE, TYPE_SKUNK, TYPE_BADGER, TYPE_FOX, TYPE_END  };
}

namespace MINIMAP {
	enum E_SIGN { SIGN_NONE, SIGN_COMMANDER, SIGN_TUNNEL, SIGN_UNIT, SIGN_WINDMILL, SIGN_END };
}

namespace MAP_OBJ {
	enum E_TYPE { TYPE_NONE, TYPE_TILE, TYPE_DECO, TYPE_END };
}

namespace PARTICLE {
	enum E_TYPE { TYPE_ELLIPSE, TYPE_RECTANGLE, TYPE_TRIANGLE, TYPE_LINE, TYPE_END };
}

namespace PLAYER {
	enum E_ATTACK_MODE { ATTACK_MODE_NORMAL, ATTACK_MODE_ENERGY, ATTACK_MODE_END };
}

namespace MONSTER {
	enum E_TYPE { TYPE_NORMAL, TYPE_BOSS, TYPE_END};
}

namespace ITEM {
	enum E_TYPE { TYPE_NONE, TYPE_POWER, TYPE_MAX_POWER, TYPE_BUDDY, TYPE_END};
}

namespace UI {
	enum E_GAUGE { GAUGE_VERTICAL, GAUGE_HORIZONTAL, GAUGE_END };
	enum E_PIVOT { PIVOT_LEFT/*, PIVOT_MIDDLE*/, PIVOT_END };
}

namespace BULLET {
	enum E_TYPE { TYPE_MONSTER, TYPE_PLAYER, TYPE_END };
}

namespace TRIGGER {
	enum E_TYPE { TYPE_DEATH, TYPE_CHECKPOINT, TYPE_SPAWN, TYPE_SPAWNER, TYPE_END };
}

namespace ANIM {
	enum E_ALICEI_ANIM {
		AliceI_Idle,                              //0	frame:[93]
		AliceI_Idle_Random_1,                     //1	frame:[191]
		AliceI_Idle_Random_2,                     //2	frame:[365]
		AliceI_Idle_Random_3,                     //3	frame:[201]
		AliceI_Idle_Random_4,                     //4	frame:[141]
		AliceI_Idle_Random_5,                     //5	frame:[181]
		AliceI_Idle_Random_6,                     //6	frame:[468]
		AliceI_Idle_Turn_Left_Short_back,         //7	frame:[10]
		AliceI_Idle_Turn_Right_Short_back,        //8	frame:[10]
		AliceI_Jump_Fall,                         //9	frame:[21]
		AliceI_Jump_Land,                         //10	frame:[18]
		AliceI_Jump_Rise,                         //11	frame:[9]
		AliceI_Jump_Start,                        //12	frame:[9]
		AliceI_JumpFwd_Fall,                      //13	frame:[26]
		AliceI_JumpFwd_LandLow,                   //14	frame:[11]
		AliceI_JumpFwd_Rise,                      //15	frame:[9]
		AliceI_JumpFwd_Start,                     //16	frame:[5]
		AliceI_Run,                               //17	frame:[22]
		AliceI_Walk,                              //18	frame:[37]
		AliceI_Walk_Fast,                         //19	frame:[27]
		ALICEI_ANIM_END
	};

	enum E_PIGFLY_ANIM {
		NoseSwich_Fly_Action,                     //0	frame:[61]
		NoseSwich_Fly_Charge_1,                   //1	frame:[41]
		NoseSwich_Fly_Charge_2,                   //2	frame:[31]
		NoseSwich_Fly_Charge_3,                   //3	frame:[21]
		NoseSwich_Fly_Charge_4,                   //4	frame:[9]
		NoseSwich_Fly_Charge_5,                   //5	frame:[7]
		NoseSwich_Fly_Disappear,                  //6	frame:[41]
		NoseSwich_Fly_Idle,                       //7	frame:[146]
		NoseSwich_Fly_Idle01,                     //8	frame:[61]
		NoseSwich_Fly_Relax,                      //9	frame:[21]
		NoseSwich_Mount_Action,                   //10	frame:[61]
		NoseSwich_Mount_Charge_1,                 //11	frame:[41]
		NoseSwich_Mount_Charge_2,                 //12	frame:[31]
		NoseSwich_Mount_Charge_3,                 //13	frame:[21]
		NoseSwich_Mount_Charge_4,                 //14	frame:[7]
		NoseSwich_Mount_Charge_5,                 //15	frame:[9]
		NoseSwich_Mount_Disappear,                //16	frame:[41]
		NoseSwich_Mount_Idle,                     //17	frame:[61]
		NoseSwich_Mount_Relax,                    //18	frame:[61]
		PIGFLY_ANIM_END
	};

	enum E_ALICEW_ANIM{
		AliceW_Walk_Watch,                        //54	frame:[33]
		AliceW_Walk,                              //53	frame:[33]
		AliceW_Slide_Start,                       //52	frame:[31]
		AliceW_Slide_End,                         //51	frame:[31]
		AliceW_Shrink,                            //50	frame:[19]
		AliceW_Run_Watch,                         //49	frame:[23]
		AliceW_Run,                               //48	frame:[23]
		AliceW_Pad_Jump_Raise_C,                  //47	frame:[91]
		AliceW_Pad_Jump_Land,                     //46	frame:[11]
		AliceW_Pad_Jump_Fall,                     //45	frame:[61]
		AliceW_JumpFwd_Start,                     //44	frame:[4]
		AliceW_JumpFwd_Rise,                      //43	frame:[12]
		AliceW_JumpFwd_LandLow,                   //42	frame:[5]
		AliceW_JumpFwd_Land_Medium,               //41	frame:[23]
		AliceW_JumpFwd_Fall,                      //40	frame:[31]
		AliceW_JumpD_Rgt_Start,                   //39	frame:[7]
		AliceW_JumpD_Rgt_Rise,                    //38	frame:[15]
		AliceW_JumpD_Rgt_Land,                    //37	frame:[7]
		AliceW_JumpD_Rgt_Fall,                    //36	frame:[26]
		AliceW_JumpD_Lft_Start,                   //35	frame:[7]
		AliceW_JumpD_Lft_Rise,                    //34	frame:[15]
		AliceW_JumpD_Lft_Land,                    //33	frame:[7]
		AliceW_JumpD_Lft_Fall,                    //32	frame:[26]
		AliceW_Jump_Start,                        //31	frame:[5]
		AliceW_Jump_Rise,                         //30	frame:[12]
		AliceW_Jump_Land_Medium,                  //29	frame:[38]
		AliceW_Jump_Land,                         //28	frame:[11]
		AliceW_Jump_Fall,                         //27	frame:[61]
		AliceW_Idle_Watch,                        //26	frame:[151]
		AliceW_Idle_Random_6,                     //25	frame:[241]
		AliceW_Idle_Random_5,                     //24	frame:[201]
		AliceW_Idle_Random_4,                     //23	frame:[141]
		AliceW_Idle_Random_3,                     //22	frame:[338]
		AliceW_Idle_Random_2,                     //21	frame:[236]
		AliceW_Idle_Random_1,                     //20	frame:[221]
		AliceW_Idle,                              //19	frame:[151]
		AliceW_ClockBomb_Spawn_Still,             //18	frame:[31]
		AliceW_ClockBomb_Spawn_Run_UB,            //17	frame:[31]
		AliceW_Attached_Walk,                     //16	frame:[33]
		AliceW_Attached_React,                    //15	frame:[16]
		AliceW_Attached_Idle_Turn_Right_Short,    //14	frame:[7]
		AliceW_Attached_Idle_Turn_Left_Short,     //13	frame:[7]
		AliceW_Attached_Idle,                     //12	frame:[41]
		AliceL_Slide_Right,                       //11	frame:[169]
		AliceL_Slide_Left,                        //10	frame:[169]
		AliceL_Slide_Forwardward,                 //9	frame:[169]
		AliceL_Slide_Damage,                      //8	frame:[23]
		AliceL_Slide_Breakthrough,                //7	frame:[23]
		AliceL_Slide_Backward,                    //6	frame:[169]
		AliceL_Slide,                             //5	frame:[169]
		AliceL_Idle_Turn_Right_Short_back,        //4	frame:[10]
		AliceL_Idle_Turn_Left_Short_back,         //3	frame:[10]
		Alice_Spawn,                              //2	frame:[66]
		Alice_Railcar_Idle,                       //1	frame:[101]
		Alice_Death,                              //0	frame:[54]

		AliceGiant_Walk,                          //9	frame:[76]
		AliceGiant_Stomp_All,                     //8	frame:[111]
		AliceGiant_Run,                           //7	frame:[61]
		AliceGiant_Idle_Long01,                   //6	frame:[191]
		AliceGiant_Idle,                          //5	frame:[151]
		AliceGiant_Damage_Lgt,                    //4	frame:[46]
		AliceGiant_Attack_Pull,                   //3	frame:[111]
		AliceGiant_Attack03,                      //2	frame:[91]
		AliceGiant_Attack02,                      //1	frame:[61]
		AliceGiant_Attack01,                      //0	frame:[71]

		Pose_WP4_Charge_Idle_UB_Up,               //209	frame:[41]
		Pose_WP4_Charge_Idle_UB_Down,             //208	frame:[41]
		Pose_WP4_Charge_Idle_UB,                  //207	frame:[41]
		Pose_WP4_Attack_Idle_UB_Up,               //206	frame:[61]
		Pose_WP4_Attack_Idle_UB_Down,             //205	frame:[61]
		Pose_WP4_Attack_Idle_UB,                  //204	frame:[61]
		Pose_WP3_Attack_Idle_UB_Up,               //203	frame:[37]
		Pose_WP3_Attack_Idle_UB_Down,             //202	frame:[37]
		Pose_WP3_Attack_Idle_UB,                  //201	frame:[37]
		Pose_WP2_Idle_Range_UB,                   //200	frame:[2]
		AliceW_WP4_Walk,                          //199	frame:[25]
		AliceW_WP4_Strafe_Rgt,                    //198	frame:[17]
		AliceW_WP4_Strafe_Lft,                    //197	frame:[17]
		AliceW_WP4_Strafe_Fwd,                    //196	frame:[17]
		AliceW_WP4_Strafe_Bk,                     //195	frame:[17]
		AliceW_WP4_Run,                           //194	frame:[23]
		AliceW_WP4_Mele_Damg_Hy_2,                //193	frame:[70]
		AliceW_WP4_Mele_Damg_Hy_1,                //192	frame:[70]
		AliceW_WP4_Mele_Attack_Rush,              //191	frame:[17]
		AliceW_WP4_Idle_Turn_Right_Short,         //190	frame:[11]
		AliceW_WP4_Idle_Turn_Left_Short,          //189	frame:[11]
		AliceW_WP4_Idle,                          //188	frame:[61]
		AliceW_WP4_Fire_NoDelay,                  //187	frame:[31]
		AliceW_WP4_Fire_2,                        //186	frame:[46]
		AliceW_WP4_Fire,                          //185	frame:[46]
		AliceW_WP4_Damage_Rgt,                    //184	frame:[25]
		AliceW_WP4_Damage_Lft,                    //183	frame:[25]
		AliceW_WP4_Damage_Heavy_KnockBack,        //182	frame:[39]
		AliceW_WP4_Charge_Strafe_Rgt,             //181	frame:[33]
		AliceW_WP4_Charge_Strafe_Lft,             //180	frame:[33]
		AliceW_WP4_Charge_Strafe_Fwd,             //179	frame:[33]
		AliceW_WP4_Charge_Strafe_Bk,              //178	frame:[33]
		AliceW_WP4_Charge,                        //177	frame:[17]
		AliceW_WP4_Attack_Idle,                   //176	frame:[61]
		AliceW_WP4_Attack_Charging_UB,            //175	frame:[21]
		AliceW_WP4_Attack_Charging,               //174	frame:[21]
		AliceW_WP4_Attack_Charged_Idle,           //173	frame:[41]
		AliceW_WP4_Attack_Charged_Fire,           //172	frame:[36]
		AliceW_WP4_Attack_Charge,                 //171	frame:[17]
		AliceW_WP3_Walk,                          //170	frame:[33]
		AliceW_WP3_Strafe_Rgt,                    //169	frame:[19]
		AliceW_WP3_Strafe_Lft,                    //168	frame:[19]
		AliceW_WP3_Strafe_Fwd,                    //167	frame:[19]
		AliceW_WP3_Strafe_Bk,                     //166	frame:[19]
		AliceW_WP3_Run,                           //165	frame:[23]
		AliceW_WP3_Release,                       //164	frame:[37]
		AliceW_WP3_Rage_Damg_Rgt_Hy,              //163	frame:[31]
		AliceW_WP3_Rage_Damg_Lft_Hy,              //162	frame:[31]
		AliceW_WP3_Rage_Damg_Hy_2,                //161	frame:[70]
		AliceW_WP3_Rage_Damg_Hy_1,                //160	frame:[69]
		AliceW_WP3_NoAmmo_Up,                     //159	frame:[13]
		AliceW_WP3_NoAmmo_Down,                   //158	frame:[13]
		AliceW_WP3_NoAmmo,                        //157	frame:[13]
		AliceW_WP3_Mele_Attack_Rush,              //156	frame:[17]
		AliceW_WP3_Idle_Turn_Right_Short,         //155	frame:[7]
		AliceW_WP3_Idle_Turn_Left_Short,          //154	frame:[7]
		AliceW_WP3_Idle,                          //153	frame:[151]
		AliceW_WP3_Fire,                          //152	frame:[37]
		AliceW_WP3_Dodge_Rgt_C,                   //151	frame:[16]
		AliceW_WP3_Dodge_Lft_C,                   //150	frame:[16]
		AliceW_WP3_Dodge_Fwd_C,                   //149	frame:[16]
		AliceW_WP3_Dodge_Bk_C,                    //148	frame:[16]
		AliceW_WP3_Damage_Heavy_KnockBack,        //147	frame:[39]
		AliceW_WP3_Charge,                        //146	frame:[25]
		AliceW_WP3_Attack_Idle,                   //145	frame:[37]
		AliceW_WP3_Attack_Alt_B,                  //144	frame:[36]
		AliceW_WP3_Attack_Alt_A_UB,               //143	frame:[26]
		AliceW_WP3_Attack_Alt_A,                  //142	frame:[26]
		AliceW_WP2_Walk,                          //141	frame:[33]
		AliceW_WP2_Strafe_Rgt,                    //140	frame:[25]
		AliceW_WP2_Strafe_Lft,                    //139	frame:[25]
		AliceW_WP2_Strafe_Fwd,                    //138	frame:[25]
		AliceW_WP2_Strafe_Bk,                     //137	frame:[25]
		AliceW_WP2_Run,                           //136	frame:[23]
		AliceW_WP2_Mele_SwitchAttack_1,           //135	frame:[67]
		AliceW_WP2_Mele_Damg_Hy_2,                //134	frame:[70]
		AliceW_WP2_Mele_Damg_Hy_1,                //133	frame:[70]
		AliceW_WP2_Mele_Attack_Rush,              //132	frame:[17]
		AliceW_WP2_Mele_Attack_5_B,               //131	frame:[36]
		AliceW_WP2_Mele_Attack_5_A,               //130	frame:[34]
		AliceW_WP2_Mele_Attack_4_B,               //129	frame:[36]
		AliceW_WP2_Mele_Attack_4_A,               //128	frame:[34]
		AliceW_WP2_Mele_Attack_3_B,               //127	frame:[36]
		AliceW_WP2_Mele_Attack_3_A,               //126	frame:[31]
		AliceW_WP2_Mele_Attack_2_B,               //125	frame:[21]
		AliceW_WP2_Mele_Attack_2_A,               //124	frame:[49]
		AliceW_WP2_Mele_Attack_1_B,               //123	frame:[44]
		AliceW_WP2_Mele_Attack_1_A,               //122	frame:[38]
		AliceW_WP2_Idle_Turn_Right_Short,         //121	frame:[11]
		AliceW_WP2_Idle_Turn_Left_Short,          //120	frame:[7]
		AliceW_WP2_Idle_Mele,                     //119	frame:[41]
		AliceW_WP2_Idle,                          //118	frame:[151]
		AliceW_WP2_Dodge_Rgt_C,                   //117	frame:[16]
		AliceW_WP2_Dodge_Lft_C,                   //116	frame:[16]
		AliceW_WP2_Dodge_Fwd_C,                   //115	frame:[16]
		AliceW_WP2_Dodge_Bk_C,                    //114	frame:[16]
		AliceW_WP2_Disarm_UB,                     //113	frame:[31]
		AliceW_WP2_Damage_Rgt,                    //112	frame:[25]
		AliceW_WP2_Damage_Lft,                    //111	frame:[25]
		AliceW_WP2_Damage_L,                      //110	frame:[25]
		AliceW_WP2_Damage_Heavy_KnockBack,        //109	frame:[39]
		AliceW_WP1_Walk,                          //108	frame:[33]
		AliceW_WP1_Strafe_Rgt,                    //107	frame:[25]
		AliceW_WP1_Strafe_Lft,                    //106	frame:[25]
		AliceW_WP1_Strafe_Fwd,                    //105	frame:[25]
		AliceW_WP1_Strafe_Bk,                     //104	frame:[25]
		AliceW_WP1_Run,                           //103	frame:[23]
		AliceW_WP1_Mele_SwitchAttack_1,           //102	frame:[37]
		AliceW_WP1_Mele_Damg_Rgt_Lt,              //101	frame:[31]
		AliceW_WP1_Mele_Damg_Lft_Lt,              //100	frame:[31]
		AliceW_WP1_Mele_Damg_Hy_2,                //99	frame:[70]
		AliceW_WP1_Mele_Damg_Hy_1,                //98	frame:[69]
		AliceW_WP1_Mele_Attack_Rush_Switch,       //97	frame:[17]
		AliceW_WP1_Mele_Attack_Rush,              //96	frame:[17]
		AliceW_WP1_Mele_Attack_React_2,           //95	frame:[31]
		AliceW_WP1_Mele_Attack_React_1,           //94	frame:[31]
		AliceW_WP1_Mele_Attack_BackHop_2,         //93	frame:[9]
		AliceW_WP1_Mele_Attack_BackHop_1,         //92	frame:[9]
		AliceW_WP1_Mele_Attack_5_B,               //91	frame:[11]
		AliceW_WP1_Mele_Attack_5_A,               //90	frame:[33]
		AliceW_WP1_Mele_Attack_4_B,               //89	frame:[11]
		AliceW_WP1_Mele_Attack_4_A,               //88	frame:[34]
		AliceW_WP1_Mele_Attack_3_B,               //87	frame:[7]
		AliceW_WP1_Mele_Attack_3_A,               //86	frame:[29]
		AliceW_WP1_Mele_Attack_2_B,               //85	frame:[16]
		AliceW_WP1_Mele_Attack_2_A,               //84	frame:[31]
		AliceW_WP1_Mele_Attack_1_B,               //83	frame:[18]
		AliceW_WP1_Mele_Attack_1_A,               //82	frame:[29]
		AliceW_WP1_Idle_Turn_Right_Short,         //81	frame:[11]
		AliceW_WP1_Idle_Turn_Left_Short,          //80	frame:[11]
		AliceW_WP1_Idle_Mele,                     //79	frame:[41]
		AliceW_WP1_Idle,                          //78	frame:[151]
		AliceW_WP1_Dodge_Rgt_C,                   //77	frame:[16]
		AliceW_WP1_Dodge_Rgt_B,                   //76	frame:[11]
		AliceW_WP1_Dodge_Rgt_A,                   //75	frame:[4]
		AliceW_WP1_Dodge_Lft_C,                   //74	frame:[16]
		AliceW_WP1_Dodge_Lft_B,                   //73	frame:[11]
		AliceW_WP1_Dodge_Lft_A,                   //72	frame:[3]
		AliceW_WP1_Dodge_Fwd_C,                   //71	frame:[16]
		AliceW_WP1_Dodge_Fwd_B,                   //70	frame:[11]
		AliceW_WP1_Dodge_Fwd_A,                   //69	frame:[4]
		AliceW_WP1_Dodge_Bk_C,                    //68	frame:[16]
		AliceW_WP1_Dodge_Bk_B,                    //67	frame:[11]
		AliceW_WP1_Dodge_Bk_A,                    //66	frame:[4]
		AliceW_WP1_Damage_Heavy_KnockBack,        //65	frame:[39]
		AliceW_Watch_Trigger,                     //64	frame:[16]
		AliceW_Watch_Hand,                        //63	frame:[2]
		AliceW_StauePose_Throw,                   //62	frame:[46]
		AliceW_StauePose_Spin,                    //61	frame:[16]
		AliceW_StauePose_Reflect,                 //60	frame:[11]
		AliceW_StauePose_Out,                     //59	frame:[6]
		AliceW_StauePose_In,                      //58	frame:[7]
		AliceW_StauePose_Damage,                  //57	frame:[21]
		AliceW_StauePose,                         //56	frame:[31]
		AliceW_Put_Bomb,                          //55	frame:[52]
		AliceW_Jump_Damage_C,                     //54	frame:[65]
		AliceW_Jump_Damage_B,                     //53	frame:[2]
		AliceW_Jump_Damage_A,                     //52	frame:[10]
		AliceW_Hysteria_On_2,                     //51	frame:[31]
		AliceW_Hysteria_On_1,                     //50	frame:[41]
		AliceW_Float_UP,                          //49	frame:[2]
		AliceW_Float_Right,                       //48	frame:[51]
		AliceW_Float_Reaction_FW,                 //47	frame:[46]
		AliceW_Float_Left,                        //46	frame:[51]
		AliceW_Float_Idle,                        //45	frame:[51]
		AliceW_Float_Forward,                     //44	frame:[51]
		AliceW_Float_Fail,                        //43	frame:[9]
		AliceW_Float_BackWard,                    //42	frame:[51]
		AliceW_Damg_Hy_2,                         //41	frame:[70]
		AliceW_Damg_Hy_1,                         //40	frame:[70]
		AliceW_Damg_Bk_2,                         //39	frame:[26]
		AliceW_Damg_Bk_1,                         //38	frame:[23]
		AliceW_Damg_Bk,                           //37	frame:[22]
		AliceW_Damage_Heavy_KnockBack,            //36	frame:[39]
		AliceW_beFreezen,                         //35	frame:[143]
		AliceW_Be_Grabed_Tank,                    //34	frame:[161]
		AliceW_Be_Grabed_HandGirl_E,              //33	frame:[72]
		AliceW_Be_Grabed_HandGirl_D,              //32	frame:[57]
		AliceW_Be_Grabed_HandGirl_C,              //31	frame:[11]
		AliceW_Be_Grabed_HandGirl_All,            //30	frame:[138]
		AliceW_Be_Grabed_HandBoy_E,               //29	frame:[71]
		AliceW_Be_Grabed_HandBoy_D,               //28	frame:[57]
		AliceW_Be_Grabed_HandBoy_C,               //27	frame:[11]
		AliceW_Be_Grabed_HandBoy_All,             //26	frame:[137]
		AliceW_Be_Grabed_CardGD,                  //25	frame:[216]
		AliceW_be_Attack_WaspInk,                 //24	frame:[57]
		ADD_Pose_WP2_Idle_Range_UB,               //23	frame:[2]
		ADD_AliceW_WP4_Strafe_Rgt,                //22	frame:[17]
		ADD_AliceW_WP4_Strafe_Lft,                //21	frame:[17]
		ADD_AliceW_WP4_Strafe_Fwd,                //20	frame:[17]
		ADD_AliceW_WP4_Strafe_Bk,                 //19	frame:[17]
		ADD_AliceW_WP4_Fire_2_UB_NoDelay,         //18	frame:[31]
		ADD_AliceW_WP4_Fire_2_UB,                 //17	frame:[46]
		ADD_AliceW_WP4_Fire_,                     //16	frame:[31]
		ADD_AliceW_WP4_Fire,                      //15	frame:[31]
		ADD_AliceW_WP4_Charge_Strafe_Rgt,         //14	frame:[33]
		ADD_AliceW_WP4_Charge_Strafe_Lft,         //13	frame:[33]
		ADD_AliceW_WP4_Charge_Strafe_Fwd,         //12	frame:[33]
		ADD_AliceW_WP4_Charge_Strafe_Bk,          //11	frame:[33]
		ADD_AliceW_WP4_Attack_Charging_UB,        //10	frame:[21]
		ADD_AliceW_WP3_Strafe_Rgt,                //9	frame:[19]
		ADD_AliceW_WP3_Strafe_Lft,                //8	frame:[19]
		ADD_AliceW_WP3_Strafe_Fwd,                //7	frame:[19]
		ADD_AliceW_WP3_Strafe_Bk,                 //6	frame:[19]
		ADD_AliceW_WP3_Release,                   //5	frame:[37]
		ADD_AliceW_WP3_NoAmmo,                    //4	frame:[13]
		ADD_AliceW_WP3_Fire,                      //3	frame:[37]
		ADD_AliceW_WP3_Attack_Idle,               //2	frame:[37]
		ADD_AliceW_Watch_Trigger,                 //1	frame:[16]
		ADD_AliceW_Watch_Hand,                    //0	frame:[2]
			
		ALICEW_ANIM_END
	};

	enum E_JUMPPAD_ANIM {
		GlowingJumpPad_ready,                     //0	frame:[7]
		GrowingJumpPad_Ground,                    //1	frame:[27]
		GrowingJumpPad_Launch,                    //2	frame:[15]
		GrowingJumpPad_UnderGround,               //3	frame:[2]

		JUMPPAD_END
	};

	enum E_SHRINKFLOWER_ANIM {
		ShrinkFlower_Attract,                     //0	frame:[36]
		ShrinkFlower_Eat,                         //1	frame:[21]
		ShrinkFlower_Idle,                        //2	frame:[141]
		ShrinkFlower_Kiss,                        //3	frame:[61]
		ShrinkFlower_Munch,                       //4	frame:[9]
		ShrinkFlower_Open,						//5	frame:[18]

		SHRINKFLOWER_END
	};

	enum E_MADCAP_ANIM {
		MadCap_Warn04,                            //67	frame:[31]
		MadCap_Warn03,                            //66	frame:[56]
		MadCap_Warn02,                            //65	frame:[56]
		MadCap_Warn01,                            //64	frame:[56]
		MadCap_WalkOutHut,                        //63	frame:[56]
		MadCap_Walk,                              //62	frame:[31]
		MadCap_WakeUp,                            //61	frame:[81]
		MadCap_Taunt03,                           //60	frame:[76]
		MadCap_Taunt02,                           //59	frame:[81]
		MadCap_Taunt01,                           //58	frame:[81]
		MadCap_Talk01,                            //57	frame:[81]
		MadCap_Strafe_Right_Protect,              //56	frame:[21]
		MadCap_Strafe_Right,                      //55	frame:[25]
		MadCap_Strafe_left_Protect,               //54	frame:[21]
		MadCap_Strafe_left,                       //53	frame:[25]
		MadCap_Strafe_Forward_Protect,            //52	frame:[21]
		MadCap_Strafe_Forward,                    //51	frame:[25]
		MadCap_Strafe_Back_Protect,               //50	frame:[21]
		MadCap_Strafe_Back_Attack,                //49	frame:[21]
		MadCap_Strafe_Back,                       //48	frame:[25]
		MadCap_Spawn_Dynamic_C,                   //47	frame:[34]
		MadCap_Spawn_Dynamic_B,                   //46	frame:[7]
		MadCap_Spawn_Dynamic_A,                   //45	frame:[13]
		MadCap_Sleep_Idle03,                      //44	frame:[101]
		MadCap_Sleep_Idle01,                      //43	frame:[81]
		MadCap_ScareRun,                          //42	frame:[33]
		MadCap_Scare,                             //41	frame:[35]
		MadCap_Run,                               //40	frame:[25]
		MadCap_JumpLand_Start,                    //39	frame:[13]
		MadCap_JumpLand_Rise,                     //38	frame:[7]
		MadCap_JumpLand_LandLow,                  //37	frame:[34]
		MadCap_JumpLand_Far_fall,                 //36	frame:[13]
		MadCap_JumpLand_fall,                     //35	frame:[13]
		MadCap_JumpLand,                          //34	frame:[56]
		MadCap_Jump_Start,                        //33	frame:[18]
		MadCap_Jump_Loop,                         //32	frame:[2]
		MadCap_Jump_Land,                         //31	frame:[39]
		MadCap_Jump,                              //30	frame:[23]
		MadCap_Idle_Long2,                        //29	frame:[151]
		MadCap_Idle_Long1,                        //28	frame:[121]
		MadCap_Idle,                              //27	frame:[91]
		MadCap_Flee,                              //26	frame:[21]
		Madcap_Dodge_Rgt,                         //25	frame:[27]
		Madcap_Dodge_Lft,                         //24	frame:[27]
		MadCap_Death02,                           //23	frame:[43]
		MadCap_Death01,                           //22	frame:[43]
		MadCap_Damaged_Light02,                   //21	frame:[26]
		MadCap_Damaged_Light01,                   //20	frame:[21]
		MadCap_Damaged_Heave02,                   //19	frame:[26]
		MadCap_Damaged_Heave01,                   //18	frame:[26]
		MadCap_Block_In,                          //17	frame:[6]
		MadCap_Block_Cycle,                       //16	frame:[21]
		MadCap_Attack_Spoon_Melee02_a,            //15	frame:[20]
		MadCap_Attack_Spoon_Melee01,              //14	frame:[35]
		MadCap_Attack_Melee03_C,                  //13	frame:[46]
		MadCap_Attack_Melee03_B,                  //12	frame:[11]
		MadCap_Attack_Melee03_A,                  //11	frame:[46]
		MadCap_Attack_Melee03,                    //10	frame:[101]
		MadCap_Attack_Melee02b,                   //9	frame:[43]
		MadCap_Attack_Idle_Turn_Right,            //8	frame:[27]
		MadCap_Attack_Idle_Turn_Left,             //7	frame:[27]
		MadCap_Attack_Idle_Protect_Trans,         //6	frame:[29]
		MadCap_Attack_Idle_Protect,               //5	frame:[41]
		MadCap_Attack_Idle,                       //4	frame:[106]
		Madcap_Attack_Fork_Tell,                  //3	frame:[33]
		MadCap_Attack_Fork_Melee02_b,             //2	frame:[34]
		MadCap_Attack_Fork_Melee02_a,             //1	frame:[25]
		MadCap_Attack_Death01,                    //0	frame:[41]
		MadCap_ROOT,

		MADCAP_END
	};

	enum E_BOLTERFLY_ANIM {
		Bolterfly_Attack_MeLee3,                  //0	frame:[7]
		Bolterfly_Idle_Damaged,                   //1	frame:[11]
		Bolterfly_Leeching,                       //2	frame:[41]
		Bolterfly_Leeching_Attach,                //3	frame:[7]
		Bolterfly_Leeching_Detach,                //4	frame:[35]
		Bolterfly_Sprint,                         //5	frame:[7]
		Bolterfly_Strafe_Lft,                     //6	frame:[17]
		Bolterfly_Strafe_Rgt,                     //7	frame:[17]
		Bolterfly_Walk,                           //8	frame:[9]
		Bolterfly_Walk_2,                         //9	frame:[49]
		BOLTERFLY_ANIM_END
	};

	// 미니 그룬트
	enum E_MINIGRUNT_ANIM {
		Grunt_Min_attack_01_A,                    //0	frame:[26]
		Grunt_Min_attack_01_B,                    //1	frame:[36]
		Grunt_Min_attack_01_C,                    //2	frame:[31]
		Grunt_Min_attack_01_D,                    //3	frame:[61]
		Grunt_Min_attack_02,                      //4	frame:[41]
		Grunt_Min_attack_02_A,                    //5	frame:[26]
		Grunt_Min_attack_02_B,                    //6	frame:[36]
		Grunt_Min_attack_02_C,                    //7	frame:[31]
		Grunt_Min_attack_02_D,                    //8	frame:[61]
		Grunt_Min_attack_05_A,                    //9	frame:[27]
		Grunt_Min_attack_05_B,                    //10	frame:[15]
		Grunt_Min_attack_06_A,                    //11	frame:[31]
		Grunt_Min_attack_06_B,                    //12	frame:[11]
		Grunt_Min_attack_Idle,                    //13	frame:[61]
		Grunt_Min_Damage_H,                       //14	frame:[51]
		Grunt_Min_Damage_H_2,                     //15	frame:[51]
		Grunt_Min_die_2,                          //16	frame:[101]
		Grunt_Min_die_3,                          //17	frame:[81]
		Grunt_Min_die_4,                          //18	frame:[96]
		Grunt_Min_Falling,                        //19	frame:[17]
		Grunt_Min_PreSpawn,                       //20	frame:[61]
		Grunt_Min_Spawn_2,                        //21	frame:[61]
		Grunt_Min_Spawn_3,                        //22	frame:[61]
		Grunt_Min_Spawn_4,                        //23	frame:[61]
		Grunt_Min_Strafe_3_B,                     //24	frame:[17]
		Grunt_Min_Strafe_3_F,                     //25	frame:[17]
		Grunt_Min_Strafe_3_Left,                  //26	frame:[17]
		Grunt_Min_Strafe_3_Right,                 //27	frame:[17]
		Grunt_Min_Strafe_B,                       //28	frame:[49]
		Grunt_Min_Strafe_F,                       //29	frame:[49]
		Grunt_Min_Strafe_Left,                    //30	frame:[49]
		Grunt_Min_Strafe_Right,                   //31	frame:[49]
		Grunt_Min_Taunt_1,                        //32	frame:[61]
		Grunt_Min_Taunt_3,                        //33	frame:[61]
		Grunt_Min_Taunt_4,                        //34	frame:[71]
		Grunt_Min_Warn,                           //35	frame:[81]
		Grunt_Mini_Damage_L,                      //36	frame:[23]
		Grunt_Mini_Damage_L_2,                    //37	frame:[31]
		Grunt_Mini_Damage_L_3,                    //38	frame:[31]
		MINIGRUNT_ANIM_END
	};

	enum E_BOSS_ANIM {
		Executioner_Turn_Rgt,                     //32	frame:[35]
		Executioner_Turn_Lft,                     //31	frame:[35]
		Executioner_Strafe_Fwd,                   //30	frame:[51]
		Executioner_Strafe_Bk,                    //29	frame:[51]
		Executioner_Run_01,                       //28	frame:[31]
		Executioner_Laugh03,                      //27	frame:[81]
		Executioner_Laugh02,                      //26	frame:[81]
		Executioner_Laugh01,                      //25	frame:[81]
		Executioner_KnockBack,                    //24	frame:[79]
		Executioner_Jump_Start,                   //23	frame:[13]
		Executioner_Jump_Rise,                    //22	frame:[7]
		Executioner_Jump_Land,                    //21	frame:[34]
		Executioner_Jump_Fall,                    //20	frame:[13]
		Executioner_Idle,                         //19	frame:[51]
		Executioner_Giant_Laugh03,                //18	frame:[81]
		Executioner_Giant_Laugh02,                //17	frame:[81]
		Executioner_Giant_Idle,                   //16	frame:[51]
		Executioner_Entrance_JumpDown,            //15	frame:[116]
		Executioner_Entrance_Fall_Start,          //14	frame:[36]
		Executioner_Entrance_Fall_Loop,           //13	frame:[20]
		Executioner_Entrance_Fall_Land,           //12	frame:[52]
		Executioner_Entrance_DoorBreak,           //11	frame:[94]
		Executioner_Entrance_BloodWall,           //10	frame:[253]
		Executioner_AttackSlam,                   //9	frame:[129]
		Executioner_Attack07,                     //8	frame:[181]
		Executioner_Attack06,                     //7	frame:[131]
		Executioner_Attack05_A,                   //6	frame:[73]
		Executioner_Attack05,                     //5	frame:[134]
		Executioner_Attack04,                     //4	frame:[153]
		Executioner_Attack03,                     //3	frame:[91]
		Executioner_Attack02,                     //2	frame:[75]
		Executioner_Attack01_Matinee,             //1	frame:[71]
		Executioner_Attack01,                     //0	frame:[71]
		Executioner_Run_Attack,
		BOSS_ANIM_END
	};

	// 고양이
	enum E_CAT_ANIM {
		Cat_Idle,                                 //0	frame:[181]
		Cat_Jump,                                 //1	frame:[33]
		Cat_Meow,                                 //2	frame:[47]
		Cat_Rub,                                  //3	frame:[111]
		Cat_Rub01,                                //4	frame:[161]
		Cat_Run,                                  //5	frame:[21]
		Cat_Walk,                                 //6	frame:[37]
		CAT_ANIM_END
	};

	enum E_BIGGUY_ANIM {
		BigGuy_LSP_Idle_a,                        //0	frame:[101]
		BigGuy_LSP_Idle_b,                        //1	frame:[295]
		BigGuy_LSP_Sit_Idle,                      //2	frame:[490]
		BigGuy_LSP_Talk_a,                        //3	frame:[326]
		BigGuy_LSP_Talk_b,                        //4	frame:[221]
		Bigguy_LSP_Walk,                          //5	frame:[40]
		E_BIGGUY_ANIM
	};

	// 후추분쇄기
	enum E_WP3_ANIM {
		WP3_Attack_Alt_A,                         //0	frame:[26]
		WP3_Attack_Alt_B,                         //1	frame:[36]
		WP3_Charge,                               //2	frame:[25]
		WP3_Fire,                                 //3	frame:[37]
		WP3_FireFail,                             //4	frame:[19]
		WP3_Idle_Pose,                            //5	frame:[21]
		WP3_NoAmmo,                               //6	frame:[14]
		WP3_Release,                              //7	frame:[37]

		WP3_END
	};

	// 토끼폭탄
	enum  E_BUNNYBOMB_ANIM {
		ClockworkBomb_ClockWork,                  //0	frame:[25]
		ClockworkBomb_ClockWork_Critical,         //1	frame:[9]
		ClockworkBomb_Fall,                       //2	frame:[9]
		ClockworkBomb_Land,                       //3	frame:[56]
		ClockworkBomb_Spawn,                      //4	frame:[34]
		ClockworkBomb_Spawn_Put,                  //5	frame:[52]
		BUNNYBOMB_END
	};

	// 뼈상어
	enum E_SHARK_ID {
		Shark_Charging,                           //0	frame:[61]
		Shark_Hit,                                //1	frame:[68]
		Shark_Stuck,                              //2	frame:[41]
		Shark_swim,                               //3	frame:[121]
		Shark_Swim_Fast,                          //4	frame:[61]
		Shark_Turn_Left,                          //5	frame:[76]
		Shark_Turn_Right,                         //6	frame:[76]
		SHARK_END
	};
};


	