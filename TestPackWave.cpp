// TestPackWave.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>

#include "cpp\PackWave.h"
#include <math.h>
#include <stdio.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static float Deviation(i16* w1, i16* w2, u16 len)
{
    float res = 0;

    for (u32 i = 0; i < len; i++)
    {
        float t = (float)(w1[i] - w2[i]);
        res += t * t;
    };

    res /= len;

    return sqrtf(res);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static float Determination(i16* w1, i16* w2, u16 len)
{
    float e = 0;
    float y = 0;

    for (u32 i = 0; i < len; i++)
    {
        float t = (float)(w1[i] - w2[i]);
        e += t * t;

        y += (float)(w1[i] * w1[i]);
    };

    return (1-e/(y-y/len));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i16 wave1[996] = {  257, 287, 265, 174, 26, -134, -248, -275, -211, -89, 34, 107, 103, 35, -59, -131,-148,-100,-8,88,144,136,77,8,-23,0,52,77,40,-43,-105,-81,
    39,189,266,206,33,-153,-243,-194,-50,93,157,127,46,-24,-47,-26,8,28, 23, 4, -13, -18, -13, -2, 3, -3, -22, -49, -74, -77, -39, 50, 179,
    312, 392, 367, 212, -47, -338, -562, -638, -538, -305, -35, 163, 216, 118, -66, -237,-301, -217, -9, 254, 493, 657, 738, 756, 725, 633, 433, 
    83, -414, -979, -1454, -1649,-1423, -757, 196, 1138, 1725, 1712, 1076, 45, -988, -1648, -1741, -1327, -646, 29, 539, 871,1085, 1205, 1166, 865, 
    283, -440, -1018, -1169, -775, 19, 860, 1360, 1295, 716, -82, -723,-943, -717, -249, 165, 306, 146, -165, -425, -520, -503, -537, -766, -1193, -1654, -1911, -1789,-1263, -437, 565, 1694, 2987, 4470, 6025, 7326, 7906, 7332, 5394, 2213, -1780, -5968, -9704, -12433,
    -13765, -13506, -11672, -8489, -4374, 127, 4424, 7997, 10486, 11744, 11845, 11023, 9577, 7749, 5636, 3170,197, -3379, -7438, -11567, -15085, -17208, -17259, -14878, -10158, -3659, 3692, 10795, 16605, 20338, 21591, 20342,
    16862, 11582, 5001, -2325, -9724, -16361, -21266, -23495, -22387, -17824, -10388, -1307, 7801, 15336, 20110, 21615,20036, 16059, 10563, 4356, -1919, -7722, -12500, -15652, -16661, -15329, -11970, -7404, -2697, 1234, 3944, 5539,
    6470, 7184, 7831, 8218, 7976, 6843, 4864, 2396, -40, -1972, -3121, -3441, -3077, -2298, -1427, -783,-605, -981, -1815, -2858, -3808, -4412, -4530, -4127, -3235, -1913, -261, 1552, 3271, 4597, 5280, 5213,
    4489, 3375, 2218, 1312, 795, 606, 538, 355, -78, -736, -1456, -2060, -2468, -2745, -3039, -3460,-3982, -4443, -4625, -4365, -3605, -2367, -712, 1276, 3433, 5447, 6861, 7204, 6203, 3964, 1004, -1905,
    -4012, -4852, -4360, -2816, -669, 1630, 3707, 5250, 5967, 5603, 4049, 1462, -1675, -4658, -6801, -7695,-7347, -6128, -4562, -3083, -1883, -927, -75, 795, 1719, 2678, 3649, 4628, 5589, 6418, 6890, 6721,
    5698, 3803, 1277, -1439, -3845, -5554, -6408, -6487, -6019, -5251, -4339, -3320, -2148, -773, 796, 2464,4067, 5413, 6326, 6661, 6328, 5292, 3606, 1433, -944, -3151, -4807, -5640, -5583, -4801, -3616, -2367,
    -1283, -420, 304, 1017, 1809, 2693, 3615, 4471, 5112, 5344, 4953, 3789, 1862, -585, -3113, -5204,-6434, -6594, -5731, -4094, -2035, 69, 1877, 3117, 3621, 3357, 2453, 1176, -130, -1132, -1590, -1409,
    -653, 480, 1691, 2647, 3055, 2741, 1707, 140, -1642, -3282, -4473, -5019, -4842, -3988, -2606, -945,679, 1961, 2699, 2875, 2656, 2303, 2042, 1961, 1987, 1948, 1671, 1039, 16, -1371, -3023, -4750,
    -6253, -7162, -7130, -5972, -3754, -804, 2372, 5231, 7316, 8335, 8180, 6908, 4727, 1977, -894, -3385,-5038, -5561, -4927, -3410, -1504, 237, 1395, 1824, 1655, 1186, 697, 327, 41, -276, -691, -1149,
    -1485, -1531, -1213, -598, 162, 923, 1632, 2315, 2998, 3599, 3891, 3578, 2455, 585, -1651, -3658,-4863, -4960, -4021, -2420, -632, 975, 2229, 3093, 3544, 3490, 2821, 1529, -176, -1876, -3087, -3463,
    -2941, -1753, -304, 996, 1877, 2249, 2167, 1750, 1129, 421, -263, -811, -1131, -1187, -1023, -756, -514, -365,-273, -114, 248, 865, 1652, 2394, 2823, 2722, 2002, 737, -877, -2585, -4142, -5338, -5998, -5957, -5078, -3312,
    -800, 2064, 4691, 6466, 6982, 6200, 4441, 2220, 21, -1844, -3225, -4024, -4105, -3320, -1640, 706, 3206, 5196, 6091,5593, 3774, 1021, -2111, -5056, -7339, -8629, -8743, -7642, -5429, -2357, 1179, 4655, 7492, 9171, 9364, 8037, 5473, 2193,
    -1187, -4106, -6150, -7085, -6845, -5518, -3342, -691, 1969, 4167, 5531, 5856, 5130, 3503, 1245, -1286, -3688, -5552, -6540,-6463, -5330, -3345, -844, 1795, 4214, 6108, 7223, 7367, 6443, 4501, 1767, -1362, -4393, -6832, -8280, -8500, -7450, -5278,
    -2305, 1019, 4185, 6709, 8218, 8520, 7623, 5721, 3137, 268, -2479, -4734, -6208, -6732, -6283, -4991, -3107, -956, 1130,2854, 3979, 4339, 3864, 2618, 848, -1029, -2506, -3143, -2719, -1327, 665, 2749, 4439, 5395, 5454, 4614, 2994, 823,
    -1568, -3776, -5401, -6155, -5944, -4893, -3288, -1477, 231, 1621, 2581, 3072, 3105, 2725, 2008, 1068, 60, -830, -1411,-1527, -1109, -217, 948, 2088, 2891, 3129, 2721, 1747, 385, -1153, -2684, -4056, -5117, -5686, -5578, -4669, -2992, -773,
    1610, 3746, 5326, 6208, 6393, 5933, 4862, 3198, 1030, -1386, -3612, -5145, -5608, -4932, -3393, -1494, 249, 1506, 2188,2375, 2176, 1651, 831, -178, -1123, -1649, -1467, -537, 847, 2159, 2871, 2696, 1688, 178, -1418, -2778, -3748, -4290,
    -4379, -3954, -2945, -1380, 544, 2471, 4002, 4841, 4905, 4319, 3321, 2140, 922, -266, -1383, -2357, -3089, -3495, -3559,-3347, -2961, -2480, -1922, -1260, -485, 350, 1124, 1707, 2023, 2085, 1978, 1799, 1611, 1440, 1297, 1208, 1207, 1288,
    1366, 1278, 850, -1, -1204, -2530, -3677, -4376, -4466, -3917, -2805, -1278, 460, 2161, 3554, 4400, 4565, 4056, 3016, 1665,229, -1091, -2126, -2725, -2769, -2219, -1163, 175, 1497, 2527, 3106, 3217, 2943, 2384, 1611, 665, -392, -1430, -2254,
    -2670, -2583, -2047, -1255, -434, 260, 804, 1268, 1713, 2097, 2260, 2023, 1321, 288, -777, -1563, -1911, -1885, -1708,-1605, -1667, -1819, -1917, -1876, -1735, -1611, -1576, -1554, -1336, -700, 426, 1870, 3255, 4159, 4289, 3590, 2233, 503,
    -1318, -3010, -4401, -5314, -5560, -4990, -3598, -1587, 654, 2671, 4101, 4782, 4743, 4124, 3066, 1673, 42, -1672, -3229, -4349,-4814, -4545, -3619, -2207, -491, 1378, 3273, 5044, 6483, 7349, 7442, 6680, 5148, 3076, 785, -1400, -3212, -4471, -5092,
    -5074, -4482, -3439, -2112, -705, 570, 1522, 2024, 2049, 1680, 1094, 515, 149, 123, 460, 1081, 1842, 2580, 3148,3430, 3331, 2774, 1709, 162, -1724, -3668, -5289, -6212, -6195, -5219, -3504, -1426, 601, 2243, 3291, 3654, 3325, 2356,
    873, -901, -2660, -4072, -4883, -4997, -4500, -3596, -2506, -1382, -286, 764, 1725, 2500, 2954, 2993, 2613, 1906, 1015,69, -852, -1691, -2375, -2785, -2791, -2326, -1447, -346, 717, 1512, 1913, 1910, 1559, 933, 95, -871, -1825, -2566, -2880,
    -2627, -1811, -589, 787, 2068, 3081, 3761, 4116, 4179, 3975, 3516, 2823, 1954, 1002, 77, -722, -1325, -1700, -1841, -1766,-1519, -1168, -794, -461, -187, 68, 369, 761, 1226, 1679, 2011, 2139, 2043, 1749, 1295, 696, -44, -879, -1678, -2237,
    -2352, -1936, -1082, -50, 846, 1375, 1480, 1264, 901, 523, 159, -239, -709, -1214, -1628, -1810, -1696, -1352
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i16 wave2[996] = { 235,281,279,205,65,-99,-227,-273,-225,-110,16,97,106,46,-45,-122,-147,-107,-18,81,143,142,85,15,-20,1,52,78,39,-52,-125,-110,12,174,271,229,64,
    -128,-234,-202,-69,73,143,121,46,-21,-44,-25,7,25,22,5,-11,-16,-10,0,5,-1,-20,-49,-74,-78,-40,48,176,308,389,366,214,-44,-335,-562,-641,-543,-310,
    -38,163,218,122,-63,-234,-301,-218,-10,253,493,657,737,755,726,635,438,91,-405,-971,-1450,-1651,-1431,-769,183,1130,1724,1719,1087,56,-982,-1649,
    -1748,-1337,-658,19,531,866,1083,1206,1172,875,295,-429,-1013,-1172,-784,8,852,1359,1300,723,-77,-722,-946,-722,-251,168,314,156,-157,-422,-522,
    -508,-543,-771,-1198,-1662,-1923,-1806,-1281,-453,553,1686,2982,4468,6027,7338,7931,7372,5446,2270,-1727,-5929,-9686,-12440,-13798,-13563,-11747,
    -8573,-4455,59,4380,7982,10501,11785,11904,11092,9648,7815,5693,3216,231,-3356,-7430,-11577,-15118,-17265,-17338,-14972,-10257,-3748,3627,10764,
    16614,20385,21668,20439,16968,11687,5096,-2248,-9675,-16349,-21298,-23571,-22498,-17957,-10522,-1422,7725,15309,20135,21684,20135,16172,10673,4452,
    -1845,-7677,-12490,-15679,-16722,-15415,-12066,-7493,-2764,1194,3930,5544,6485,7201,7851,8242,8006,6881,4904,2432,-13,-1957,-3117,-3446,-3087,-2310,
    -1439,-792,-611,-984,-1816,-2861,-3813,-4420,-4541,-4142,-3251,-1931,-279,1536,3259,4593,5284,5224,4505,3392,2233,1324,804,614,547,368,-65,-723,-1447,
    -2057,-2471,-2752,-3048,-3470,-3993,-4454,-4639,-4382,-3625,-2389,-733,1259,3424,5447,6873,7229,6238,4003,1039,-1881,-4006,-4865,-4387,-2850,-703,1603,
    3690,5246,5977,5629,4087,1506,-1634,-4631,-6795,-7710,-7379,-6168,-4603,-3118,-1911,-950,-93,780,1708,2672,3649,4634,5600,6433,6910,6748,5732,3842,1315,
    -1409,-3828,-5554,-6423,-6513,-6051,-5284,-4372,-3352,-2178,-798,777,2454,4066,5422,6344,6689,6363,5332,3646,1468,-918,-3138,-4807,-5654,-5608,-4832,
    -3647,-2393,-1301,-430,299,1016,1810,2696,3620,4479,5123,5358,4972,3812,1887,-565,-3101,-5204,-6446,-6617,-5760,-4125,-2065,45,1862,3113,3628,3373,2474,
    1197,-114,-1123,-1590,-1418,-668,463,1677,2641,3059,2755,1729,165,-1620,-3268,-4470,-5028,-4865,-4020,-2642,-981,650,1942,2694,2883,2672,2323,2062,1978,
    2001,1961,1684,1052,28,-1361,-3018,-4752,-6265,-7183,-7162,-6011,-3795,-842,2343,5216,7318,8356,8216,6954,4776,2021,-863,-3371,-5043,-5583,-4960,-3446,
    -1536,215,1387,1827,1667,1200,710,337,47,-272,-691,-1151,-1491,-1539,-1223,-608,154,919,1631,2317,3001,3605,3901,3593,2475,605,-1636,-3654,-4873,-4982,
    -4049,-2447,-654,962,2224,3096,3553,3506,2843,1554,-153,-1861,-3086,-3477,-2966,-1782,-331,979,1871,2255,2181,1769,1149,439,-250,-805,-1132,-1194,-1036,
    -772,-531,-380,-285,-121,245,867,1658,2404,2837,2738,2020,753,-866,-2580,-4143,-5347,-6015,-5983,-5109,-3345,-832,2041,4680,6471,7001,6230,4475,2251,43,
    -1832,-3222,-4030,-4120,-3343,-1667,680,3188,5192,6104,5621,3811,1059,-2081,-5039,-7339,-8644,-8772,-7681,-5475,-2403,1139,4628,7485,9187,9400,8087,5528,
    2243,-1152,-4090,-6155,-7106,-6878,-5557,-3381,-725,1944,4156,5536,5878,5164,3542,1283,-1258,-3675,-5556,-6560,-6496,-5370,-3386,-879,1770,4203,6112,7242,
    7400,6486,4548,1812,-1327,-4374,-6832,-8299,-8536,-7497,-5330,-2355,979,4162,6706,8235,8553,7667,5768,3181,303,-2458,-4727,-6216,-6753,-6313,-5025,-3140,
    -983,1112,2847,3982,4352,3885,2645,875,-1008,-2498,-3149,-2738,-1354,638,2729,4432,5401,5473,4642,3026,854,-1543,-3761,-5400,-6168,-5969,-4925,-3322,-1507,
    210,1611,2580,3078,3117,2740,2024,1082,70,-828,-1416,-1537,-1122,-229,940,2087,2898,3142,2740,1767,404,-1138,-2676,-4057,-5127,-5706,-5605,-4701,-3023,-800,
    1591,3738,5329,6221,6414,5960,4893,3230,1059,-1363,-3601,-5148,-5626,-4960,-3424,-1522,230,1497,2188,2382,2188,1666,847,-164,-1113,-1648,-1475,-552,832,
    2150,2875,2712,1711,201,-1401,-2772,-3752,-4301,-4397,-3976,-2972,-1408,520,2456,4000,4853,4928,4348,3350,2166,942,-253,-1377,-2359,-3099,-3511,-3579,-3367,
    -2978,-2492,-1930,-1266,-489,346,1123,1709,2027,2091,1984,1803,1615,1443,1299,1211,1209,1290,1369,1283,859,10,-1192,-2522,-3677,-4385,-4483,-3940,-2831,-1303,
    440,2147,3549,4405,4579,4077,3041,1689,249,-1077,-2120,-2728,-2780,-2236,-1182,158,1486,2523,3109,3227,2957,2401,1628,680,-382,-1426,-2256,-2679,-2595,-2062,
    -1269,-445,254,802,1268,1715,2100,2267,2034,1334,301,-768,-1561,-1915,-1891,-1714,-1610,-1670,-1823,-1923,-1883,-1741,-1616,-1578,-1555,-1340,-707,416,1860,
    3251,4163,4300,3608,2253,521,-1305,-3005,-4403,-5326,-5581,-5019,-3631,-1618,630,2659,4103,4794,4763,4147,3089,1694,59,-1662,-3227,-4359,-4834,-4572,-3648,
    -2232,-511,1364,3267,5044,6491,7366,7467,6711,5179,3105,808,-1386,-3207,-4475,-5104,-5093,-4505,-3462,-2133,-721,560,1519,2028,2057,1690,1104,523,153,124,459,
    1079,1841,2581,3153,3440,3346,2791,1729,181,-1710,-3660,-5291,-6226,-6220,-5252,-3538,-1456,581,2234,3293,3666,3344,2380,898,-879,-2646,-4070,-4893,-5018,
    -4528,-3625,-2532,-1403,-301,755,1723,2503,2963,3006,2628,1921,1027,76,-851,-1696,-2384,-2798,-2807,-2344,-1465,-362,705,1508,1918,1921,1574,948,107,-864,
    -1826,-2575,-2896,-2650,-1839,-618,761,2048,3069,3756,4117,4185,3985,3529,2838,1968,1013,84,-719,-1327,-1706,-1850,-1776,-1530,-1179,-805,-471,-194,62,364,
    756,1221,1676,2011,2143,2050,1759,1307,707,-34,-872,-1676,-2240,-2363,-1951,-1099,-63,840,1378,1490,1279,919,542,179,-218,-689,-1197,-1616,-1805,-1697,-1357
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i16 wave3[512] = { 2,14,16,13,7,-1,-13,-9,-12,-10,-3,8,9,16,23,24,22,27,26,24,25,25,24,25,23,18,16,8,11,12,12,14,19,17,16,18,17,11,14,15,11,10,13,10,9,10,9,8,5,7,7,4,6,3,
    -5,-23,-43,-63,-72,-45,29,156,305,426,435,261,-121,-651,-1174,-1483,-1390,-811,165,1290,2216,2620,2325,1368,19,-1337,-2319,-2706,-2487,-1805,-861,178,
    1200,2089,2662,2698,2020,648,-1132,-2813,-3801,-3659,-2316,-129,2217,3950,4517,3746,1891,-471,-2703,-4281,-4834,-4195,-2427,171,3015,5284,6186,5291,2751,
    -647,-3804,-5772,-6082,-4872,-2695,-233,1987,3678,4661,4837,4190,2797,894,-1164,-3000,-4317,-4962,-4916,-4236,-2975,-1185,984,3306,5470,7034,7591,6842,
    4699,1430,-2318,-5707,-7929,-8414,-7034,-4220,-771,2342,4304,4671,3335,622,-2762,-5916,-7829,-7606,-4739,681,7692,14658,19765,21378,18587,11483,1137,
    -10553,-21415,-29403,-32176,-27030,-22208,-11620,2663,16770,27967,32136,31274,26366,20220,9066,-2911,-13393,-21288,-26056,-27733,-26434,-22059,-14620,
    -4723,6320,16715,24537,28018,26446,20392,11280,1080,-8096,-14677,-17770,-17216,-13722,-8593,-3239,1174,3907,4829,4394,3332,2330,1786,1777,1993,1992,1658,
    1086,491,116,-19,-105,-304,-593,-726,-510,38,715,1090,546,-1193,-3660,-5964,-7185,-6653,-4223,-359,4012,7815,10120,10344,8444,4766,-59,-4978,-8720,-10301,
    -9401,-6294,-1801,2848,6531,8652,9043,7724,4912,995,-3421,-7389,-9972,-10605,-9245,-6270,-2309,1757,5081,7316,8448,8574,7801,6189,3894,1080,-1986,-4813,
    -6935,-8017,-7826,-6329,-3790,-687,2480,5141,6706,6819,5471,2974,-59,-2738,-4259,-4280,-2935,-810,1229,2446,2442,1200,-804,-2845,-4464,-5430,-5430,-4131,
    -1476,2196,6306,9959,11991,11588,8688,3986,-1300,-6019,-9533,-11536,-11858,-10518,-7756,-4033,22,3666,6137,7072,6809,5962,5017,4281,3724,3059,1899,56,-2232,
    -4436,-5974,-6509,-5999,-4545,-2495,-459,1071,1958,2257,2025,1419,774,451,700,1534,2742,4028,4939,4804,3167,129,-3671,-7272,-9655,-9972,-7970,-4209,313,4580,
    7844,9624,9534,7367,3439,-1379,-5913,-8962,-9640,-7740,-3893,636,4576,7107,7965,7251,5126,1968,-1491,-4460,-6259,-6478,-5128,-2771,-355,1358,2084,1981,1374,
    515,-332,-873,-779,132,1774,3859,5712,6362,5155,2153,-1888,-5921,-9126,-10951,-10936,-8820,-4833,418,6141,11198,14256,14350,11429,6345,379,-5281,-9645,-11966,
    -11948,-9773,-5993,-1341,3368,7126,9110,9104,7387,4482,1018,-2302,-4774,-5991,-5964,-5031,-3638,-2169,-886,152,1164,2482,4158,5762,6766,6786,5622,3350,271,
    -3233,-6705,-9610,-11265,-10958,-8291,-3549,2231,7633,11424,12862,11814,8754,4440,-418,-5169,-9181,-11808,-12393,-10544,-6546,-1334,3870,7983,10395,10977,
    9865,7327,3692,-607,-4915,-8425,-10340,-10238,-8267,-5034,-1385,1843,4157,5566,6332,6516,6079,5106,3678,1896,-45,-1983,-3838,-5524,-6927,-7773,-7694,-6447,
    -4041,-723,3049,6682,9562,11115
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i16 wave4[1024] = { -1,-1,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,2,0,-6,-17,-34,-45,-33,25,132,261,339,281,37,-354,-747,-954,-836,-395,225,808,1170,1222,978,513,
     -61,-608,-992,-1106,-927,-523,-35,392,645,674,528,323,160,67,-6,-156,-392,-597,-673,-615,-468,-291,-84,232,658,1042,1201,1027,521,-162,-739,-942,-654,-55,419,
     376,-192,-843,-1024,-561,208,709,580,38,-344,-225,287,787,859,461,-63,-394,-425,-214,64,267,361,276,-50,-543,-1016,-1220,-968,-315,465,1076,1298,1074,518,-176,
     -742,-947,-745,-278,225,509,497,314,76,-129,-183,-16,343,745,874,460,-409,-1294,-1732,-1532,-908,-255,251,666,1082,1470,1617,1307,645,21,-255,-173,-73,-377,
     -1106,-1804,-1941,-1542,-482,462,1012,1225,1272,1249,1096,652,-88,-804,-1081,-800,-242,133,87,-140,-132,171,448,347,-158,-631,-568,15,622,754,248,-509,-796,
     -247,860,1843,1754,1255,-94,-1224,-1824,-1921,-1635,-1329,-1156,-1226,-1425,-1245,-1077,-921,-780,845,2048,2048,2048,1819,1608,1411,1229,1061,907,765,635,515,
     407,308,205,-881,-1879,-1935,-1935,-1935,-1940,-1817,-1572,-1371,-1179,-1005,-845,96,1284,2048,2048,1572,600,-195,-416,-90,466,841,812,480,105,-106,-58,207,534,
     781,873,802,603,341,98,-29,76,460,1037,1640,1967,1737,1536,652,-132,-450,-138,486,1007,940,791,157,-558,-1046,-1208,-1040,-714,-392,-141,-76,-282,-686,-1140,
     -1499,-1675,-1690,-1587,-1425,-1295,-1287,-1497,-1695,-1454,-1256,-1075,-908,-756,-567,-88,42,78,247,662,1190,1680,2048,2048,1783,1117,455,124,269,761,1399,2045,
     2048,2048,1821,1595,1385,1193,1016,852,702,565,452,-268,-775,-1195,-1649,-1937,-1935,-1935,-1609,-734,-20,228,44,-283,-460,-459,-467,-598,-825,-1099,-1445,-1881,
     -1921,-1630,-1433,-1239,-1061,-897,-747,-610,-258,492,1101,1633,2012,2048,2048,2048,2048,2048,1676,807,-121,-744,-921,-658,-180,269,669,1021,1214,1171,899,512,285,
     446,943,870,731,603,485,-218,-1343,-1934,-1935,-1935,-1935,-1937,-1556,-1512,-1759,-1534,-1334,-1152,-644,671,1682,2048,2038,1579,901,111,-609,-1113,-1276,-1033,
     -578,-203,-36,-109,-331,-414,-93,680,1768,2048,2048,1957,1740,1525,1335,230,-511,-760,-575,-233,-70,-288,-829,-1356,-1518,-1190,-477,346,996,1398,1258,1097,948,
     440,-412,-1189,-1665,-1820,-1811,-1753,-1733,-1783,-1838,-1806,-1606,-1399,-884,-76,862,1597,1797,1377,641,31,-217,-117,114,212,122,0,-17,75,209,319,514,1013,1824,
     2048,2041,1808,1595,1399,751,-17,-530,-856,-1026,-1071,-1088,-1226,-1593,-1935,-1941,-1802,-1584,-616,830,1959,2048,2048,1925,1709,1107,432,-161,-585,-825,-984,
     -1199,-1529,-1833,-1904,-1460,-700,153,767,929,678,168,-425,-916,-1218,-1334,-1170,-775,-145,491,977,1216,1296,1439,1758,2048,2048,2016,1795,1578,1376,1191,1020,
     863,720,588,86,-630,-1434,-1935,-1935,-1935,-1940,-1760,-1575,-1377,-383,354,850,1076,1009,740,448,239,134,58,-118,-427,-766,-1009,-1053,-870,-548,-199,107,362,593,
     800,939,1013,1090,1276,1631,2048,2048,2048,1835,1603,1312,552,-165,-720,-954,-761,-212,431,860,920,717,483,382,424,334,232,-83,-623,-1191,-1737,-1937,-1935,-1935,
     -1935,-1537,-831,-446,-499,-878,-1307,-1596,-1488,-1293,-1111,-943,-789,-648,-519,-401,-293,-196,-107,-28,41,688,1472,2014,2048,2048,2048,2048,2048,2048,1833,1596,
     1381,1184,1004,839,-151,-573,-498,-147,222,494,397,289,191,102,23,-509,-970,-1221,-1386,-1636,-1927,-1935,-1935,-1935,-1944,-1930,-1671,-1438,-1239,-1056,-730,-525,
     -549,-504,-382,-275,280,910,1386,1559,1423,1066,619,262,148,358,869,1522,2048,2048,2048,2048,2048,1822,1584,1371,1174,994,841,387,-64,-574,-1063,-1372,-1367,-1050,
     -560,-94,180,217,55,-283,-776,-1357,-1859,-1938,-1937,-1941,-1920,-1757,-1708,-1643,-1487,-1236,-943,-639,-292,130,561,843,851,622,361,292,475,807,1145,1366,1403,
     1280,1072,861,748,787,943,1172,1421,1601,1458,1265,843,195,-361,-712,-836,-812,-766,-771,-832,-920,-1001,-1060,-1069,-987,-798,-567,-410,-416,-590,-811,-909,-797,
     -505,-129,195,345,291,88,-169,-354,-382,-256,-39,185,341,391,329,206,109,112,233,423,608,762,906,1040,1118,1088,939,730,564,516,550,571,520,356,94,-207,-515,-818,
     -1067,-1237,-1350,-1426,-1475,-1492,-1430,-1221,-869,-455,-67,207,309,278,197,139,171,290,409,488,538,525,418,199,-138,-502,-751,-791,-627,-349,-37,297,655,996,1249,
     1355,1312,1198,1084,947,728,442,137,-134,-353,-554,-789,-1023,-1156,-1083,-769,-331,29,205,218,145,81,58,29,-40,-144,-283,-469,-694,-902,-998,-907,-650,-345,-122,
     -12,54,145,274,410,518,612,745,928,1087,1119,966,653,280,-51,-325,-580,-806,-912,-803,-493,-137,106,206,225,222,224,212,154,60,-44,-152,-268,-404,-561,-700,-779,
     -791,-764,-737,-706,-620,-449,-205,62,318,580,871,1141,1332,1415,1362,1157,824,431,87,-111,-149,-128,-147,-238,-363,-443,-401,-216,79,423,739,945,993,885,644,316,
     -34,-341,-574,-755,-934,-1145,-1368,-1515,-1485,-1256,-898,-502,-138,169,410,555,566,449,258,66,-98,-279,-533,-857,-1153,-1301,-1248,-1032,-716,-326,155,720,1272,
     1669,1826,1749,1516,1210,858,467,92,-200,-360,-399,-365,-298,-204,-58,176,484,809,1101,1324,1464,1457,1263,1090,944,362,-204,-685,-1055,-1345,-1552,-1654
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i16 wave5[512] = { -26,-26,11,83,112,42,-134,-272,-341,-332,-218,-74,73,84,44,-70,-153,-180,-53,68,172,232,364,465,400,203,51,-47,-229,-393,-655,-784,-935,-1179,-1472,-1339,
    -922,-272,201,497,546,457,77,-190,-236,821,2863,3772,2589,171,-1498,-1909,-1440,-370,685,305,-2011,-3935,-5001,-2999,1057,2831,1089,-2962,-5052,-3624,330,
    5341,7825,5802,670,-3851,-4789,-1033,3971,7063,5701,906,-3748,-5748,-3608,577,2914,1589,-1520,-5090,-6752,-5719,-2114,2014,3534,2789,313,-1692,-2258,-1209,
    1587,4336,4769,3382,1139,-685,-1359,-373,1362,2099,1364,421,-792,-2361,-3384,-3669,-3147,-2067,-903,631,1570,1094,-495,-2498,-3548,-2664,-159,3211,5159,5072,
    3484,462,-2046,-2432,-871,874,2653,3618,2795,456,-2194,-4039,-4471,-3253,-952,1366,2361,1671,-272,-2895,-4726,-3972,-1056,2235,4384,4453,2380,-627,-2270,
    -1932,-127,2464,4520,4781,2859,265,-1455,-2372,-2461,-1652,-669,-461,-1030,-1625,-2369,-3043,-2816,-1858,-1054,-690,276,1514,2279,2741,3087,2829,2102,1794,
    1798,1807,1755,1411,376,-931,-2186,-3013,-3497,-3695,-3476,-3182,-2944,-2465,-1538,-574,63,764,1519,2034,2550,3081,3557,3663,3282,2653,1917,1083,253,-529,
    -1238,-1966,-2398,-2660,-2818,-2982,-3302,-3780,-4143,-3754,-2365,-90,2161,3526,3640,2515,1089,523,1417,3234,5087,5738,4506,1634,-1507,-3810,-4535,-3595,
    -1975,-686,-532,-1614,-3300,-4599,-4804,-3568,-1213,1619,3957,4893,4267,2461,364,-737,98,2170,3968,4415,3099,80,-3259,-5019,-4729,-2986,-678,972,952,-450,
    -1936,-2624,-2244,-870,1040,2601,3466,3358,2535,1319,-89,-973,-906,-326,333,967,757,-617,-2392,-3704,-4146,-3231,-1072,1032,2113,2151,1442,455,-11,508,1673,
    2669,3153,2833,1757,335,-764,-1418,-1794,-1876,-1868,-2013,-2300,-2353,-2230,-2036,-1805,-1269,-480,381,1274,2168,2650,2899,3149,3114,2575,2039,1548,964,304,
    -189,-593,-1224,-1980,-2669,-3342,-3754,-3543,-2761,-1815,-976,-497,-426,-466,-132,761,2081,3301,4055,4153,3348,2083,831,103,-143,-105,-111,-452,-1173,-2108,
    -2859,-3247,-3036,-2360,-1419,-418,348,743,806,601,456,603,993,1578,2068,2062,1423,346,-719,-1398,-1588,-1233,-491,193,475,334,-140,-762,-1148,-792,59,977,
    1724,1938,1335,249,-734,-1168,-1154,-682,-80,127,-147,-649,-1101,-1389,-1377,-858,-132,484,912,968,637,163,-99,75,680,1340,1562,1214,483,-301,-844,-906,-626,
    -244,-67,-200,-592,-1038,-1121,-818,-330,114,362,294,5,-203,-116,120,467,805,802,437,-116,-466,-462,-149,321,591,366,-105,-498,-703,-714,-421,35,276,402,472,
    273,-143,-493,-611,-582,-297,261,659,565,8,-697,-1319,-1504,-934,62,894,1307,1138,359,-620,-1066,-643,337,1449,2291,2346,1372,-123,-1408,-2072,-1868,-885,306,
    795,370,-760,-2082,-2915,-2634,-1375,223,1582,2298,2180,1401,518,130,335,975
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static byte buf1[2048];
static byte buf2[2048];
static i16  unpWave1[2048];
static i16  unpWave2[2048];
static i16  unpWave3[2048];
static i16  unpWave4[2048];

static MDCT_DATA mdctw[1024];

static float    fdctw[512];
static i16      fdWave3[512];

static float dev1 = 0;
static float dev2 = 0;
static float det1 = 0;
static float det2 = 0;
static float dev3 = 0;
static float det3 = 0;
static float dev4 = 0;
static float det4 = 0;
static float dev5 = 0;
static float det5 = 0;

static float dev6 = 0;
static float det6 = 0;

const char* packStr[] = { "No", "uLaw 12bit", "uLaw 16bit", "ADPCMIMA", "FDCT0", "FDCT1", "FDCT2", "FDCT3" };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void Test(i16* src, i16* dst, u16 len, const char* str)
{
    float cmpRat = 0, det = 0, dev = 0;
    u32 _len;

    //const char* p = (packType >= ArraySize(packStr)) ? "???" : packStr[packType];

    for (u16 i = PACK_ULAW12; i <= PACK_DCT3; i++)
    {
        _len = PW_TestPackUnpack(src, dst, len, i, &cmpRat, &det, &dev);

        printf("%s : %-10s, len = %-4lu, compRat = %4.1f, det = %.4f, dev = %.1f\n", str, packStr[i], _len, cmpRat, det, dev);
    };
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

const byte log2n = 6;
const u16 N = 1UL << log2n;

MDCT_TRIG   trig[N*2];
MDCT_TRIG   win[N / 2];
MDCT_BITREV bitrev[N / 4];
MDCT_DATA   work[N];
MDCT_DATA   temp1[N];
MDCT_DATA   temp2[N];

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main()
{
    float cmpRat = 0, det = 0, dev = 0;
    //u32 len;

    //PW_Pack_uLaw_16Bit(wave1, buf1, 996);
    //PW_Unpack_uLaw_16Bit(buf1, unpWave1, 996);


    MDCT_LookUp init;

    mdct_init(&init, log2n, bitrev, trig, win);
    init.work = work;
    init.temp1 = temp1;
    init.temp2 = temp2;

    //mdct_window(&init, wave1, mdctw);

    //for (u16 i = 0; i < N; i++) mdctw[i] = wave1[i];

    //mdct_forward(&init, mdctw, mdctw);
    //mdct_backward(&init, mdctw, mdctw);

    WavePack_MDCT(&init, wave4, mdctw, 1024, 0, 0);
    WaveUnpack_MDCT(&init, mdctw, unpWave1, 1024);

    Test(wave1, unpWave1, 996,  "Wave1 4AK");
    Test(wave2, unpWave1, 996,  "Wave2 4AK");
    Test(wave3, unpWave1, 512,  "Wave3 4AK");
    Test(wave4, unpWave1, 1024, "Wave4 2AK");
    Test(wave5, unpWave1, 512,  "Wave5 8AD-X");

    //det1 = PW_Determination(wave1, unpWave1, 996, &dev1);

    //PW_Pack_ADPCMIMA(wave1, buf2, 996);
    //PW_Unpack_ADPCMIMA(buf2, unpWave2, 996);

    //det2 = PW_Determination(wave1, unpWave2, 996, &dev2);

    //PW_Pack_uLaw_16Bit(wave3, buf1, 512);
    //PW_Unpack_uLaw_16Bit(buf1, unpWave3, 512);

    //det3 = PW_Determination(wave3, unpWave3, 512, &dev3);

    //PW_Pack_ADPCMIMA(wave3, buf2, 512);
    //PW_Unpack_ADPCMIMA(buf2, unpWave4, 512);

    //det4 = PW_Determination(wave3, unpWave4, 512, &dev4);

  //  PW_FDCT_Forward(wave3, fdctw, 9, 1);
  ////  for (u32 i = 256; i < 512; i++) fdctw[i] = 0;
  //  PW_FDCT_Inverse(fdctw, fdWave3, 9, 2.0/512);

  //  det5 = PW_Determination(wave3, fdWave3, 512, &dev5);

  //  u16 packedLen = 0;
  //  u16 OVRLAP = 7;

  //  u16 len1 = PW_Pack_FDCT(wave3, buf1, 512, 1, OVRLAP, 64, &packedLen);
  //  u16 len2 = PW_Unpack_FDCT(buf1, unpWave3, packedLen, OVRLAP);

  //  det6 = PW_Determination(wave3, unpWave3, len1, &dev6);



    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

