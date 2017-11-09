#ifndef __MSCHART_DEFINE_H__
#define __MSCHART_DEFINE_H__

/********************************************************************
	filename: 	mschart_def.h
	author:		涂学锋
	
	purpose:	MSChart控件所需要的头文件，以及各枚举值的定义
*********************************************************************/

#pragma once

#include ".\mschart\font.h"
#include ".\mschart\vcaxis.h"
#include ".\mschart\vcaxisgrid.h"
#include ".\mschart\vcaxisscale.h"
#include ".\mschart\vcaxistitle.h"
#include ".\mschart\vcbackdrop.h"
#include ".\mschart\vcbrush.h"
#include ".\mschart\vccategoryscale.h"
#include ".\mschart\vccolor.h"
#include ".\mschart\vccoor.h"
#include ".\mschart\vcdatagrid.h"
#include ".\mschart\vcdatapoint.h"
#include ".\mschart\vcdatapointlabel.h"
#include ".\mschart\vcdatapoints.h"
#include ".\mschart\vcfill.h"
#include ".\mschart\vcfont.h"
#include ".\mschart\vcfootnote.h"
#include ".\mschart\vcframe.h"
#include ".\mschart\vcintersection.h"
#include ".\mschart\vclabel.h"
#include ".\mschart\vclabels.h"
#include ".\mschart\vclcoor.h"
#include ".\mschart\vclegend.h"
#include ".\mschart\vclight.h"
#include ".\mschart\vclightsource.h"
#include ".\mschart\vclightsources.h"
#include ".\mschart\vclocation.h"
#include ".\mschart\vcmarker.h"
#include ".\mschart\vcpen.h"
#include ".\mschart\vcplot.h"
#include ".\mschart\vcplotbase.h"
#include ".\mschart\vcrect.h"
#include ".\mschart\vcseries.h"
#include ".\mschart\vcseriescollection.h"
#include ".\mschart\vcseriesmarker.h"
#include ".\mschart\vcseriesposition.h"
#include ".\mschart\vcshadow.h"
#include ".\mschart\vcstatline.h"
#include ".\mschart\vctextlayout.h"
#include ".\mschart\vctick.h"
#include ".\mschart\vctitle.h"
#include ".\mschart\vcvaluescale.h"
#include ".\mschart\vcview3d.h"
#include ".\mschart\vcwall.h"
#include ".\mschart\vcweighting.h"
#include ".\mschart\mschart.h"

#if 1

//see mschrt20.tlh
/*VtChChartType provides chart type options*/
typedef enum _VtChChartType
{
    VtChChartType3dBar = 0,
    VtChChartType2dBar = 1,
    VtChChartType3dLine = 2,
    VtChChartType2dLine = 3,
    VtChChartType3dArea = 4,
    VtChChartType2dArea = 5,
    VtChChartType3dStep = 6,
    VtChChartType2dStep = 7,
    VtChChartType3dCombination = 8,
    VtChChartType2dCombination = 9,
    VtChChartType3dHorizontalBar = 10,
    VtChChartType2dHorizontalBar = 11,
    VtChChartType3dClusteredBar = 12,
    VtChChartType3dPie = 13,
    VtChChartType2dPie = 14,
    VtChChartType3dDoughnut = 15,
    VtChChartType2dXY = 16,
    VtChChartType2dPolar = 17,
    VtChChartType2dRadar = 18,
    VtChChartType2dBubble = 19,
    VtChChartType2dHiLo = 20,
    VtChChartType2dGantt = 21,
    VtChChartType3dGantt = 22,
    VtChChartType3dSurface = 23,
    VtChChartType2dContour = 24,
    VtChChartType3dScatter = 25,
    VtChChartType3dXYZ = 26,
    VtChChartTypeCount = 27
}VtChChartType;

/*The VtChDrawMode constants provide options for redisplaying a chart after it has been altered*/
typedef enum _VtChDrawMode
{
    VtChDrawModeDraw = 0,   // Draws directly to the display device
    VtChDrawModeBlit,       // Blits an offscreen drawing to the display device
}VtChDrawMode;

