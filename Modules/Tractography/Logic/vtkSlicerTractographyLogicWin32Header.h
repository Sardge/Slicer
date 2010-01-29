/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkFiducilaSeedingWin32Header.h,v $
  Date:      $Date: 2006/01/06 17:56:51 $
  Version:   $Revision: 1.4 $

=========================================================================auto=*/
// .NAME vtkFiducilaSeedingWin32Header - manage Windows system differences
// .SECTION Description
// The vtkFiducilaSeedingWin32Header captures some system differences between Unix
// and Windows operating systems. 

#ifndef __vtkSlicerTractographyLogicWin32Header_h
#define __vtkSlicerTractographyLogicWin32Header_h

#include <vtkSlicerTractographyLogicConfigure.h>

#if defined(WIN32) && !defined(VTKSLICER_STATIC)
#if defined(SlicerTractographyLogic_EXPORTS)
#define VTK_TRCACTOGRAPHYLOGIC_EXPORT __declspec( dllexport ) 
#else
#define VTK_TRCACTOGRAPHYLOGIC_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_TRCACTOGRAPHYLOGIC_EXPORT 
#endif

#endif
