/*==========================================================================

  Portions (c) Copyright 2008 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $HeadURL: $
Date:      $Date: $
Version:   $Revision: $

==========================================================================*/

// .NAME vtkPatientToImageRegistrationLogic - slicer logic class for Locator module 
// .SECTION Description
// This class manages the logic associated with tracking device for
// IGT. 


#ifndef __vtkPatientToImageRegistrationLogic_h
#define __vtkPatientToImageRegistrationLogic_h

#include <vector>

#include "vtkPatientToImageRegistrationWin32Header.h"

#include "vtkSlicerBaseLogic.h"
#include "vtkSlicerModuleLogic.h"
#include "vtkSlicerApplication.h"
#include "vtkCallbackCommand.h"

#include "vtkMRMLFiducialListNode.h"
#include "vtkMRMLSliceNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkIGTPat2ImgRegistration.h"
#include "PivotCalibration.h"


class VTK_PatientToImageRegistration_EXPORT vtkPatientToImageRegistrationLogic : public vtkSlicerModuleLogic 
{
public:
  //BTX
  enum WorkPhase {
    StartUp = 0,
    Planning,
    Calibration,
    Targeting,
    Manual,
    Emergency,
    NumPhases,
  };

  enum {
    SLICE_DRIVER_USER    = 0,
    SLICE_DRIVER_LOCATOR = 1,
    SLICE_DRIVER_RTIMAGE = 2
  };
  enum ImageOrient{
    SLICE_RTIMAGE_NONE      = 0,
    SLICE_RTIMAGE_PERP      = 1,
    SLICE_RTIMAGE_INPLANE90 = 2,
    SLICE_RTIMAGE_INPLANE   = 3
  };
  enum {  // Events
    LocatorUpdateEvent      = 50000,
    StatusUpdateEvent       = 50001,
    //SliceUpdateEvent        = 50002,
  };

  //ETX

  // Work phase keywords used in NaviTrack (defined in BRPTPRInterface.h)

public:

  static vtkPatientToImageRegistrationLogic *New();

  vtkTypeRevisionMacro(vtkPatientToImageRegistrationLogic,vtkObject);

  vtkSetMacro ( NeedRealtimeImageUpdate0, int );
  vtkGetMacro ( NeedRealtimeImageUpdate0, int );
  vtkSetMacro ( NeedRealtimeImageUpdate1, int );
  vtkGetMacro ( NeedRealtimeImageUpdate1, int );
  vtkSetMacro ( NeedRealtimeImageUpdate2, int );
  vtkGetMacro ( NeedRealtimeImageUpdate2, int );

  vtkSetMacro ( NeedUpdateLocator,       bool );
  vtkGetMacro ( NeedUpdateLocator,       bool );

  void SetSliceDriver0(int v) { this->SliceDriver[0] = v; };
  void SetSliceDriver1(int v) { this->SliceDriver[1] = v; };
  void SetSliceDriver2(int v) { this->SliceDriver[2] = v; };
  int  SetSliceDriver0() { return this->SliceDriver[0]; };
  int  SetSliceDriver1() { return this->SliceDriver[1]; };
  int  SetSliceDriver2() { return this->SliceDriver[2]; };


  vtkGetMacro ( Connection,              bool );

  vtkSetMacro ( EnableOblique,           bool );
  vtkGetMacro ( EnableOblique,           bool );
  vtkSetMacro ( FreezePlane,             bool );
  vtkGetMacro ( FreezePlane,              bool );

  vtkGetObjectMacro ( LocatorTransform, vtkTransform );
  vtkGetObjectMacro ( LocatorMatrix,    vtkMatrix4x4 );
  vtkGetObjectMacro ( Pat2ImgReg, vtkIGTPat2ImgRegistration );

  vtkSetObjectMacro(OriginalTrackerNode, vtkMRMLLinearTransformNode);

  vtkSetStringMacro(TransformNodeName); 
  vtkGetStringMacro(TransformNodeName);

  vtkSetMacro (UseRegistration, bool);
  vtkGetMacro (UseRegistration, bool);

  vtkSetMacro (UsePivotCalibration, bool);
  vtkGetMacro (UsePivotCalibration, bool);

  vtkGetMacro(RMSE, double);

  void PrintSelf(ostream&, vtkIndent);
  //void AddRealtimeVolumeNode(const char* name);

  void UpdateDisplay(int sliceNo1, int sliceNo2, int sliceNo3);
  void GetCurrentPosition(double *px, double *py, double *pz);
  void UpdateTransformNodeByName(const char *name);
  int PerformPatientToImageRegistration();

  void UpdateFiducialSeeding(const char *name, double offset);

  void CollectDataForPivotCalibration(int start);
  void ComputePivotCalibration();
  void ApplyPivotCalibration(int yes);
  void GetTranslation(double trans[3]);


protected:

  double PivotPosition[3];
  double Translation[3];
  double RMSE;
  bool CollectPCData;
  bool UsePivotCalibration;
  PivotCalibration PVCalibration;

  vtkPatientToImageRegistrationLogic();
  ~vtkPatientToImageRegistrationLogic();
  vtkPatientToImageRegistrationLogic(const vtkPatientToImageRegistrationLogic&);
  void operator=(const vtkPatientToImageRegistrationLogic&);



private:

  //----------------------------------------------------------------
  // Monitor Timer
  //----------------------------------------------------------------

  int MonitorFlag;
  int MonitorInterval;

  void UpdateSliceNode(int sliceNo1, int sliceNo2, int sliceNo3,
                       float nx, float ny, float nz,
                       float tx, float ty, float tz,
                       float px, float py, float pz);

  void CheckSliceNodes();
  void ApplyTransform(float *position, float *norm, float *transnorm);
  void UpdateLocatorTransform();

  //----------------------------------------------------------------
  // Real-time image
  //----------------------------------------------------------------

  vtkMRMLVolumeNode     *RealtimeVolumeNode;

  int   NeedRealtimeImageUpdate0;
  int   NeedRealtimeImageUpdate1;
  int   NeedRealtimeImageUpdate2;

  vtkMRMLSliceNode *SliceNode[3];

  int   SliceDriver[3];

  bool  ImagingControl;
  bool  NeedUpdateLocator;

  bool  EnableOblique;
  bool  FreezePlane;

  long  RealtimeImageTimeStamp;
  //int   RealtimeImageSerial;
  int   RealtimeImageOrient;


  //----------------------------------------------------------------
  // Locator
  //----------------------------------------------------------------

  // What's a difference between LocatorMatrix and Locator Transform???
  vtkMatrix4x4*         LocatorMatrix;
  vtkTransform*         LocatorTransform;

  bool  Connection;  

  bool UseRegistration;
  char *TransformNodeName;
  int SliceNo1Last;
  int SliceNo2Last;
  int SliceNo3Last;

  vtkIGTPat2ImgRegistration *Pat2ImgReg;

  // holds transform from tracking device
  vtkMRMLLinearTransformNode *OriginalTrackerNode;

  // updated transform after patient to image registration
  vtkMRMLLinearTransformNode *UpdatedTrackerNode;

};

#endif


  
