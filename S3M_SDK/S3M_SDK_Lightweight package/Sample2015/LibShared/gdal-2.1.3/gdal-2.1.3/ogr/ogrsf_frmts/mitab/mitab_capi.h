/**********************************************************************
 * $Id: mitab_capi.h,v 1.39 2010-09-07 17:19:22 aboudreault Exp $
 *
 * Name:     mitab_capi.h
 * Project:  MapInfo TAB Read/Write library
 * Language: C
 * Purpose:  Simple C access API prototypes.  Suitable for limited access
 *           to MITAB from an application able to call C style DLL
 *           entry points.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 **********************************************************************
 * Copyright (c) 2000-2005, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 *
 * $Log: mitab_capi.h,v $
 * Revision 1.39  2010-09-07 17:19:22  aboudreault
 * Removed an unimplemented function from the C API: mitab_c_get_feature_id
 *
 * Revision 1.38  2010-07-05 18:23:53  aboudreault
 * C API: added mitab_c_bounds_set() function (bug 2233)
 *
 * Revision 1.37  2010-07-05 18:13:12  aboudreault
 * Added support for extended text attributes - new function for text style and symbol style (bug 2232)
 *
 * Revision 1.36  2009-07-28 21:35:29  aboudreault
 * Added functions to get the file version (bug 1961)
 *
 * Revision 1.35  2009-02-25 17:18:08  aboudreault
 * C API: Added mitab_c_set_charset() (bug 2013)
 *
 * Revision 1.34  2008-12-15 20:54:41  aboudreault
 * C API: Added mitab_c_get_table_class() (bug 1957)
 *
 * Revision 1.33  2008/10/20 21:00:20  aboudreault
 * C API: Added mitab_c_get_feature_count_by_type() (bug 1952)
 *
 * Revision 1.32  2008/01/29 20:46:32  dmorissette
 * Added support for v9 Time and DateTime fields (byg 1754)
 *
 * Revision 1.31  2007/03/30 18:03:51  dmorissette
 * Added mitab_c_set_quick_spatial_index_mode() to C API (bug 1669)
 *
 * Revision 1.30  2006/12/01 20:26:27  dmorissette
 * Added mitab_is_field_indexed() and mitab_is_field_unique() (bug 1621)
 *
 * Revision 1.29  2005/10/07 18:49:40  dmorissette
 * Added methods for collections in C API (bug 1126)
 *
 * Revision 1.28  2005/09/29 20:09:52  dmorissette
 * New C API methods to access projection params (ADJ, bug 1155)
 *
 * Revision 1.27  2005/04/07 15:56:27  dmorissette
 * Added mitab_c_set_symbol_angle() and mitab_c_get_symbol_angle() for
 * point symbols of type TABFC_FontPoint (bug 1002)
 *
 * Revision 1.26  2004/07/07 21:51:35  dmorissette
 * Added mitab_c_get_mif_coordsys_vb() (patch from BVT)
 *
 * Revision 1.25  2004/06/30 20:05:34  dmorissette
 * Added mitab_c_load_coordsys_table() to C API (bug 469)
 *
 * Revision 1.24  2004/06/03 23:10:11  dmorissette
 * Added missing MITAB_DLL for mitab_c_get_extended_mif_coordsys()
 *
 * Revision 1.23  2003/08/12 20:20:49  dmorissette
 * Changes from Anthony Dunk, Encom:
 * - Added ability to create a feature of type TABFC_NoGeom
 * - Added mitab_c_get_feature_count(), mitab_c_get_field_as_double() and
 *   mitab_c_get_extended_mif_coordsys()
 *
 * Revision 1.22  2003/08/07 03:20:46  dmorissette
 * Added mitab_c_getlibversion() to C API. (Uffe K. - bug 21)
 *
 * Revision 1.21  2003/01/18 21:44:33  daniel
 * Added 'indexed' and 'unique' parameters to mitab_c_add_field().
 *
 * Revision 1.20  2002/06/18 14:31:07  julien
 * Change a function name to be consistent with other function in mitab_capi
 * (mitab_c_region_isinteriorring become mitab_c_is_interior_ring)
 *
 * Revision 1.19  2002/06/17 15:00:30  julien
 * Add IsInteriorRing() function in TABRegion to validate if a ring is internal
 *
 * Revision 1.18  2002/05/08 21:38:16  daniel
 * (Hopefully) fixed the STDCALL stuff.
 *
 * Revision 1.17  2002/05/08 20:27:07  daniel
 * Use cdecl calling convention if MITAB_CDECL is set at compile time.
 *
 * Revision 1.16  2002/05/08 19:59:23  daniel
 * Use stdcall by default for all public functions
 *
 * Revision 1.15  2002/05/03 15:09:14  daniel
 * Added mitab_c_get_field_width() and mitab_c_get_field_precision()
 *
 * Revision 1.14  2002/04/26 14:16:49  julien
 * Finishing the implementation of Multipoint (support for MIF)
 *
 * Revision 1.13  2002/02/22 13:50:28  daniel
 * (From Bo Thomsen) New VB interface functions
 *
 * Revision 1.13  2002/02/20 12:35:00  bvt
 * Added alternative functions to fetch various strings for VB compatibility
 *
 * Revision 1.12  2001/11/02 17:30:02  daniel
 * Added mitab_c_get/set_projinfo() and mitab_c_get_mif_coordsys().
 * Changed mitab_c_create() to make bounds optional and allow using default
 * projection bounds if available.
 *
 * Revision 1.11  2001/07/02 20:03:28  daniel
 * Added mitab_c_get_text().
 *
 * Revision 1.10  2001/06/25 01:49:47  daniel
 * Added get methods for all pen, brush, font and symbol properties.
 *
 * Revision 1.9  2001/01/22 16:03:59  warmerda
 * expanded tabs
 *
 * Revision 1.8  2000/10/03 20:43:36  daniel
 * Added support for writing arcs,ellipses and rectangles in C API
 *
 * Revision 1.7  2000/04/21 12:53:41  daniel
 * Added funcs to fetch feature coordinates and attributes
 *
 * Revision 1.6  2000/02/28 16:46:53  daniel
 * Removed style param to mitab_c_set_pen() since this param is actually
 * used inside the format to define pen width in points (version 450)
 *
 * Revision 1.5  2000/01/15 22:30:43  daniel
 * Switch to MIT/X-Consortium OpenSource license
 *
 * Revision 1.4  2000/01/14 21:58:40  warmerda
 * added error handling
 *
 * Revision 1.3  2000/01/14 17:30:39  warmerda
 * added capi dll support
 *
 * Revision 1.2  2000/01/14 16:33:24  warmerda
 * initial implementation complete
 *
 * Revision 1.1  2000/01/14 14:53:59  warmerda
 * New
 *
 */

