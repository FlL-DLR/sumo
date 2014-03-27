/****************************************************************************/
/// @file    HelpersHBEFA3.cpp
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    Mon, 10.05.2004
/// @version $Id$
///
// Helper methods for HBEFA3-based emission computation
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2013 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "HelpersHBEFA3.h"
#include <limits>
#include <cmath>

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// static definitions
// ===========================================================================
double
HelpersHBEFA3::myFunctionParameter[45][6][6] = {
    { // LNF
      { 7192, 1022, 0.0, -161, 23.33, 0.0 }, // CO2(total)
      { 961.4, 69.4, 0.0, -114.3, 3.851, 0.0 }, // CO
      { 59.65, 1.963, 0.0, -5.305, 0.159, 0.0 }, // HC
      { 2294, 323.8, 0.0, -52.78, 7.43, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 5.695, 0.2347, 0.0, -0.8032, 0.05115, -0.0007435 }, // PM
    },
    { // LNF_B_Euro_0
      { 1.045e+04, 1105, 0.0, -430.5, 29.24, 0.0 }, // CO2(total)
      { 956.6, 219.3, 0.0, -41.98, 0.0, 0.13 }, // CO
      { 80.96, 4.731, 0.0, -4.85, 0.1828, 0.0 }, // HC
      { 3332, 352.5, 0.0, -137.3, 9.328, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 1.243, 0.2422, 0.0, -0.1785, 0.008536, 0.0 }, // PM
    },
    { // LNF_B_Euro_1
      { 4906, 1144, 0.0, 295.4, 0.0, 0.3967 }, // CO2(total)
      { 2176, 243.2, 0.0, -303.2, 11.33, 0.0 }, // CO
      { 26.97, 3.78, 0.0, -3.277, 0.1403, 0.0 }, // HC
      { 1565, 365, 0.0, 94.21, 0.0, 0.1266 }, // mKr
      { 19.35, 3.279, 0.0, 0.8384, 0.0, 0.001006 }, // NOx
      { 0.915, 0.18, 0.2317, -0.1635, 0.006222, 0.0 }, // PM
    },
    { // LNF_B_Euro_2
      { 5122, 1131, 0.0, 277.6, 0.0, 0.391 }, // CO2(total)
      { 1296, 123.5, 0.0, -174.8, 6.212, 0.0 }, // CO
      { 10.94, 1.322, 0.0, -1.316, 0.05277, 0.0 }, // HC
      { 1634, 360.9, 0.0, 88.54, 0.0, 0.1247 }, // mKr
      { 6.574, 1.161, 0.0, 0.3115, 0.0, 0.0003318 }, // NOx
      { 1.988, 0.3357, 0.0, -0.2804, 0.01234, 0.0 }, // PM
    },
    { // LNF_B_Euro_3
      { 5363, 1115, 0.0, 264, 0.0, 0.394 }, // CO2(total)
      { 809.6, 64.08, 0.0, -108, 3.632, 0.0 }, // CO
      { 3.502, 0.3612, 0.0, -0.4307, 0.01612, 0.0 }, // HC
      { 1710, 355.8, 0.0, 84.18, 0.0, 0.1257 }, // mKr
      { 0.5866, 0.2926, 0.0, 0.1955, 0.0, 0.0 }, // NOx
      { 0.7376, 0.09247, 0.0, -0.0981, 0.004036, 0.0 }, // PM
    },
    { // LNF_B_Euro_4
      { 5056, 1015, 0.0, 286.5, 0.0, 0.3172 }, // CO2(total)
      { 567.4, 43.43, 0.0, -76.69, 2.618, 0.0 }, // CO
      { 2.052, 0.1911, 0.0, -0.2652, 0.009888, 0.0 }, // HC
      { 1613, 323.8, 0.0, 91.39, 0.0, 0.1012 }, // mKr
      { 1.1, 0.1892, 0.0, 0.08809, 0.0, 0.0 }, // NOx
      { 0.5892, 0.06059, 0.0, -0.07563, 0.002986, 0.0 }, // PM
    },
    { // LNF_B_Euro_5
      { 4635, 985.8, 0.0, 296.2, 0.0, 0.3007 }, // CO2(total)
      { 639.8, 43.92, 0.0, -85.21, 2.829, 0.0 }, // CO
      { 2.248, 0.1968, 0.0, -0.2896, 0.01057, 0.0 }, // HC
      { 1479, 314.4, 0.0, 94.47, 0.0, 0.0959 }, // mKr
      { 1.101, 0.1824, 0.0, 0.08517, 0.0, 0.0 }, // NOx
      { 0.6097, 0.06993, 0.0, -0.08108, 0.003213, 0.0 }, // PM
    },
    { // LNF_B_Euro_6
      { 4423, 927.9, 0.0, 279, 0.0, 0.2844 }, // CO2(total)
      { 645.1, 41.88, 0.0, -85.32, 2.798, 0.0 }, // CO
      { 2.311, 0.1872, 0.0, -0.2952, 0.01056, 0.0 }, // HC
      { 1411, 296, 0.0, 88.98, 0.0, 0.09072 }, // mKr
      { 1.109, 0.1806, 0.0, 0.08319, 0.0, 0.0 }, // NOx
      { 0.6029, 0.06641, 0.0, -0.08017, 0.003149, 0.0 }, // PM
    },
    { // LNF_B_Ost
      { 8365, 832.4, 0.0, -368.6, 22.94, 0.0 }, // CO2(total)
      { 2266, 172.8, 0.0, -235.2, 8.393, 0.0 }, // CO
      { 2226, 46.86, 0.0, -212.9, 5.895, 0.0 }, // HC
      { 2668, 265.5, 0.0, -117.6, 7.317, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // PM
    },
    { // LNF_D_Euro_0
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // CO2(total)
      { 7.189, 2.276, 0.0, 2.542, 0.0, 0.0 }, // CO
      { 4.995, 0.2038, 0.0, 0.4551, 0.0, 0.0 }, // HC
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // mKr
      { 53.84, 11.77, 0.0, -3.702, 0.3365, 0.0 }, // NOx
      { 17.82, 0.9409, 0.0, -1.8, 0.1364, -0.001849 }, // PM
    },
    { // LNF_D_Euro_1
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // CO2(total)
      { 4.958, 1.428, 0.0, 1.624, 0.0, 0.0 }, // CO
      { 2.849, 0.1092, 0.0, 0.2372, 0.0, 0.0 }, // HC
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // mKr
      { 45.13, 10.42, 0.0, -2.941, 0.2852, 0.0 }, // NOx
      { 15.3, 0.6367, 0.0, -2.007, 0.1254, -0.001671 }, // PM
    },
    { // LNF_D_Euro_2
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // CO2(total)
      { 1.437, 0.4449, 0.0, 0.4966, 0.0, 0.0 }, // CO
      { 1.262, 0.04612, 0.0, 0.1011, 0.0, 0.0 }, // HC
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // mKr
      { 42.83, 9.371, 0.0, -2.973, 0.2623, 0.0 }, // NOx
      { 6.58, 0.2888, 0.0, -0.7254, 0.04933, -0.0006649 }, // PM
    },
    { // LNF_D_Euro_3
      { 3779, 902, 0.0, 288.6, 0.0, 0.3084 }, // CO2(total)
      { 1.94, 0.02085, 0.0, -0.03984, 0.0, 0.0 }, // CO
      { 0.4687, 0.02017, 0.0, -0.02515, 0.00116, 0.0 }, // HC
      { 1189, 283.7, 0.0, 90.77, 0.0, 0.09701 }, // mKr
      { 33.02, 6.747, 1.706, -1.94, 0.1688, 0.0 }, // NOx
      { 1.553, 0.2034, 0.0, -0.09003, 0.006345, 0.0 }, // PM
    },
    { // LNF_D_Euro_4
      { 3682, 915.4, 0.0, 331, 0.0, 0.306 }, // CO2(total)
      { 1.724, 0.03099, 0.0, -0.02995, 0.0, 0.0 }, // CO
      { 0.03577, 0.01844, 0.0, 0.02506, 0.0, 0.0 }, // HC
      { 1158, 288, 0.0, 104.1, 0.0, 0.09624 }, // mKr
      { 40.68, 6.272, 0.0, -3.613, 0.2148, 0.0 }, // NOx
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // PM
    },
    { // LNF_D_Euro_5
      { 3505, 918.8, 0.0, 319.7, 0.0, 0.2978 }, // CO2(total)
      { 1.135, 0.01769, 0.0, -0.01882, 0.0, 0.0 }, // CO
      { 0.01997, 0.01916, 0.0, 0.02654, 0.0, 0.0 }, // HC
      { 1102, 289, 0.0, 100.6, 0.0, 0.09367 }, // mKr
      { 38.6, 6.079, 0.0, -3.414, 0.2026, 0.0 }, // NOx
      { 0.1111, 0.007346, 0.005486, -0.009925, 0.0004092, 0.0 }, // PM
    },
    { // LNF_D_Euro_6
      { 3374, 880.2, 0.0, 308.9, 0.0, 0.2798 }, // CO2(total)
      { 1.651, 0.03157, 0.0, -0.02797, 0.0, 0.0 }, // CO
      { 0.03311, 0.01971, 0.0, 0.02401, 0.0, 0.0 }, // HC
      { 1061, 276.9, 0.0, 97.17, 0.0, 0.088 }, // mKr
      { 13.57, 2.119, 0.0, -1.211, 0.07068, 0.0 }, // NOx
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // PM
    },
    { // PKW
      { 9034, 925.8, 0.0, -394.3, 25.71, 0.0 }, // CO2(total)
      { 428.7, 37.11, 0.0, -40.02, 1.494, 0.0 }, // CO
      { 48.97, 1.325, 0.0, -3.261, 0.1002, 0.0 }, // HC
      { 2937, 301, 0.0, -128.6, 8.373, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 1.021, 0.1731, 0.0, -0.03389, 0.0, 0.0001301 }, // PM
    },
    { // PKW_Alternative
      { 7932, 785, 0.0, -393, 23.68, 0.0 }, // CO2(total)
      { 505.6, 20.69, 0.0, -62.72, 1.831, 0.0 }, // CO
      { 3.917, 0.2646, 0.0, -0.4245, 0.01417, 0.0 }, // HC
      { 3040, 300.9, 0.0, -150.6, 9.077, 0.0 }, // mKr
      { 7.5, 0.563, 0.0, -0.6094, 0.02294, 0.0 }, // NOx
      { 0.6383, 0.05188, 0.0, -0.08341, 0.002963, 0.0 }, // PM
    },
    { // PKW_B_Euro_0
      { 1.172e+04, 1160, 0.0, -580.5, 34.98, 0.0 }, // CO2(total)
      { 738.6, 83.85, 0.0, -52.74, 2.41, 0.0 }, // CO
      { 26.96, 2.829, 0.0, 2.118, 0.0, 0.0 }, // HC
      { 3737, 369.9, 0.0, -185.2, 11.16, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 2.159, 0.1897, 0.0, -0.2832, 0.00997, 0.0 }, // PM
    },
    { // PKW_B_Euro_1
      { 1.093e+04, 1081, 0.0, -541.4, 32.63, 0.0 }, // CO2(total)
      { 291.2, 24.14, 0.0, -32.65, 1.125, 0.0 }, // CO
      { 14.58, 1.01, 0.0, -1.172, 0.03809, 0.0 }, // HC
      { 3486, 345, 0.0, -172.7, 10.41, 0.0 }, // mKr
      { 30.63, 2.031, 0.0, -3.595, 0.2137, -0.002299 }, // NOx
      { 1.371, 0.09943, 0.0, -0.1742, 0.005969, 0.0 }, // PM
    },
    { // PKW_B_Euro_2
      { 1.039e+04, 1028, 0.0, -514.9, 31.03, 0.0 }, // CO2(total)
      { 620.1, 35.08, 0.0, -78.49, 2.429, 0.0 }, // CO
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // HC
      { 3315, 328, 0.0, -164.2, 9.897, 0.0 }, // mKr
      { 23.05, 1.089, 0.0, -2.081, 0.06925, 0.0 }, // NOx
      { 2.371, 0.18, 0.0, -0.3078, 0.01044, 0.0 }, // PM
    },
    { // PKW_B_Euro_3
      { 9858, 975.6, 0.0, -488.4, 29.43, 0.0 }, // CO2(total)
      { 597.5, 32.52, 0.0, -75.27, 2.305, 0.0 }, // CO
      { 2.303, 0.25, 0.0, -0.2555, 0.00992, 0.0 }, // HC
      { 3144, 311.2, 0.0, -155.8, 9.388, 0.0 }, // mKr
      { 7.204, 0.3327, 0.0, -0.6641, 0.0215, 0.0 }, // NOx
      { 0.714, 0.05158, 0.0, -0.08903, 0.00308, 0.0 }, // PM
    },
    { // PKW_B_Euro_4
      { 9449, 938.4, 0.0, -467.1, 28.26, 0.0 }, // CO2(total)
      { 593.2, 19.32, 0.0, -73.25, 2.086, 0.0 }, // CO
      { 2.923, 0.1113, 0.0, -0.3476, 0.01032, 0.0 }, // HC
      { 3014, 299.3, 0.0, -149, 9.014, 0.0 }, // mKr
      { 4.336, 0.4428, 0.0, -0.3204, 0.01371, 0.0 }, // NOx
      { 0.2375, 0.0245, 0.0, -0.03251, 0.001325, 0.0 }, // PM
    },
    { // PKW_B_Euro_5
      { 8823, 886, 0.0, -432.9, 26.53, 0.0 }, // CO2(total)
      { 499.5, 16.93, 0.0, -61.37, 1.737, 0.0 }, // CO
      { 2.554, 0.1011, 0.0, -0.3009, 0.008911, 0.0 }, // HC
      { 2814, 282.6, 0.0, -138.1, 8.462, 0.0 }, // mKr
      { 4.02, 0.4289, 0.0, -0.286, 0.01257, 0.0 }, // NOx
      { 0.2786, 0.0241, 0.0, -0.03718, 0.001396, 0.0 }, // PM
    },
    { // PKW_B_Euro_6
      { 8273, 845.1, 0.0, -401.1, 25.08, 0.0 }, // CO2(total)
      { 405.3, 13.7, 0.0, -49.38, 1.389, 0.0 }, // CO
      { 2.016, 0.08505, 0.0, -0.2338, 0.006998, 0.0 }, // HC
      { 2639, 269.6, 0.0, -127.9, 8.001, 0.0 }, // mKr
      { 3.444, 0.4035, 0.0, -0.2313, 0.01086, 0.0 }, // NOx
      { 0.3107, 0.02274, 0.0, -0.04042, 0.001421, 0.0 }, // PM
    },
    { // PKW_B_Ost
      { 9396, 929.9, 0.0, -465.5, 28.05, 0.0 }, // CO2(total)
      { 3034, 176.7, 0.0, -332.4, 10.88, 0.0 }, // CO
      { 1363, 16.62, 0.0, -133.4, 3.537, 0.0 }, // HC
      { 2997, 296.6, 0.0, -148.5, 8.948, 0.0 }, // mKr
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // NOx
      { 2.394, 0.2126, 0.0, -0.3147, 0.01108, 0.0 }, // PM
    },
    { // PKW_D_Euro_0
      { 8212, 888.8, 0.0, -237.3, 19.77, 0.0 }, // CO2(total)
      { 84.56, 3.162, 0.0, -6.251, 0.1955, 0.0 }, // CO
      { 6.153, 0.1644, 0.0, 0.1236, 0.0, 0.0 }, // HC
      { 2583, 279.6, 0.0, -74.65, 6.22, 0.0 }, // mKr
      { 28.84, 5.756, 0.0, -1.228, 0.1234, 0.0 }, // NOx
      { 18.43, 0.7548, 0.0, -1.834, 0.06254, 0.0 }, // PM
    },
    { // PKW_D_Euro_1
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // CO2(total)
      { 2.811, 1.004, 0.0, 0.9606, 0.0, 0.0 }, // CO
      { 1.989, 0.06299, 0.0, 0.05963, 0.0, 0.0 }, // HC
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // mKr
      { 43.38, 5.386, 0.0, -3.436, 0.1704, 0.0 }, // NOx
      { 2.569, 0.4934, 0.0, 0.1002, 0.0, 0.0003083 }, // PM
    },
    { // PKW_D_Euro_2
      { 4485, 760.9, 0.0, 160, 0.0, 0.2488 }, // CO2(total)
      { 6.66, 0.08232, 0.0, 0.01522, 0.0, 0.0 }, // CO
      { 1.145, 0.01858, 0.0, 0.04206, -0.0009262, 0.0 }, // HC
      { 1411, 239.3, 0.0, 50.32, 0.0, 0.07827 }, // mKr
      { 58.93, 5.709, 0.0, -5.174, 0.2186, 0.0 }, // NOx
      { 1.496, 0.2026, 0.0, 0.0777, 0.0, 0.0001067 }, // PM
    },
    { // PKW_D_Euro_3
      { 4234, 691.1, 0.0, 149.4, 0.0, 0.2161 }, // CO2(total)
      { 4.436, 0.04963, 0.0, -0.1117, 0.0, 0.0 }, // CO
      { 0.3601, 0.04356, 0.0, 0.02848, 0.0, 0.0 }, // HC
      { 1332, 217.4, 0.0, 47, 0.0, 0.06797 }, // mKr
      { 65.19, 7.25, 0.0, -6.12, 0.2648, 0.0 }, // NOx
      { 1.773, 0.175, 0.0, -0.1295, 0.006047, 0.0 }, // PM
    },
    { // PKW_D_Euro_4
      { 6824, 742.3, 0.0, -195.3, 16.46, 0.0 }, // CO2(total)
      { 2.105, 0.02898, 0.0, -0.03593, 0.0, 0.0 }, // CO
      { 0.1208, 0.01649, 0.0, 0.02261, 0.0, 0.0 }, // HC
      { 2147, 233.5, 0.0, -61.45, 5.178, 0.0 }, // mKr
      { 47.45, 4.011, 0.0, -4.061, 0.1619, 0.0 }, // NOx
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // PM
    },
    { // PKW_D_Euro_5
      { 4125, 686.1, 0.0, 153.1, 0.0, 0.2125 }, // CO2(total)
      { 1.319, 0.02428, 0.0, -0.01683, 0.0, 0.0 }, // CO
      { 0.1285, 0.0168, 0.0, 0.02269, 0.0, 0.0 }, // HC
      { 1298, 215.8, 0.0, 48.15, 0.0, 0.06684 }, // mKr
      { 46.01, 4.064, 0.0, -3.872, 0.1567, 0.0 }, // NOx
      { 0.6729, 0.02248, 0.0, -0.0764, 0.002153, 0.0 }, // PM
    },
    { // PKW_D_Euro_6
      { 3946, 661.8, 0.0, 149.8, 0.0, 0.2041 }, // CO2(total)
      { 2.096, 0.02906, 0.0, -0.03578, 0.0, 0.0 }, // CO
      { 0.1368, 0.01692, 0.0, 0.02037, 0.0, 0.0 }, // HC
      { 1241, 208.2, 0.0, 47.1, 0.0, 0.06419 }, // mKr
      { 16.31, 1.39, 0.0, -1.391, 0.05512, 0.0 }, // NOx
      { 0.553, 0.01902, 0.0, -0.06184, 0.001758, 0.0 }, // PM
    },
    { // LBus
      { 1.903e+04, 6475, 0.0, 2073, 0.0, 0.0 }, // CO2(total)
      { 72.61, 7.482, 0.0, 0.6348, 0.0, 0.0 }, // CO
      { 17.46, 0.8473, 0.0, 0.254, 0.0, 0.0 }, // HC
      { 6016, 2049, 0.0, 656.6, 0.0, 0.0 }, // mKr
      { 218.7, 46.17, 0.0, 11.27, 0.0, 0.0 }, // NOx
      { 7.222, 0.8024, 0.0, 0.1201, 0.0, 0.0 }, // PM
    },
    { // RBus
      { 1.667e+04, 7763, 0.0, 2610, 0.0, 0.0 }, // CO2(total)
      { 70.85, 7.73, 0.0, 1.306, 0.0, 0.0 }, // CO
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // HC
      { 5244, 2442, 0.0, 820.9, 0.0, 0.0 }, // mKr
      { 226.7, 60.86, 0.0, 16.97, 0.0, 0.0 }, // NOx
      { 8.297, 0.8085, 0.0, 0.1417, 0.0, 0.0 }, // PM
    },
    { // SNF
      { 2.657e+04, 7076, 0.0, 1753, 0.0, 0.0 }, // CO2(total)
      { 54.72, 7.864, 0.0, 1.669, 0.0, 0.0 }, // CO
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // HC
      { 8358, 2226, 0.0, 551.4, 0.0, 0.0 }, // mKr
      { 305.6, 55.28, 0.0, 9.505, 0.0, 0.0 }, // NOx
      { 7.952, 0.854, 0.0, 0.1195, 0.0, 0.0 }, // PM
    },
    { // SNF_B
      { 7628, 3462, 0.0, 1058, 0.0, 0.9676 }, // CO2(total)
      { 464, 22.34, 0.0, -81.01, 5.955, -0.1182 }, // CO
      { 220.4, 3.65, 0.0, -13.27, 0.4346, 0.0 }, // HC
      { 2434, 1104, 0.0, 337.4, 0.0, 0.3087 }, // mKr
      { 53.53, 33.43, 0.0, 11.47, 0.0, 0.007806 }, // NOx
      { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_0
      { 3.251e+04, 7256, 0.0, 1631, 0.0, 0.0 }, // CO2(total)
      { 52.89, 12.75, 0.0, 4.547, 0.0, 0.0 }, // CO
      { 27.82, 1.453, 0.0, 0.2468, 0.0, 0.0 }, // HC
      { 1.023e+04, 2283, 0.0, 513.1, 0.0, 0.0 }, // mKr
      { 428.6, 104.2, 0.0, 24.18, 0.0, 0.0 }, // NOx
      { 14.15, 2.335, 0.0, 0.6566, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_1
      { 2.749e+04, 6533, 0.0, 1538, 0.0, 0.0 }, // CO2(total)
      { 58.3, 8.423, 0.0, 1.86, 0.0, 0.0 }, // CO
      { 19.78, 1.929, 0.0, 0.2819, 0.0, 0.0 }, // HC
      { 8647, 2055, 0.0, 483.7, 0.0, 0.0 }, // mKr
      { 299.3, 67.56, 0.0, 14.89, 0.0, 0.0 }, // NOx
      { 11.9, 1.744, 0.0, 0.3954, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_2
      { 2.537e+04, 6723, 0.0, 1689, 0.0, 0.0 }, // CO2(total)
      { 41.4, 5.325, 0.0, 1.366, 0.0, 0.0 }, // CO
      { 13.69, 1.19, 0.0, 0.1655, 0.0, 0.0 }, // HC
      { 7980, 2115, 0.0, 531.2, 0.0, 0.0 }, // mKr
      { 298.4, 67.21, 0.0, 15.36, 0.0, 0.0 }, // NOx
      { 4.584, 0.789, 0.0, 0.2434, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_3
      { 2.598e+04, 6712, 0.0, 1728, 0.0, 0.0 }, // CO2(total)
      { 51.47, 4.238, 0.0, 1.078, 0.0, 0.0 }, // CO
      { 12.25, 0.9033, 0.0, 0.1305, 0.0, 0.0 }, // HC
      { 8174, 2111, 0.0, 543.5, 0.0, 0.0 }, // mKr
      { 241.1, 51.57, 0.0, 11.42, 0.0, 0.0 }, // NOx
      { 5.436, 0.5336, 0.0, 0.1604, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_4
      { 2.429e+04, 7180, 0.0, 1835, 0.0, 0.0 }, // CO2(total)
      { 50.68, 6.655, 0.0, 0.8349, 0.0, 0.0 }, // CO
      { 1.119, 0.1747, 0.0, 0.03217, 0.0, 0.0 }, // HC
      { 7639, 2259, 0.0, 577.1, 0.0, 0.0 }, // mKr
      { 202.2, 42.34, 0.0, 8.858, 0.0, 0.0 }, // NOx
      { 1.267, 0.131, 0.0, -0.006405, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_5
      { 2.396e+04, 7231, 0.0, 1868, 0.0, 0.0 }, // CO2(total)
      { 51.51, 6.785, 0.0, 0.8496, 0.0, 0.0 }, // CO
      { 1.124, 0.175, 0.0, 0.03241, 0.0, 0.0 }, // HC
      { 7536, 2274, 0.0, 587.6, 0.0, 0.0 }, // mKr
      { 164.8, 33.84, 0.0, 7.036, 0.0, 0.0 }, // NOx
      { 1.305, 0.1324, 0.0, -0.007715, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Euro_6
      { 2.353e+04, 7227, 0.0, 1889, 0.0, 0.0 }, // CO2(total)
      { 30.78, 4.512, 0.0, 0.6383, 0.0, 0.0 }, // CO
      { 0.7469, 0.136, 0.0, 0.03002, 0.0, 0.0 }, // HC
      { 7400, 2273, 0.0, 594.2, 0.0, 0.0 }, // mKr
      { 47.47, 11.68, 0.0, 2.737, 0.0, 0.0 }, // NOx
      { 0.1279, 0.01668, 0.0, 0.00329, 0.0, 0.0 }, // PM
    },
    { // SNF_D_Ost
      { 2.017e+04, 6019, 0.0, 1723, 0.0, 0.0 }, // CO2(total)
      { 51.97, 16.95, 0.0, 6.523, 0.0, 0.0 }, // CO
      { 78.82, 2.36, 0.0, -4.421, 0.1487, 0.0 }, // HC
      { 6344, 1893, 0.0, 541.9, 0.0, 0.0 }, // mKr
      { 299.7, 91.14, 0.0, 25.15, 0.0, 0.0 }, // NOx
      { 13.59, 3.709, 0.0, 1.39, 0.0, 0.0 }, // PM
    },
};


// ===========================================================================
// method definitions
// ===========================================================================
HelpersHBEFA3::HelpersHBEFA3() : PollutantsInterface::Helper("HBEFA3") {
    int index = HBEFA3_BASE;
    myEmissionClassStrings.insert("zero", index++);
    std::string light[] = { "LNF", "LNF_B_Euro_0", "LNF_B_Euro_1", "LNF_B_Euro_2", "LNF_B_Euro_3", "LNF_B_Euro_4", "LNF_B_Euro_5", "LNF_B_Euro_6", "LNF_B_Ost",
                            "LNF_D_Euro_0", "LNF_D_Euro_1", "LNF_D_Euro_2", "LNF_D_Euro_3", "LNF_D_Euro_4", "LNF_D_Euro_5", "LNF_D_Euro_6", 
                            "PKW", "PKW_Alternative", "PKW_B_Euro_0", "PKW_B_Euro_1", "PKW_B_Euro_2", "PKW_B_Euro_3", "PKW_B_Euro_4", "PKW_B_Euro_5", "PKW_B_Euro_6", "PKW_B_Ost",
                            "PKW_D_Euro_0", "PKW_D_Euro_1", "PKW_D_Euro_2", "PKW_D_Euro_3", "PKW_D_Euro_4", "PKW_D_Euro_5", "PKW_D_Euro_6" };
    std::string heavy[] = { "LBus", "RBus", "SNF", "SNF_B", "SNF_D_Euro_0", "SNF_D_Euro_1", "SNF_D_Euro_2", "SNF_D_Euro_3", "SNF_D_Euro_4", "SNF_D_Euro_5", "SNF_D_Euro_6", "SNF_D_Ost"};
    for (int i = 0; i < 33; i++) {
        myEmissionClassStrings.insert(light[i], index++);
    }
    for (int i = 0; i < 12; i++) {
        myEmissionClassStrings.insert(heavy[i], index++ | PollutantsInterface::HEAVY_BIT);
    }
    myEmissionClassStrings.addAlias("unknown", myEmissionClassStrings.get("PKW"));
}


SUMOEmissionClass
HelpersHBEFA3::getClass(const SUMOEmissionClass base, const std::string& vClass, const std::string& fuel, const std::string& eClass, const double weight) const {
    std::string eClassOffset = "0";
    if (eClass.length() == 5 && eClass.substr(0, 4) == "Euro") {
        if (eClass[4] >= '0' && eClass[4] <= '6') {
            eClassOffset = eClass.substr(4, 1);
        }
    }
    std::string desc;
    if (vClass == "Passenger") {
        desc = "PKW_";
        if (fuel == "Gasoline") {
            desc += "B_";
        } else if (fuel == "Diesel") {
            desc += "D_";
        }
        desc += "Euro_" + eClassOffset;
    } else if (vClass == "Delivery") {
        desc = "LNF_";
        if (fuel == "Gasoline") {
            desc += "B_";
        } else if (fuel == "Diesel") {
            desc += "D_";
        }
        desc += "Euro_" + eClassOffset;
    } else if (vClass == "UrbanBus") {
        desc = "LBus";
    } else if (vClass == "Coach") {
        desc = "RBus";
    } else if (vClass == "Truck" || vClass == "Trailer") {
        if (fuel == "Gasoline") {
            desc = "SNF_B";
        } else if (fuel == "Diesel") {
            desc = "SNF_D_Euro_" + eClassOffset;
        }
    }
    if (myEmissionClassStrings.hasString(desc)) {
        return myEmissionClassStrings.get(desc);
    }
    return myEmissionClassStrings.get("unknown");
}


std::string
HelpersHBEFA3::getAmitranVehicleClass(const SUMOEmissionClass c) const {
    const std::string name = myEmissionClassStrings.getString(c);
    if (name.find("RBus") != std::string::npos) {
        return "Coach";
    } else if (name.find("LBus") != std::string::npos) {
        return "UrbanBus";
    } else if (name.find("LNF") != std::string::npos) {
        return "Delivery";
    } else if (name.find("SNF") != std::string::npos) {
        return "Truck";
    }
    return "Passenger";
}


std::string
HelpersHBEFA3::getFuel(const SUMOEmissionClass c) const {
    const std::string name = myEmissionClassStrings.getString(c);
    std::string fuel = "Gasoline";
    if (name.find("_D_") != std::string::npos) {
        fuel = "Diesel";
    }
    return fuel;
}


int
HelpersHBEFA3::getEuroClass(const SUMOEmissionClass c) const {
    const std::string name = myEmissionClassStrings.getString(c);
    if (name.find("_Euro_1") != std::string::npos) {
        return 1;
    } else if (name.find("_Euro_2") != std::string::npos) {
        return 2;
    } else if (name.find("_Euro_3") != std::string::npos) {
        return 3;
    } else if (name.find("_Euro_4") != std::string::npos) {
        return 4;
    } else if (name.find("_Euro_5") != std::string::npos) {
        return 5;
    } else if (name.find("_Euro_6") != std::string::npos) {
        return 6;
    }
    return 0;
}


/****************************************************************************/

