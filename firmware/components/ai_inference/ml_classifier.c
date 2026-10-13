/**
 * @file ml_classifier.c
 * @brief Embedded AI inference implementation for SENTINEL-BELT.
 * Auto-transpiled from trained Random Forest ensemble (n_estimators=15).
 */

#include "model_weights.h"
#include <math.h>

static int predict_tree_0(const float features[ML_FEATURE_COUNT]) {
    if (features[0] <= 19.5034f) { /* O2 */
        if (features[11] <= 0.0257f) { /* dH2S_dt */
            if (features[4] <= 25.1225f) { /* Temp */
                if (features[0] <= 16.0027f) { /* O2 */
                    if (features[0] <= 7.2171f) { /* O2 */
                        return 1;
                    } else {
                        if (features[2] <= 0.2650f) { /* CO */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                } else {
                    if (features[10] <= -0.0113f) { /* dO2_dt */
                        return 1;
                    } else {
                        return 1;
                    }
                }
            } else {
                return 1;
            }
        } else {
            if (features[1] <= 2.4884f) { /* H2S */
                return 1;
            } else {
                return 2;
            }
        }
    } else {
        if (features[3] <= 25.2305f) { /* CH4 */
            if (features[7] <= 0.4311f) { /* Ax */
                if (features[1] <= 10.0596f) { /* H2S */
                    if (features[11] <= 0.0513f) { /* dH2S_dt */
                        if (features[6] <= 59.8950f) { /* WaterLevel */
                            if (features[12] <= 0.3789f) { /* dCO_dt */
                                if (features[2] <= 25.7476f) { /* CO */
                                    if (features[7] <= -0.0378f) { /* Ax */
                                        return 0;
                                    } else {
                                        if (features[5] <= 86.0111f) { /* Humidity */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[8] <= -0.0231f) { /* Ay */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[0] <= 20.7663f) { /* O2 */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[0] <= 20.7989f) { /* O2 */
                            if (features[5] <= 64.2153f) { /* Humidity */
                                if (features[10] <= -0.0071f) { /* dO2_dt */
                                    return 2;
                                } else {
                                    return 0;
                                }
                            } else {
                                if (features[8] <= -0.0004f) { /* Ay */
                                    if (features[6] <= 6.3293f) { /* WaterLevel */
                                        return 0;
                                    } else {
                                        return 2;
                                    }
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[10] <= -0.0071f) { /* dO2_dt */
                                return 2;
                            } else {
                                if (features[11] <= 0.0990f) { /* dH2S_dt */
                                    return 0;
                                } else {
                                    if (features[5] <= 65.1760f) { /* Humidity */
                                        if (features[7] <= 0.0597f) { /* Ax */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (features[7] <= 0.0525f) { /* Ax */
                        if (features[1] <= 516.2057f) { /* H2S */
                            if (features[5] <= 64.8391f) { /* Humidity */
                                if (features[5] <= 64.8074f) { /* Humidity */
                                    if (features[11] <= 0.1543f) { /* dH2S_dt */
                                        return 1;
                                    } else {
                                        return 2;
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[6] <= 4.6610f) { /* WaterLevel */
                                    if (features[4] <= 25.6022f) { /* Temp */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[7] <= 0.0362f) { /* Ax */
                                        if (features[4] <= 25.5095f) { /* Temp */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[0] <= 20.9439f) { /* O2 */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[11] <= 0.1546f) { /* dH2S_dt */
                            return 1;
                        } else {
                            if (features[1] <= 516.6874f) { /* H2S */
                                return 2;
                            } else {
                                return 1;
                            }
                        }
                    }
                }
            } else {
                if (features[4] <= 24.0673f) { /* Temp */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[7] <= 0.0136f) { /* Ax */
                return 2;
            } else {
                return 2;
            }
        }
    }
}

static int predict_tree_1(const float features[ML_FEATURE_COUNT]) {
    if (features[11] <= 0.0513f) { /* dH2S_dt */
        if (features[9] <= 0.4745f) { /* Az */
            return 2;
        } else {
            if (features[2] <= 49.1146f) { /* CO */
                if (features[0] <= 19.4616f) { /* O2 */
                    if (features[4] <= 25.0554f) { /* Temp */
                        if (features[9] <= 0.9514f) { /* Az */
                            if (features[8] <= 0.0409f) { /* Ay */
                                if (features[2] <= 0.4580f) { /* CO */
                                    return 2;
                                } else {
                                    if (features[9] <= 0.9451f) { /* Az */
                                        if (features[10] <= -0.0116f) { /* dO2_dt */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            if (features[6] <= 5.3520f) { /* WaterLevel */
                                if (features[3] <= 0.2556f) { /* CH4 */
                                    if (features[6] <= 5.2922f) { /* WaterLevel */
                                        if (features[6] <= 4.7643f) { /* WaterLevel */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[8] <= -0.0023f) { /* Ay */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[8] <= -0.0050f) { /* Ay */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        if (features[7] <= 0.0050f) { /* Ax */
                            if (features[3] <= 0.0683f) { /* CH4 */
                                return 1;
                            } else {
                                if (features[6] <= 5.0914f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 1;
                                }
                            }
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[13] <= 0.2082f) { /* dCH4_dt */
                        if (features[6] <= 59.7902f) { /* WaterLevel */
                            if (features[8] <= -0.0353f) { /* Ay */
                                if (features[3] <= 5.2777f) { /* CH4 */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[7] <= -0.0396f) { /* Ax */
                                    return 0;
                                } else {
                                    if (features[4] <= 25.6361f) { /* Temp */
                                        if (features[0] <= 20.7903f) { /* O2 */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[3] <= 9.7333f) { /* CH4 */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (features[1] <= 0.0031f) { /* H2S */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[7] <= 0.0072f) { /* Ax */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                }
            } else {
                if (features[3] <= 0.0018f) { /* CH4 */
                    return 2;
                } else {
                    return 2;
                }
            }
        }
    } else {
        if (features[4] <= 25.4457f) { /* Temp */
            if (features[10] <= -1.2071f) { /* dO2_dt */
                return 1;
            } else {
                if (features[10] <= 12.9500f) { /* dO2_dt */
                    if (features[12] <= 0.5000f) { /* dCO_dt */
                        if (features[9] <= 1.0203f) { /* Az */
                            return 2;
                        } else {
                            if (features[9] <= 1.0209f) { /* Az */
                                return 1;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        return 0;
                    }
                } else {
                    return 1;
                }
            }
        } else {
            if (features[0] <= 20.1920f) { /* O2 */
                return 2;
            } else {
                if (features[5] <= 64.9924f) { /* Humidity */
                    if (features[1] <= 9.9176f) { /* H2S */
                        return 0;
                    } else {
                        return 1;
                    }
                } else {
                    if (features[1] <= 10.0678f) { /* H2S */
                        if (features[2] <= 0.1251f) { /* CO */
                            return 0;
                        } else {
                            return 0;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        }
    }
}

static int predict_tree_2(const float features[ML_FEATURE_COUNT]) {
    if (features[12] <= 0.3799f) { /* dCO_dt */
        if (features[1] <= 0.7897f) { /* H2S */
            if (features[3] <= 25.0083f) { /* CH4 */
                if (features[9] <= 0.1936f) { /* Az */
                    return 2;
                } else {
                    if (features[4] <= 25.6391f) { /* Temp */
                        if (features[10] <= -0.0226f) { /* dO2_dt */
                            if (features[10] <= -12.9620f) { /* dO2_dt */
                                return 1;
                            } else {
                                if (features[1] <= 0.0046f) { /* H2S */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[13] <= 0.0020f) { /* dCH4_dt */
                                if (features[8] <= -0.0255f) { /* Ay */
                                    if (features[4] <= 25.5971f) { /* Temp */
                                        return 0;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[7] <= -0.0207f) { /* Ax */
                                        if (features[1] <= 0.1802f) { /* H2S */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[7] <= 0.0829f) { /* Ax */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    }
                                }
                            } else {
                                return 0;
                            }
                        }
                    } else {
                        if (features[9] <= 1.0312f) { /* Az */
                            if (features[0] <= 19.4568f) { /* O2 */
                                return 1;
                            } else {
                                if (features[4] <= 37.9121f) { /* Temp */
                                    if (features[5] <= 66.4014f) { /* Humidity */
                                        if (features[6] <= 30.4993f) { /* WaterLevel */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    if (features[7] <= 0.0126f) { /* Ax */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[0] <= 20.8689f) { /* O2 */
                                if (features[3] <= 0.0219f) { /* CH4 */
                                    return 0;
                                } else {
                                    if (features[0] <= 20.8198f) { /* O2 */
                                        return 0;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                return 0;
                            }
                        }
                    }
                }
            } else {
                if (features[9] <= 0.9308f) { /* Az */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[6] <= 5.5675f) { /* WaterLevel */
                if (features[11] <= 0.1503f) { /* dH2S_dt */
                    if (features[11] <= 0.1063f) { /* dH2S_dt */
                        if (features[10] <= -0.0120f) { /* dO2_dt */
                            return 2;
                        } else {
                            if (features[4] <= 25.6039f) { /* Temp */
                                if (features[11] <= 0.0136f) { /* dH2S_dt */
                                    return 1;
                                } else {
                                    if (features[11] <= 0.0291f) { /* dH2S_dt */
                                        return 0;
                                    } else {
                                        return 0;
                                    }
                                }
                            } else {
                                if (features[4] <= 25.6524f) { /* Temp */
                                    return 0;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[0] <= 20.8184f) { /* O2 */
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[10] <= 12.9500f) { /* dO2_dt */
                        if (features[1] <= 516.6219f) { /* H2S */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[1] <= 5.2968f) { /* H2S */
                    if (features[1] <= 5.2109f) { /* H2S */
                        return 2;
                    } else {
                        return 0;
                    }
                } else {
                    return 2;
                }
            }
        }
    } else {
        if (features[9] <= 1.0083f) { /* Az */
            if (features[2] <= 1.1782f) { /* CO */
                if (features[1] <= 0.0398f) { /* H2S */
                    return 0;
                } else {
                    return 1;
                }
            } else {
                if (features[8] <= 0.0881f) { /* Ay */
                    if (features[2] <= 25.5511f) { /* CO */
                        if (features[8] <= 0.0160f) { /* Ay */
                            return 2;
                        } else {
                            return 0;
                        }
                    } else {
                        return 2;
                    }
                } else {
                    return 0;
                }
            }
        } else {
            if (features[7] <= 0.0274f) { /* Ax */
                return 0;
            } else {
                return 0;
            }
        }
    }
}

static int predict_tree_3(const float features[ML_FEATURE_COUNT]) {
    if (features[0] <= 19.4748f) { /* O2 */
        if (features[6] <= 11.8634f) { /* WaterLevel */
            if (features[6] <= 5.3141f) { /* WaterLevel */
                if (features[9] <= 0.9942f) { /* Az */
                    if (features[10] <= -0.0226f) { /* dO2_dt */
                        if (features[4] <= 25.1661f) { /* Temp */
                            if (features[12] <= 0.5000f) { /* dCO_dt */
                                if (features[6] <= 4.4999f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            if (features[10] <= -12.9614f) { /* dO2_dt */
                                if (features[11] <= -499.4500f) { /* dH2S_dt */
                                    return 1;
                                } else {
                                    return 1;
                                }
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[8] <= 0.0202f) { /* Ay */
                            if (features[6] <= 5.2296f) { /* WaterLevel */
                                return 1;
                            } else {
                                if (features[6] <= 5.2417f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    if (features[7] <= 0.0264f) { /* Ax */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[0] <= 15.9746f) { /* O2 */
                        if (features[10] <= -0.0115f) { /* dO2_dt */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[8] <= -0.0294f) { /* Ay */
                    return 2;
                } else {
                    if (features[4] <= 24.2424f) { /* Temp */
                        if (features[2] <= 0.9391f) { /* CO */
                            return 1;
                        } else {
                            return 2;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        } else {
            if (features[0] <= 15.9363f) { /* O2 */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[12] <= 0.3804f) { /* dCO_dt */
            if (features[3] <= 25.0083f) { /* CH4 */
                if (features[8] <= 0.3674f) { /* Ay */
                    if (features[11] <= 0.1497f) { /* dH2S_dt */
                        if (features[6] <= 60.1946f) { /* WaterLevel */
                            if (features[1] <= 10.0596f) { /* H2S */
                                if (features[3] <= 9.9813f) { /* CH4 */
                                    if (features[4] <= 37.9005f) { /* Temp */
                                        if (features[10] <= -0.0142f) { /* dO2_dt */
                                            return 2;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            if (features[4] <= 24.0785f) { /* Temp */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[8] <= -0.0144f) { /* Ay */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                } else {
                    if (features[1] <= 0.0029f) { /* H2S */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            } else {
                if (features[2] <= 0.0082f) { /* CO */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[11] <= 0.0500f) { /* dH2S_dt */
                return 2;
            } else {
                if (features[1] <= 0.0362f) { /* H2S */
                    return 0;
                } else {
                    return 0;
                }
            }
        }
    }
}

static int predict_tree_4(const float features[ML_FEATURE_COUNT]) {
    if (features[10] <= -0.0143f) { /* dO2_dt */
        if (features[10] <= -0.0143f) { /* dO2_dt */
            if (features[10] <= -0.0226f) { /* dO2_dt */
                if (features[4] <= 25.3602f) { /* Temp */
                    if (features[3] <= 0.3309f) { /* CH4 */
                        if (features[7] <= 0.0168f) { /* Ax */
                            return 2;
                        } else {
                            return 2;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[5] <= 64.9662f) { /* Humidity */
                        return 1;
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[10] <= -0.0169f) { /* dO2_dt */
                    return 1;
                } else {
                    if (features[5] <= 63.9605f) { /* Humidity */
                        return 0;
                    } else {
                        return 0;
                    }
                }
            }
        } else {
            if (features[3] <= 0.0008f) { /* CH4 */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[7] <= 0.4311f) { /* Ax */
            if (features[4] <= 25.6654f) { /* Temp */
                if (features[3] <= 19.3407f) { /* CH4 */
                    if (features[12] <= 0.3470f) { /* dCO_dt */
                        if (features[9] <= 0.9217f) { /* Az */
                            return 0;
                        } else {
                            if (features[6] <= 59.9910f) { /* WaterLevel */
                                if (features[13] <= 0.0020f) { /* dCH4_dt */
                                    if (features[12] <= 0.0257f) { /* dCO_dt */
                                        if (features[9] <= 1.0345f) { /* Az */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    return 0;
                                }
                            } else {
                                if (features[4] <= 23.9619f) { /* Temp */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        }
                    } else {
                        if (features[11] <= 0.0500f) { /* dH2S_dt */
                            return 2;
                        } else {
                            if (features[9] <= 0.9632f) { /* Az */
                                return 2;
                            } else {
                                if (features[3] <= 0.0051f) { /* CH4 */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    }
                } else {
                    if (features[3] <= 29.0723f) { /* CH4 */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            } else {
                if (features[0] <= 20.8008f) { /* O2 */
                    if (features[9] <= 1.0601f) { /* Az */
                        if (features[7] <= 0.0072f) { /* Ax */
                            return 0;
                        } else {
                            if (features[4] <= 25.7502f) { /* Temp */
                                if (features[9] <= 0.9857f) { /* Az */
                                    return 1;
                                } else {
                                    if (features[0] <= 19.6349f) { /* O2 */
                                        if (features[1] <= 0.0983f) { /* H2S */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[4] <= 25.7084f) { /* Temp */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    }
                                }
                            } else {
                                if (features[7] <= 0.0827f) { /* Ax */
                                    return 1;
                                } else {
                                    if (features[9] <= 0.9705f) { /* Az */
                                        return 0;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        }
                    } else {
                        return 0;
                    }
                } else {
                    if (features[2] <= 48.7330f) { /* CO */
                        if (features[5] <= 86.0111f) { /* Humidity */
                            if (features[11] <= 0.1063f) { /* dH2S_dt */
                                if (features[5] <= 66.8640f) { /* Humidity */
                                    if (features[13] <= 0.1389f) { /* dCH4_dt */
                                        if (features[6] <= 31.2091f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[13] <= 0.2070f) { /* dCH4_dt */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[8] <= 0.0397f) { /* Ay */
                                        return 0;
                                    } else {
                                        if (features[7] <= 0.0785f) { /* Ax */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            } else {
                                if (features[8] <= -0.0197f) { /* Ay */
                                    return 1;
                                } else {
                                    return 1;
                                }
                            }
                        } else {
                            if (features[5] <= 86.8049f) { /* Humidity */
                                if (features[4] <= 37.7624f) { /* Temp */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        if (features[3] <= 0.0932f) { /* CH4 */
                            return 2;
                        } else {
                            if (features[3] <= 0.1209f) { /* CH4 */
                                return 1;
                            } else {
                                return 2;
                            }
                        }
                    }
                }
            }
        } else {
            if (features[2] <= 0.0425f) { /* CO */
                return 2;
            } else {
                return 2;
            }
        }
    }
}

static int predict_tree_5(const float features[ML_FEATURE_COUNT]) {
    if (features[7] <= 0.4311f) { /* Ax */
        if (features[1] <= 1.6841f) { /* H2S */
            if (features[13] <= 0.2090f) { /* dCH4_dt */
                if (features[12] <= 0.3804f) { /* dCO_dt */
                    if (features[4] <= 25.5998f) { /* Temp */
                        if (features[4] <= 23.9464f) { /* Temp */
                            if (features[8] <= 0.0418f) { /* Ay */
                                if (features[8] <= 0.0055f) { /* Ay */
                                    return 0;
                                } else {
                                    if (features[2] <= 1.0175f) { /* CO */
                                        return 2;
                                    } else {
                                        if (features[4] <= 23.9230f) { /* Temp */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    }
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            if (features[12] <= 0.0257f) { /* dCO_dt */
                                if (features[10] <= -0.0227f) { /* dO2_dt */
                                    return 2;
                                } else {
                                    if (features[0] <= 19.4972f) { /* O2 */
                                        return 1;
                                    } else {
                                        if (features[0] <= 20.5059f) { /* O2 */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    }
                                }
                            } else {
                                return 0;
                            }
                        }
                    } else {
                        if (features[4] <= 38.2499f) { /* Temp */
                            if (features[6] <= 30.6419f) { /* WaterLevel */
                                if (features[5] <= 66.8407f) { /* Humidity */
                                    if (features[9] <= 1.0269f) { /* Az */
                                        if (features[3] <= 9.9325f) { /* CH4 */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    if (features[5] <= 86.0111f) { /* Humidity */
                                        return 0;
                                    } else {
                                        if (features[3] <= 0.0738f) { /* CH4 */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    }
                                }
                            } else {
                                if (features[6] <= 59.9057f) { /* WaterLevel */
                                    return 1;
                                } else {
                                    if (features[6] <= 61.2212f) { /* WaterLevel */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            }
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[11] <= 0.0500f) { /* dH2S_dt */
                        return 2;
                    } else {
                        if (features[4] <= 23.9412f) { /* Temp */
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[1] <= 0.0012f) { /* H2S */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[10] <= -0.0071f) { /* dO2_dt */
                return 2;
            } else {
                if (features[2] <= 0.8414f) { /* CO */
                    if (features[9] <= 0.9962f) { /* Az */
                        if (features[0] <= 20.8064f) { /* O2 */
                            return 0;
                        } else {
                            if (features[8] <= 0.0446f) { /* Ay */
                                if (features[6] <= 5.3047f) { /* WaterLevel */
                                    if (features[0] <= 20.8793f) { /* O2 */
                                        if (features[3] <= 0.1219f) { /* CH4 */
                                            return 2;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[4] <= 25.3139f) { /* Temp */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[3] <= 0.0579f) { /* CH4 */
                                        if (features[9] <= 0.9668f) { /* Az */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        return 0;
                                    }
                                }
                            } else {
                                if (features[4] <= 24.6950f) { /* Temp */
                                    return 2;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[1] <= 516.4269f) { /* H2S */
                            if (features[0] <= 20.9642f) { /* O2 */
                                if (features[2] <= 0.2381f) { /* CO */
                                    return 1;
                                } else {
                                    if (features[1] <= 12.1889f) { /* H2S */
                                        return 0;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[4] <= 25.3755f) { /* Temp */
                        if (features[0] <= 20.8711f) { /* O2 */
                            if (features[6] <= 4.7402f) { /* WaterLevel */
                                return 1;
                            } else {
                                if (features[11] <= 0.0753f) { /* dH2S_dt */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[4] <= 25.2863f) { /* Temp */
                                if (features[11] <= 0.0764f) { /* dH2S_dt */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            } else {
                                if (features[9] <= 0.9840f) { /* Az */
                                    return 0;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[11] <= 0.1060f) { /* dH2S_dt */
                            if (features[1] <= 504.0907f) { /* H2S */
                                if (features[0] <= 20.7661f) { /* O2 */
                                    return 0;
                                } else {
                                    return 0;
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            return 1;
                        }
                    }
                }
            }
        }
    } else {
        if (features[6] <= 4.2604f) { /* WaterLevel */
            return 2;
        } else {
            return 2;
        }
    }
}

static int predict_tree_6(const float features[ML_FEATURE_COUNT]) {
    if (features[11] <= 0.0513f) { /* dH2S_dt */
        if (features[0] <= 19.4616f) { /* O2 */
            if (features[10] <= -0.0226f) { /* dO2_dt */
                if (features[11] <= -499.4500f) { /* dH2S_dt */
                    return 1;
                } else {
                    if (features[4] <= 25.4131f) { /* Temp */
                        return 2;
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[10] <= -0.0204f) { /* dO2_dt */
                    if (features[3] <= 0.0048f) { /* CH4 */
                        if (features[0] <= 15.3210f) { /* O2 */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[2] <= 0.8644f) { /* CO */
                        if (features[6] <= 4.5872f) { /* WaterLevel */
                            if (features[9] <= 0.9802f) { /* Az */
                                return 2;
                            } else {
                                return 1;
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[6] <= 4.0919f) { /* WaterLevel */
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[7] <= 0.4216f) { /* Ax */
                if (features[4] <= 25.7218f) { /* Temp */
                    if (features[1] <= 500.6335f) { /* H2S */
                        if (features[8] <= -0.0353f) { /* Ay */
                            return 0;
                        } else {
                            if (features[2] <= 43.2691f) { /* CO */
                                if (features[3] <= 18.2814f) { /* CH4 */
                                    if (features[12] <= 0.0291f) { /* dCO_dt */
                                        if (features[10] <= -0.0023f) { /* dO2_dt */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[3] <= 0.0001f) { /* CH4 */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    }
                                } else {
                                    if (features[4] <= 24.0084f) { /* Temp */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                if (features[8] <= -0.0135f) { /* Ay */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[7] <= 0.0271f) { /* Ax */
                        if (features[4] <= 25.8795f) { /* Temp */
                            if (features[0] <= 20.9403f) { /* O2 */
                                if (features[12] <= 0.1588f) { /* dCO_dt */
                                    if (features[8] <= 0.0083f) { /* Ay */
                                        if (features[8] <= -0.0339f) { /* Ay */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[7] <= 0.0247f) { /* Ax */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[4] <= 25.8612f) { /* Temp */
                                    if (features[5] <= 64.6656f) { /* Humidity */
                                        return 0;
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    if (features[1] <= 0.0478f) { /* H2S */
                                        return 0;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[12] <= 0.1697f) { /* dCO_dt */
                                if (features[5] <= 86.8140f) { /* Humidity */
                                    if (features[3] <= 8.5344f) { /* CH4 */
                                        if (features[2] <= 0.9627f) { /* CO */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[6] <= 4.8675f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 1;
                                }
                            }
                        }
                    } else {
                        if (features[2] <= 50.6642f) { /* CO */
                            if (features[8] <= -0.0396f) { /* Ay */
                                return 0;
                            } else {
                                if (features[12] <= 0.2973f) { /* dCO_dt */
                                    if (features[4] <= 37.9005f) { /* Temp */
                                        if (features[3] <= 9.9062f) { /* CH4 */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[5] <= 86.0111f) { /* Humidity */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[1] <= 0.0170f) { /* H2S */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[3] <= 0.0031f) { /* CH4 */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    }
                }
            } else {
                if (features[8] <= 0.6044f) { /* Ay */
                    return 2;
                } else {
                    return 2;
                }
            }
        }
    } else {
        if (features[4] <= 25.4483f) { /* Temp */
            if (features[12] <= 0.5000f) { /* dCO_dt */
                if (features[10] <= 12.9500f) { /* dO2_dt */
                    if (features[0] <= 20.9978f) { /* O2 */
                        if (features[6] <= 5.0816f) { /* WaterLevel */
                            if (features[11] <= 0.1511f) { /* dH2S_dt */
                                return 1;
                            } else {
                                return 2;
                            }
                        } else {
                            return 2;
                        }
                    } else {
                        if (features[0] <= 21.0088f) { /* O2 */
                            return 1;
                        } else {
                            return 2;
                        }
                    }
                } else {
                    return 1;
                }
            } else {
                if (features[9] <= 0.9580f) { /* Az */
                    return 2;
                } else {
                    if (features[1] <= 0.3493f) { /* H2S */
                        return 0;
                    } else {
                        return 1;
                    }
                }
            }
        } else {
            if (features[0] <= 20.1771f) { /* O2 */
                return 2;
            } else {
                if (features[0] <= 20.9118f) { /* O2 */
                    if (features[8] <= 0.0031f) { /* Ay */
                        if (features[1] <= 8.8105f) { /* H2S */
                            return 0;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[11] <= 0.1049f) { /* dH2S_dt */
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[8] <= 0.0481f) { /* Ay */
                        if (features[9] <= 0.9674f) { /* Az */
                            if (features[2] <= 0.8902f) { /* CO */
                                return 0;
                            } else {
                                return 1;
                            }
                        } else {
                            if (features[1] <= 9.8346f) { /* H2S */
                                return 0;
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        return 0;
                    }
                }
            }
        }
    }
}

static int predict_tree_7(const float features[ML_FEATURE_COUNT]) {
    if (features[6] <= 5.9381f) { /* WaterLevel */
        if (features[8] <= 0.3599f) { /* Ay */
            if (features[1] <= 9.7362f) { /* H2S */
                if (features[12] <= 0.3799f) { /* dCO_dt */
                    if (features[7] <= -0.0207f) { /* Ax */
                        if (features[5] <= 65.3876f) { /* Humidity */
                            if (features[0] <= 20.8629f) { /* O2 */
                                if (features[7] <= -0.0287f) { /* Ax */
                                    return 0;
                                } else {
                                    if (features[2] <= 0.7838f) { /* CO */
                                        return 0;
                                    } else {
                                        if (features[4] <= 25.0944f) { /* Temp */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            if (features[0] <= 20.8601f) { /* O2 */
                                if (features[2] <= 1.3694f) { /* CO */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[8] <= -0.0020f) { /* Ay */
                                    if (features[4] <= 25.2585f) { /* Temp */
                                        if (features[3] <= 0.1230f) { /* CH4 */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[10] <= -0.0226f) { /* dO2_dt */
                            if (features[10] <= -12.9620f) { /* dO2_dt */
                                return 1;
                            } else {
                                return 2;
                            }
                        } else {
                            if (features[13] <= 0.2097f) { /* dCH4_dt */
                                if (features[4] <= 25.5755f) { /* Temp */
                                    if (features[3] <= 0.4183f) { /* CH4 */
                                        if (features[0] <= 19.0620f) { /* O2 */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    if (features[4] <= 37.9554f) { /* Temp */
                                        if (features[5] <= 66.4463f) { /* Humidity */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[6] <= 4.6218f) { /* WaterLevel */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            } else {
                                if (features[6] <= 4.5753f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        }
                    }
                } else {
                    if (features[12] <= 0.7562f) { /* dCO_dt */
                        return 2;
                    } else {
                        if (features[2] <= 1.2705f) { /* CO */
                            if (features[0] <= 20.9524f) { /* O2 */
                                if (features[6] <= 5.2954f) { /* WaterLevel */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            return 0;
                        }
                    }
                }
            } else {
                if (features[11] <= 0.1497f) { /* dH2S_dt */
                    if (features[5] <= 63.6803f) { /* Humidity */
                        return 1;
                    } else {
                        return 1;
                    }
                } else {
                    if (features[1] <= 516.6874f) { /* H2S */
                        if (features[5] <= 64.2785f) { /* Humidity */
                            return 2;
                        } else {
                            return 2;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        } else {
            if (features[9] <= 0.0792f) { /* Az */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[8] <= -0.0132f) { /* Ay */
            if (features[10] <= -0.0071f) { /* dO2_dt */
                return 2;
            } else {
                if (features[8] <= -0.0203f) { /* Ay */
                    if (features[8] <= -0.0225f) { /* Ay */
                        return 1;
                    } else {
                        return 2;
                    }
                } else {
                    return 1;
                }
            }
        } else {
            if (features[4] <= 25.7774f) { /* Temp */
                if (features[10] <= -0.0123f) { /* dO2_dt */
                    return 2;
                } else {
                    if (features[9] <= 1.0108f) { /* Az */
                        if (features[4] <= 25.6342f) { /* Temp */
                            if (features[1] <= 0.2725f) { /* H2S */
                                if (features[3] <= 0.2045f) { /* CH4 */
                                    if (features[5] <= 63.8182f) { /* Humidity */
                                        return 0;
                                    } else {
                                        if (features[0] <= 20.9482f) { /* O2 */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[6] <= 48.5132f) { /* WaterLevel */
                                        if (features[9] <= 0.5335f) { /* Az */
                                            return 2;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                if (features[1] <= 0.3361f) { /* H2S */
                                    if (features[9] <= 0.9893f) { /* Az */
                                        return 0;
                                    } else {
                                        if (features[3] <= 0.1668f) { /* CH4 */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[1] <= 0.5124f) { /* H2S */
                                        if (features[2] <= 1.8115f) { /* CO */
                                            return 2;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 0;
                                    }
                                }
                            }
                        } else {
                            if (features[5] <= 64.5133f) { /* Humidity */
                                if (features[1] <= 0.0463f) { /* H2S */
                                    return 2;
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[6] <= 59.5444f) { /* WaterLevel */
                                    if (features[7] <= 0.0423f) { /* Ax */
                                        if (features[6] <= 31.3947f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[9] <= 0.9772f) { /* Az */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            }
                        }
                    } else {
                        if (features[6] <= 27.1036f) { /* WaterLevel */
                            return 0;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[9] <= 0.9519f) { /* Az */
                    if (features[5] <= 65.3763f) { /* Humidity */
                        return 2;
                    } else {
                        return 1;
                    }
                } else {
                    if (features[10] <= -0.0071f) { /* dO2_dt */
                        return 2;
                    } else {
                        if (features[1] <= 0.0952f) { /* H2S */
                            return 1;
                        } else {
                            if (features[3] <= 0.1347f) { /* CH4 */
                                return 1;
                            } else {
                                if (features[6] <= 30.7132f) { /* WaterLevel */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

static int predict_tree_8(const float features[ML_FEATURE_COUNT]) {
    if (features[0] <= 19.4568f) { /* O2 */
        if (features[0] <= 18.4019f) { /* O2 */
            if (features[5] <= 64.1214f) { /* Humidity */
                if (features[8] <= 0.0050f) { /* Ay */
                    return 1;
                } else {
                    if (features[10] <= -0.0178f) { /* dO2_dt */
                        return 1;
                    } else {
                        return 2;
                    }
                }
            } else {
                if (features[9] <= 0.9519f) { /* Az */
                    if (features[11] <= 0.0257f) { /* dH2S_dt */
                        if (features[7] <= 0.0362f) { /* Ax */
                            if (features[8] <= 0.0092f) { /* Ay */
                                return 1;
                            } else {
                                return 2;
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        return 2;
                    }
                } else {
                    if (features[4] <= 25.1226f) { /* Temp */
                        if (features[6] <= 22.9485f) { /* WaterLevel */
                            if (features[7] <= 0.0315f) { /* Ax */
                                if (features[3] <= 0.0545f) { /* CH4 */
                                    if (features[6] <= 5.3181f) { /* WaterLevel */
                                        if (features[10] <= -0.0114f) { /* dO2_dt */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[10] <= -0.0226f) { /* dO2_dt */
                                    return 2;
                                } else {
                                    if (features[1] <= 0.1179f) { /* H2S */
                                        return 1;
                                    } else {
                                        if (features[9] <= 0.9834f) { /* Az */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                }
                            }
                        } else {
                            return 2;
                        }
                    } else {
                        if (features[6] <= 16.6657f) { /* WaterLevel */
                            if (features[5] <= 65.8656f) { /* Humidity */
                                return 1;
                            } else {
                                if (features[2] <= 0.5958f) { /* CO */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            return 2;
                        }
                    }
                }
            }
        } else {
            if (features[10] <= -0.0071f) { /* dO2_dt */
                if (features[8] <= 0.0168f) { /* Ay */
                    if (features[1] <= 3.1357f) { /* H2S */
                        return 1;
                    } else {
                        return 2;
                    }
                } else {
                    if (features[6] <= 11.6047f) { /* WaterLevel */
                        return 1;
                    } else {
                        if (features[6] <= 18.1158f) { /* WaterLevel */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                }
            } else {
                if (features[0] <= 18.4047f) { /* O2 */
                    return 1;
                } else {
                    return 1;
                }
            }
        }
    } else {
        if (features[9] <= 0.1940f) { /* Az */
            return 2;
        } else {
            if (features[2] <= 49.5510f) { /* CO */
                if (features[4] <= 25.6572f) { /* Temp */
                    if (features[11] <= 0.0513f) { /* dH2S_dt */
                        if (features[1] <= 500.5923f) { /* H2S */
                            if (features[3] <= 18.6449f) { /* CH4 */
                                if (features[3] <= 0.4150f) { /* CH4 */
                                    if (features[7] <= -0.0331f) { /* Ax */
                                        if (features[9] <= 0.9701f) { /* Az */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[4] <= 25.4350f) { /* Temp */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    }
                                } else {
                                    return 0;
                                }
                            } else {
                                if (features[8] <= -0.0276f) { /* Ay */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[10] <= -0.0071f) { /* dO2_dt */
                            return 2;
                        } else {
                            if (features[9] <= 1.0202f) { /* Az */
                                if (features[1] <= 20.0313f) { /* H2S */
                                    if (features[6] <= 5.5013f) { /* WaterLevel */
                                        if (features[11] <= 0.1170f) { /* dH2S_dt */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 2;
                                }
                            } else {
                                if (features[1] <= 1.4957f) { /* H2S */
                                    return 0;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    }
                } else {
                    if (features[9] <= 1.0248f) { /* Az */
                        if (features[4] <= 37.7776f) { /* Temp */
                            if (features[5] <= 66.3580f) { /* Humidity */
                                if (features[3] <= 25.2305f) { /* CH4 */
                                    if (features[6] <= 30.6741f) { /* WaterLevel */
                                        if (features[13] <= 0.1384f) { /* dCH4_dt */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[6] <= 59.8950f) { /* WaterLevel */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[7] <= 0.0213f) { /* Ax */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            if (features[9] <= 0.9439f) { /* Az */
                                return 1;
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        if (features[1] <= 0.4392f) { /* H2S */
                            if (features[8] <= 0.7889f) { /* Ay */
                                if (features[4] <= 26.1183f) { /* Temp */
                                    if (features[7] <= 0.0117f) { /* Ax */
                                        return 0;
                                    } else {
                                        if (features[5] <= 64.2221f) { /* Humidity */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    }
                                } else {
                                    if (features[4] <= 26.1624f) { /* Temp */
                                        return 1;
                                    } else {
                                        return 0;
                                    }
                                }
                            } else {
                                if (features[6] <= 4.8346f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[1] <= 0.4399f) { /* H2S */
                                return 2;
                            } else {
                                return 0;
                            }
                        }
                    }
                }
            } else {
                if (features[6] <= 4.2299f) { /* WaterLevel */
                    return 2;
                } else {
                    return 2;
                }
            }
        }
    }
}

static int predict_tree_9(const float features[ML_FEATURE_COUNT]) {
    if (features[10] <= -0.0143f) { /* dO2_dt */
        if (features[1] <= 0.6246f) { /* H2S */
            if (features[2] <= 0.8816f) { /* CO */
                if (features[4] <= 25.7122f) { /* Temp */
                    if (features[8] <= 0.0281f) { /* Ay */
                        if (features[0] <= 7.4654f) { /* O2 */
                            return 1;
                        } else {
                            if (features[10] <= -0.0225f) { /* dO2_dt */
                                return 2;
                            } else {
                                if (features[11] <= 0.0257f) { /* dH2S_dt */
                                    if (features[6] <= 4.9210f) { /* WaterLevel */
                                        if (features[0] <= 17.9432f) { /* O2 */
                                            return 1;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[10] <= -0.0223f) { /* dO2_dt */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[9] <= 0.9566f) { /* Az */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            }
                        }
                    } else {
                        if (features[7] <= 0.0563f) { /* Ax */
                            if (features[7] <= 0.0235f) { /* Ax */
                                return 2;
                            } else {
                                if (features[0] <= 17.9624f) { /* O2 */
                                    if (features[8] <= 0.0400f) { /* Ay */
                                        return 1;
                                    } else {
                                        return 2;
                                    }
                                } else {
                                    return 0;
                                }
                            }
                        } else {
                            if (features[6] <= 5.0766f) { /* WaterLevel */
                                if (features[1] <= 0.3231f) { /* H2S */
                                    return 0;
                                } else {
                                    return 2;
                                }
                            } else {
                                return 2;
                            }
                        }
                    }
                } else {
                    return 1;
                }
            } else {
                if (features[4] <= 25.0575f) { /* Temp */
                    if (features[6] <= 4.7573f) { /* WaterLevel */
                        if (features[10] <= -0.0228f) { /* dO2_dt */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[2] <= 1.6007f) { /* CO */
                            if (features[10] <= -0.0240f) { /* dO2_dt */
                                return 2;
                            } else {
                                return 2;
                            }
                        } else {
                            if (features[1] <= 0.2692f) { /* H2S */
                                return 0;
                            } else {
                                return 2;
                            }
                        }
                    }
                } else {
                    if (features[3] <= 0.2389f) { /* CH4 */
                        if (features[2] <= 1.3997f) { /* CO */
                            if (features[5] <= 65.4938f) { /* Humidity */
                                if (features[0] <= 19.5787f) { /* O2 */
                                    return 1;
                                } else {
                                    if (features[10] <= -0.0153f) { /* dO2_dt */
                                        return 0;
                                    } else {
                                        return 0;
                                    }
                                }
                            } else {
                                if (features[6] <= 4.7636f) { /* WaterLevel */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            }
                        } else {
                            if (features[8] <= 0.0091f) { /* Ay */
                                if (features[10] <= -0.0183f) { /* dO2_dt */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            } else {
                                if (features[0] <= 19.3524f) { /* O2 */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[4] <= 25.4477f) { /* Temp */
                            if (features[9] <= 0.9734f) { /* Az */
                                return 1;
                            } else {
                                return 0;
                            }
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[2] <= 0.0345f) { /* CO */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[9] <= 0.4712f) { /* Az */
            return 2;
        } else {
            if (features[0] <= 19.4074f) { /* O2 */
                if (features[6] <= 4.5862f) { /* WaterLevel */
                    if (features[7] <= 0.0872f) { /* Ax */
                        return 1;
                    } else {
                        return 2;
                    }
                } else {
                    return 1;
                }
            } else {
                if (features[5] <= 86.1302f) { /* Humidity */
                    if (features[12] <= 0.3804f) { /* dCO_dt */
                        if (features[3] <= 25.2889f) { /* CH4 */
                            if (features[11] <= 0.1500f) { /* dH2S_dt */
                                if (features[12] <= 0.2986f) { /* dCO_dt */
                                    if (features[11] <= 0.1066f) { /* dH2S_dt */
                                        if (features[6] <= 59.7902f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[12] <= 0.3018f) { /* dCO_dt */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                if (features[7] <= 0.0737f) { /* Ax */
                                    if (features[10] <= 12.9500f) { /* dO2_dt */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[11] <= 499.5484f) { /* dH2S_dt */
                                        if (features[5] <= 64.6168f) { /* Humidity */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[6] <= 4.5571f) { /* WaterLevel */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[11] <= 0.0500f) { /* dH2S_dt */
                            return 2;
                        } else {
                            if (features[9] <= 0.9812f) { /* Az */
                                if (features[2] <= 1.1782f) { /* CO */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            } else {
                                return 0;
                            }
                        }
                    }
                } else {
                    return 1;
                }
            }
        }
    }
}

static int predict_tree_10(const float features[ML_FEATURE_COUNT]) {
    if (features[10] <= -0.0143f) { /* dO2_dt */
        if (features[11] <= 0.0257f) { /* dH2S_dt */
            if (features[9] <= 0.9870f) { /* Az */
                if (features[2] <= 0.8841f) { /* CO */
                    if (features[10] <= -0.0226f) { /* dO2_dt */
                        return 2;
                    } else {
                        if (features[4] <= 25.6528f) { /* Temp */
                            if (features[0] <= 17.9866f) { /* O2 */
                                return 1;
                            } else {
                                return 0;
                            }
                        } else {
                            if (features[3] <= 0.0048f) { /* CH4 */
                                return 1;
                            } else {
                                return 1;
                            }
                        }
                    }
                } else {
                    if (features[6] <= 4.8471f) { /* WaterLevel */
                        if (features[4] <= 25.0424f) { /* Temp */
                            return 2;
                        } else {
                            if (features[0] <= 19.6478f) { /* O2 */
                                if (features[8] <= -0.0147f) { /* Ay */
                                    return 1;
                                } else {
                                    return 1;
                                }
                            } else {
                                return 0;
                            }
                        }
                    } else {
                        if (features[4] <= 25.1164f) { /* Temp */
                            return 2;
                        } else {
                            if (features[3] <= 0.1730f) { /* CH4 */
                                return 1;
                            } else {
                                if (features[10] <= -0.0173f) { /* dO2_dt */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    }
                }
            } else {
                if (features[7] <= 0.0814f) { /* Ax */
                    if (features[4] <= 25.6976f) { /* Temp */
                        if (features[3] <= 0.0581f) { /* CH4 */
                            if (features[1] <= 0.0136f) { /* H2S */
                                if (features[5] <= 65.3981f) { /* Humidity */
                                    if (features[9] <= 0.9907f) { /* Az */
                                        return 0;
                                    } else {
                                        return 2;
                                    }
                                } else {
                                    return 2;
                                }
                            } else {
                                if (features[4] <= 24.7855f) { /* Temp */
                                    return 2;
                                } else {
                                    if (features[7] <= 0.0475f) { /* Ax */
                                        if (features[3] <= 0.0198f) { /* CH4 */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[4] <= 25.2056f) { /* Temp */
                                return 1;
                            } else {
                                if (features[8] <= 0.0615f) { /* Ay */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            }
                        }
                    } else {
                        if (features[3] <= 0.0091f) { /* CH4 */
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[8] <= 0.0115f) { /* Ay */
                        return 0;
                    } else {
                        return 2;
                    }
                }
            }
        } else {
            if (features[0] <= 15.8865f) { /* O2 */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[13] <= 0.2090f) { /* dCH4_dt */
            if (features[0] <= 19.4227f) { /* O2 */
                if (features[6] <= 4.5916f) { /* WaterLevel */
                    if (features[6] <= 4.5739f) { /* WaterLevel */
                        return 1;
                    } else {
                        return 2;
                    }
                } else {
                    if (features[1] <= 0.2910f) { /* H2S */
                        return 1;
                    } else {
                        if (features[1] <= 0.2930f) { /* H2S */
                            return 2;
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[2] <= 50.1507f) { /* CO */
                    if (features[6] <= 59.8950f) { /* WaterLevel */
                        if (features[6] <= 29.1440f) { /* WaterLevel */
                            if (features[7] <= 0.4311f) { /* Ax */
                                if (features[4] <= 37.9554f) { /* Temp */
                                    if (features[5] <= 66.2955f) { /* Humidity */
                                        if (features[8] <= -0.0353f) { /* Ay */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[5] <= 66.5199f) { /* Humidity */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[7] <= 0.7104f) { /* Ax */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[0] <= 20.7872f) { /* O2 */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                } else {
                    if (features[12] <= 0.3849f) { /* dCO_dt */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            }
        } else {
            if (features[0] <= 20.8132f) { /* O2 */
                return 2;
            } else {
                return 2;
            }
        }
    }
}

static int predict_tree_11(const float features[ML_FEATURE_COUNT]) {
    if (features[6] <= 5.9793f) { /* WaterLevel */
        if (features[0] <= 19.4602f) { /* O2 */
            if (features[1] <= 0.7261f) { /* H2S */
                if (features[5] <= 65.5129f) { /* Humidity */
                    if (features[5] <= 64.5519f) { /* Humidity */
                        if (features[5] <= 64.5375f) { /* Humidity */
                            if (features[4] <= 24.8763f) { /* Temp */
                                if (features[6] <= 5.5040f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 1;
                                }
                            } else {
                                return 1;
                            }
                        } else {
                            if (features[2] <= 0.2705f) { /* CO */
                                return 2;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[10] <= -0.0226f) { /* dO2_dt */
                            if (features[9] <= 0.9701f) { /* Az */
                                if (features[7] <= 0.0389f) { /* Ax */
                                    if (features[4] <= 25.2757f) { /* Temp */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 2;
                                }
                            } else {
                                if (features[1] <= 0.0907f) { /* H2S */
                                    return 1;
                                } else {
                                    if (features[10] <= -12.9619f) { /* dO2_dt */
                                        return 1;
                                    } else {
                                        return 2;
                                    }
                                }
                            }
                        } else {
                            if (features[5] <= 64.8513f) { /* Humidity */
                                if (features[3] <= 0.2100f) { /* CH4 */
                                    return 1;
                                } else {
                                    if (features[0] <= 15.6038f) { /* O2 */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                return 1;
                            }
                        }
                    }
                } else {
                    if (features[10] <= -0.0224f) { /* dO2_dt */
                        return 2;
                    } else {
                        return 1;
                    }
                }
            } else {
                return 2;
            }
        } else {
            if (features[12] <= 0.3819f) { /* dCO_dt */
                if (features[1] <= 10.0596f) { /* H2S */
                    if (features[9] <= 0.1816f) { /* Az */
                        return 2;
                    } else {
                        if (features[2] <= 24.6252f) { /* CO */
                            if (features[7] <= -0.0376f) { /* Ax */
                                return 0;
                            } else {
                                if (features[1] <= 0.6601f) { /* H2S */
                                    if (features[2] <= 1.7532f) { /* CO */
                                        if (features[4] <= 37.9005f) { /* Temp */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[13] <= 0.1133f) { /* dCH4_dt */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    return 0;
                                }
                            }
                        } else {
                            if (features[9] <= 0.9519f) { /* Az */
                                if (features[4] <= 25.7762f) { /* Temp */
                                    return 0;
                                } else {
                                    return 1;
                                }
                            } else {
                                return 1;
                            }
                        }
                    }
                } else {
                    if (features[3] <= 0.1046f) { /* CH4 */
                        if (features[4] <= 25.4408f) { /* Temp */
                            if (features[8] <= 0.0246f) { /* Ay */
                                if (features[6] <= 5.2246f) { /* WaterLevel */
                                    if (features[11] <= 0.1480f) { /* dH2S_dt */
                                        return 1;
                                    } else {
                                        if (features[9] <= 0.9422f) { /* Az */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[11] <= 0.1814f) { /* dH2S_dt */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                if (features[7] <= 0.0460f) { /* Ax */
                                    if (features[8] <= 0.0312f) { /* Ay */
                                        return 2;
                                    } else {
                                        if (features[6] <= 5.1434f) { /* WaterLevel */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[9] <= 1.0119f) { /* Az */
                                        if (features[5] <= 64.7208f) { /* Humidity */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            if (features[1] <= 10.1158f) { /* H2S */
                                return 1;
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        if (features[9] <= 0.9993f) { /* Az */
                            if (features[11] <= 0.1577f) { /* dH2S_dt */
                                return 1;
                            } else {
                                return 2;
                            }
                        } else {
                            if (features[0] <= 20.8672f) { /* O2 */
                                return 1;
                            } else {
                                if (features[1] <= 17.9367f) { /* H2S */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            }
                        }
                    }
                }
            } else {
                if (features[6] <= 4.9469f) { /* WaterLevel */
                    return 2;
                } else {
                    if (features[12] <= 0.7566f) { /* dCO_dt */
                        return 2;
                    } else {
                        if (features[0] <= 20.8747f) { /* O2 */
                            return 2;
                        } else {
                            if (features[5] <= 65.0685f) { /* Humidity */
                                if (features[7] <= 0.0220f) { /* Ax */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            } else {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (features[2] <= 0.6054f) { /* CO */
            if (features[9] <= 0.9586f) { /* Az */
                return 2;
            } else {
                if (features[0] <= 20.7370f) { /* O2 */
                    return 2;
                } else {
                    if (features[4] <= 25.6592f) { /* Temp */
                        if (features[8] <= -0.0128f) { /* Ay */
                            return 1;
                        } else {
                            if (features[1] <= 0.2595f) { /* H2S */
                                if (features[7] <= 0.0173f) { /* Ax */
                                    return 0;
                                } else {
                                    if (features[8] <= -0.0026f) { /* Ay */
                                        return 0;
                                    } else {
                                        if (features[0] <= 20.8854f) { /* O2 */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                }
                            } else {
                                if (features[9] <= 0.9727f) { /* Az */
                                    return 0;
                                } else {
                                    return 0;
                                }
                            }
                        }
                    } else {
                        if (features[4] <= 25.6955f) { /* Temp */
                            return 1;
                        } else {
                            return 1;
                        }
                    }
                }
            }
        } else {
            if (features[8] <= -0.0133f) { /* Ay */
                if (features[11] <= 0.0257f) { /* dH2S_dt */
                    if (features[1] <= 0.0164f) { /* H2S */
                        return 1;
                    } else {
                        if (features[4] <= 25.5589f) { /* Temp */
                            if (features[6] <= 40.2154f) { /* WaterLevel */
                                return 0;
                            } else {
                                return 2;
                            }
                        } else {
                            if (features[8] <= -0.0275f) { /* Ay */
                                return 1;
                            } else {
                                return 1;
                            }
                        }
                    }
                } else {
                    return 2;
                }
            } else {
                if (features[8] <= 0.0220f) { /* Ay */
                    if (features[4] <= 25.6448f) { /* Temp */
                        if (features[10] <= -0.0071f) { /* dO2_dt */
                            return 2;
                        } else {
                            if (features[1] <= 0.2603f) { /* H2S */
                                if (features[6] <= 47.3004f) { /* WaterLevel */
                                    if (features[6] <= 30.0470f) { /* WaterLevel */
                                        return 0;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[8] <= -0.0041f) { /* Ay */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                if (features[6] <= 46.7173f) { /* WaterLevel */
                                    return 0;
                                } else {
                                    if (features[1] <= 0.4782f) { /* H2S */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            }
                        }
                    } else {
                        if (features[7] <= 0.0383f) { /* Ax */
                            if (features[11] <= 0.0257f) { /* dH2S_dt */
                                if (features[7] <= 0.0342f) { /* Ax */
                                    return 1;
                                } else {
                                    return 0;
                                }
                            } else {
                                if (features[8] <= 0.0021f) { /* Ay */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[10] <= -0.0071f) { /* dO2_dt */
                                return 2;
                            } else {
                                if (features[5] <= 64.5831f) { /* Humidity */
                                    return 2;
                                } else {
                                    return 1;
                                }
                            }
                        }
                    }
                } else {
                    if (features[2] <= 0.7296f) { /* CO */
                        if (features[4] <= 25.6036f) { /* Temp */
                            if (features[0] <= 20.9593f) { /* O2 */
                                return 2;
                            } else {
                                return 0;
                            }
                        } else {
                            if (features[11] <= 0.0257f) { /* dH2S_dt */
                                return 1;
                            } else {
                                return 2;
                            }
                        }
                    } else {
                        if (features[1] <= 0.7363f) { /* H2S */
                            if (features[2] <= 1.7787f) { /* CO */
                                if (features[2] <= 1.4044f) { /* CO */
                                    if (features[5] <= 64.8832f) { /* Humidity */
                                        if (features[6] <= 60.9980f) { /* WaterLevel */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        if (features[7] <= 0.0268f) { /* Ax */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[3] <= 0.0017f) { /* CH4 */
                                        if (features[6] <= 31.3007f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    } else {
                                        if (features[6] <= 39.0493f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            } else {
                                if (features[5] <= 64.6640f) { /* Humidity */
                                    return 0;
                                } else {
                                    if (features[7] <= 0.0719f) { /* Ax */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            return 2;
                        }
                    }
                }
            }
        }
    }
}

static int predict_tree_12(const float features[ML_FEATURE_COUNT]) {
    if (features[0] <= 19.4568f) { /* O2 */
        if (features[3] <= 0.4213f) { /* CH4 */
            if (features[1] <= 2.8182f) { /* H2S */
                if (features[5] <= 65.8446f) { /* Humidity */
                    if (features[8] <= 0.0764f) { /* Ay */
                        if (features[10] <= -0.0227f) { /* dO2_dt */
                            if (features[2] <= 0.7144f) { /* CO */
                                if (features[5] <= 64.6475f) { /* Humidity */
                                    return 2;
                                } else {
                                    if (features[6] <= 4.6508f) { /* WaterLevel */
                                        return 2;
                                    } else {
                                        if (features[10] <= -1.2118f) { /* dO2_dt */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    }
                                }
                            } else {
                                if (features[8] <= 0.0017f) { /* Ay */
                                    if (features[9] <= 0.9617f) { /* Az */
                                        return 1;
                                    } else {
                                        return 2;
                                    }
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[3] <= 0.0002f) { /* CH4 */
                                if (features[6] <= 4.5916f) { /* WaterLevel */
                                    if (features[4] <= 24.6868f) { /* Temp */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                return 1;
                            }
                        }
                    } else {
                        return 2;
                    }
                } else {
                    if (features[4] <= 24.7901f) { /* Temp */
                        if (features[2] <= 1.2530f) { /* CO */
                            if (features[1] <= 0.0648f) { /* H2S */
                                return 2;
                            } else {
                                return 1;
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[9] <= 0.9302f) { /* Az */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[11] <= 0.0257f) { /* dH2S_dt */
                return 1;
            } else {
                return 2;
            }
        }
    } else {
        if (features[8] <= 0.3598f) { /* Ay */
            if (features[3] <= 25.2889f) { /* CH4 */
                if (features[2] <= 48.7330f) { /* CO */
                    if (features[6] <= 59.9057f) { /* WaterLevel */
                        if (features[10] <= -0.0143f) { /* dO2_dt */
                            if (features[11] <= 0.0257f) { /* dH2S_dt */
                                return 0;
                            } else {
                                if (features[6] <= 5.2405f) { /* WaterLevel */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            if (features[1] <= 10.0596f) { /* H2S */
                                if (features[2] <= 24.7888f) { /* CO */
                                    if (features[4] <= 37.9554f) { /* Temp */
                                        if (features[8] <= -0.0347f) { /* Ay */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[4] <= 38.1285f) { /* Temp */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                } else {
                                    if (features[4] <= 25.7055f) { /* Temp */
                                        return 0;
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                if (features[11] <= 0.1506f) { /* dH2S_dt */
                                    if (features[0] <= 20.7903f) { /* O2 */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[4] <= 25.0791f) { /* Temp */
                                        return 2;
                                    } else {
                                        if (features[10] <= 12.9500f) { /* dO2_dt */
                                            return 2;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        if (features[1] <= 0.0046f) { /* H2S */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                } else {
                    if (features[6] <= 4.8385f) { /* WaterLevel */
                        if (features[4] <= 26.0753f) { /* Temp */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        if (features[9] <= 0.9434f) { /* Az */
                            return 2;
                        } else {
                            return 2;
                        }
                    }
                }
            } else {
                if (features[2] <= 0.0716f) { /* CO */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[7] <= 0.7055f) { /* Ax */
                return 2;
            } else {
                return 2;
            }
        }
    }
}

static int predict_tree_13(const float features[ML_FEATURE_COUNT]) {
    if (features[11] <= 0.0513f) { /* dH2S_dt */
        if (features[0] <= 19.4568f) { /* O2 */
            if (features[10] <= -0.0226f) { /* dO2_dt */
                if (features[11] <= -499.4500f) { /* dH2S_dt */
                    return 1;
                } else {
                    if (features[10] <= -12.9620f) { /* dO2_dt */
                        return 1;
                    } else {
                        return 2;
                    }
                }
            } else {
                if (features[1] <= 0.2898f) { /* H2S */
                    return 1;
                } else {
                    if (features[7] <= 0.0396f) { /* Ax */
                        if (features[1] <= 0.2944f) { /* H2S */
                            return 2;
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                }
            }
        } else {
            if (features[9] <= 0.1816f) { /* Az */
                return 2;
            } else {
                if (features[12] <= 0.3804f) { /* dCO_dt */
                    if (features[4] <= 38.2422f) { /* Temp */
                        if (features[2] <= 23.9973f) { /* CO */
                            if (features[6] <= 59.9057f) { /* WaterLevel */
                                if (features[1] <= 500.4591f) { /* H2S */
                                    if (features[3] <= 25.0083f) { /* CH4 */
                                        if (features[8] <= -0.0355f) { /* Ay */
                                            return 0;
                                        } else {
                                            return 0;
                                        }
                                    } else {
                                        if (features[13] <= 0.2101f) { /* dCH4_dt */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    return 1;
                                }
                            } else {
                                if (features[1] <= 0.0046f) { /* H2S */
                                    return 2;
                                } else {
                                    return 2;
                                }
                            }
                        } else {
                            return 1;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    if (features[7] <= 0.0070f) { /* Ax */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            }
        }
    } else {
        if (features[1] <= 10.0596f) { /* H2S */
            if (features[4] <= 25.4151f) { /* Temp */
                if (features[4] <= 23.8936f) { /* Temp */
                    return 1;
                } else {
                    if (features[8] <= 0.0759f) { /* Ay */
                        if (features[4] <= 24.0904f) { /* Temp */
                            if (features[2] <= 1.2705f) { /* CO */
                                if (features[11] <= 0.0757f) { /* dH2S_dt */
                                    return 2;
                                } else {
                                    if (features[5] <= 64.7362f) { /* Humidity */
                                        return 0;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            return 2;
                        }
                    } else {
                        return 0;
                    }
                }
            } else {
                if (features[6] <= 10.9235f) { /* WaterLevel */
                    return 0;
                } else {
                    if (features[3] <= 0.0063f) { /* CH4 */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            }
        } else {
            if (features[4] <= 25.3807f) { /* Temp */
                if (features[9] <= 1.0203f) { /* Az */
                    return 2;
                } else {
                    if (features[3] <= 0.0892f) { /* CH4 */
                        return 2;
                    } else {
                        return 1;
                    }
                }
            } else {
                if (features[6] <= 17.3193f) { /* WaterLevel */
                    if (features[8] <= 0.0435f) { /* Ay */
                        return 1;
                    } else {
                        if (features[8] <= 0.0463f) { /* Ay */
                            return 2;
                        } else {
                            return 1;
                        }
                    }
                } else {
                    if (features[9] <= 0.9458f) { /* Az */
                        return 2;
                    } else {
                        return 2;
                    }
                }
            }
        }
    }
}

static int predict_tree_14(const float features[ML_FEATURE_COUNT]) {
    if (features[1] <= 2.2481f) { /* H2S */
        if (features[2] <= 50.6642f) { /* CO */
            if (features[13] <= 0.2090f) { /* dCH4_dt */
                if (features[9] <= 0.1936f) { /* Az */
                    return 2;
                } else {
                    if (features[0] <= 19.5097f) { /* O2 */
                        if (features[7] <= 0.0411f) { /* Ax */
                            if (features[10] <= -0.0226f) { /* dO2_dt */
                                if (features[5] <= 65.2344f) { /* Humidity */
                                    if (features[0] <= 7.3663f) { /* O2 */
                                        return 1;
                                    } else {
                                        if (features[5] <= 64.6700f) { /* Humidity */
                                            return 2;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[10] <= -0.0239f) { /* dO2_dt */
                                        return 2;
                                    } else {
                                        return 2;
                                    }
                                }
                            } else {
                                if (features[7] <= 0.0394f) { /* Ax */
                                    return 1;
                                } else {
                                    if (features[9] <= 0.9873f) { /* Az */
                                        return 1;
                                    } else {
                                        if (features[6] <= 5.1484f) { /* WaterLevel */
                                            return 1;
                                        } else {
                                            return 2;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (features[10] <= -0.0227f) { /* dO2_dt */
                                if (features[10] <= -1.2120f) { /* dO2_dt */
                                    return 1;
                                } else {
                                    return 2;
                                }
                            } else {
                                if (features[4] <= 23.9553f) { /* Temp */
                                    if (features[5] <= 64.9905f) { /* Humidity */
                                        return 2;
                                    } else {
                                        return 1;
                                    }
                                } else {
                                    if (features[9] <= 0.9965f) { /* Az */
                                        return 1;
                                    } else {
                                        if (features[0] <= 15.3584f) { /* O2 */
                                            return 1;
                                        } else {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        if (features[2] <= 25.7476f) { /* CO */
                            if (features[4] <= 37.9614f) { /* Temp */
                                if (features[10] <= -0.0143f) { /* dO2_dt */
                                    if (features[2] <= 1.2293f) { /* CO */
                                        if (features[11] <= 0.0257f) { /* dH2S_dt */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        if (features[10] <= -0.0143f) { /* dO2_dt */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    }
                                } else {
                                    if (features[3] <= 10.3563f) { /* CH4 */
                                        if (features[6] <= 29.6602f) { /* WaterLevel */
                                            return 0;
                                        } else {
                                            return 2;
                                        }
                                    } else {
                                        return 1;
                                    }
                                }
                            } else {
                                if (features[5] <= 86.1302f) { /* Humidity */
                                    if (features[5] <= 85.9736f) { /* Humidity */
                                        return 1;
                                    } else {
                                        return 0;
                                    }
                                } else {
                                    if (features[4] <= 38.1951f) { /* Temp */
                                        return 1;
                                    } else {
                                        return 1;
                                    }
                                }
                            }
                        } else {
                            return 1;
                        }
                    }
                }
            } else {
                if (features[0] <= 20.7625f) { /* O2 */
                    return 2;
                } else {
                    return 2;
                }
            }
        } else {
            if (features[6] <= 4.2203f) { /* WaterLevel */
                return 2;
            } else {
                return 2;
            }
        }
    } else {
        if (features[4] <= 25.3924f) { /* Temp */
            if (features[10] <= -0.0071f) { /* dO2_dt */
                return 2;
            } else {
                if (features[1] <= 516.6111f) { /* H2S */
                    if (features[4] <= 25.2813f) { /* Temp */
                        if (features[0] <= 20.8407f) { /* O2 */
                            return 2;
                        } else {
                            return 2;
                        }
                    } else {
                        return 1;
                    }
                } else {
                    return 1;
                }
            }
        } else {
            if (features[10] <= -0.0071f) { /* dO2_dt */
                return 2;
            } else {
                if (features[1] <= 9.9176f) { /* H2S */
                    return 0;
                } else {
                    if (features[11] <= 0.1506f) { /* dH2S_dt */
                        return 1;
                    } else {
                        return 2;
                    }
                }
            }
        }
    }
}

ml_risk_class_t sentinel_ml_predict_risk(const float features[ML_FEATURE_COUNT]) {
    int votes[3] = {0, 0, 0};
    
    votes[predict_tree_0(features)]++;
    votes[predict_tree_1(features)]++;
    votes[predict_tree_2(features)]++;
    votes[predict_tree_3(features)]++;
    votes[predict_tree_4(features)]++;
    votes[predict_tree_5(features)]++;
    votes[predict_tree_6(features)]++;
    votes[predict_tree_7(features)]++;
    votes[predict_tree_8(features)]++;
    votes[predict_tree_9(features)]++;
    votes[predict_tree_10(features)]++;
    votes[predict_tree_11(features)]++;
    votes[predict_tree_12(features)]++;
    votes[predict_tree_13(features)]++;
    votes[predict_tree_14(features)]++;

    if (votes[ML_CLASS_CRITICAL] >= 2) {
        return ML_CLASS_CRITICAL;
    }
    if (votes[ML_CLASS_WARNING] > votes[ML_CLASS_SAFE]) {
        return ML_CLASS_WARNING;
    }
    if (votes[ML_CLASS_SAFE] >= votes[ML_CLASS_WARNING] && votes[ML_CLASS_SAFE] >= votes[ML_CLASS_CRITICAL]) {
        return ML_CLASS_SAFE;
    }
    return ML_CLASS_WARNING;
}

bool sentinel_ml_detect_anomaly(const float features[ML_FEATURE_COUNT]) {
    /* 
     * Embedded Anomaly Score:
     * Evaluates multi-variate statistical deviance beyond safe confined space envelopes:
     * 1. Extreme gas rates of change (surges)
     * 2. Simultaneous high humidity + high temp + gas presence
     * 3. Abnormal acceleration dynamics
     */
    if (features[FEAT_O2] < 19.5f || features[FEAT_O2] > 23.5f) return true;
    if (features[FEAT_H2S] > 5.0f) return true;
    if (features[FEAT_CO] > 20.0f) return true;
    if (features[FEAT_CH4] > 8.0f) return true;
    if (fabsf(features[FEAT_DO2_DT]) > 0.5f) return true;
    if (features[FEAT_DH2S_DT] > 1.0f) return true;
    if (features[FEAT_DCO_DT] > 2.0f) return true;
    if (features[FEAT_WATER] > 25.0f) return true;
    
    float acc_mag = sqrtf(features[FEAT_AX]*features[FEAT_AX] + 
                          features[FEAT_AY]*features[FEAT_AY] + 
                          features[FEAT_AZ]*features[FEAT_AZ]);
    if (acc_mag < 0.4f || acc_mag > 2.2f) return true;
    
    return false;
}