#ifndef _MITAB_CAPI_H_INCLUDED_
#define _MITAB_CAPI_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
# ifndef MITAB_CDECL  /* STDCALL is the default */
#   define MITAB_DLL  __declspec(dllexport)
#   define MITAB_STDCALL __stdcall
# else                /* Use CDECL only if MITAB_CDECL explicitly set */
#   define MITAB_DLL __declspec(dllexport)
#   define MITAB_STDCALL
# endif
#else
#define MITAB_DLL
#define MITAB_STDCALL
#endif
    

typedef void * mitab_handle;
typedef void * mitab_feature;
typedef void * mitab_projinfo;

/* File class values (match values from TABFileClass enum in mitab.h )*/
#define TABFC_IMapInfoFile 0
#define TABFC_TABFile      1
#define TABFC_TABView      2
#define TABFC_TABSeamless  3
#define TABFC_MIFFile      4

/* feature type values (match values from TABFeatureClass enum in mitab.h) */
#define TABFC_NoGeom    0
#define TABFC_Point     1
#define TABFC_FontPoint 2
#define TABFC_CustomPoint 3
#define TABFC_Text      4
#define TABFC_Polyline  5
#define TABFC_Arc       6
#define TABFC_Region    7
#define TABFC_Rectangle 8
#define TABFC_Ellipse   9
#define TABFC_MultiPoint 10
#define TABFC_Collection 11

/* field types */
#define TABFT_Char      1
#define TABFT_Integer   2
#define TABFT_SmallInt  3
#define TABFT_Decimal   4
#define TABFT_Float     5
#define TABFT_Date      6
#define TABFT_Logical   7
#define TABFT_Time      8
#define TABFT_DateTime  9

/* text justification */
#define TABTJ_Left      0
#define TABTJ_Center    1
#define TABTJ_Right     2

