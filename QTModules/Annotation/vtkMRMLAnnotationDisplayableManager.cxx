/*=auto=========================================================================

 Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

 See Doc/copyright/copyright.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Program:   3D Slicer
 Module:    $RCSfile: vtkMRMLAnnotationDisplayableManager.cxx,v $
 Date:      $Date: 2010/10/06 11:42:53 $
 Version:   $Revision: 1.1 $

 =========================================================================auto=*/

#include <vtkMRMLAnnotationDisplayableManager.h>
#include <qSlicerApplication.h>
#include <qSlicerLayoutManager.h>
#include <qMRMLThreeDRenderView.h>
#include <vtkMRMLDisplayableManagerFactory.h>

//---------------------------------------------------------------------------
vtkStandardNewMacro(vtkMRMLAnnotationDisplayableManager);
vtkCxxRevisionMacro(vtkMRMLAnnotationDisplayableManager, "$Revision: 1.1 $");

vtkMRMLAnnotationDisplayableManager::vtkMRMLAnnotationDisplayableManager()
{

}

// Destructor
vtkMRMLAnnotationDisplayableManager::~vtkMRMLAnnotationDisplayableManager()
{

}


void vtkMRMLAnnotationDisplayableManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

/// Register the manager with the factory.
bool vtkMRMLAnnotationDisplayableManager::RegisterManager()
{

//  int cnt = qSlicerApplication::application()->layoutManager()->threeDRenderViewCount();
//
//  if (cnt > 0 && (this != NULL))
//    {
//    vtkDebugMacro("Starting registration of a new displayable manager instance...");
//
//    // at least one render view exists, get the first
//
//    qSlicerApplication * application = qSlicerApplication::application();
//    qSlicerLayoutManager * layoutManager = NULL;
//    qMRMLThreeDRenderView * threeDRenderView = NULL;
//    vtkMRMLDisplayableManagerFactory * displayableManagerFactory = NULL;
//    if (application != NULL)
//      {
//      layoutManager = application->layoutManager();
//      }
//    else
//      {
//      vtkErrorMacro("Could not get the application!");
//      return false;
//      }
//    if (layoutManager != NULL)
//      {
//
//      threeDRenderView = layoutManager->threeDRenderView(0);
//
//      }
//    else
//      {
//      vtkErrorMacro("Could not get the layout manager!");
//      return false;
//      }
//
//    if (threeDRenderView != NULL)
//      {
//      displayableManagerFactory = threeDRenderView->displayableManagerFactory();
//      }
//    else
//      {
//
//      vtkErrorMacro("Could not get the 3D Render View!");
//      return false;
//      }
//
//    // make sure we got the factory
//    Q_ASSERT(displayableManagerFactory);
//
//    if (displayableManagerFactory != NULL)
//      {
//
//      displayableManagerFactory->RegisterDisplayableManager(this);
//      displayableManagerFactory->SetMRMLViewNode(displayableManagerFactory->GetMRMLViewNode());
//
//      }
//    else
//      {
//      vtkErrorMacro("Could not get displayable manager factory!");
//      return false;
//      }
//
//    return true;
//    }

  // something went wrong
  return false;

}