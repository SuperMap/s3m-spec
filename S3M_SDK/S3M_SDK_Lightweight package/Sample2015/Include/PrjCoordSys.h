#pragma once
/*----------------------------------------------------------------------------*/
/*          P R O J E C T E D   C O O R D I N A T E   S Y S T E M S           */
/*                           (W O R L D   W I D E)                            */
/*----------------------------------------------------------------------------*/
typedef enum tagPJCoordSysType
{
	PCS_USER_DEFINED = -1,
	//{{ 世界地图 54001-54031
	PCS_NON_EARTH = 0,		//普通平面坐标
	PCS_EARTH_LONGITUDE_LATITUDE = 1,		//地理经纬坐标
	PCS_WORLD_PLATE_CARREE = 54001,	   /* Plate Carree           */
	PCS_WORLD_EQUIDISTANT_CYLINDRICAL = 54002,     /* Equidistant Cyl.       */
	PCS_WORLD_MILLER_CYLINDRICAL = 54003,     /* Miller Cylindrical     */
	PCS_WORLD_MERCATOR = 54004,     /* Mercator               */
	PCS_WORLD_SINUSOIDAL = 54008,     /* Sinusoidal             */
	PCS_WORLD_MOLLWEIDE = 54009,     /* Mollweide              */
	PCS_WORLD_ECKERT_VI = 54010,     /* Eckert VI              */
	PCS_WORLD_ECKERT_V = 54011,     /* Eckert V               */
	PCS_WORLD_ECKERT_IV = 54012,     /* Eckert IV              */
	PCS_WORLD_ECKERT_III = 54013,     /* Eckert III             */
	PCS_WORLD_ECKERT_II = 54014,     /* Eckert II              */
	PCS_WORLD_ECKERT_I = 54015,     /* Eckert I               */
	PCS_WORLD_GALL_STEREOGRAPHIC = 54016,     /* Gall Stereographic     */
	PCS_WORLD_BEHRMANN = 54017,     /* Behrmann               */
	PCS_WORLD_WINKEL_I = 54018,     /* Winkel I               */
	PCS_WORLD_WINKEL_II = 54019,     /* Winkel II              */
	PCS_WORLD_POLYCONIC = 54021,     /* Polyconic              */
	PCS_WORLD_QUARTIC_AUTHALIC = 54022,     /* Quartic Authalic       */
	PCS_WORLD_LOXIMUTHAL = 54023,     /* Loximuthal             */
	PCS_WORLD_BONNE = 54024,     /* Bonne                  */
	PCS_WORLD_HOTINE = 54025,     /* Hotine                 */
	PCS_WORLD_STEREOGRAPHIC = 54026,     /* Stereographic          */
	PCS_WORLD_EQUIDISTANT_CONIC = 54027,     /* Equidistant Conic      */
	PCS_WORLD_CASSINI = 54028,     /* Cassini                */
	PCS_WORLD_VAN_DER_GRINTEN_I = 54029,     /* Van der Grinten I      */
	PCS_WORLD_ROBINSON = 54030,     /* Robinson               */
	PCS_WORLD_TWO_POINT_EQUIDISTANT = 54031,     /* Two-Point Equidistant  */
//}}

/*----------------------------------------------------------------------------*/
/*          P R O J E C T E D   C O O R D I N A T E   S Y S T E M S           */
/*                          (S P H E R E   O N L Y)                           */
/*----------------------------------------------------------------------------*/
//{{ 球体投影系53001-53031
PCS_SPHERE_PLATE_CARREE = 53001,     /* Plate Carree          */
PCS_SPHERE_EQUIDISTANT_CYLINDRICAL = 53002,     /* Equidistant Cyl.      */
PCS_SPHERE_MILLER_CYLINDRICAL = 53003,     /* Miller Cylindrical    */
PCS_SPHERE_MERCATOR = 53004,     /* Mercator              */
PCS_SPHERE_SINUSOIDAL = 53008,     /* Sinusoidal            */
PCS_SPHERE_MOLLWEIDE = 53009,     /* Mollweide             */
PCS_SPHERE_ECKERT_VI = 53010,     /* Eckert VI             */
PCS_SPHERE_ECKERT_V = 53011,     /* Eckert V              */
PCS_SPHERE_ECKERT_IV = 53012,     /* Eckert IV             */
PCS_SPHERE_ECKERT_III = 53013,     /* Eckert III            */
PCS_SPHERE_ECKERT_II = 53014,     /* Eckert II             */
PCS_SPHERE_ECKERT_I = 53015,     /* Eckert I              */
PCS_SPHERE_GALL_STEREOGRAPHIC = 53016,     /* Gall Stereographic    */
PCS_SPHERE_BEHRMANN = 53017,     /* Behrmann              */
PCS_SPHERE_WINKEL_I = 53018,     /* Winkel I              */
PCS_SPHERE_WINKEL_II = 53019,     /* Winkel II             */
PCS_SPHERE_POLYCONIC = 53021,     /* Polyconic             */
PCS_SPHERE_QUARTIC_AUTHALIC = 53022,     /* Quartic Authalic      */
PCS_SPHERE_LOXIMUTHAL = 53023,     /* Loximuthal            */
PCS_SPHERE_BONNE = 53024,     /* Bonne                 */
PCS_SPHERE_HOTINE = 53025,     /* Hotine                */
PCS_SPHERE_STEREOGRAPHIC = 53026,     /* Stereographic         */
PCS_SPHERE_EQUIDISTANT_CONIC = 53027,     /* Equidistant Conic     */
PCS_SPHERE_CASSINI = 53028,     /* Cassini               */
PCS_SPHERE_VAN_DER_GRINTEN_I = 53029,     /* Van der Grinten I     */
PCS_SPHERE_ROBINSON = 53030,     /* Robinson              */
PCS_SPHERE_TWO_POINT_EQUIDISTANT = 53031,     /* Two-Point Equidistant */
//}}

/*----------------------------------------------------------------------------*/
/*          P R O J E C T E D   C O O R D I N A T E   S Y S T E M S           */
/*----------------------------------------------------------------------------*/
//{{ PCS_WGS1984 32601 - 32760
PCS_WGS_1984_UTM_1N = 32601,     /* WGS 1984 UTM Zone 1N               */
PCS_WGS_1984_UTM_2N = 32602,     /* WGS 1984 UTM Zone 2N               */
PCS_WGS_1984_UTM_3N = 32603,     /* WGS 1984 UTM Zone 3N               */
PCS_WGS_1984_UTM_4N = 32604,     /* WGS 1984 UTM Zone 4N               */
PCS_WGS_1984_UTM_5N = 32605,     /* WGS 1984 UTM Zone 5N               */
PCS_WGS_1984_UTM_6N = 32606,     /* WGS 1984 UTM Zone 6N               */
PCS_WGS_1984_UTM_7N = 32607,     /* WGS 1984 UTM Zone 7N               */
PCS_WGS_1984_UTM_8N = 32608,     /* WGS 1984 UTM Zone 8N               */
PCS_WGS_1984_UTM_9N = 32609,     /* WGS 1984 UTM Zone 9N               */
PCS_WGS_1984_UTM_10N = 32610,     /* WGS 1984 UTM Zone 10N              */
PCS_WGS_1984_UTM_11N = 32611,     /* WGS 1984 UTM Zone 11N              */
PCS_WGS_1984_UTM_12N = 32612,     /* WGS 1984 UTM Zone 12N              */
PCS_WGS_1984_UTM_13N = 32613,     /* WGS 1984 UTM Zone 13N              */
PCS_WGS_1984_UTM_14N = 32614,     /* WGS 1984 UTM Zone 14N              */
PCS_WGS_1984_UTM_15N = 32615,     /* WGS 1984 UTM Zone 15N              */
PCS_WGS_1984_UTM_16N = 32616,     /* WGS 1984 UTM Zone 16N              */
PCS_WGS_1984_UTM_17N = 32617,     /* WGS 1984 UTM Zone 17N              */
PCS_WGS_1984_UTM_18N = 32618,     /* WGS 1984 UTM Zone 18N              */
PCS_WGS_1984_UTM_19N = 32619,     /* WGS 1984 UTM Zone 19N              */
PCS_WGS_1984_UTM_20N = 32620,     /* WGS 1984 UTM Zone 20N              */
PCS_WGS_1984_UTM_21N = 32621,     /* WGS 1984 UTM Zone 21N              */
PCS_WGS_1984_UTM_22N = 32622,     /* WGS 1984 UTM Zone 22N              */
PCS_WGS_1984_UTM_23N = 32623,     /* WGS 1984 UTM Zone=  23N              */
PCS_WGS_1984_UTM_24N = 32624,     /* WGS 1984 UTM Zone 24N              */
PCS_WGS_1984_UTM_25N = 32625,     /* WGS 1984 UTM Zone 25N              */
PCS_WGS_1984_UTM_26N = 32626,     /* WGS 1984 UTM Zone 26N              */
PCS_WGS_1984_UTM_27N = 32627,     /* WGS 1984 UTM Zone 27N              */
PCS_WGS_1984_UTM_28N = 32628,     /* WGS 1984 UTM Zone=  28N              */
PCS_WGS_1984_UTM_29N = 32629,     /* WGS 1984 UTM Zone=  29N              */
PCS_WGS_1984_UTM_30N = 32630,     /* WGS 1984 UTM Zone 30N              */
PCS_WGS_1984_UTM_31N = 32631,     /* WGS 1984 UTM Zone 31N              */
PCS_WGS_1984_UTM_32N = 32632,     /* WGS 1984 UTM Zone= 32N              */
PCS_WGS_1984_UTM_33N = 32633,     /* WGS 1984 UTM Zone 33N              */
PCS_WGS_1984_UTM_34N = 32634,     /* WGS 1984 UTM Zone 34N              */
PCS_WGS_1984_UTM_35N = 32635,     /* WGS 1984 UTM Zone 35N              */
PCS_WGS_1984_UTM_36N = 32636,     /* WGS 1984 UTM Zone 36N              */
PCS_WGS_1984_UTM_37N = 32637,     /* WGS 1984 UTM Zone 37N              */
PCS_WGS_1984_UTM_38N = 32638,     /* WGS 1984 UTM Zone 38N              */
PCS_WGS_1984_UTM_39N = 32639,     /* WGS 1984 UTM Zone 39N              */
PCS_WGS_1984_UTM_40N = 32640,     /* WGS 1984 UTM Zone 40N              */
PCS_WGS_1984_UTM_41N = 32641,     /* WGS 1984 UTM Zone 41N              */
PCS_WGS_1984_UTM_42N = 32642,     /* WGS 1984 UTM Zone 42N              */
PCS_WGS_1984_UTM_43N = 32643,     /* WGS 1984 UTM Zone 43N              */
PCS_WGS_1984_UTM_44N = 32644,     /* WGS 1984 UTM Zone 44N              */
PCS_WGS_1984_UTM_45N = 32645,     /* WGS 1984 UTM Zone 45N              */
PCS_WGS_1984_UTM_46N = 32646,     /* WGS 1984 UTM Zone 46N              */
PCS_WGS_1984_UTM_47N = 32647,     /* WGS 1984 UTM Zone 47N              */
PCS_WGS_1984_UTM_48N = 32648,     /* WGS 1984 UTM Zone 48N              */
PCS_WGS_1984_UTM_49N = 32649,     /* WGS 1984 UTM Zone 49N              */
PCS_WGS_1984_UTM_50N = 32650,     /* WGS 1984 UTM Zone 50N              */
PCS_WGS_1984_UTM_51N = 32651,     /* WGS 1984 UTM Zone 51N              */
PCS_WGS_1984_UTM_52N = 32652,     /* WGS 1984 UTM Zone 52N              */
PCS_WGS_1984_UTM_53N = 32653,     /* WGS 1984 UTM Zone= 53N              */
PCS_WGS_1984_UTM_54N = 32654,     /* WGS 1984 UTM Zone= 54N              */
PCS_WGS_1984_UTM_55N = 32655,     /* WGS 1984 UTM Zone 55N              */
PCS_WGS_1984_UTM_56N = 32656,     /* WGS 1984 UTM Zone 56N              */
PCS_WGS_1984_UTM_57N = 32657,     /* WGS 1984 UTM Zone 57N              */
PCS_WGS_1984_UTM_58N = 32658,     /* WGS 1984 UTM Zone 58N              */
PCS_WGS_1984_UTM_59N = 32659,     /* WGS 1984 UTM Zone 59N              */
PCS_WGS_1984_UTM_60N = 32660,     /* WGS 1984 UTM Zone 60N              */
PCS_WGS_1984_UTM_1S = 32701,     /* WGS 1984 UTM Zone 1S               */
PCS_WGS_1984_UTM_2S = 32702,     /* WGS 1984 UTM Zone 2S               */
PCS_WGS_1984_UTM_3S = 32703,     /* WGS 1984 UTM Zone 3S               */
PCS_WGS_1984_UTM_4S = 32704,     /* WGS 1984 UTM Zone 4S               */
PCS_WGS_1984_UTM_5S = 32705,     /* WGS 1984 UTM Zone 5S               */
PCS_WGS_1984_UTM_6S = 32706,     /* WGS 1984 UTM Zone 6S               */
PCS_WGS_1984_UTM_7S = 32707,     /* WGS 1984 UTM Zone 7S               */
PCS_WGS_1984_UTM_8S = 32708,     /* WGS 1984 UTM Zone 8S               */
PCS_WGS_1984_UTM_9S = 32709,     /* WGS 1984 UTM Zone 9S               */
PCS_WGS_1984_UTM_10S = 32710,     /* WGS 1984 UTM Zone 10S              */
PCS_WGS_1984_UTM_11S = 32711,     /* WGS 1984 UTM Zone 11S              */
PCS_WGS_1984_UTM_12S = 32712,     /* WGS 1984 UTM Zone 12S              */
PCS_WGS_1984_UTM_13S = 32713,     /* WGS 1984 UTM Zone 13S              */
PCS_WGS_1984_UTM_14S = 32714,     /* WGS 1984 UTM Zone 14S              */
PCS_WGS_1984_UTM_15S = 32715,     /* WGS 1984 UTM Zone 15S              */
PCS_WGS_1984_UTM_16S = 32716,     /* WGS 1984 UTM Zone 16S              */
PCS_WGS_1984_UTM_17S = 32717,     /* WGS 1984 UTM Zone 17S              */
PCS_WGS_1984_UTM_18S = 32718,     /* WGS 1984 UTM Zone 18S              */
PCS_WGS_1984_UTM_19S = 32719,     /* WGS 1984 UTM Zone 19S              */
PCS_WGS_1984_UTM_20S = 32720,     /* WGS 1984 UTM Zone 20S              */
PCS_WGS_1984_UTM_21S = 32721,     /* WGS 1984 UTM Zone 21S              */
PCS_WGS_1984_UTM_22S = 32722,     /* WGS 1984 UTM Zone 22S              */
PCS_WGS_1984_UTM_23S = 32723,     /* WGS 1984 UTM Zone=  23S              */
PCS_WGS_1984_UTM_24S = 32724,     /* WGS 1984 UTM Zone 24S              */
PCS_WGS_1984_UTM_25S = 32725,     /* WGS 1984 UTM Zone 25S              */
PCS_WGS_1984_UTM_26S = 32726,     /* WGS 1984 UTM Zone 26S              */
PCS_WGS_1984_UTM_27S = 32727,     /* WGS 1984 UTM Zone 27S              */
PCS_WGS_1984_UTM_28S = 32728,     /* WGS 1984 UTM Zone=  28S              */
PCS_WGS_1984_UTM_29S = 32729,     /* WGS 1984 UTM Zone=  29S              */
PCS_WGS_1984_UTM_30S = 32730,     /* WGS 1984 UTM Zone 30S              */
PCS_WGS_1984_UTM_31S = 32731,     /* WGS 1984 UTM Zone 31S              */
PCS_WGS_1984_UTM_32S = 32732,     /* WGS 1984 UTM Zone= 32S              */
PCS_WGS_1984_UTM_33S = 32733,     /* WGS 1984 UTM Zone 33S              */
PCS_WGS_1984_UTM_34S = 32734,     /* WGS 1984 UTM Zone 34S              */
PCS_WGS_1984_UTM_35S = 32735,     /* WGS 1984 UTM Zone 35S              */
PCS_WGS_1984_UTM_36S = 32736,     /* WGS 1984 UTM Zone 36S              */
PCS_WGS_1984_UTM_37S = 32737,     /* WGS 1984 UTM Zone 37S              */
PCS_WGS_1984_UTM_38S = 32738,     /* WGS 1984 UTM Zone 38S              */
PCS_WGS_1984_UTM_39S = 32739,     /* WGS 1984 UTM Zone 39S              */
PCS_WGS_1984_UTM_40S = 32740,     /* WGS 1984 UTM Zone 40S              */
PCS_WGS_1984_UTM_41S = 32741,     /* WGS 1984 UTM Zone 41S              */
PCS_WGS_1984_UTM_42S = 32742,     /* WGS 1984 UTM Zone 42S              */
PCS_WGS_1984_UTM_43S = 32743,     /* WGS 1984 UTM Zone 43S              */
PCS_WGS_1984_UTM_44S = 32744,     /* WGS 1984 UTM Zone 44S              */
PCS_WGS_1984_UTM_45S = 32745,     /* WGS 1984 UTM Zone 45S              */
PCS_WGS_1984_UTM_46S = 32746,     /* WGS 1984 UTM Zone 46S              */
PCS_WGS_1984_UTM_47S = 32747,     /* WGS 1984 UTM Zone 47S              */
PCS_WGS_1984_UTM_48S = 32748,     /* WGS 1984 UTM Zone 48S              */
PCS_WGS_1984_UTM_49S = 32749,     /* WGS 1984 UTM Zone 49S              */
PCS_WGS_1984_UTM_50S = 32750,     /* WGS 1984 UTM Zone 50S              */
PCS_WGS_1984_UTM_51S = 32751,     /* WGS 1984 UTM Zone 51S              */
PCS_WGS_1984_UTM_52S = 32752,     /* WGS 1984 UTM Zone 52S              */
PCS_WGS_1984_UTM_53S = 32753,     /* WGS 1984 UTM Zone 53S              */
PCS_WGS_1984_UTM_54S = 32754,     /* WGS 1984 UTM Zone= 54S              */
PCS_WGS_1984_UTM_55S = 32755,     /* WGS 1984 UTM Zone 55S              */
PCS_WGS_1984_UTM_56S = 32756,     /* WGS 1984 UTM Zone 56S              */
PCS_WGS_1984_UTM_57S = 32757,     /* WGS 1984 UTM Zone 57S              */
PCS_WGS_1984_UTM_58S = 32758,     /* WGS 1984 UTM Zone 58S              */
PCS_WGS_1984_UTM_59S = 32759,     /* WGS 1984 UTM Zone 59S              */
PCS_WGS_1984_UTM_60S = 32760,     /* WGS 1984 UTM Zone 60S              */
//}}

//{{ 32761 - 32811
	//日本东京测地系,平面直角投影系(实际上使用TM投影法) 
	PCS_TOKYO_PLATE_ZONE_I = 32761,     //Japanese Zone I	    
	PCS_TOKYO_PLATE_ZONE_II = 32762,     //Japanese Zone II	
	PCS_TOKYO_PLATE_ZONE_III = 32763,     //Japanese Zone III	
	PCS_TOKYO_PLATE_ZONE_IV = 32764,     //Japanese Zone IV	
	PCS_TOKYO_PLATE_ZONE_V = 32765,     //Japanese Zone V	    
	PCS_TOKYO_PLATE_ZONE_VI = 32766,     //Japanese Zone VI	
	PCS_TOKYO_PLATE_ZONE_VII = 32767,     //Japanese Zone VII	
	PCS_TOKYO_PLATE_ZONE_VIII = 32768,     //Japanese Zone VIII	
	PCS_TOKYO_PLATE_ZONE_IX = 32769,     //Japanese Zone IX	
	PCS_TOKYO_PLATE_ZONE_X = 32770,     //Japanese Zone X		
	PCS_TOKYO_PLATE_ZONE_XI = 32771,     //Japanese Zone XI	
	PCS_TOKYO_PLATE_ZONE_XII = 32772,     //Japanese Zone XII	
	PCS_TOKYO_PLATE_ZONE_XIII = 32773,     //Japanese Zone XIII	
	PCS_TOKYO_PLATE_ZONE_XIV = 32774,     //Japanese Zone XIV	
	PCS_TOKYO_PLATE_ZONE_XV = 32775,     //Japanese Zone XV	
	PCS_TOKYO_PLATE_ZONE_XVI = 32776,     //Japanese Zone XVI	
	PCS_TOKYO_PLATE_ZONE_XVII = 32777,     //Japanese Zone XVII	
	PCS_TOKYO_PLATE_ZONE_XVIII = 32778,     //Japanese Zone XVIII	
	PCS_TOKYO_PLATE_ZONE_XIX = 32779,     //Japanese Zone XIX	

	//日本东京测地系,UTM投影系 
	PCS_TOKYO_UTM_51 = 32780,		//日本东京测地系,UTM投影系51 
	PCS_TOKYO_UTM_52 = 32781,
	PCS_TOKYO_UTM_53 = 32782,
	PCS_TOKYO_UTM_54 = 32783,
	PCS_TOKYO_UTM_55 = 32784,
	PCS_TOKYO_UTM_56 = 32785,

	//日本测地系2000,平面直角投影系(GRS80椭球体,ITRF94参照系)
	PCS_JAPAN_PLATE_ZONE_I = 32786,     //Japanese Zone I	    
	PCS_JAPAN_PLATE_ZONE_II = 32787,     //Japanese Zone II	
	PCS_JAPAN_PLATE_ZONE_III = 32788,     //Japanese Zone III	
	PCS_JAPAN_PLATE_ZONE_IV = 32789,     //Japanese Zone IV	
	PCS_JAPAN_PLATE_ZONE_V = 32790,     //Japanese Zone V	    
	PCS_JAPAN_PLATE_ZONE_VI = 32791,     //Japanese Zone VI	
	PCS_JAPAN_PLATE_ZONE_VII = 32792,     //Japanese Zone VII	
	PCS_JAPAN_PLATE_ZONE_VIII = 32793,     //Japanese Zone VIII	
	PCS_JAPAN_PLATE_ZONE_IX = 32794,     //Japanese Zone IX	
	PCS_JAPAN_PLATE_ZONE_X = 32795,     //Japanese Zone X		
	PCS_JAPAN_PLATE_ZONE_XI = 32796,     //Japanese Zone XI	
	PCS_JAPAN_PLATE_ZONE_XII = 32797,     //Japanese Zone XII	
	PCS_JAPAN_PLATE_ZONE_XIII = 32798,     //Japanese Zone XIII	
	PCS_JAPAN_PLATE_ZONE_XIV = 32800,     //Japanese Zone XIV	
	PCS_JAPAN_PLATE_ZONE_XV = 32801,     //Japanese Zone XV	
	PCS_JAPAN_PLATE_ZONE_XVI = 32802,     //Japanese Zone XVI	
	PCS_JAPAN_PLATE_ZONE_XVII = 32803,     //Japanese Zone XVII	
	PCS_JAPAN_PLATE_ZONE_XVIII = 32804,     //Japanese Zone XVIII	
	PCS_JAPAN_PLATE_ZONE_XIX = 32805,     //Japanese Zone XIX	

	//日本测地系2000,UTM投影系(GRS80椭球体,ITRF94参照系)
	PCS_JAPAN_UTM_51 = 32806,
	PCS_JAPAN_UTM_52 = 32807,
	PCS_JAPAN_UTM_53 = 32808,
	PCS_JAPAN_UTM_54 = 32809,
	PCS_JAPAN_UTM_55 = 32810,
	PCS_JAPAN_UTM_56 = 32811,

	//}}

	//{{PCS_WGS_1972 32201 - 32360
	PCS_WGS_1972_UTM_1N = 32201,     /* WGS 1972 UTM Zone 1N               */
	PCS_WGS_1972_UTM_2N = 32202,     /* WGS 1972 UTM Zone 2N               */
	PCS_WGS_1972_UTM_3N = 32203,     /* WGS 1972 UTM Zone 3N               */
	PCS_WGS_1972_UTM_4N = 32204,     /* WGS 1972 UTM Zone 4N               */
	PCS_WGS_1972_UTM_5N = 32205,     /* WGS 1972 UTM Zone 5N               */
	PCS_WGS_1972_UTM_6N = 32206,     /* WGS 1972 UTM Zone 6N               */
	PCS_WGS_1972_UTM_7N = 32207,     /* WGS 1972 UTM Zone 7N               */
	PCS_WGS_1972_UTM_8N = 32208,     /* WGS 1972 UTM Zone 8N               */
	PCS_WGS_1972_UTM_9N = 32209,     /* WGS 1972 UTM Zone 9N               */
	PCS_WGS_1972_UTM_10N = 32210,     /* WGS 1972 UTM Zone 10N              */
	PCS_WGS_1972_UTM_11N = 32211,     /* WGS 1972 UTM Zone 11N              */
	PCS_WGS_1972_UTM_12N = 32212,     /* WGS 1972 UTM Zone 12N              */
	PCS_WGS_1972_UTM_13N = 32213,     /* WGS 1972 UTM Zone 13N              */
	PCS_WGS_1972_UTM_14N = 32214,     /* WGS 1972 UTM Zone 14N              */
	PCS_WGS_1972_UTM_15N = 32215,     /* WGS 1972 UTM Zone 15N              */
	PCS_WGS_1972_UTM_16N = 32216,     /* WGS 1972 UTM Zone 16N              */
	PCS_WGS_1972_UTM_17N = 32217,     /* WGS 1972 UTM Zone 17N              */
	PCS_WGS_1972_UTM_18N = 32218,     /* WGS 1972 UTM Zone 18N              */
	PCS_WGS_1972_UTM_19N = 32219,     /* WGS 1972 UTM Zone 19N              */
	PCS_WGS_1972_UTM_20N = 32220,     /* WGS 1972 UTM Zone 20N              */
	PCS_WGS_1972_UTM_21N = 32221,     /* WGS 1972 UTM Zone 21N              */
	PCS_WGS_1972_UTM_22N = 32222,     /* WGS 1972 UTM Zone 22N              */
	PCS_WGS_1972_UTM_23N = 32223,     /* WGS 1972 UTM Zone=  23N              */
	PCS_WGS_1972_UTM_24N = 32224,     /* WGS 1972 UTM Zone 24N              */
	PCS_WGS_1972_UTM_25N = 32225,     /* WGS 1972 UTM Zone 25N              */
	PCS_WGS_1972_UTM_26N = 32226,     /* WGS 1972 UTM Zone 26N              */
	PCS_WGS_1972_UTM_27N = 32227,     /* WGS 1972 UTM Zone 27N              */
	PCS_WGS_1972_UTM_28N = 32228,     /* WGS 1972 UTM Zone=  28N              */
	PCS_WGS_1972_UTM_29N = 32229,     /* WGS 1972 UTM Zone=  29N              */
	PCS_WGS_1972_UTM_30N = 32230,     /* WGS 1972 UTM Zone 30N              */
	PCS_WGS_1972_UTM_31N = 32231,     /* WGS 1972 UTM Zone 31N              */
	PCS_WGS_1972_UTM_32N = 32232,     /* WGS 1972 UTM Zone= 32N              */
	PCS_WGS_1972_UTM_33N = 32233,     /* WGS 1972 UTM Zone 33N              */
	PCS_WGS_1972_UTM_34N = 32234,     /* WGS 1972 UTM Zone 34N              */
	PCS_WGS_1972_UTM_35N = 32235,     /* WGS 1972 UTM Zone 35N              */
	PCS_WGS_1972_UTM_36N = 32236,     /* WGS 1972 UTM Zone 36N              */
	PCS_WGS_1972_UTM_37N = 32237,     /* WGS 1972 UTM Zone 37N              */
	PCS_WGS_1972_UTM_38N = 32238,     /* WGS 1972 UTM Zone 38N              */
	PCS_WGS_1972_UTM_39N = 32239,     /* WGS 1972 UTM Zone 39N              */
	PCS_WGS_1972_UTM_40N = 32240,     /* WGS 1972 UTM Zone 40N              */
	PCS_WGS_1972_UTM_41N = 32241,     /* WGS 1972 UTM Zone 41N              */
	PCS_WGS_1972_UTM_42N = 32242,     /* WGS 1972 UTM Zone 42N              */
	PCS_WGS_1972_UTM_43N = 32243,     /* WGS 1972 UTM Zone 43N              */
	PCS_WGS_1972_UTM_44N = 32244,     /* WGS 1972 UTM Zone 44N              */
	PCS_WGS_1972_UTM_45N = 32245,     /* WGS 1972 UTM Zone 45N              */
	PCS_WGS_1972_UTM_46N = 32246,     /* WGS 1972 UTM Zone 46N              */
	PCS_WGS_1972_UTM_47N = 32247,     /* WGS 1972 UTM Zone 47N              */
	PCS_WGS_1972_UTM_48N = 32248,     /* WGS 1972 UTM Zone 48N              */
	PCS_WGS_1972_UTM_49N = 32249,     /* WGS 1972 UTM Zone 49N              */
	PCS_WGS_1972_UTM_50N = 32250,     /* WGS 1972 UTM Zone 50N              */
	PCS_WGS_1972_UTM_51N = 32251,     /* WGS 1972 UTM Zone 51N              */
	PCS_WGS_1972_UTM_52N = 32252,     /* WGS 1972 UTM Zone 52N              */
	PCS_WGS_1972_UTM_53N = 32253,     /* WGS 1972 UTM Zone 53N              */
	PCS_WGS_1972_UTM_54N = 32254,     /* WGS 1972 UTM Zone= 54N              */
	PCS_WGS_1972_UTM_55N = 32255,     /* WGS 1972 UTM Zone 55N              */
	PCS_WGS_1972_UTM_56N = 32256,     /* WGS 1972 UTM Zone 56N              */
	PCS_WGS_1972_UTM_57N = 32257,     /* WGS 1972 UTM Zone 57N              */
	PCS_WGS_1972_UTM_58N = 32258,     /* WGS 1972 UTM Zone 58N              */
	PCS_WGS_1972_UTM_59N = 32259,     /* WGS 1972 UTM Zone 59N              */
	PCS_WGS_1972_UTM_60N = 32260,     /* WGS 1972 UTM Zone 60N              */
	PCS_WGS_1972_UTM_1S = 32301,     /* WGS 1972 UTM Zone 1S               */
	PCS_WGS_1972_UTM_2S = 32302,     /* WGS 1972 UTM Zone 2S               */
	PCS_WGS_1972_UTM_3S = 32303,     /* WGS 1972 UTM Zone 3S               */
	PCS_WGS_1972_UTM_4S = 32304,     /* WGS 1972 UTM Zone 4S               */
	PCS_WGS_1972_UTM_5S = 32305,     /* WGS 1972 UTM Zone 5S               */
	PCS_WGS_1972_UTM_6S = 32306,     /* WGS 1972 UTM Zone 6S               */
	PCS_WGS_1972_UTM_7S = 32307,     /* WGS 1972 UTM Zone 7S               */
	PCS_WGS_1972_UTM_8S = 32308,     /* WGS 1972 UTM Zone 8S               */
	PCS_WGS_1972_UTM_9S = 32309,     /* WGS 1972 UTM Zone 9S               */
	PCS_WGS_1972_UTM_10S = 32310,     /* WGS 1972 UTM Zone 10S              */
	PCS_WGS_1972_UTM_11S = 32311,     /* WGS 1972 UTM Zone 11S              */
	PCS_WGS_1972_UTM_12S = 32312,     /* WGS 1972 UTM Zone 12S              */
	PCS_WGS_1972_UTM_13S = 32313,     /* WGS 1972 UTM Zone 13S              */
	PCS_WGS_1972_UTM_14S = 32314,     /* WGS 1972 UTM Zone 14S              */
	PCS_WGS_1972_UTM_15S = 32315,     /* WGS 1972 UTM Zone 15S              */
	PCS_WGS_1972_UTM_16S = 32316,     /* WGS 1972 UTM Zone 16S              */
	PCS_WGS_1972_UTM_17S = 32317,     /* WGS 1972 UTM Zone 17S              */
	PCS_WGS_1972_UTM_18S = 32318,     /* WGS 1972 UTM Zone 18S              */
	PCS_WGS_1972_UTM_19S = 32319,     /* WGS 1972 UTM Zone 19S              */
	PCS_WGS_1972_UTM_20S = 32320,     /* WGS 1972 UTM Zone 20S              */
	PCS_WGS_1972_UTM_21S = 32321,     /* WGS 1972 UTM Zone 21S              */
	PCS_WGS_1972_UTM_22S = 32322,     /* WGS 1972 UTM Zone 22S              */
	PCS_WGS_1972_UTM_23S = 32323,     /* WGS 1972 UTM Zone 23S              */
	PCS_WGS_1972_UTM_24S = 32324,     /* WGS 1972 UTM Zone 24S              */
	PCS_WGS_1972_UTM_25S = 32325,     /* WGS 1972 UTM Zone 25S              */
	PCS_WGS_1972_UTM_26S = 32326,     /* WGS 1972 UTM Zone 26S              */
	PCS_WGS_1972_UTM_27S = 32327,     /* WGS 1972 UTM Zone 27S              */
	PCS_WGS_1972_UTM_28S = 32328,     /* WGS 1972 UTM Zone=  28S              */
	PCS_WGS_1972_UTM_29S = 32329,     /* WGS 1972 UTM Zone=  29S              */
	PCS_WGS_1972_UTM_30S = 32330,     /* WGS 1972 UTM Zone 30S              */
	PCS_WGS_1972_UTM_31S = 32331,     /* WGS 1972 UTM Zone 31S              */
	PCS_WGS_1972_UTM_32S = 32332,     /* WGS 1972 UTM Zone= 32S              */
	PCS_WGS_1972_UTM_33S = 32333,     /* WGS 1972 UTM Zone 33S              */
	PCS_WGS_1972_UTM_34S = 32334,     /* WGS 1972 UTM Zone 34S              */
	PCS_WGS_1972_UTM_35S = 32335,     /* WGS 1972 UTM Zone 35S              */
	PCS_WGS_1972_UTM_36S = 32336,     /* WGS 1972 UTM Zone 36S              */
	PCS_WGS_1972_UTM_37S = 32337,     /* WGS 1972 UTM Zone 37S              */
	PCS_WGS_1972_UTM_38S = 32338,     /* WGS 1972 UTM Zone 38S              */
	PCS_WGS_1972_UTM_39S = 32339,     /* WGS 1972 UTM Zone 39S              */
	PCS_WGS_1972_UTM_40S = 32340,     /* WGS 1972 UTM Zone 40S              */
	PCS_WGS_1972_UTM_41S = 32341,     /* WGS 1972 UTM Zone 41S              */
	PCS_WGS_1972_UTM_42S = 32342,     /* WGS 1972 UTM Zone 42S              */
	PCS_WGS_1972_UTM_43S = 32343,     /* WGS 1972 UTM Zone 43S              */
	PCS_WGS_1972_UTM_44S = 32344,     /* WGS 1972 UTM Zone 44S              */
	PCS_WGS_1972_UTM_45S = 32345,     /* WGS 1972 UTM Zone 45S              */
	PCS_WGS_1972_UTM_46S = 32346,     /* WGS 1972 UTM Zone 46S              */
	PCS_WGS_1972_UTM_47S = 32347,     /* WGS 1972 UTM Zone 47S              */
	PCS_WGS_1972_UTM_48S = 32348,     /* WGS 1972 UTM Zone 48S              */
	PCS_WGS_1972_UTM_49S = 32349,     /* WGS 1972 UTM Zone 49S              */
	PCS_WGS_1972_UTM_50S = 32350,     /* WGS 1972 UTM Zone 50S              */
	PCS_WGS_1972_UTM_51S = 32351,     /* WGS 1972 UTM Zone 51S              */
	PCS_WGS_1972_UTM_52S = 32352,     /* WGS 1972 UTM Zone 52S              */
	PCS_WGS_1972_UTM_53S = 32353,     /* WGS 1972 UTM Zone 53S              */
	PCS_WGS_1972_UTM_54S = 32354,     /* WGS 1972 UTM Zone 54S              */
	PCS_WGS_1972_UTM_55S = 32355,     /* WGS 1972 UTM Zone 55S              */
	PCS_WGS_1972_UTM_56S = 32356,     /* WGS 1972 UTM Zone 56S              */
	PCS_WGS_1972_UTM_57S = 32357,     /* WGS 1972 UTM Zone 57S              */
	PCS_WGS_1972_UTM_58S = 32358,     /* WGS 1972 UTM Zone 58S              */
	PCS_WGS_1972_UTM_59S = 32359,     /* WGS 1972 UTM Zone 59S              */
	PCS_WGS_1972_UTM_60S = 32360,     /* WGS 1972 UTM Zone 60S              */
//}}

//{{
PCS_NAD_1927_BLM_14N = 32074,     /* NAD 1927 BLM Zone 14N           */
PCS_NAD_1927_BLM_15N = 32075,     /* NAD 1927 BLM Zone 15N           */
PCS_NAD_1927_BLM_16N = 32076,     /* NAD 1927 BLM Zone 16N           */
PCS_NAD_1927_BLM_17N = 32077,     /* NAD 1927 BLM Zone 17N           */
//}}

//{{
PCS_NAD_1927_UTM_3N = 26703,     /* NAD 1927 UTM Zone 3N               */
PCS_NAD_1927_UTM_4N = 26704,     /* NAD 1927 UTM Zone 4N               */
PCS_NAD_1927_UTM_5N = 26705,     /* NAD 1927 UTM Zone 5N               */
PCS_NAD_1927_UTM_6N = 26706,     /* NAD 1927 UTM Zone 6N               */
PCS_NAD_1927_UTM_7N = 26707,     /* NAD 1927 UTM Zone 7N               */
PCS_NAD_1927_UTM_8N = 26708,     /* NAD 1927 UTM Zone 8N               */
PCS_NAD_1927_UTM_9N = 26709,     /* NAD 1927 UTM Zone 9N               */
PCS_NAD_1927_UTM_10N = 26710,     /* NAD 1927 UTM Zone 10N              */
PCS_NAD_1927_UTM_11N = 26711,     /* NAD 1927 UTM Zone 11N              */
PCS_NAD_1927_UTM_12N = 26712,     /* NAD 1927 UTM Zone 12N              */
PCS_NAD_1927_UTM_13N = 26713,     /* NAD 1927 UTM Zone 13N              */
PCS_NAD_1927_UTM_14N = 26714,     /* NAD 1927 UTM Zone 14N              */
PCS_NAD_1927_UTM_15N = 26715,     /* NAD 1927 UTM Zone 15N              */
PCS_NAD_1927_UTM_16N = 26716,     /* NAD 1927 UTM Zone 16N              */
PCS_NAD_1927_UTM_17N = 26717,     /* NAD 1927 UTM Zone 17N              */
PCS_NAD_1927_UTM_18N = 26718,     /* NAD 1927 UTM Zone 18N              */
PCS_NAD_1927_UTM_19N = 26719,     /* NAD 1927 UTM Zone 19N              */
PCS_NAD_1927_UTM_20N = 26720,     /* NAD 1927 UTM Zone 20N              */
PCS_NAD_1927_UTM_21N = 26721,     /* NAD 1927 UTM Zone 21N              */
PCS_NAD_1927_UTM_22N = 26722,     /* NAD 1927 UTM Zone 22N              */
//}}

//{{
PCS_NAD_1983_UTM_3N = 26903,     /* NAD 1983 UTM Zone 3N               */
PCS_NAD_1983_UTM_4N = 26904,     /* NAD 1983 UTM Zone 4N               */
PCS_NAD_1983_UTM_5N = 26905,     /* NAD 1983 UTM Zone 5N               */
PCS_NAD_1983_UTM_6N = 26906,     /* NAD 1983 UTM Zone 6N               */
PCS_NAD_1983_UTM_7N = 26907,     /* NAD 1983 UTM Zone 7N               */
PCS_NAD_1983_UTM_8N = 26908,     /* NAD 1983 UTM Zone 8N               */
PCS_NAD_1983_UTM_9N = 26909,     /* NAD 1983 UTM Zone 9N               */
PCS_NAD_1983_UTM_10N = 26910,     /* NAD 1983 UTM Zone 10N              */
PCS_NAD_1983_UTM_11N = 26911,     /* NAD 1983 UTM Zone 11N              */
PCS_NAD_1983_UTM_12N = 26912,     /* NAD 1983 UTM Zone 12N              */
PCS_NAD_1983_UTM_13N = 26913,     /* NAD 1983 UTM Zone 13N              */
PCS_NAD_1983_UTM_14N = 26914,     /* NAD 1983 UTM Zone 14N              */
PCS_NAD_1983_UTM_15N = 26915,     /* NAD 1983 UTM Zone 15N              */
PCS_NAD_1983_UTM_16N = 26916,     /* NAD 1983 UTM Zone 16N              */
PCS_NAD_1983_UTM_17N = 26917,     /* NAD 1983 UTM Zone 17N              */
PCS_NAD_1983_UTM_18N = 26918,     /* NAD 1983 UTM Zone 18N              */
PCS_NAD_1983_UTM_19N = 26919,     /* NAD 1983 UTM Zone 19N              */
PCS_NAD_1983_UTM_20N = 26920,     /* NAD 1983 UTM Zone 20N              */
PCS_NAD_1983_UTM_21N = 26921,     /* NAD 1983 UTM Zone 21N              */
PCS_NAD_1983_UTM_22N = 26922,     /* NAD 1983 UTM Zone 22N              */
PCS_NAD_1983_UTM_23N = 26923,     /* NAD 1983 UTM Zone 23N              */
//}}

//{{
PCS_ETRS_1989_UTM_28N = 25828,     /* ETRS 1989 UTM Zone 28N            */
PCS_ETRS_1989_UTM_29N = 25829,     /* ETRS 1989 UTM Zone 29N            */
PCS_ETRS_1989_UTM_30N = 25830,     /* ETRS 1989 UTM Zone 30N            */
PCS_ETRS_1989_UTM_31N = 25831,     /* ETRS 1989 UTM Zone 31N            */
PCS_ETRS_1989_UTM_32N = 25832,     /* ETRS 1989 UTM Zone 32N            */
PCS_ETRS_1989_UTM_33N = 25833,     /* ETRS 1989 UTM Zone 33N            */
PCS_ETRS_1989_UTM_34N = 25834,     /* ETRS 1989 UTM Zone 34N            */
PCS_ETRS_1989_UTM_35N = 25835,     /* ETRS 1989 UTM Zone 35N            */
PCS_ETRS_1989_UTM_36N = 25836,     /* ETRS 1989 UTM Zone 36N            */
PCS_ETRS_1989_UTM_37N = 25837,     /* ETRS 1989 UTM Zone 37N            */
PCS_ETRS_1989_UTM_38N = 25838,     /* ETRS 1989 UTM Zone 38N            */
//}}

//{{
PCS_PULKOVO_1942_GK_4 = 28404,     /* Pulkovo 1942 GK Zone 4          */
PCS_PULKOVO_1942_GK_5 = 28405,     /* Pulkovo 1942 GK Zone 5          */
PCS_PULKOVO_1942_GK_6 = 28406,     /* Pulkovo 1942 GK Zone 6          */
PCS_PULKOVO_1942_GK_7 = 28407,     /* Pulkovo 1942 GK Zone 7          */
PCS_PULKOVO_1942_GK_8 = 28408,     /* Pulkovo 1942 GK Zone 8          */
PCS_PULKOVO_1942_GK_9 = 28409,     /* Pulkovo 1942 GK Zone 9          */
PCS_PULKOVO_1942_GK_10 = 28410,     /* Pulkovo 1942 GK Zone 10         */
PCS_PULKOVO_1942_GK_11 = 28411,     /* Pulkovo 1942 GK Zone 11         */
PCS_PULKOVO_1942_GK_12 = 28412,     /* Pulkovo 1942 GK Zone 12         */
PCS_PULKOVO_1942_GK_13 = 28413,     /* Pulkovo 1942 GK Zone 13         */
PCS_PULKOVO_1942_GK_14 = 28414,     /* Pulkovo 1942 GK Zone 14         */
PCS_PULKOVO_1942_GK_15 = 28415,     /* Pulkovo 1942 GK Zone 15         */
PCS_PULKOVO_1942_GK_16 = 28416,     /* Pulkovo 1942 GK Zone 16         */
PCS_PULKOVO_1942_GK_17 = 28417,     /* Pulkovo 1942 GK Zone 17         */
PCS_PULKOVO_1942_GK_18 = 28418,     /* Pulkovo 1942 GK Zone 18         */
PCS_PULKOVO_1942_GK_19 = 28419,     /* Pulkovo 1942 GK Zone 19         */
PCS_PULKOVO_1942_GK_20 = 28420,     /* Pulkovo 1942 GK Zone 20         */
PCS_PULKOVO_1942_GK_21 = 28421,     /* Pulkovo 1942 GK Zone 21         */
PCS_PULKOVO_1942_GK_22 = 28422,     /* Pulkovo 1942 GK Zone 22         */
PCS_PULKOVO_1942_GK_23 = 28423,     /* Pulkovo 1942 GK Zone 23         */
PCS_PULKOVO_1942_GK_24 = 28424,     /* Pulkovo 1942 GK Zone 24         */
PCS_PULKOVO_1942_GK_25 = 28425,     /* Pulkovo 1942 GK Zone 25         */
PCS_PULKOVO_1942_GK_26 = 28426,     /* Pulkovo 1942 GK Zone= 26         */
PCS_PULKOVO_1942_GK_27 = 28427,     /* Pulkovo 1942 GK Zone 27         */
PCS_PULKOVO_1942_GK_28 = 28428,     /* Pulkovo 1942 GK Zone=  28         */
PCS_PULKOVO_1942_GK_29 = 28429,     /* Pulkovo 1942 GK Zone=  29         */
PCS_PULKOVO_1942_GK_30 = 28430,     /* Pulkovo 1942 GK Zone 30         */
PCS_PULKOVO_1942_GK_31 = 28431,     /* Pulkovo 1942 GK Zone 31         */
PCS_PULKOVO_1942_GK_32 = 28432,     /* Pulkovo 1942 GK Zone= 32         */
PCS_PULKOVO_1942_GK_4N = 28464,     /* Pulkovo 1942 GK Zone 4N         */
PCS_PULKOVO_1942_GK_5N = 28465,     /* Pulkovo 1942 GK Zone 5N         */
PCS_PULKOVO_1942_GK_6N = 28466,     /* Pulkovo 1942 GK Zone 6N         */
PCS_PULKOVO_1942_GK_7N = 28467,     /* Pulkovo 1942 GK Zone 7N         */
PCS_PULKOVO_1942_GK_8N = 28468,     /* Pulkovo 1942 GK Zone 8N         */
PCS_PULKOVO_1942_GK_9N = 28469,     /* Pulkovo 1942 GK Zone 9N         */
PCS_PULKOVO_1942_GK_10N = 28470,     /* Pulkovo 1942 GK Zone 10N        */
PCS_PULKOVO_1942_GK_11N = 28471,     /* Pulkovo 1942 GK Zone 11N        */
PCS_PULKOVO_1942_GK_12N = 28472,     /* Pulkovo 1942 GK Zone 12N        */
PCS_PULKOVO_1942_GK_13N = 28473,     /* Pulkovo 1942 GK Zone 13N        */
PCS_PULKOVO_1942_GK_14N = 28474,     /* Pulkovo 1942 GK Zone 14N        */
PCS_PULKOVO_1942_GK_15N = 28475,     /* Pulkovo 1942 GK Zone 15N        */
PCS_PULKOVO_1942_GK_16N = 28476,     /* Pulkovo 1942 GK Zone 16N        */
PCS_PULKOVO_1942_GK_17N = 28477,     /* Pulkovo 1942 GK Zone 17N        */
PCS_PULKOVO_1942_GK_18N = 28478,     /* Pulkovo 1942 GK Zone 18N        */
PCS_PULKOVO_1942_GK_19N = 28479,     /* Pulkovo 1942 GK Zone 19N        */
PCS_PULKOVO_1942_GK_20N = 28480,     /* Pulkovo 1942 GK Zone 20N        */
PCS_PULKOVO_1942_GK_21N = 28481,     /* Pulkovo 1942 GK Zone 21N        */
PCS_PULKOVO_1942_GK_22N = 28482,     /* Pulkovo 1942 GK Zone 22N        */
PCS_PULKOVO_1942_GK_23N = 28483,     /* Pulkovo 1942 GK Zone 23N        */
PCS_PULKOVO_1942_GK_24N = 28484,     /* Pulkovo 1942 GK Zone 24N        */
PCS_PULKOVO_1942_GK_25N = 28485,     /* Pulkovo 1942 GK Zone 25N        */
PCS_PULKOVO_1942_GK_26N = 28486,     /* Pulkovo 1942 GK Zone= 26N        */
PCS_PULKOVO_1942_GK_27N = 28487,     /* Pulkovo 1942 GK Zone 27N        */
PCS_PULKOVO_1942_GK_28N = 28488,     /* Pulkovo 1942 GK Zone=  28N        */
PCS_PULKOVO_1942_GK_29N = 28489,     /* Pulkovo 1942 GK Zone=  29N        */
PCS_PULKOVO_1942_GK_30N = 28490,     /* Pulkovo 1942 GK Zone 30N        */
PCS_PULKOVO_1942_GK_31N = 28491,     /* Pulkovo 1942 GK Zone 31N        */
PCS_PULKOVO_1942_GK_32N = 28492,     /* Pulkovo 1942 GK Zone= 32N        */
//}}

//{{
PCS_PULKOVO_1995_GK_4 = 20004,     /* Pulkovo 1995 GK Zone 4          */
PCS_PULKOVO_1995_GK_5 = 20005,     /* Pulkovo 1995 GK Zone 5          */
PCS_PULKOVO_1995_GK_6 = 20006,     /* Pulkovo 1995 GK Zone 6          */
PCS_PULKOVO_1995_GK_7 = 20007,     /* Pulkovo 1995 GK Zone 7          */
PCS_PULKOVO_1995_GK_8 = 20008,     /* Pulkovo 1995 GK Zone 8          */
PCS_PULKOVO_1995_GK_9 = 20009,     /* Pulkovo 1995 GK Zone 9          */
PCS_PULKOVO_1995_GK_10 = 20010,     /* Pulkovo 1995 GK Zone 10         */
PCS_PULKOVO_1995_GK_11 = 20011,     /* Pulkovo 1995 GK Zone 11         */
PCS_PULKOVO_1995_GK_12 = 20012,     /* Pulkovo 1995 GK Zone 12         */
PCS_PULKOVO_1995_GK_13 = 20013,     /* Pulkovo 1995 GK Zone 13         */
PCS_PULKOVO_1995_GK_14 = 20014,     /* Pulkovo 1995 GK Zone 14         */
PCS_PULKOVO_1995_GK_15 = 20015,     /* Pulkovo 1995 GK Zone 15         */
PCS_PULKOVO_1995_GK_16 = 20016,     /* Pulkovo 1995 GK Zone 16         */
PCS_PULKOVO_1995_GK_17 = 20017,     /* Pulkovo 1995 GK Zone 17         */
PCS_PULKOVO_1995_GK_18 = 20018,     /* Pulkovo 1995 GK Zone 18         */
PCS_PULKOVO_1995_GK_19 = 20019,     /* Pulkovo 1995 GK Zone 19         */
PCS_PULKOVO_1995_GK_20 = 20020,     /* Pulkovo 1995 GK Zone= 20         */
PCS_PULKOVO_1995_GK_21 = 20021,     /* Pulkovo 1995 GK Zone 21         */
PCS_PULKOVO_1995_GK_22 = 20022,     /* Pulkovo 1995 GK Zone 22         */
PCS_PULKOVO_1995_GK_23 = 20023,     /* Pulkovo 1995 GK Zone 23         */
PCS_PULKOVO_1995_GK_24 = 20024,     /* Pulkovo 1995 GK Zone 24         */
PCS_PULKOVO_1995_GK_25 = 20025,     /* Pulkovo 1995 GK Zone 25         */
PCS_PULKOVO_1995_GK_26 = 20026,     /* Pulkovo 1995 GK Zone= 26         */
PCS_PULKOVO_1995_GK_27 = 20027,     /* Pulkovo 1995 GK Zone 27         */
PCS_PULKOVO_1995_GK_28 = 20028,     /* Pulkovo 1995 GK Zone=  28         */
PCS_PULKOVO_1995_GK_29 = 20029,     /* Pulkovo 1995 GK Zone=  29         */
PCS_PULKOVO_1995_GK_30 = 20030,     /* Pulkovo 1995 GK Zone 30         */
PCS_PULKOVO_1995_GK_31 = 20031,     /* Pulkovo 1995 GK Zone 31         */
PCS_PULKOVO_1995_GK_32 = 20032,     /* Pulkovo 1995 GK Zone= 32         */
PCS_PULKOVO_1995_GK_4N = 20064,     /* Pulkovo 1995 GK Zone 4N         */
PCS_PULKOVO_1995_GK_5N = 20065,     /* Pulkovo 1995 GK Zone 5N         */
PCS_PULKOVO_1995_GK_6N = 20066,     /* Pulkovo 1995 GK Zone 6N         */
PCS_PULKOVO_1995_GK_7N = 20067,     /* Pulkovo 1995 GK Zone 7N         */
PCS_PULKOVO_1995_GK_8N = 20068,     /* Pulkovo 1995 GK Zone 8N         */
PCS_PULKOVO_1995_GK_9N = 20069,     /* Pulkovo 1995 GK Zone 9N         */
PCS_PULKOVO_1995_GK_10N = 20070,     /* Pulkovo 1995 GK Zone 10N        */
PCS_PULKOVO_1995_GK_11N = 20071,     /* Pulkovo 1995 GK Zone 11N        */
PCS_PULKOVO_1995_GK_12N = 20072,     /* Pulkovo 1995 GK Zone 12N        */
PCS_PULKOVO_1995_GK_13N = 20073,     /* Pulkovo 1995 GK Zone 13N        */
PCS_PULKOVO_1995_GK_14N = 20074,     /* Pulkovo 1995 GK Zone 14N        */
PCS_PULKOVO_1995_GK_15N = 20075,     /* Pulkovo 1995 GK Zone 15N        */
PCS_PULKOVO_1995_GK_16N = 20076,     /* Pulkovo 1995 GK Zone 16N        */
PCS_PULKOVO_1995_GK_17N = 20077,     /* Pulkovo 1995 GK Zone 17N        */
PCS_PULKOVO_1995_GK_18N = 20078,     /* Pulkovo 1995 GK Zone 18N        */
PCS_PULKOVO_1995_GK_19N = 20079,     /* Pulkovo 1995 GK Zone 19N        */
PCS_PULKOVO_1995_GK_20N = 20080,     /* Pulkovo 1995 GK Zone= 20N        */
PCS_PULKOVO_1995_GK_21N = 20081,     /* Pulkovo 1995 GK Zone 21N        */
PCS_PULKOVO_1995_GK_22N = 20082,     /* Pulkovo 1995 GK Zone 22N        */
PCS_PULKOVO_1995_GK_23N = 20083,     /* Pulkovo 1995 GK Zone 23N        */
PCS_PULKOVO_1995_GK_24N = 20084,     /* Pulkovo 1995 GK Zone 24N        */
PCS_PULKOVO_1995_GK_25N = 20085,     /* Pulkovo 1995 GK Zone 25N        */
PCS_PULKOVO_1995_GK_26N = 20086,     /* Pulkovo 1995 GK Zone 26N        */
PCS_PULKOVO_1995_GK_27N = 20087,     /* Pulkovo 1995 GK Zone 27N        */
PCS_PULKOVO_1995_GK_28N = 20088,     /* Pulkovo 1995 GK Zone 28N        */
PCS_PULKOVO_1995_GK_29N = 20089,     /* Pulkovo 1995 GK Zone=  29N        */
PCS_PULKOVO_1995_GK_30N = 20090,     /* Pulkovo 1995 GK Zone 30N        */
PCS_PULKOVO_1995_GK_31N = 20091,     /* Pulkovo 1995 GK Zone 31N        */
PCS_PULKOVO_1995_GK_32N = 20092,     /* Pulkovo 1995 GK Zone= 32N        */
//}}

//{{
PCS_BEIJING_1954_GK_13 = 21413,     /* Beijing 1954 GK Zone 13         */
PCS_BEIJING_1954_GK_14 = 21414,     /* Beijing 1954 GK Zone 14         */
PCS_BEIJING_1954_GK_15 = 21415,     /* Beijing 1954 GK Zone 15         */
PCS_BEIJING_1954_GK_16 = 21416,     /* Beijing 1954 GK Zone 16         */
PCS_BEIJING_1954_GK_17 = 21417,     /* Beijing 1954 GK Zone 17         */
PCS_BEIJING_1954_GK_18 = 21418,     /* Beijing 1954 GK Zone 18         */
PCS_BEIJING_1954_GK_19 = 21419,     /* Beijing 1954 GK Zone 19         */
PCS_BEIJING_1954_GK_20 = 21420,     /* Beijing 1954 GK Zone= 20         */
PCS_BEIJING_1954_GK_21 = 21421,     /* Beijing 1954 GK Zone= 21         */
PCS_BEIJING_1954_GK_22 = 21422,     /* Beijing 1954 GK Zone 22         */
PCS_BEIJING_1954_GK_23 = 21423,     /* Beijing 1954 GK Zone 23         */
PCS_BEIJING_1954_GK_13N = 21473,     /* Beijing 1954 GK Zone 13N        */
PCS_BEIJING_1954_GK_14N = 21474,     /* Beijing 1954 GK Zone 14N        */
PCS_BEIJING_1954_GK_15N = 21475,     /* Beijing 1954 GK Zone 15N        */
PCS_BEIJING_1954_GK_16N = 21476,     /* Beijing 1954 GK Zone 16N        */
PCS_BEIJING_1954_GK_17N = 21477,     /* Beijing 1954 GK Zone 17N        */
PCS_BEIJING_1954_GK_18N = 21478,     /* Beijing 1954 GK Zone 18N        */
PCS_BEIJING_1954_GK_19N = 21479,     /* Beijing 1954 GK Zone 19N        */
PCS_BEIJING_1954_GK_20N = 21480,     /* Beijing 1954 GK Zone 20N        */
PCS_BEIJING_1954_GK_21N = 21481,     /* Beijing 1954 GK Zone= 21N        */
PCS_BEIJING_1954_GK_22N = 21482,     /* Beijing 1954 GK Zone 22N        */
PCS_BEIJING_1954_GK_23N = 21483,     /* Beijing 1954 GK Zone=  23N        */
//}}

//{{{增加中国最常用投影的预定义投影

PCS_BEIJING_1954_3_DEGREE_GK_25 = 2401,     /* Beijing 1954 3 degree GK zone 25         */
PCS_BEIJING_1954_3_DEGREE_GK_26 = 2402,     /* Beijing 1954 3 degree GK zone 26         */
PCS_BEIJING_1954_3_DEGREE_GK_27 = 2403,     /* Beijing 1954 3 degree GK zone 27         */
PCS_BEIJING_1954_3_DEGREE_GK_28 = 2404,     /* Beijing 1954 3 degree GK zone 28         */
PCS_BEIJING_1954_3_DEGREE_GK_29 = 2405,     /* Beijing 1954 3 degree GK zone 29         */
PCS_BEIJING_1954_3_DEGREE_GK_30 = 2406,     /* Beijing 1954 3 degree GK zone 30         */
PCS_BEIJING_1954_3_DEGREE_GK_31 = 2407,     /* Beijing 1954 3 degree GK zone 31         */
PCS_BEIJING_1954_3_DEGREE_GK_32 = 2408,     /* Beijing 1954 3 degree GK zone 32       */
PCS_BEIJING_1954_3_DEGREE_GK_33 = 2409,     /* Beijing 1954 3 degree GK zone 33         */
PCS_BEIJING_1954_3_DEGREE_GK_34 = 2410,     /* Beijing 1954 3 degree GK zone 34         */
PCS_BEIJING_1954_3_DEGREE_GK_35 = 2411,     /* Beijing 1954 3 degree GK zone 35         */
PCS_BEIJING_1954_3_DEGREE_GK_36 = 2412,     /* Beijing 1954 3 degree GK zone 36         */
PCS_BEIJING_1954_3_DEGREE_GK_37 = 2413,     /* Beijing 1954 3 degree GK zone 37        */
PCS_BEIJING_1954_3_DEGREE_GK_38 = 2414,     /* Beijing 1954 3 degree GK zone 38         */
PCS_BEIJING_1954_3_DEGREE_GK_39 = 2415,     /* Beijing 1954 3 degree GK zone 39         */
PCS_BEIJING_1954_3_DEGREE_GK_40 = 2416,     /* Beijing 1954 3 degree GK zone 40         */
PCS_BEIJING_1954_3_DEGREE_GK_41 = 2417,     /* Beijing 1954 3 degree GK zone 41      */
PCS_BEIJING_1954_3_DEGREE_GK_42 = 2418,     /* Beijing 1954 3 degree GK zone 42         */
PCS_BEIJING_1954_3_DEGREE_GK_43 = 2419,     /* Beijing 1954 3 degree GK zone 43         */
PCS_BEIJING_1954_3_DEGREE_GK_44 = 2420,     /* Beijing 1954 3 degree GK zone 44        */
PCS_BEIJING_1954_3_DEGREE_GK_45 = 2421,     /* Beijing 1954 3 degree GK zone 45         */
PCS_BEIJING_1954_3_DEGREE_GK_25N = 2422,     /* Beijing 1954 3 degree GK zone 25N         */
PCS_BEIJING_1954_3_DEGREE_GK_26N = 2423,     /* Beijing 1954 3 degree GK zone 26N         */
PCS_BEIJING_1954_3_DEGREE_GK_27N = 2424,     /* Beijing 1954 3 degree GK zone 27N         */
PCS_BEIJING_1954_3_DEGREE_GK_28N = 2425,     /* Beijing 1954 3 degree GK zone 28N         */
PCS_BEIJING_1954_3_DEGREE_GK_29N = 2426,     /* Beijing 1954 3 degree GK zone 29N        */
PCS_BEIJING_1954_3_DEGREE_GK_30N = 2427,     /* Beijing 1954 3 degree GK zone 30N         */
PCS_BEIJING_1954_3_DEGREE_GK_31N = 2428,     /* Beijing 1954 3 degree GK zone 31N         */
PCS_BEIJING_1954_3_DEGREE_GK_32N = 2429,     /* Beijing 1954 3 degree GK zone 32N       */
PCS_BEIJING_1954_3_DEGREE_GK_33N = 2430,     /* Beijing 1954 3 degree GK zone 33N         */
PCS_BEIJING_1954_3_DEGREE_GK_34N = 2431,     /* Beijing 1954 3 degree GK zone 34N         */
PCS_BEIJING_1954_3_DEGREE_GK_35N = 2432,     /* Beijing 1954 3 degree GK zone 35N         */
PCS_BEIJING_1954_3_DEGREE_GK_36N = 2433,     /* Beijing 1954 3 degree GK zone 36N         */
PCS_BEIJING_1954_3_DEGREE_GK_37N = 2434,     /* Beijing 1954 3 degree GK zone 37N        */
PCS_BEIJING_1954_3_DEGREE_GK_38N = 2435,     /* Beijing 1954 3 degree GK zone 38N         */
PCS_BEIJING_1954_3_DEGREE_GK_39N = 2436,     /* Beijing 1954 3 degree GK zone 39N         */
PCS_BEIJING_1954_3_DEGREE_GK_40N = 2437,     /* Beijing 1954 3 degree GK zone 40N         */
PCS_BEIJING_1954_3_DEGREE_GK_41N = 2438,     /* Beijing 1954 3 degree GK zone 41N      */
PCS_BEIJING_1954_3_DEGREE_GK_42N = 2439,     /* Beijing 1954 3 degree GK zone 42N         */
PCS_BEIJING_1954_3_DEGREE_GK_43N = 2440,     /* Beijing 1954 3 degree GK zone 43N         */
PCS_BEIJING_1954_3_DEGREE_GK_44N = 2441,     /* Beijing 1954 3 degree GK zone 44N        */
PCS_BEIJING_1954_3_DEGREE_GK_45N = 2442,     /* Beijing 1954 3 degree GK zone 45N         */

PCS_CHINA_2000_GK_13 = 21513,     /* China 2000 GK Zone 13         */
PCS_CHINA_2000_GK_14 = 21514,     /* China 2000 GK Zone 14         */
PCS_CHINA_2000_GK_15 = 21515,     /* China 2000 GK Zone 15         */
PCS_CHINA_2000_GK_16 = 21516,     /* China 2000 GK Zone 16         */
PCS_CHINA_2000_GK_17 = 21517,     /* China 2000 GK Zone 17         */
PCS_CHINA_2000_GK_18 = 21518,     /* China 2000 GK Zone 18         */
PCS_CHINA_2000_GK_19 = 21519,     /* China 2000 GK Zone 19         */
PCS_CHINA_2000_GK_20 = 21520,     /* China 2000 GK Zone= 20         */
PCS_CHINA_2000_GK_21 = 21521,     /* China 2000 GK Zone= 21         */
PCS_CHINA_2000_GK_22 = 21522,     /* China 2000 GK Zone 22         */
PCS_CHINA_2000_GK_23 = 21523,     /* China 2000 GK Zone 23         */
PCS_CHINA_2000_GK_13N = 21573,     /* China 2000 GK Zone 13N        */
PCS_CHINA_2000_GK_14N = 21574,     /* China 2000 GK Zone 14N        */
PCS_CHINA_2000_GK_15N = 21575,     /* China 2000 GK Zone 15N        */
PCS_CHINA_2000_GK_16N = 21576,     /* China 2000 GK Zone 16N        */
PCS_CHINA_2000_GK_17N = 21577,     /* China 2000 GK Zone 17N        */
PCS_CHINA_2000_GK_18N = 21578,     /* China 2000 GK Zone 18N        */
PCS_CHINA_2000_GK_19N = 21579,     /* China 2000 GK Zone 19N        */
PCS_CHINA_2000_GK_20N = 21580,     /* China 2000 GK Zone 20N        */
PCS_CHINA_2000_GK_21N = 21581,     /* China 2000 GK Zone= 21N        */
PCS_CHINA_2000_GK_22N = 21582,     /* China 2000 GK Zone 22N        */
PCS_CHINA_2000_GK_23N = 21583,     /* China 2000 GK Zone=  23N        */

PCS_CHINA_2000_3_DEGREE_GK_25 = 21625,     /* China 2000 3 degree GK zone 25         */
PCS_CHINA_2000_3_DEGREE_GK_26 = 21626,     /* China 2000 3 degree GK zone 26         */
PCS_CHINA_2000_3_DEGREE_GK_27 = 21627,     /* China 2000 3 degree GK zone 27         */
PCS_CHINA_2000_3_DEGREE_GK_28 = 21628,     /* China 2000 3 degree GK zone 28         */
PCS_CHINA_2000_3_DEGREE_GK_29 = 21629,     /* China 2000 3 degree GK zone 29         */
PCS_CHINA_2000_3_DEGREE_GK_30 = 21630,     /* China 2000 3 degree GK zone 30         */
PCS_CHINA_2000_3_DEGREE_GK_31 = 21631,     /* China 2000 3 degree GK zone 31         */
PCS_CHINA_2000_3_DEGREE_GK_32 = 21632,     /* China 2000 3 degree GK zone 32       */
PCS_CHINA_2000_3_DEGREE_GK_33 = 21633,     /* China 2000 3 degree GK zone 33         */
PCS_CHINA_2000_3_DEGREE_GK_34 = 21634,     /* China 2000 3 degree GK zone 34         */
PCS_CHINA_2000_3_DEGREE_GK_35 = 21635,     /* China 2000 3 degree GK zone 35         */
PCS_CHINA_2000_3_DEGREE_GK_36 = 21636,     /* China 2000 3 degree GK zone 36         */
PCS_CHINA_2000_3_DEGREE_GK_37 = 21637,     /* China 2000 3 degree GK zone 37        */
PCS_CHINA_2000_3_DEGREE_GK_38 = 21638,     /* China 2000 3 degree GK zone 38         */
PCS_CHINA_2000_3_DEGREE_GK_39 = 21639,     /* China 2000 3 degree GK zone 39         */
PCS_CHINA_2000_3_DEGREE_GK_40 = 21640,     /* China 2000 3 degree GK zone 40         */
PCS_CHINA_2000_3_DEGREE_GK_41 = 21641,     /* China 2000 3 degree GK zone 41      */
PCS_CHINA_2000_3_DEGREE_GK_42 = 21642,     /* China 2000 3 degree GK zone 42         */
PCS_CHINA_2000_3_DEGREE_GK_43 = 21643,     /* China 2000 3 degree GK zone 43         */
PCS_CHINA_2000_3_DEGREE_GK_44 = 21644,     /* China 2000 3 degree GK zone 44        */
PCS_CHINA_2000_3_DEGREE_GK_45 = 21645,     /* China 2000 3 degree GK zone 45         */
PCS_CHINA_2000_3_DEGREE_GK_25N = 21675,     /* China 2000 3 degree GK zone 25N         */
PCS_CHINA_2000_3_DEGREE_GK_26N = 21676,     /* China 2000 3 degree GK zone 26N         */
PCS_CHINA_2000_3_DEGREE_GK_27N = 21677,     /* China 2000 3 degree GK zone 27N         */
PCS_CHINA_2000_3_DEGREE_GK_28N = 21678,     /* China 2000 3 degree GK zone 28N         */
PCS_CHINA_2000_3_DEGREE_GK_29N = 21679,     /* China 2000 3 degree GK zone 29N        */
PCS_CHINA_2000_3_DEGREE_GK_30N = 21680,     /* China 2000 3 degree GK zone 30N         */
PCS_CHINA_2000_3_DEGREE_GK_31N = 21681,     /* China 2000 3 degree GK zone 31N         */
PCS_CHINA_2000_3_DEGREE_GK_32N = 21682,     /* China 2000 3 degree GK zone 32N       */
PCS_CHINA_2000_3_DEGREE_GK_33N = 21683,     /* China 2000 3 degree GK zone 33N         */
PCS_CHINA_2000_3_DEGREE_GK_34N = 21684,     /* China 2000 3 degree GK zone 34N         */
PCS_CHINA_2000_3_DEGREE_GK_35N = 21685,     /* China 2000 3 degree GK zone 35N         */
PCS_CHINA_2000_3_DEGREE_GK_36N = 21686,     /* China 2000 3 degree GK zone 36N         */
PCS_CHINA_2000_3_DEGREE_GK_37N = 21687,     /* China 2000 3 degree GK zone 37N        */
PCS_CHINA_2000_3_DEGREE_GK_38N = 21688,     /* China 2000 3 degree GK zone 38N         */
PCS_CHINA_2000_3_DEGREE_GK_39N = 21689,     /* China 2000 3 degree GK zone 39N         */
PCS_CHINA_2000_3_DEGREE_GK_40N = 21690,     /* China 2000 3 degree GK zone 40N         */
PCS_CHINA_2000_3_DEGREE_GK_41N = 21691,     /* China 2000 3 degree GK zone 41N      */
PCS_CHINA_2000_3_DEGREE_GK_42N = 21692,     /* China 2000 3 degree GK zone 42N         */
PCS_CHINA_2000_3_DEGREE_GK_43N = 21693,     /* China 2000 3 degree GK zone 43N         */
PCS_CHINA_2000_3_DEGREE_GK_44N = 21694,     /* China 2000 3 degree GK zone 44N        */
PCS_CHINA_2000_3_DEGREE_GK_45N = 21695,     /* China 2000 3 degree GK zone 45N         */

PCS_XIAN_1980_GK_13 = 2327,     /* Xian 1980 GK Zone 13         */
PCS_XIAN_1980_GK_14 = 2328,     /* Xian 1980 GK Zone 14         */
PCS_XIAN_1980_GK_15 = 2329,     /* Xian 1980 GK Zone 15         */
PCS_XIAN_1980_GK_16 = 2330,     /* Xian 1980 GK Zone 16         */
PCS_XIAN_1980_GK_17 = 2331,     /* Xian 1980 GK Zone 17         */
PCS_XIAN_1980_GK_18 = 2332,     /* Xian 1980 GK Zone 18         */
PCS_XIAN_1980_GK_19 = 2333,     /* Xian 1980 GK Zone 19         */
PCS_XIAN_1980_GK_20 = 2334,     /* Xian 1980 GK Zone= 20         */
PCS_XIAN_1980_GK_21 = 2335,     /* Xian 1980 GK Zone= 21         */
PCS_XIAN_1980_GK_22 = 2336,     /* Xian 1980 GK Zone 22         */
PCS_XIAN_1980_GK_23 = 2337,     /* Xian 1980 GK Zone 23         */
PCS_XIAN_1980_GK_13N = 2338,     /* Xian 1980 GK Zone 13N        */
PCS_XIAN_1980_GK_14N = 2339,     /* Xian 1980 GK Zone 14N        */
PCS_XIAN_1980_GK_15N = 2340,     /* Xian 1980 GK Zone 15N        */
PCS_XIAN_1980_GK_16N = 2341,     /* Xian 1980 GK Zone 16N        */
PCS_XIAN_1980_GK_17N = 2342,     /* Xian 1980 GK Zone 17N        */
PCS_XIAN_1980_GK_18N = 2343,     /* Xian 1980 GK Zone 18N        */
PCS_XIAN_1980_GK_19N = 2344,     /* Xian 1980 GK Zone 19N        */
PCS_XIAN_1980_GK_20N = 2345,     /* Xian 1980 GK Zone 20N        */
PCS_XIAN_1980_GK_21N = 2346,     /* Xian 1980 GK Zone= 21N        */
PCS_XIAN_1980_GK_22N = 2347,     /* Xian 1980 GK Zone 22N        */
PCS_XIAN_1980_GK_23N = 2348,     /* Xian 1980 GK Zone=  23N        */

PCS_XIAN_1980_3_DEGREE_GK_25 = 2349,     /* Xian 1980 3 degree GK zone 25         */
PCS_XIAN_1980_3_DEGREE_GK_26 = 2350,     /* Xian 1980 3 degree GK zone 26         */
PCS_XIAN_1980_3_DEGREE_GK_27 = 2351,     /* Xian 1980 3 degree GK zone 27         */
PCS_XIAN_1980_3_DEGREE_GK_28 = 2352,     /* Xian 1980 3 degree GK zone 28         */
PCS_XIAN_1980_3_DEGREE_GK_29 = 2353,     /* Xian 1980 3 degree GK zone 29         */
PCS_XIAN_1980_3_DEGREE_GK_30 = 2354,     /* Xian 1980 3 degree GK zone 30         */
PCS_XIAN_1980_3_DEGREE_GK_31 = 2355,     /* Xian 1980 3 degree GK zone 31         */
PCS_XIAN_1980_3_DEGREE_GK_32 = 2356,     /* Xian 1980 3 degree GK zone 32       */
PCS_XIAN_1980_3_DEGREE_GK_33 = 2357,     /* Xian 1980 3 degree GK zone 33         */
PCS_XIAN_1980_3_DEGREE_GK_34 = 2358,     /* Xian 1980 3 degree GK zone 34         */
PCS_XIAN_1980_3_DEGREE_GK_35 = 2359,     /* Xian 1980 3 degree GK zone 35         */
PCS_XIAN_1980_3_DEGREE_GK_36 = 2360,     /* Xian 1980 3 degree GK zone 36         */
PCS_XIAN_1980_3_DEGREE_GK_37 = 2361,     /* Xian 1980 3 degree GK zone 37        */
PCS_XIAN_1980_3_DEGREE_GK_38 = 2362,     /* Xian 1980 3 degree GK zone 38         */
PCS_XIAN_1980_3_DEGREE_GK_39 = 2363,     /* Xian 1980 3 degree GK zone 39         */
PCS_XIAN_1980_3_DEGREE_GK_40 = 2364,     /* Xian 1980 3 degree GK zone 40         */
PCS_XIAN_1980_3_DEGREE_GK_41 = 2365,     /* Xian 1980 3 degree GK zone 41      */
PCS_XIAN_1980_3_DEGREE_GK_42 = 2366,     /* Xian 1980 3 degree GK zone 42         */
PCS_XIAN_1980_3_DEGREE_GK_43 = 2367,     /* Xian 1980 3 degree GK zone 43         */
PCS_XIAN_1980_3_DEGREE_GK_44 = 2368,     /* Xian 1980 3 degree GK zone 44        */
PCS_XIAN_1980_3_DEGREE_GK_45 = 2369,     /* Xian 1980 3 degree GK zone 45         */
PCS_XIAN_1980_3_DEGREE_GK_25N = 2370,     /* Xian 1980 3 degree GK zone 25N         */
PCS_XIAN_1980_3_DEGREE_GK_26N = 2371,     /* Xian 1980 3 degree GK zone 26N         */
PCS_XIAN_1980_3_DEGREE_GK_27N = 2372,     /* Xian 1980 3 degree GK zone 27N         */
PCS_XIAN_1980_3_DEGREE_GK_28N = 2373,     /* Xian 1980 3 degree GK zone 28N         */
PCS_XIAN_1980_3_DEGREE_GK_29N = 2374,     /* Xian 1980 3 degree GK zone 29N        */
PCS_XIAN_1980_3_DEGREE_GK_30N = 2375,     /* Xian 1980 3 degree GK zone 30N         */
PCS_XIAN_1980_3_DEGREE_GK_31N = 2376,     /* Xian 1980 3 degree GK zone 31N         */
PCS_XIAN_1980_3_DEGREE_GK_32N = 2377,     /* Xian 1980 3 degree GK zone 32N       */
PCS_XIAN_1980_3_DEGREE_GK_33N = 2378,     /* Xian 1980 3 degree GK zone 33N         */
PCS_XIAN_1980_3_DEGREE_GK_34N = 2379,     /* Xian 1980 3 degree GK zone 34N         */
PCS_XIAN_1980_3_DEGREE_GK_35N = 2380,     /* Xian 1980 3 degree GK zone 35N         */
PCS_XIAN_1980_3_DEGREE_GK_36N = 2381,     /* Xian 1980 3 degree GK zone 36N         */
PCS_XIAN_1980_3_DEGREE_GK_37N = 2382,     /* Xian 1980 3 degree GK zone 37N        */
PCS_XIAN_1980_3_DEGREE_GK_38N = 2383,     /* Xian 1980 3 degree GK zone 38N         */
PCS_XIAN_1980_3_DEGREE_GK_39N = 2384,     /* Xian 1980 3 degree GK zone 39N         */
PCS_XIAN_1980_3_DEGREE_GK_40N = 2385,     /* Xian 1980 3 degree GK zone 40N         */
PCS_XIAN_1980_3_DEGREE_GK_41N = 2386,     /* Xian 1980 3 degree GK zone 41N      */
PCS_XIAN_1980_3_DEGREE_GK_42N = 2387,     /* Xian 1980 3 degree GK zone 42N         */
PCS_XIAN_1980_3_DEGREE_GK_43N = 2388,     /* Xian 1980 3 degree GK zone 43N         */
PCS_XIAN_1980_3_DEGREE_GK_44N = 2389,     /* Xian 1980 3 degree GK zone 44N        */
PCS_XIAN_1980_3_DEGREE_GK_45N = 2390,     /* Xian 1980 3 degree GK zone 45N         */

//}}






//{{
PCS_ED_1950_UTM_28N = 23028,     /* European Datum 1950 UTM Zone 28N    */
PCS_ED_1950_UTM_29N = 23029,     /* European Datum 1950 UTM Zone=  29N    */
PCS_ED_1950_UTM_30N = 23030,     /* European Datum 1950 UTM Zone 30N    */
PCS_ED_1950_UTM_31N = 23031,     /* European Datum 1950 UTM Zone 31N    */
PCS_ED_1950_UTM_32N = 23032,     /* European Datum 1950 UTM Zone= 32N    */
PCS_ED_1950_UTM_33N = 23033,     /* European Datum 1950 UTM Zone 33N    */
PCS_ED_1950_UTM_34N = 23034,     /* European Datum 1950 UTM Zone 34N    */
PCS_ED_1950_UTM_35N = 23035,     /* European Datum 1950 UTM Zone 35N    */
PCS_ED_1950_UTM_36N = 23036,     /* European Datum 1950 UTM Zone 36N    */
PCS_ED_1950_UTM_37N = 23037,     /* European Datum 1950 UTM Zone 37N    */
PCS_ED_1950_UTM_38N = 23038,     /* European Datum 1950 UTM Zone 38N    */
//}}

//{{{

PCS_GCS_KERTAU_MALAYA_METERS = 23110,		/* Kertau Malaya Meters    */

PCS_TIMBALAI_1948_RSO_BORNEO = 23130,		/* Timbalai 1948 RSO Borneo    */

//}}


//{{
PCS_ATS_1977_UTM_19N = 2219,     /* ATS 1977 UTM Zone 19N              */
PCS_ATS_1977_UTM_20N = 2220,     /* ATS 1977 UTM Zone 20N              */
//}}

//{{
PCS_KKJ_FINLAND_1 = 2391,     /* Finland Zone 1                      */
PCS_KKJ_FINLAND_2 = 2392,     /* Finland Zone 2                      */
PCS_KKJ_FINLAND_3 = 2393,     /* Finland Zone 3                      */
PCS_KKJ_FINLAND_4 = 2394,     /* Finland Zone 4                      */
//}}

//{{
PCS_SAD_1969_UTM_18N = 29118,     /* South American 1969 UTM Zone 18N   */
PCS_SAD_1969_UTM_19N = 29119,     /* South American 1969 UTM Zone 19N   */
PCS_SAD_1969_UTM_20N = 29120,     /* South American 1969 UTM Zone 20N   */
PCS_SAD_1969_UTM_21N = 29121,     /* South American 1969 UTM Zone= 21N   */
PCS_SAD_1969_UTM_22N = 29122,     /* South American 1969 UTM Zone 22N   */
PCS_SAD_1969_UTM_17S = 29177,     /* South American 1969 UTM Zone 17S   */
PCS_SAD_1969_UTM_18S = 29178,     /* South American 1969 UTM Zone 18S   */
PCS_SAD_1969_UTM_19S = 29179,     /* South American 1969 UTM Zone 19S   */
PCS_SAD_1969_UTM_20S = 29180,     /* South American 1969 UTM Zone 20S   */
PCS_SAD_1969_UTM_21S = 29181,     /* South American 1969 UTM Zone 21S   */
PCS_SAD_1969_UTM_22S = 29182,     /* South American 1969 UTM Zone 22S   */
PCS_SAD_1969_UTM_23S = 29183,     /* South American 1969 UTM Zone=  23S   */
PCS_SAD_1969_UTM_24S = 29184,     /* South American 1969 UTM Zone 24S   */
PCS_SAD_1969_UTM_25S = 29185,     /* South American 1969 UTM Zone 25S   */
//}}

//{{
PCS_AGD_1966_AMG_48 = 20248,     /* AGD 1966 AMG Zone 48                */
PCS_AGD_1966_AMG_49 = 20249,     /* AGD 1966 AMG Zone 49                */
PCS_AGD_1966_AMG_50 = 20250,     /* AGD 1966 AMG Zone 50                */
PCS_AGD_1966_AMG_51 = 20251,     /* AGD 1966 AMG Zone 51                */
PCS_AGD_1966_AMG_52 = 20252,     /* AGD 1966 AMG Zone 52                */
PCS_AGD_1966_AMG_53 = 20253,     /* AGD 1966 AMG Zone 53                */
PCS_AGD_1966_AMG_54 = 20254,     /* AGD 1966 AMG Zone 54                */
PCS_AGD_1966_AMG_55 = 20255,     /* AGD 1966 AMG Zone 55                */
PCS_AGD_1966_AMG_56 = 20256,     /* AGD 1966 AMG Zone 56                */
PCS_AGD_1966_AMG_57 = 20257,     /* AGD 1966 AMG Zone 57                */
PCS_AGD_1966_AMG_58 = 20258,     /* AGD 1966 AMG Zone 58                */
//}}

//{{
PCS_AGD_1984_AMG_48 = 20348,     /* AGD 1984 AMG Zone 48                */
PCS_AGD_1984_AMG_49 = 20349,     /* AGD 1984 AMG Zone 49                */
PCS_AGD_1984_AMG_50 = 20350,     /* AGD 1984 AMG Zone 50                */
PCS_AGD_1984_AMG_51 = 20351,     /* AGD 1984 AMG Zone 51                */
PCS_AGD_1984_AMG_52 = 20352,     /* AGD 1984 AMG Zone 52                */
PCS_AGD_1984_AMG_53 = 20353,     /* AGD 1984 AMG Zone 53                */
PCS_AGD_1984_AMG_54 = 20354,     /* AGD 1984 AMG Zone 54                */
PCS_AGD_1984_AMG_55 = 20355,     /* AGD 1984 AMG Zone 55                */
PCS_AGD_1984_AMG_56 = 20356,     /* AGD 1984 AMG Zone 56                */
PCS_AGD_1984_AMG_57 = 20357,     /* AGD 1984 AMG Zone 57                */
PCS_AGD_1984_AMG_58 = 20358,     /* AGD 1984 AMG Zone 58                */
//}}

//{{
PCS_GDA_1994_MGA_48 = 28348,     /* GDA 1994 MGA Zone 48                */
PCS_GDA_1994_MGA_49 = 28349,     /* GDA 1994 MGA Zone 49                */
PCS_GDA_1994_MGA_50 = 28350,     /* GDA 1994 MGA Zone 50                */
PCS_GDA_1994_MGA_51 = 28351,     /* GDA 1994 MGA Zone 51                */
PCS_GDA_1994_MGA_52 = 28352,     /* GDA 1994 MGA Zone 52                */
PCS_GDA_1994_MGA_53 = 28353,     /* GDA 1994 MGA Zone 53                */
PCS_GDA_1994_MGA_54 = 28354,     /* GDA 1994 MGA Zone 54                */
PCS_GDA_1994_MGA_55 = 28355,     /* GDA 1994 MGA Zone 55                */
PCS_GDA_1994_MGA_56 = 28356,     /* GDA 1994 MGA Zone 56                */
PCS_GDA_1994_MGA_57 = 28357,     /* GDA 1994 MGA Zone 57                */
PCS_GDA_1994_MGA_58 = 28358,     /* GDA 1994 MGA Zone 58                */
//}}

//{{
PCS_NAD_1927_AL_E = 26729,     /* NAD 1927 SPCS Zone Alabama East      */
PCS_NAD_1927_AL_W = 26730,     /* NAD 1927 SPCS Zone Alabama West      */
PCS_NAD_1927_AK_1 = 26731,     /* NAD 1927 SPCS Zone Alaska 1          */
PCS_NAD_1927_AK_2 = 26732,     /* NAD 1927 SPCS Zone Alaska 2          */
PCS_NAD_1927_AK_3 = 26733,     /* NAD 1927 SPCS Zone Alaska 3          */
PCS_NAD_1927_AK_4 = 26734,     /* NAD 1927 SPCS Zone Alaska 4          */
PCS_NAD_1927_AK_5 = 26735,     /* NAD 1927 SPCS Zone Alaska 5          */
PCS_NAD_1927_AK_6 = 26736,     /* NAD 1927 SPCS Zone Alaska 6          */
PCS_NAD_1927_AK_7 = 26737,     /* NAD 1927 SPCS Zone Alaska 7          */
PCS_NAD_1927_AK_8 = 26738,     /* NAD 1927 SPCS Zone Alaska 8          */
PCS_NAD_1927_AK_9 = 26739,     /* NAD 1927 SPCS Zone Alaska 9          */
PCS_NAD_1927_AK_10 = 26740,     /* NAD 1927 SPCS Zone Alaska 10         */
PCS_NAD_1927_AZ_E = 26748,     /* NAD 1927 SPCS Zone Arizona East      */
PCS_NAD_1927_AZ_C = 26749,     /* NAD 1927 SPCS Zone Arizona Central   */
PCS_NAD_1927_AZ_W = 26750,     /* NAD 1927 SPCS Zone Arizona West      */
PCS_NAD_1927_AR_N = 26751,     /* NAD 1927 SPCS Zone Arkansas North    */
PCS_NAD_1927_AR_S = 26752,     /* NAD 1927 SPCS Zone Arkansas South    */
PCS_NAD_1927_CA_I = 26741,     /* NAD 1927 SPCS Zone California I      */
PCS_NAD_1927_CA_II = 26742,     /* NAD 1927 SPCS Zone California II     */
PCS_NAD_1927_CA_III = 26743,     /* NAD 1927 SPCS Zone California II     */
PCS_NAD_1927_CA_IV = 26744,     /* NAD 1927 SPCS Zone California IV     */
PCS_NAD_1927_CA_V = 26745,     /* NAD 1927 SPCS Zone California V      */
PCS_NAD_1927_CA_VI = 26746,     /* NAD 1927 SPCS Zone California VI     */
PCS_NAD_1927_CA_VII = 26747,     /* NAD 1927 SPCS Zone California VII    */
PCS_NAD_1927_CO_N = 26753,     /* NAD 1927 SPCS Zone Colorado North    */
PCS_NAD_1927_CO_C = 26754,     /* NAD 1927 SPCS Zone Colorado Central  */
PCS_NAD_1927_CO_S = 26755,     /* NAD 1927 SPCS Zone Colorado South    */
PCS_NAD_1927_CT = 26756,     /* NAD 1927 SPCS Zone Connecticut       */
PCS_NAD_1927_DE = 26757,     /* NAD 1927 SPCS Zone Delaware          */
PCS_NAD_1927_FL_E = 26758,     /* NAD 1927 SPCS Zone Florida East      */
PCS_NAD_1927_FL_W = 26759,     /* NAD 1927 SPCS Zone Florida West      */
PCS_NAD_1927_FL_N = 26760,     /* NAD 1927 SPCS Zone Florida North     */
PCS_NAD_1927_GA_E = 26766,     /* NAD 1927 SPCS Zone Georgia East      */
PCS_NAD_1927_GA_W = 26767,     /* NAD 1927 SPCS Zone Georgia West      */
PCS_NAD_1927_HI_1 = 26761,     /* NAD 1927 SPCS Zone Hawaii 1          */
PCS_NAD_1927_HI_2 = 26762,     /* NAD 1927 SPCS Zone Hawaii 2          */
PCS_NAD_1927_HI_3 = 26763,     /* NAD 1927 SPCS Zone Hawaii 3          */
PCS_NAD_1927_HI_4 = 26764,     /* NAD 1927 SPCS Zone Hawaii 4          */
PCS_NAD_1927_HI_5 = 26765,     /* NAD 1927 SPCS Zone Hawaii 5          */
PCS_NAD_1927_ID_E = 26768,     /* NAD 1927 SPCS Zone Idaho East        */
PCS_NAD_1927_ID_C = 26769,     /* NAD 1927 SPCS Zone Idaho Central     */
PCS_NAD_1927_ID_W = 26770,     /* NAD 1927 SPCS Zone Idaho West        */
PCS_NAD_1927_IL_E = 26771,     /* NAD 1927 SPCS Zone Illinois East     */
PCS_NAD_1927_IL_W = 26772,     /* NAD 1927 SPCS Zone Illinois West     */
PCS_NAD_1927_IN_E = 26773,     /* NAD 1927 SPCS Zone Indiana East      */
PCS_NAD_1927_IN_W = 26774,     /* NAD 1927 SPCS Zone Indiana West      */
PCS_NAD_1927_IA_N = 26775,     /* NAD 1927 SPCS Zone Iowa North        */
PCS_NAD_1927_IA_S = 26776,     /* NAD 1927 SPCS Zone Iowa South        */
PCS_NAD_1927_KS_N = 26777,     /* NAD 1927 SPCS Zone Kansas North      */
PCS_NAD_1927_KS_S = 26778,     /* NAD 1927 SPCS Zone Kansas South      */
PCS_NAD_1927_KY_N = 26779,     /* NAD 1927 SPCS Zone Kentucky North    */
PCS_NAD_1927_KY_S = 26780,     /* NAD 1927 SPCS Zone Kentucky South    */
PCS_NAD_1927_LA_N = 26781,     /* NAD 1927 SPCS Zone Louisiana North   */
PCS_NAD_1927_LA_S = 26782,     /* NAD 1927 SPCS Zone Louisiana South   */
PCS_NAD_1927_ME_E = 26783,     /* NAD 1927 SPCS Zone Maine East        */
PCS_NAD_1927_ME_W = 26784,     /* NAD 1927 SPCS Zone Maine West        */
PCS_NAD_1927_MD = 26785,     /* NAD 1927 SPCS Zone Maryland          */
PCS_NAD_1927_MA_M = 26786,     /* NAD 1927 SPCS Zone Mass. Mainland    */
PCS_NAD_1927_MA_I = 26787,     /* NAD 1927 SPCS Zone Mass. Island      */
PCS_NAD_1927_MI_N = 26788,     /* NAD 1927 SPCS Zone Michigan North    */
PCS_NAD_1927_MI_C = 26789,     /* NAD 1927 SPCS Zone Michigan Central  */
PCS_NAD_1927_MI_S = 26790,     /* NAD 1927 SPCS Zone Michigan South    */
PCS_NAD_1927_MN_N = 26791,     /* NAD 1927 SPCS Zone Minnesota North   */
PCS_NAD_1927_MN_C = 26792,     /* NAD 1927 SPCS Zone Minnesota Central */
PCS_NAD_1927_MN_S = 26793,     /* NAD 1927 SPCS Zone Minnesota South   */
PCS_NAD_1927_MS_E = 26794,     /* NAD 1927 SPCS Zone Mississippi East  */
PCS_NAD_1927_MS_W = 26795,     /* NAD 1927 SPCS Zone Mississippi West  */
PCS_NAD_1927_MO_E = 26796,     /* NAD 1927 SPCS Zone Missouri East     */
PCS_NAD_1927_MO_C = 26797,     /* NAD 1927 SPCS Zone Missouri Central  */
PCS_NAD_1927_MO_W = 26798,     /* NAD 1927 SPCS Zone Missouri West     */
//}}
//{{ 支持荷兰投影RD_NEW
PCS_AMERSFOORT_RD_NEW = 28992,	 /*	Amersfoort_RD_New				 */
//}}
//{{
PCS_NAD_1927_MT_N = 32001,     /* NAD 1927 SPCS Zone Montana North     */
PCS_NAD_1927_MT_C = 32002,     /* NAD 1927 SPCS Zone Montana Central   */
PCS_NAD_1927_MT_S = 32003,     /* NAD 1927 SPCS Zone Montana South     */
PCS_NAD_1927_NE_N = 32005,     /* NAD 1927 SPCS Zone Nebraska North    */
PCS_NAD_1927_NE_S = 32006,     /* NAD 1927 SPCS Zone Nebraska South    */
PCS_NAD_1927_NV_E = 32007,     /* NAD 1927 SPCS Zone Nevada East       */
PCS_NAD_1927_NV_C = 32008,     /* NAD 1927 SPCS Zone Nevada Central    */
PCS_NAD_1927_NV_W = 32009,     /* NAD 1927 SPCS Zone Nevada West       */
PCS_NAD_1927_NH = 32010,     /* NAD 1927 SPCS Zone New Hampshire     */
PCS_NAD_1927_NJ = 32011,     /* NAD 1927 SPCS Zone New Jersey        */
PCS_NAD_1927_NM_E = 32012,     /* NAD 1927 SPCS Zone New Mexico East   */
PCS_NAD_1927_NM_C = 32013,     /* NAD 1927 SPCS Zone New Mexico Cent.  */
PCS_NAD_1927_NM_W = 32014,     /* NAD 1927 SPCS Zone New Mexico West   */
PCS_NAD_1927_NY_E = 32015,     /* NAD 1927 SPCS Zone New York East     */
PCS_NAD_1927_NY_C = 32016,     /* NAD 1927 SPCS Zone New York Central  */
PCS_NAD_1927_NY_W = 32017,     /* NAD 1927 SPCS Zone New York West     */
PCS_NAD_1927_NY_LI = 32018,     /* NAD 1927 SPCS Zone NY Long Island    */
PCS_NAD_1927_NC = 32019,     /* NAD 1927 SPCS Zone North Carolina    */
PCS_NAD_1927_ND_N = 32020,     /* NAD 1927 SPCS Zone North Dakota N    */
PCS_NAD_1927_ND_S = 32021,     /* NAD 1927 SPCS Zone North Dakota S    */
PCS_NAD_1927_OH_N = 32022,     /* NAD 1927 SPCS Zone Ohio North        */
PCS_NAD_1927_OH_S = 32023,     /* NAD 1927 SPCS Zone Ohio South        */
PCS_NAD_1927_OK_N = 32024,     /* NAD 1927 SPCS Zone Oklahoma North    */
PCS_NAD_1927_OK_S = 32025,     /* NAD 1927 SPCS Zone Oklahoma South    */
PCS_NAD_1927_OR_N = 32026,     /* NAD 1927 SPCS Zone Oregon North      */
PCS_NAD_1927_OR_S = 32027,     /* NAD 1927 SPCS Zone Oregon South      */
PCS_NAD_1927_PA_N = 32028,     /* NAD 1927 SPCS Zone Pennsylvania N    */
PCS_NAD_1927_PA_S = 32029,     /* NAD 1927 SPCS Zone Pennsylvania S    */
PCS_NAD_1927_RI = 32030,     /* NAD 1927 SPCS Zone Rhode Island      */
PCS_NAD_1927_SC_N = 32031,     /* NAD 1927 SPCS Zone South Carolina N  */
PCS_NAD_1927_SC_S = 32033,     /* NAD 1927 SPCS Zone South Carolina S  */
PCS_NAD_1927_SD_N = 32034,     /* NAD 1927 SPCS Zone South Dakota N    */
PCS_NAD_1927_SD_S = 32035,     /* NAD 1927 SPCS Zone South Dakota S    */
PCS_NAD_1927_TN = 32036,     /* NAD 1927 SPCS Zone Tennessee         */
PCS_NAD_1927_TX_N = 32037,     /* NAD 1927 SPCS Zone Texas North       */
PCS_NAD_1927_TX_NC = 32038,     /* NAD 1927 SPCS Zone Texas North Cent. */
PCS_NAD_1927_TX_C = 32039,     /* NAD 1927 SPCS Zone Texas Central     */
PCS_NAD_1927_TX_SC = 32040,     /* NAD 1927 SPCS Zone Texas South Cent. */
PCS_NAD_1927_TX_S = 32041,     /* NAD 1927 SPCS Zone Texas South       */
PCS_NAD_1927_UT_N = 32042,     /* NAD 1927 SPCS Zone Utah North        */
PCS_NAD_1927_UT_C = 32043,     /* NAD 1927 SPCS Zone Utah Central      */
PCS_NAD_1927_UT_S = 32044,     /* NAD 1927 SPCS Zone Utah South        */
PCS_NAD_1927_VT = 32045,     /* NAD 1927 SPCS Zone Vermont           */
PCS_NAD_1927_VA_N = 32046,     /* NAD 1927 SPCS Zone Virginia North    */
PCS_NAD_1927_VA_S = 32047,     /* NAD 1927 SPCS Zone Virginia South    */
PCS_NAD_1927_WA_N = 32048,     /* NAD 1927 SPCS Zone Washington North  */
PCS_NAD_1927_WA_S = 32049,     /* NAD 1927 SPCS Zone Washington South  */
PCS_NAD_1927_WV_N = 32050,     /* NAD 1927 SPCS Zone West Virginia N   */
PCS_NAD_1927_WV_S = 32051,     /* NAD 1927 SPCS Zone West Virginia S   */
PCS_NAD_1927_WI_N = 32052,     /* NAD 1927 SPCS Zone Wisconsin North   */
PCS_NAD_1927_WI_C = 32053,     /* NAD 1927 SPCS Zone Wisconsin Central */
PCS_NAD_1927_WI_S = 32054,     /* NAD 1927 SPCS Zone Wisconsin South   */
PCS_NAD_1927_WY_E = 32055,     /* NAD 1927 SPCS Zone Wyoming I East    */
PCS_NAD_1927_WY_EC = 32056,     /* NAD 1927 SPCS Zone Wyoming II EC     */
PCS_NAD_1927_WY_WC = 32057,     /* NAD 1927 SPCS Zone Wyoming III WC    */
PCS_NAD_1927_WY_W = 32058,     /* NAD 1927 SPCS Zone Wyoming IV West   */
PCS_NAD_1927_PR = 32059,     /* NAD 1927 SPCS Zone Puerto Rico       */
PCS_NAD_1927_VI = 32060,     /* NAD 1927 SPCS Zone St. Croix         */
PCS_NAD_1927_GU = (32061 + 33000),     /* NAD 1927 SPCS Zone Guam      */
//}}

//{{
PCS_NAD_1983_AL_E = 26929,     /* NAD 1983 SPCS Zone Alabama East      */
PCS_NAD_1983_AL_W = 26930,     /* NAD 1983 SPCS Zone Alabama West      */
PCS_NAD_1983_AK_1 = 26931,     /* NAD 1983 SPCS Zone Alaska 1          */
PCS_NAD_1983_AK_2 = 26932,     /* NAD 1983 SPCS Zone Alaska 2          */
PCS_NAD_1983_AK_3 = 26933,     /* NAD 1983 SPCS Zone Alaska 3          */
PCS_NAD_1983_AK_4 = 26934,     /* NAD 1983 SPCS Zone Alaska 4          */
PCS_NAD_1983_AK_5 = 26935,     /* NAD 1983 SPCS Zone Alaska 5          */
PCS_NAD_1983_AK_6 = 26936,     /* NAD 1983 SPCS Zone Alaska 6          */
PCS_NAD_1983_AK_7 = 26937,     /* NAD 1983 SPCS Zone Alaska 7          */
PCS_NAD_1983_AK_8 = 26938,     /* NAD 1983 SPCS Zone Alaska 8          */
PCS_NAD_1983_AK_9 = 26939,     /* NAD 1983 SPCS Zone Alaska 9          */
PCS_NAD_1983_AK_10 = 26940,     /* NAD 1983 SPCS Zone Alaska 10         */
PCS_NAD_1983_AZ_E = 26948,     /* NAD 1983 SPCS Zone Arizona East      */
PCS_NAD_1983_AZ_C = 26949,     /* NAD 1983 SPCS Zone Arizona Central   */
PCS_NAD_1983_AZ_W = 26950,     /* NAD 1983 SPCS Zone Arizona West      */
PCS_NAD_1983_AR_N = 26951,     /* NAD 1983 SPCS Zone Arkansas North    */
PCS_NAD_1983_AR_S = 26952,     /* NAD 1983 SPCS Zone Arkansas South    */
PCS_NAD_1983_CA_I = 26941,     /* NAD 1983 SPCS Zone California I      */
PCS_NAD_1983_CA_II = 26942,     /* NAD 1983 SPCS Zone California II     */
PCS_NAD_1983_CA_III = 26943,     /* NAD 1983 SPCS Zone California III    */
PCS_NAD_1983_CA_IV = 26944,     /* NAD 1983 SPCS Zone California IV     */
PCS_NAD_1983_CA_V = 26945,     /* NAD 1983 SPCS Zone California V      */
PCS_NAD_1983_CA_VI = 26946,     /* NAD 1983 SPCS Zone California VI     */
PCS_NAD_1983_CO_N = 26953,     /* NAD 1983 SPCS Zone Colorado North    */
PCS_NAD_1983_CO_C = 26954,     /* NAD 1983 SPCS Zone Colorado Central  */
PCS_NAD_1983_CO_S = 26955,     /* NAD 1983 SPCS Zone Colorado South    */
PCS_NAD_1983_CT = 26956,     /* NAD 1983 SPCS Zone Connecticut       */
PCS_NAD_1983_DE = 26957,     /* NAD 1983 SPCS Zone Delaware          */
PCS_NAD_1983_FL_E = 26958,     /* NAD 1983 SPCS Zone Florida East      */
PCS_NAD_1983_FL_W = 26959,     /* NAD 1983 SPCS Zone Florida West      */
PCS_NAD_1983_FL_N = 26960,     /* NAD 1983 SPCS Zone Florida North     */
PCS_NAD_1983_GA_E = 26966,     /* NAD 1983 SPCS Zone Georgia East      */
PCS_NAD_1983_GA_W = 26967,     /* NAD 1983 SPCS Zone Georgia West      */
PCS_NAD_1983_HI_1 = 26961,     /* NAD 1983 SPCS Zone Hawaii Zone 1     */
PCS_NAD_1983_HI_2 = 26962,     /* NAD 1983 SPCS Zone Hawaii Zone 2     */
PCS_NAD_1983_HI_3 = 26963,     /* NAD 1983 SPCS Zone Hawaii Zone 3     */
PCS_NAD_1983_HI_4 = 26964,     /* NAD 1983 SPCS Zone Hawaii Zone 4     */
PCS_NAD_1983_HI_5 = 26965,     /* NAD 1983 SPCS Zone Hawaii Zone 5     */
PCS_NAD_1983_ID_E = 26968,     /* NAD 1983 SPCS Zone Idaho East        */
PCS_NAD_1983_ID_C = 26969,     /* NAD 1983 SPCS Zone Idaho Central     */
PCS_NAD_1983_ID_W = 26970,     /* NAD 1983 SPCS Zone Idaho West        */
PCS_NAD_1983_IL_E = 26971,     /* NAD 1983 SPCS Zone Illinois East     */
PCS_NAD_1983_IL_W = 26972,     /* NAD 1983 SPCS Zone Illinois West     */
PCS_NAD_1983_IN_E = 26973,     /* NAD 1983 SPCS Zone Indiana East      */
PCS_NAD_1983_IN_W = 26974,     /* NAD 1983 SPCS Zone Indiana West      */
PCS_NAD_1983_IA_N = 26975,     /* NAD 1983 SPCS Zone Iowa North        */
PCS_NAD_1983_IA_S = 26976,     /* NAD 1983 SPCS Zone Iowa South        */
PCS_NAD_1983_KS_N = 26977,     /* NAD 1983 SPCS Zone Kansas North      */
PCS_NAD_1983_KS_S = 26978,     /* NAD 1983 SPCS Zone Kansas South      */
PCS_NAD_1983_KY_N = 26979,     /* NAD 1983 SPCS Zone Kentucky North    */
PCS_NAD_1983_KY_S = 26980,     /* NAD 1983 SPCS Zone Kentucky South    */
PCS_NAD_1983_LA_N = 26981,     /* NAD 1983 SPCS Zone Louisiana North   */
PCS_NAD_1983_LA_S = 26982,     /* NAD 1983 SPCS Zone Louisiana South   */
PCS_NAD_1983_ME_E = 26983,     /* NAD 1983 SPCS Zone Maine East        */
PCS_NAD_1983_ME_W = 26984,     /* NAD 1983 SPCS Zone Maine West        */
PCS_NAD_1983_MD = 26985,     /* NAD 1983 SPCS Zone Maryland          */
PCS_NAD_1983_MA_M = 26986,     /* NAD 1983 SPCS Zone Mass. Mainland    */
PCS_NAD_1983_MA_I = 26987,     /* NAD 1983 SPCS Zone Mass. Island      */
PCS_NAD_1983_MI_N = 26988,     /* NAD 1983 SPCS Zone Michigan North    */
PCS_NAD_1983_MI_C = 26989,     /* NAD 1983 SPCS Zone Michigan Central  */
PCS_NAD_1983_MI_S = 26990,     /* NAD 1983 SPCS Zone Michigan South    */
PCS_NAD_1983_MN_N = 26991,     /* NAD 1983 SPCS Zone Minnesota North   */
PCS_NAD_1983_MN_C = 26992,     /* NAD 1983 SPCS Zone Minnesota Central */
PCS_NAD_1983_MN_S = 26993,     /* NAD 1983 SPCS Zone Minnesota South   */
PCS_NAD_1983_MS_E = 26994,     /* NAD 1983 SPCS Zone Mississippi East  */
PCS_NAD_1983_MS_W = 26995,     /* NAD 1983 SPCS Zone Mississippi West  */
PCS_NAD_1983_MO_E = 26996,     /* NAD 1983 SPCS Zone Missouri East     */
PCS_NAD_1983_MO_C = 26997,     /* NAD 1983 SPCS Zone Missouri Central  */
PCS_NAD_1983_MO_W = 26998,     /* NAD 1983 SPCS Zone Missouri West     */
//}}

//{{
PCS_NAD_1983_MT = 32100,     /* NAD 1983 SPCS Zone Montana           */
PCS_NAD_1983_NE = 32104,     /* NAD 1983 SPCS Zone Nebraska          */
PCS_NAD_1983_NV_E = 32107,     /* NAD 1983 SPCS Zone Nevada East       */
PCS_NAD_1983_NV_C = 32108,     /* NAD 1983 SPCS Zone Nevada Central    */
PCS_NAD_1983_NV_W = 32109,     /* NAD 1983 SPCS Zone Nevada West       */
PCS_NAD_1983_NH = 32110,     /* NAD 1983 SPCS Zone New Hampshire     */
PCS_NAD_1983_NJ = 32111,     /* NAD 1983 SPCS Zone New Jersey        */
PCS_NAD_1983_NM_E = 32112,     /* NAD 1983 SPCS Zone New Mexico East   */
PCS_NAD_1983_NM_C = 32113,     /* NAD 1983 SPCS Zone New Mexico Cent.  */
PCS_NAD_1983_NM_W = 32114,     /* NAD 1983 SPCS Zone New Mexico West   */
PCS_NAD_1983_NY_E = 32115,     /* NAD 1983 SPCS Zone New York East     */
PCS_NAD_1983_NY_C = 32116,     /* NAD 1983 SPCS Zone New York Central  */
PCS_NAD_1983_NY_W = 32117,     /* NAD 1983 SPCS Zone New York West     */
PCS_NAD_1983_NY_LI = 32118,     /* NAD 1983 SPCS Zone NY Long Island    */
PCS_NAD_1983_NC = 32119,     /* NAD 1983 SPCS Zone North Carolina    */
PCS_NAD_1983_ND_N = 32120,     /* NAD 1983 SPCS Zone North Dakota N    */
PCS_NAD_1983_ND_S = 32121,     /* NAD 1983 SPCS Zone North Dakota S    */
PCS_NAD_1983_OH_N = 32122,     /* NAD 1983 SPCS Zone Ohio North        */
PCS_NAD_1983_OH_S = 32123,     /* NAD 1983 SPCS Zone Ohio South        */
PCS_NAD_1983_OK_N = 32124,     /* NAD 1983 SPCS Zone Oklahoma North    */
PCS_NAD_1983_OK_S = 32125,     /* NAD 1983 SPCS Zone Oklahoma South    */
PCS_NAD_1983_OR_N = 32126,     /* NAD 1983 SPCS Zone Oregon North      */
PCS_NAD_1983_OR_S = 32127,     /* NAD 1983 SPCS Zone Oregon South      */
PCS_NAD_1983_PA_N = 32128,     /* NAD 1983 SPCS Zone Pennsylvania N    */
PCS_NAD_1983_PA_S = 32129,     /* NAD 1983 SPCS Zone Pennsylvania S    */
PCS_NAD_1983_RI = 32130,     /* NAD 1983 SPCS Zone Rhode Island      */
PCS_NAD_1983_SC = 32133,     /* NAD 1983 SPCS Zone South Carolina    */
PCS_NAD_1983_SD_N = 32134,     /* NAD 1983 SPCS Zone South Dakota N    */
PCS_NAD_1983_SD_S = 32135,     /* NAD 1983 SPCS Zone South Dakota S    */
PCS_NAD_1983_TN = 32136,     /* NAD 1983 SPCS Zone Tennessee         */
PCS_NAD_1983_TX_N = 32137,     /* NAD 1983 SPCS Zone Texas North       */
PCS_NAD_1983_TX_NC = 32138,     /* NAD 1983 SPCS Zone Texas North Cent. */
PCS_NAD_1983_TX_C = 32139,     /* NAD 1983 SPCS Zone Texas Central     */
PCS_NAD_1983_TX_SC = 32140,     /* NAD 1983 SPCS Zone Texas South Cent. */
PCS_NAD_1983_TX_S = 32141,     /* NAD 1983 SPCS Zone Texas South       */
PCS_NAD_1983_UT_N = 32142,     /* NAD 1983 SPCS Zone Utah North        */
PCS_NAD_1983_UT_C = 32143,     /* NAD 1983 SPCS Zone Utah Central      */
PCS_NAD_1983_UT_S = 32144,     /* NAD 1983 SPCS Zone Utah South        */
PCS_NAD_1983_VT = 32145,     /* NAD 1983 SPCS Zone Vermont           */
PCS_NAD_1983_VA_N = 32146,     /* NAD 1983 SPCS Zone Virginia North    */
PCS_NAD_1983_VA_S = 32147,     /* NAD 1983 SPCS Zone Virginia South    */
PCS_NAD_1983_WA_N = 32148,     /* NAD 1983 SPCS Zone Washington North  */
PCS_NAD_1983_WA_S = 32149,     /* NAD 1983 SPCS Zone Washington South  */
PCS_NAD_1983_WV_N = 32150,     /* NAD 1983 SPCS Zone West Virginia N   */
PCS_NAD_1983_WV_S = 32151,     /* NAD 1983 SPCS Zone West Virginia S   */
PCS_NAD_1983_WI_N = 32152,     /* NAD 1983 SPCS Zone Wisconsin North   */
PCS_NAD_1983_WI_C = 32153,     /* NAD 1983 SPCS Zone Wisconsin Central */
PCS_NAD_1983_WI_S = 32154,     /* NAD 1983 SPCS Zone Wisconsin South   */
PCS_NAD_1983_WY_E = 32155,     /* NAD 1983 SPCS Zone Wyoming I East    */
PCS_NAD_1983_WY_EC = 32156,     /* NAD 1983 SPCS Zone Wyoming II EC     */
PCS_NAD_1983_WY_WC = 32157,     /* NAD 1983 SPCS Zone Wyoming III WC    */
PCS_NAD_1983_WY_W = 32158,     /* NAD 1983 SPCS Zone Wyoming IV West   */
PCS_NAD_1983_PR_VI = 32161,     /* NAD 1983 SPCS Zone PR & St. Croix    */
PCS_NAD_1983_GU = (32161 + 33000),     /* NAD 1983 SPCS Zone Guam      */
//}}

//{{
PCS_ADINDAN_UTM_37N = 20137,     /* Adindan UTM Zone 37N          */
PCS_ADINDAN_UTM_38N = 20138,     /* Adindan UTM Zone 38N          */
PCS_AFGOOYE_UTM_38N = 20538,     /* Afgooye UTM Zone 38N          */
PCS_AFGOOYE_UTM_39N = 20539,     /* Afgooye UTM Zone 39N          */
PCS_AIN_EL_ABD_UTM_37N = 20437,     /* Ain el Abd 1970 UTM Zone 37N  */
PCS_AIN_EL_ABD_UTM_38N = 20438,     /* Ain el Abd 1970 UTM Zone 38N  */
PCS_AIN_EL_ABD_UTM_39N = 20439,     /* Ain el Abd 1970 UTM Zone 39N  */
PCS_ARATU_UTM_22S = 20822,     /* Aratu UTM Zone 22S            */
PCS_ARATU_UTM_23S = 20823,     /* Aratu UTM Zone=  23S            */
PCS_ARATU_UTM_24S = 20824,     /* Aratu UTM Zone 24S            */
PCS_BATAVIA_UTM_48S = 21148,     /* Batavia UTM Zone 48S          */
PCS_BATAVIA_UTM_49S = 21149,     /* Batavia UTM Zone 49S          */
PCS_BATAVIA_UTM_50S = 21150,     /* Batavia UTM Zone 50S          */
PCS_BOGOTA_UTM_17N = 21817,     /* Bogota UTM Zone 17N           */
PCS_BOGOTA_UTM_18N = 21818,     /* Bogota UTM Zone 18N           */
PCS_CAMACUPA_UTM_32S = 22032,     /* Camacupa UTM Zone= 32S         */
PCS_CAMACUPA_UTM_33S = 22033,     /* Camacupa UTM Zone 33S         */
PCS_CARTHAGE_UTM_32N = 22332,     /* Carthage UTM Zone= 32N         */
PCS_CORREGO_ALEGRE_UTM_23S = 22523,     /* Corrego Alegre UTM Zone=  23S   */
PCS_CORREGO_ALEGRE_UTM_24S = 22524,     /* Corrego Alegre UTM Zone 24S   */
PCS_DATUM_73_UTM_ZONE_29N = 27429,     /* Datum 73 UTM Zone=  29N         */
PCS_DOUALA_UTM_32N = 22832,     /* Douala UTM Zone= 32N           */
PCS_FAHUD_UTM_39N = 23239,     /* Fahud UTM Zone 39N            */
PCS_FAHUD_UTM_40N = 23240,     /* Fahud UTM Zone 40N            */
PCS_GAROUA_UTM_33N = 23433,     /* Garoua UTM Zone 33N           */
PCS_GGRS_1987_GREEK_GRID = 2100,     /* Greek Grid                    */
PCS_ID_1974_UTM_46N = 23846,     /* Indonesia 1974 UTM Zone 46N   */
PCS_ID_1974_UTM_47N = 23847,     /* Indonesia 1974 UTM Zone 47N   */
PCS_ID_1974_UTM_48N = 23848,     /* Indonesia 1974 UTM Zone 48N   */
PCS_ID_1974_UTM_49N = 23849,     /* Indonesia 1974 UTM Zone 49N   */
PCS_ID_1974_UTM_50N = 23850,     /* Indonesia 1974 UTM Zone 50N   */
PCS_ID_1974_UTM_51N = 23851,     /* Indonesia 1974 UTM Zone 51N   */
PCS_ID_1974_UTM_52N = 23852,     /* Indonesia 1974 UTM Zone 52N   */
PCS_ID_1974_UTM_53N = 23853,     /* Indonesia 1974 UTM Zone 53N   */
PCS_ID_1974_UTM_46S = 23886,     /* Indonesia 1974 UTM Zone 46S   */
PCS_ID_1974_UTM_47S = 23887,     /* Indonesia 1974 UTM Zone 47S   */
PCS_ID_1974_UTM_48S = 23888,     /* Indonesia 1974 UTM Zone 48S   */
PCS_ID_1974_UTM_49S = 23889,     /* Indonesia 1974 UTM Zone 49S   */
PCS_ID_1974_UTM_50S = 23890,     /* Indonesia 1974 UTM Zone 50S   */
PCS_ID_1974_UTM_51S = 23891,     /* Indonesia 1974 UTM Zone 51S   */
PCS_ID_1974_UTM_52S = 23892,     /* Indonesia 1974 UTM Zone 52S   */
PCS_ID_1974_UTM_53S = 23893,     /* Indonesia 1974 UTM Zone 53S   */
PCS_ID_1974_UTM_54S = 23894,     /* Indonesia 1974 UTM Zone 54S   */
PCS_INDIAN_1954_UTM_47N = 23947,     /* Indian 1954 UTM Zone 47N      */
PCS_INDIAN_1954_UTM_48N = 23948,     /* Indian 1954 UTM Zone 48N      */
PCS_INDIAN_1975_UTM_47N = 24047,     /* Indian 1975 UTM Zone 47N      */
PCS_INDIAN_1975_UTM_48N = 24048,     /* Indian 1975 UTM Zone 48N      */
PCS_KERTAU_UTM_47N = 24547,     /* Kertau UTM Zone 47N           */
PCS_KERTAU_UTM_48N = 24548,     /* Kertau UTM Zone 48N           */
PCS_LA_CANOA_UTM_20N = 24720,     /* La Canoa UTM Zone=  20N         */
PCS_LA_CANOA_UTM_21N = 24721,     /* La Canoa UTM Zone 21N         */
PCS_LOME_UTM_31N = 25231,     /* Lome UTM Zone 31N             */
PCS_MPORALOKO_UTM_32N = 26632,     /* M'poraloko UTM Zone= 32N       */
PCS_MPORALOKO_UTM_32S = 26692,     /* M'poraloko UTM Zone= 32S       */
PCS_MALONGO_1987_UTM_32S = 25932,     /* Malongo 1987 UTM Zone= 32S     */
PCS_MASSAWA_UTM_37N = 26237,     /* Massawa UTM Zone 37N          */
PCS_MHAST_UTM_32S = 26432,     /* Mhast UTM Zone= 32S            */
PCS_MINNA_UTM_31N = 26331,     /* Minna UTM Zone 31N            */
PCS_MINNA_UTM_32N = 26332,     /* Minna UTM Zone 32N            */
PCS_NAHRWAN_1967_UTM_38N = 27038,     /* Nahrwan 1967 UTM Zone 38N     */
PCS_NAHRWAN_1967_UTM_39N = 27039,     /* Nahrwan 1967 UTM Zone 39N     */
PCS_NAHRWAN_1967_UTM_40N = 27040,     /* Nahrwan 1967 UTM Zone 40N     */
PCS_NGN_UTM_38N = 31838,     /* NGN UTM Zone 38N              */
PCS_NGN_UTM_39N = 31839,     /* NGN UTM Zone 39N              */
PCS_NORD_SAHARA_UTM_29N = 30729,     /* Nord Sahara 1959 UTM Zone=  29N */
PCS_NORD_SAHARA_UTM_30N = 30730,     /* Nord Sahara 1959 UTM Zone 30N */
PCS_NORD_SAHARA_UTM_31N = 30731,     /* Nord Sahara 1959 UTM Zone 31N */
PCS_NORD_SAHARA_UTM_32N = 30732,     /* Nord Sahara 1959 UTM Zone 32N */
PCS_NAPARIMA_1972_UTM_20N = 27120,     /* Naparima 1972 UTM Zone 20N    */
PCS_POINTE_NOIRE_UTM_32S = 28232,     /* Pointe Noire UTM Zone 32S     */

PCS_PSAD_1956_UTM_18N = 24818,     /* Prov. S. Amer. Datum UTM Zone 18N */
PCS_PSAD_1956_UTM_19N = 24819,     /* Prov. S. Amer. Datum UTM Zone 19N */
PCS_PSAD_1956_UTM_20N = 24820,     /* Prov. S. Amer. Datum UTM Zone 20N */
PCS_PSAD_1956_UTM_21N = 24821,     /* Prov. S. Amer. Datum UTM Zone 21N */
PCS_PSAD_1956_UTM_17S = 24877,     /* Prov. S. Amer. Datum UTM Zone 17S */
PCS_PSAD_1956_UTM_18S = 24878,     /* Prov. S. Amer. Datum UTM Zone 18S */
PCS_PSAD_1956_UTM_19S = 24879,     /* Prov. S. Amer. Datum UTM Zone 19S */
PCS_PSAD_1956_UTM_20S = 24880,     /* Prov. S. Amer. Datum UTM Zone 20S */

PCS_SAPPER_HILL_UTM_20S = 29220,     /* Sapper Hill 1943 UTM Zone 20S */
PCS_SAPPER_HILL_UTM_21S = 29221,     /* Sapper Hill 1943 UTM Zone 21S */
PCS_SCHWARZECK_UTM_33S = 29333,     /* Schwarzeck UTM Zone 33S       */
PCS_SUDAN_UTM_35N = 29635,     /* Sudan UTM Zone 35N            */
PCS_SUDAN_UTM_36N = 29636,     /* Sudan UTM Zone 36N            */
PCS_TANANARIVE_UTM_38S = 29738,     /* Tananarive 1925 UTM Zone 38S  */
PCS_TANANARIVE_UTM_39S = 29739,     /* Tananarive 1925 UTM Zone 39S  */
PCS_TC_1948_UTM_39N = 30339,     /* Trucial Coast 1948 UTM Zone 39N */
PCS_TC_1948_UTM_40N = 30340,     /* Trucial Coast 1948 UTM Zone 40N */
PCS_TIMBALAI_1948_UTM_49N = 29849,     /* Timbalai 1948 UTM Zone 49N    */
PCS_TIMBALAI_1948_UTM_50N = 29850,     /* Timbalai 1948 UTM Zone 50N    */
PCS_YOFF_1972_UTM_28N = 31028,     /* Yoff 1972 UTM Zone=  28N        */
PCS_ZANDERIJ_1972_UTM_21N = 31121,     /* Zanderij 1972 UTM Zone 21N    */
//}}

//{{
PCS_KUDAMS_KTM = 31900,     /* Kuwait Utility KTM         */

PCS_LUZON_PHILIPPINES_I = 25391,     /* Philippines Zone I         */
PCS_LUZON_PHILIPPINES_II = 25392,     /* Philippines Zone II        */
PCS_LUZON_PHILIPPINES_III = 25393,     /* Philippines Zone III       */
PCS_LUZON_PHILIPPINES_IV = 25394,     /* Philippines Zone IV        */
PCS_LUZON_PHILIPPINES_V = 25395,     /* Philippines Zone V         */

PCS_MGI_FERRO_AUSTRIA_WEST = 31291,     /* Austria (Ferro) West Zone  */
PCS_MGI_FERRO_AUSTRIA_CENTRAL = 31292,     /* Austria (Ferro) Cent. Zone */
PCS_MGI_FERRO_AUSTRIA_EAST = 31293,     /* Austria (Ferro) East Zone  */
PCS_MONTE_MARIO_ROME_ITALY_1 = 26591,     /* Monte Mario (Rome) Italy 1 */
PCS_MONTE_MARIO_ROME_ITALY_2 = 26592,     /* Monte Mario (Rome) Italy 2 */

PCS_C_INCHAUSARGENTINA_1 = 22191,     /* Argentina Zone 1           */
PCS_C_INCHAUSARGENTINA_2 = 22192,     /* Argentina Zone 2           */
PCS_C_INCHAUSARGENTINA_3 = 22193,     /* Argentina Zone 3           */
PCS_C_INCHAUSARGENTINA_4 = 22194,     /* Argentina Zone 4           */
PCS_C_INCHAUSARGENTINA_5 = 22195,     /* Argentina Zone 5           */
PCS_C_INCHAUSARGENTINA_6 = 22196,     /* Argentina Zone 6           */
PCS_C_INCHAUSARGENTINA_7 = 22197,     /* Argentina Zone 7           */

PCS_DHDN_GERMANY_1 = 31491,     /* Germany Zone 1             */
PCS_DHDN_GERMANY_2 = 31492,     /* Germany Zone 2             */
PCS_DHDN_GERMANY_3 = 31493,     /* Germany Zone 3             */
PCS_DHDN_GERMANY_4 = 31494,     /* Germany Zone 4             */
PCS_DHDN_GERMANY_5 = 31495,     /* Germany Zone 5             */

PCS_AIN_EL_ABD_BAHRAIN_GRID = 20499,     /* Bahrain State Grid         */

PCS_BOGOTA_COLOMBIA_WEST = 21891,     /* Colombia West Zone         */
PCS_BOGOTA_COLOMBIA_BOGOTA = 21892,     /* Colombia Bogota Zone       */
PCS_BOGOTA_COLOMBIA_E_CENTRAL = 21893,     /* Colombia E Central Zone    */
PCS_BOGOTA_COLOMBIA_EAST = 21894,     /* Colombia East Zone         */

PCS_EGYPT_RED_BELT = 22992,     /* Egypt Red Belt             */
PCS_EGYPT_PURPLE_BELT = 22993,     /* Egypt Purple Belt          */
PCS_EGYPT_EXT_PURPLE_BELT = 22994,     /* Egypt Extended Purple Belt */

PCS_LEIGON_GHANA_GRID = 25000,     /* Ghana Metre Grid           */

PCS_TM65_IRISH_GRID = 29900,     /* Irish National Grid        */

PCS_NZGD_1949_NORTH_ISLAND = 27291,     /* New Zealand North Island   */
PCS_NZGD_1949_SOUTH_ISLAND = 27292,     /* New Zealand South Island   */

PCS_MINNA_NIGERIA_WEST_BELT = 26391,     /* Nigeria West Belt          */
PCS_MINNA_NIGERIA_MID_BELT = 26392,     /* Nigeria Mid Belt           */
PCS_MINNA_NIGERIA_EAST_BELT = 26393,     /* Nigeria East Belt          */

PCS_PSAD_1956_PERU_WEST = 24891,     /* Peru West Zone             */
PCS_PSAD_1956_PERU_CENTRAL = 24892,     /* Peru Central Zone          */
PCS_PSAD_1956_PERU_EAST = 24893,     /* Peru East Zone             */

PCS_LISBON_PORTUGUESE_GRID = 20700,     /* Portuguese National Grid   */

//{{{

PCS_LISBON_PORTUGUESE_OFFICIAL_GRID = 20791,     /* Portuguese Grid   */
PCS_DATUM73_MODIFIED_PORTUGUESE_NATIONAL_GRID = 27492,     /* Modified Portuguese National Grid   */
PCS_DATUM73_MODIFIED_PORTUGUESE_GRID = 27493,     /* Modified Portuguese Grid   */

PCS_Lisboa_Hayford_Gauss_IGeoE = (20700 + 33000),	/* Lisboa Hayford Gauss IGeoE	*/
PCS_Lisboa_Hayford_Gauss_IPCC = (20791 + 33000),	/* Lisboa Hayford Gauss IPCC	*/

//}}


PCS_QATAR_GRID = 28600,     /* Qatar National Grid        */

PCS_OSGB_1936_BRITISH_GRID = 27700,     /* British National Grid      */
PCS_RT38_STOCKHOLM_SWEDISH_GRID = 30800,     /* Swedish National Grid    */
//}}

//{{
PCS_VOIROL_N_ALGERIE_ANCIENNE = 30491,     /* Nord Algerie ancienne      */
PCS_VOIROL_S_ALGERIE_ANCIENNE = 30492,     /* Nord Algerie ancienne      */
PCS_VOIROL_UNIFIE_N_ALGERIE = 30591,     /* Nord Algerie               */
PCS_VOIROL_UNIFIE_S_ALGERIE = 30592,     /* Nord Algerie               */
PCS_ATF_NORD_DE_GUERRE = 27500,     /* Nord de Guerre             */

PCS_NTF_FRANCE_I = 27581,     /* France I                   */
PCS_NTF_FRANCE_II = 27582,     /* France II                  */
PCS_NTF_FRANCE_III = 27583,     /* France III                 */
PCS_NTF_FRANCE_IV = 27584,     /* France IV                  */
PCS_NTF_NORD_FRANCE = 27591,     /* Nord France                */
PCS_NTF_CENTRE_FRANCE = 27592,     /* Centre France              */
PCS_NTF_SUD_FRANCE = 27593,     /* Sud France                 */

PCS_NTF_CORSE = 27594,     /* Corse                      */

PCS_KALIANPUR_INDIA_0 = 24370,     /* India Zone 0               */
PCS_KALIANPUR_INDIA_I = 24371,     /* India Zone I               */
PCS_KALIANPUR_INDIA_IIA = 24372,     /* India Zone IIa             */
PCS_KALIANPUR_INDIA_IIB = 24382,     /* India Zone IIb             */
PCS_KALIANPUR_INDIA_IIIA = 24373,     /* India Zone IIIa            */
PCS_KALIANPUR_INDIA_IIIB = 24383,     /* India Zone IIIb            */
PCS_KALIANPUR_INDIA_IVA = 24374,     /* India Zone IVa             */
PCS_KALIANPUR_INDIA_IVB = 24384,     /* India Zone IVb             */

PCS_JAMAICA_1875_OLD_GRID = 24100,     /* Jamaica 1875 Old Grid      */
PCS_JAD_1969_JAMAICA_GRID = 24200,     /* Jamaica Grid               */

PCS_MERCHICH_NORD_MAROC = 26191,     /* Nord Maroc                 */
PCS_MERCHICH_SUD_MAROC = 26192,     /* Sud Maroc                  */
PCS_MERCHICH_SAHARA = 26193,     /* Sahara                     */
PCS_CARTHAGE_NORD_TUNISIE = 22391,     /* Nord Tunisie               */
PCS_CARTHAGE_SUD_TUNISIE = 22392,     /* Sud Tunisie                */
PCS_KOC_LAMBERT = 24600,     /* Kuwait Oil Co - Lambert    */
PCS_BELGE_LAMBERT_1950 = 21500,     /* Belge Lambert 1950         */
PCS_DEALUL_PISCULUI_1933_STEREO_33 = 31600,     /* Stereo 1933           */
PCS_DEALUL_PISCULUI_1970_STEREO_70 = 31700,     /* Stereo 1970           */

//{{{

PCS_ETRS89_PORTUGAL_TM06 = 3763,     /* ETRS89 / Portugal TM06          */
PCS_LISBON_1890_PORTUGAL_BONNE = 61008,     /* Bessel Datum Lisboa          */
PCS_AZORES_OCCIDENTAL_1939_UTM_ZONE_25N = 2188,     /* Azores Occidental 1939 / UTM zone 25N           */
PCS_AZORES_CENTRAL_1948_UTM_ZONE_26N = 2189,     /* Azores Central 1948 / UTM zone 26N           */
PCS_AZORES_ORIENTAL_1940_UTM_ZONE_26N = 2190,     /* Azores Oriental 1940 / UTM zone 26N           */
PCS_MADEIRA_1936_UTM_ZONE_28N = 2191,     /* Madeira 1936 / UTM zone 28N           */

PCS_ED50_ORIENTAL_GROUP = 61001,     /* ED50 / Oriental group           */
PCS_ED50_CENTRAL_GROUP = 61002,     /* ED50 / Central group           */
PCS_ED50_OCCIDENTAL_GROUP = 61003,     /* ED50 / Occidental group           */
PCS_PTRA08_UTM28_ITRF93 = 61004,     /* PTRA08-UTM28/ITRF93           */
PCS_PTRA08_UTM26_ITRF93 = 61006,     /* PTRA08-UTM26/ITRF93           */
PCS_PTRA08_UTM25_ITRF93 = 61007,     /* PTRA08-UTM25/ITRF93           */

//}}
//{{
PCS_WGS_1984_WORLD_MERCATOR = 3395,	     /* WGS_1984_World_Mercator         */
//}}

//{{增加原因：常用投影加到预定义投影
PCS_WGS_1984_WEB_MERCATOR = 3857,	     /* WGS 84/Web-Mercator        */
PCS_POPULAR_VISUALISATON_CRS_MERCATOR = 3785,	     /* Popular Visualisation CRS/Mercator(deprecated)        */
//}}

//{{ 增加原因：马来投影添加到预定义投影
PCS_CASSINI_PAHANG = 4392,	     /* Kertau 1968/Pahang Grid        */
PCS_CASSINI_SELANGOR = 4393,	     /* Kertau 1968/Selangor Grid        */
PCS_CASSINI_TERENGGANU = 4394,	     /* Kertau 1968/Terengganu Grid        */
PCS_CASSINI_PERAK = 4397,	     /* Kertau 1968/Perak Revised Grid        */
PCS_KERTAU_SINGAPORE_GRID = 24500,	 /* Kertau 1968/Singapore Grid        */
PCS_KERTAU_RSO_MALAYA_METERS = 3168,	     /* Kertau (RSO)/RSO Malaya(m)        */
PCS_TIMBALAI_1948_RSO_BORNEO_METERS = 29873,	 /* Timbalai 1948/RSO Borneo(m)        */

//}}


//}}
} EmPrjCoordSysType;