/* text spacing */
#define TABTS_Single    0
#define TABTS_1_5       1
#define TABTS_Double    2

/* test linetype */
#define TABTL_NoLine    0
#define TABTL_Simple    1
#define TABTL_Arrow     2

int MITAB_DLL MITAB_STDCALL mitab_c_getlibversion();
int MITAB_DLL MITAB_STDCALL mitab_c_get_file_version( mitab_handle handle );
const char MITAB_DLL * MITAB_STDCALL mitab_c_getlasterrormsg();
int MITAB_DLL MITAB_STDCALL mitab_c_getlasterrormsg_vb (char * errormsg, int l);
int MITAB_DLL MITAB_STDCALL mitab_c_getlasterrorno();

mitab_handle MITAB_DLL MITAB_STDCALL mitab_c_open( const char * filename );
void MITAB_DLL MITAB_STDCALL mitab_c_close( mitab_handle handle );
int MITAB_DLL MITAB_STDCALL mitab_c_get_table_class( mitab_handle handle ); 

mitab_handle MITAB_DLL MITAB_STDCALL mitab_c_create( const char * filename,
                                       const char * mif_or_tab,
                                       const char * mif_projectiondef,
                                       double north, double south,
                                       double east, double west );
int MITAB_DLL MITAB_STDCALL mitab_c_bounds_set( mitab_handle handle ); // Returns 1 if bounds set, 0 otherwise.

int MITAB_DLL MITAB_STDCALL mitab_c_set_quick_spatial_index_mode( mitab_handle handle );

int MITAB_DLL MITAB_STDCALL mitab_c_add_field( mitab_handle handle, const char * field_name,
                                 int field_type, int width, int precision, 
                                 int indexed, int unique );

int MITAB_DLL MITAB_STDCALL mitab_c_set_charset( mitab_handle handle, const char * charset);

int MITAB_DLL MITAB_STDCALL
mitab_c_write_feature( mitab_handle handle, mitab_feature feature );

int MITAB_DLL MITAB_STDCALL
mitab_c_next_feature_id( mitab_handle handle, int last_feature_id );

mitab_feature MITAB_DLL MITAB_STDCALL
mitab_c_read_feature( mitab_handle handle, int feature_id );

void MITAB_DLL MITAB_STDCALL mitab_c_destroy_feature( mitab_feature );

mitab_feature MITAB_DLL MITAB_STDCALL
mitab_c_create_feature( mitab_handle, int feature_type );

void MITAB_DLL MITAB_STDCALL
mitab_c_set_field( mitab_feature, int field_index, const char * value );
    