/*The VtFillStyle constants provide options for indicating the type of fill used to paint a backdrop*/
typedef enum _VtFillStyle
{
    VtFillStyleNull = 0,    // No fill (background shows through)
    VtFillStyleBrush,       // A solid color or pattern fill
}VtFillStyle;

/*The VtChLabelLocationType constants provide options for determining series label location*/
typedef enum _vtChLabelLocationType
{
    VtChLabelLocationTypeNone = 0,      // No label displayed. 
    VtChLabelLocationTypeAbovePoint,    // The label is displayed above the data point. 
    VtChLabelLocationTypeBelowPoint,    // The label is displayed below the data point. 
    VtChLabelLocationTypeCenter,        // The label is displayed centered on the data point. 
    VtChLabelLocationTypeBase,          // The label is displayed at the base along the category axis, directly beneath the data point. 
    VtChLabelLocationTypeInside,        // The label is displayed inside a pie slice. 
    VtChLabelLocationTypeOutside,       // The label is displayed outside a pie slice. 
    VtChLabelLocationTypeLeft,          // The label is displayed to the left of the data point. 
    VtChLabelLocationTypeRight,         // The label is displayed to the right of the data point. 
}vtChLabelLocationType;

typedef enum _VtChLocationType
{
    VtChLocationTypeTopLeft = 0,
    VtChLocationTypeTop = 1,
    VtChLocationTypeTopRight = 2,
    VtChLocationTypeLeft = 3,
    VtChLocationTypeRight = 4,
    VtChLocationTypeBottomLeft = 5,
    VtChLocationTypeBottom = 6,
    VtChLocationTypeBottomRight = 7,
    VtChLocationTypeCustom = 8
}VtChLocationType;

/*The VtChAxisId constants provide options for identifying a chart axis.*/
typedef enum _VtChAxisId
{
    VtChAxisIdX = 0,    // Identifies the x axis. 
    VtChAxisIdY,        // Identifies the y axis. 
    VtChAxisIdY2,       // Identifies the secondary y axis. 
    VtChAxisIdZ,        // Identifies the z axis. 
}VtChAxisId;

/*The VtPenStyle constants provide options for the pen used to draw chart lines.*/
typedef enum _VtPenStyle
{
    VtPenStyleNull = 0,     // No pen is applied 
    VtPenStyleSolid,        // Solid line pen 
    VtPenStyleDashed,       // Dashed line pen 
    VtPenStyleDotted,       // Dotted line pen 
    VtPenStyleDashDot,      // Dash-dot line pen 
    VtPenStyleDashDotDot,   // Dash-dot-dot line pen 
    VtPenStyleDitted,       // Ditted line pen 
    VtPenStyleDashDit,      // Dash-ditted line pen 
    VtPenStyleDashDitDit,   // Dash-dit-dit line pen 
}VtPenStyle;

/*The VtChAxisTickStyle constants provide options for indicating axis tick mark location.*/
typedef enum _VtChAxisTickStyle
{
    VtChAxisTickStyleNone = 0,  //No tick marks are displayed on the axis.  
    VtChAxisTickStyleCenter,    // Tick marks are centered across the axis. 
    VtChAxisTickStyleInside,    // Tick marks are displayed inside the axis. 
    VtChAxisTickStyleOutside,   // Tick marks are displayed outside the axis. 
}VtChAxisTickStyle;

/*The VtMarkerStyle constants provide options for displaying data point markers.*/
typedef enum _VtMarkerStyle
{
    VtMarkerStyleNull = 0,              // Suppressed 
    VtMarkerStyleDash,                  // Dash marker 
    VtMarkerStylePlus,                  // Plus marker 
    VtMarkerStyleX,                     // X marker 
    VtMarkerStyleStar,                  // Star marker 
    VtMarkerStyleCircle,                // Circle marker 
    VtMarkerStyleSquare,                // Square marker 
    VtMarkerStyleDiamond,               // Diamond marker 
    VtMarkerStyleUpTriangle,            // Triangle marker 
    VtMarkerStyleDownTriangle,          // Down triangle marker 
    VtMarkerStyleFilledCircle,          // Filled circle marker 
    VtMarkerStyleFilledSquare,          // Filled square marker 
    VtMarkerStyleFilledDiamond,         // Filled diamond marker 
    VtMarkerStyleFilledUpTriangle,      // Filled triangle marker 
    VtMarkerStyleFilledDownTriangle,    // Filled down triangle marker 
    VtMarkerStyle3dBall,                // Three-dimensional ball marker 
}VtMarkerStyle;