void MITAB_DLL MITAB_STDCALL
mitab_c_set_text( mitab_feature feature, const char * text );
const char MITAB_DLL * MITAB_STDCALL
mitab_c_get_text( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL 
mitab_c_get_text_vb( mitab_feature feature, char * text, int l );

void MITAB_DLL MITAB_STDCALL
mitab_c_get_text_line_end_point( mitab_feature feature, 
								double* centerX, double* centerY);

void MITAB_DLL MITAB_STDCALL
mitab_c_get_text_boxsize( mitab_feature feature, 
								double* boxWidth, double* boxHeight);

void MITAB_DLL MITAB_STDCALL
mitab_c_set_text_display( mitab_feature feature,
                          double angle, double height, double width,
                          int fg_color, int bg_color,
                          int justification, int spacing, int linetype );
double MITAB_DLL MITAB_STDCALL
mitab_c_get_text_angle( mitab_feature feature );
double MITAB_DLL MITAB_STDCALL
mitab_c_get_text_height( mitab_feature feature );
double MITAB_DLL MITAB_STDCALL
mitab_c_get_text_width( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_text_fgcolor( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_text_bgcolor( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_text_justification( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_text_spacing( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_text_linetype( mitab_feature feature );

void MITAB_DLL MITAB_STDCALL mitab_c_set_pen( mitab_feature feature,
                                int width, int pattern, int color );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_pen_color( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_pen_width( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_pen_pattern( mitab_feature feature );

void MITAB_DLL MITAB_STDCALL mitab_c_set_brush( mitab_feature feature,
                                  int fg_color, int bg_color, int pattern,
                                  int transparent );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_brush_fgcolor( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_brush_bgcolor( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_brush_pattern( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_brush_transparent( mitab_feature feature );

void MITAB_DLL MITAB_STDCALL
mitab_c_set_font( mitab_feature feature, const char * font_name );
const char MITAB_DLL * MITAB_STDCALL
mitab_c_get_font( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL 
mitab_c_get_font_vb( mitab_feature feature, char * font, int l );

void MITAB_DLL MITAB_STDCALL mitab_c_set_symbol( mitab_feature feature, int symbol_no,
                                   int symbol_size, int symbol_color );
void MITAB_DLL MITAB_STDCALL
mitab_c_set_symbol_angle( mitab_feature feature, double symbol_angle );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_symbol_color( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_symbol_no( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL
mitab_c_get_symbol_size( mitab_feature feature );
double MITAB_DLL MITAB_STDCALL
mitab_c_get_symbol_angle( mitab_feature feature );
int  MITAB_DLL MITAB_STDCALL mitab_c_get_symbol_style( mitab_feature feature); 
void MITAB_DLL MITAB_STDCALL mitab_c_set_symbol_style( mitab_feature feature, int symbol_style );
int  MITAB_DLL MITAB_STDCALL mitab_c_get_text_style( mitab_feature feature);
void MITAB_DLL MITAB_STDCALL mitab_c_set_text_style( mitab_feature feature, int text_style );

void MITAB_DLL MITAB_STDCALL mitab_c_set_points( mitab_feature feature, int part,
                                   int vertex_count, double * x, double * y );
void MITAB_DLL MITAB_STDCALL mitab_c_set_arc( mitab_feature feature, 
                                double center_x, double center_y,
                                double x_radius, double y_radius,
                                double start_angle, double end_angle);

int MITAB_DLL MITAB_STDCALL mitab_c_get_type( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL mitab_c_get_parts( mitab_feature feature );
int MITAB_DLL MITAB_STDCALL mitab_c_get_vertex_count( mitab_feature feature, int part );
double MITAB_DLL MITAB_STDCALL mitab_c_get_vertex_x( mitab_feature, int part, int vertex );
double MITAB_DLL MITAB_STDCALL mitab_c_get_vertex_y( mitab_feature, int part, int vertex );

int MITAB_DLL MITAB_STDCALL mitab_c_get_feature_count( mitab_handle handle );
int MITAB_DLL MITAB_STDCALL mitab_c_get_feature_count_by_type( mitab_handle handle,
                                                               int *numPoints, int *numLines, 
                                                               int *numRegions, int *numTexts );

int MITAB_DLL MITAB_STDCALL mitab_c_get_field_count( mitab_handle handle );
int MITAB_DLL MITAB_STDCALL mitab_c_get_field_type( mitab_handle handle, int field );
const char MITAB_DLL * MITAB_STDCALL mitab_c_get_field_name(mitab_handle handle, int field);
int MITAB_DLL MITAB_STDCALL mitab_c_get_field_name_vb( mitab_handle handle, int field, char * name, int l );
int MITAB_DLL MITAB_STDCALL mitab_c_get_field_width( mitab_handle handle, int field );
int MITAB_DLL MITAB_STDCALL mitab_c_get_field_precision( mitab_handle handle, int field );
int MITAB_DLL MITAB_STDCALL mitab_c_is_field_indexed( mitab_handle handle, int field );
int MITAB_DLL MITAB_STDCALL mitab_c_is_field_unique( mitab_handle handle, int field );

const char MITAB_DLL * MITAB_STDCALL mitab_c_get_field_as_string( mitab_feature feature, 
                                                   int field );

double MITAB_DLL MITAB_STDCALL mitab_c_get_field_as_double(mitab_feature feature, int field);

int MITAB_DLL MITAB_STDCALL mitab_c_get_field_as_string_vb( mitab_feature feature, int field, char * value, int l );

mitab_projinfo MITAB_DLL MITAB_STDCALL mitab_c_get_projinfo( mitab_handle dataset );
int            MITAB_DLL MITAB_STDCALL mitab_c_set_projinfo( mitab_handle dataset, 
                                               mitab_projinfo projinfo );
const char MITAB_DLL * MITAB_STDCALL mitab_c_get_mif_coordsys( mitab_handle dataset);
const char MITAB_DLL * MITAB_STDCALL mitab_c_get_extended_mif_coordsys( mitab_handle dataset);

int MITAB_DLL MITAB_STDCALL mitab_c_get_mif_coordsys_vb( mitab_handle dataset, char * coordsys, int l);
int MITAB_DLL MITAB_STDCALL mitab_c_get_extended_mif_coordsys_vb( mitab_handle dataset, char * coordsys, int l);

int MITAB_DLL MITAB_STDCALL mitab_c_load_coordsys_table( const char *filename );

int MITAB_DLL MITAB_STDCALL 
mitab_c_is_interior_ring( mitab_feature feature, int requestedringindex );

void MITAB_DLL MITAB_STDCALL
mitab_c_get_projection_info( mitab_projinfo projInfo,
                             int *nProjId, int *nEllipsoidId, int *nUnitsId,
                             double *adProjParams /* array with six entries */);
void MITAB_DLL MITAB_STDCALL
mitab_c_set_projection_info( mitab_projinfo projInfo,
                             int nProjId, int nEllipsoidId, int nUnitsId,
                             double *adProjParams /* array with six entries */);
void MITAB_DLL MITAB_STDCALL
mitab_c_get_datum_info( mitab_projinfo projInfo,
                        double *dDatumShiftX, double *dDatumShiftY, double *dDatumShiftZ,
                        double *adDatumParams /* array with five entries */);
void MITAB_DLL MITAB_STDCALL
mitab_c_set_datum_info( mitab_projinfo projInfo,
                        double dDatumShiftX, double dDatumShiftY, double dDatumShiftZ,
                        double *adDatumParams /* array with five entries */);
int MITAB_DLL MITAB_STDCALL                /* Returns 0 if no affine params*/
mitab_c_get_affine_params( mitab_projinfo projInfo, int *nAffineUnits,
                           double *adAffineParams /* array with 6 entries */);
void MITAB_DLL MITAB_STDCALL
mitab_c_set_affine_params( mitab_projinfo projInfo, int nAffineUnits,
                           double *adAffineParams /* array with 6 entries */);


mitab_feature MITAB_DLL MITAB_STDCALL 
  mitab_c_get_collection_region_ref( mitab_feature feature );
mitab_feature MITAB_DLL MITAB_STDCALL 
  mitab_c_get_collection_polyline_ref( mitab_feature feature );
mitab_feature MITAB_DLL MITAB_STDCALL 
  mitab_c_get_collection_multipoint_ref( mitab_feature feature );

int MITAB_DLL MITAB_STDCALL 
  mitab_c_set_collection_region( mitab_feature feature,
                                 mitab_feature region,
                                 int make_copy );
int MITAB_DLL MITAB_STDCALL 
  mitab_c_set_collection_polyline( mitab_feature feature,
                                   mitab_feature polyline,
                                   int make_copy );
int MITAB_DLL MITAB_STDCALL 
  mitab_c_set_collection_multipoint( mitab_feature feature,
                                     mitab_feature multipoint,
                                     int make_copy );

int MITAB_DLL MITAB_STDCALL
mitab_c_get_bound( mitab_handle dataset, double *dXMin, double *dYMin, 
                          double *dXMax, double *dYMax);

void MITAB_DLL MITAB_STDCALL
mitab_c_reset_reading( mitab_handle dataset );

int MITAB_DLL MITAB_STDCALL
mitab_c_get_arc( mitab_feature feature, 
				double* centerX, double* centerY, 
				double* xradius, double* yradius,
				double* startAngle, double* endAngle
				);

int MITAB_DLL MITAB_STDCALL
mitab_c_get_rectangle( mitab_feature feature, 
					  double* xMin, double* yMin, double* xMax, double* yMax, 
					  int* roundCorners, double* roundXRadius, 
					  double* roundYRadius);

int MITAB_DLL MITAB_STDCALL
mitab_c_get_ellipse( mitab_feature feature, 
					double* centerX, double* centerY, 
					double* xRadius, double* yRadius);

int MITAB_DLL MITAB_STDCALL
mitab_c_set_rectangle( mitab_feature feature, 
					  double xMin, double yMin, double xMax, double yMax, 
					  int roundCorners, double roundXRadius, 
					  double roundYRadius);

int MITAB_DLL MITAB_STDCALL
mitab_c_set_ellipse( mitab_feature feature, 
					double centerX, double centerY, 
					double xRadius, double yRadius);

#ifdef __cplusplus
}
#endif
    
#endif /* def _MITAB_CAPI_H_INCLUDED */