/*The VtChStats constants provide methods of displaying statistic lines on a chart.*/
typedef enum _VtChStats
{
    VtChStatsMinimum    = 1,   // Shows the minimum value in the series. 
    VtChStatsMaximum    = 2,   // Shows the maximum value in the series. 
    VtChStatsMean       = 4,   // Shows the mathematical mean of the values in the series. 
    VtChStatsStddev     = 8,   // Shows the standard deviation of the values in the series. 
    VtChStatsRegression = 16,  // Shows a trend line indicated by the values in a series. 
}VtChStats;

typedef enum _ErrorConstants
{
    VtOk = 0,
    VtFail = 1000,
    VtErrorDeletingUsedObject = 1001,
    VtErrorDeletingDeletedObject = 1002,
    VtErrorCorruptData = 1003,
    VtErrorNotImplemented = 1004,
    VtErrorNoMemory = 1100,
    VtErrorInvalidArgument = 1101,
    VtErrorNotFound = 1102,
    VtErrorTooSmall = 1103,
    VtErrorInvalidRequest = 1104,
    VtErrorStreamIo = 1105,
    VtErrorUserIo = 1106,
    VtErrorCorruptArchive = 1107,
    VtErrorArchiveVersion = 1108,
    VtErrorArchiveTypeMismatch = 1109,
    VtErrorArchivePointerMismatch = 1110,
    VtErrorCannotOpenFile = 1111,
    VtErrorUnableToLoadString = 1112,
    VtErrorBufferTooSmall = 1113,
    VtErrorCopyingObject = 1114,
    VtErrorDuplicateObject = 1115,
    VtErrorActionCanceled = 1116,
    VtErrorInvalidIndex = 1117,
    VtErrorInvalidTypeConversion = 1118,
    VtErrorInvalidObject = 1119,
    VtErrorCreateWindow = 1120,
    VtErrorOSVersion = 1121,
    VtErrorLoadPicture = 1122,
    VtErrorInvalidSyntax = 1200,
    VtErrorIdentifierTooBig = 1201,
    VtErrorUnrecongizedFunction = 1202,
    VtErrorUnrecongizedSymbol = 1203,
    VtErrorUnexpectedEOS = 1204,
    VtErrorDuplicateSymbol = 1205,
    VtErrorDisplay = 1500,
    VtErrorInvalidFontName = 1501,
    VtErrorInvalidFont = 1502,
    VtErrorNoDisplayResources = 1503,
    VtErrorDefaultFontSubstituted = 1504,
    VtChError = 2000,
    VtChErrorInvalidHandle = 2001,
    VtChErrorNoData = 2002,
    VtChErrorInvalidSeriesNumber = 2003,
    VtChErrorInvalidAxis = 2004,
    VtChErrorRestrictedVersion = 2005,
    InvalidPropertyValue = 380,
    GetNotSupported = 394,
    SetNotSupported = 383,
    InvalidProcedureCall = 5,
    InvalidObjectUse = 425,
    WrongClipboardFormat = 461,
    DataObjectLocked = 672,
    ExpectedAnArgument = 673,
    RecursiveOleDrag = 674,
    FormatNotByteArray = 675,
    DataNotSetForFormat = 676
}ErrorConstants;

typedef enum _VtHorizontalAlignment
{
    VtHorizontalAlignmentLeft = 0,
    VtHorizontalAlignmentRight = 1,
    VtHorizontalAlignmentCenter = 2,
    VtHorizontalAlignmentFill = 3,
    VtHorizontalAlignmentFlush = 4
}VtHorizontalAlignment;

typedef enum _VtVerticalAlignment
{
    VtVerticalAlignmentTop = 0,
    VtVerticalAlignmentBottom = 1,
    VtVerticalAlignmentCenter = 2
}VtVerticalAlignment;

#endif

#endif