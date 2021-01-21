/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkSlicerInteractorStyle.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSlicerInteractorStyle.h"
#include "vtkMath.h"
#include "vtkCellPicker.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"
#include "vtkCommand.h"


vtkCxxRevisionMacro(vtkSlicerInteractorStyle, "$Revision$");
vtkStandardNewMacro(vtkSlicerInteractorStyle);

//----------------------------------------------------------------------------
vtkSlicerInteractorStyle::vtkSlicerInteractorStyle()
{
  // Tell the parent class not to handle observers
  // that has to be done here
  this->HandleObserversOff();
  this->LastPos[0] = this->LastPos[1] = 0;
  this->OldPos[0] = this->OldPos[1] = 0;
  this->ShiftKey = 0;
  this->CtrlKey = 0;
  this->Char = '\0';
  this->KeySym = const_cast<char *>("");
  this->Button = 0;
}

//----------------------------------------------------------------------------
vtkSlicerInteractorStyle::~vtkSlicerInteractorStyle() 
{
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::PrintSelf(ostream& os, vtkIndent indent) 
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "LastPos: (" << this->LastPos[0] << ", " 
                               << this->LastPos[1] << ")\n";  
  os << indent << "OldPos: (" << this->OldPos[0] << ", " 
                              << this->OldPos[1] << ")\n";
  os << indent << "ShiftKey: " << this->ShiftKey << "\n";
  os << indent << "CtrlKey: " << this->CtrlKey << "\n";
  os << indent << "Char: " << this->Char << "\n";
  os << indent << "KeySym: " << this->KeySym << "\n";
  os << indent << "Button: " << this->Button << "\n";
}

//----------------------------------------------------------------------------
// checks for USERINTERACTION state, then defers to the superclass modes
void vtkSlicerInteractorStyle::OnTimer() 
{
  if (this->HasObserver(vtkCommand::TimerEvent)) 
    {
    this->InvokeEvent(vtkCommand::TimerEvent,NULL);
    }

  if (this->State == VTKIS_USERINTERACTION)
    {
    if (this->HasObserver(vtkCommand::UserEvent)) 
      {
      this->InvokeEvent(vtkCommand::UserEvent,NULL);
      this->OldPos[0] = this->LastPos[0];
      this->OldPos[1] = this->LastPos[1];
      if (this->UseTimers) 
        {
        this->Interactor->CreateTimer(VTKI_TIMER_UPDATE);
        }
      }
    }
  else if (!(this->HasObserver(vtkCommand::MouseMoveEvent) && 
             (this->Button == 0 ||
              (this->HasObserver(vtkCommand::LeftButtonPressEvent) && this->Button == 1) ||
              (this->HasObserver(vtkCommand::MiddleButtonPressEvent) && this->Button == 2) ||
              (this->HasObserver(vtkCommand::RightButtonPressEvent) && this->Button == 3))))
    {
    this->vtkInteractorStyle::OnTimer();
    }
  else if (this->HasObserver(vtkCommand::TimerEvent))
    {
    if (this->UseTimers) 
      {
      this->Interactor->CreateTimer(VTKI_TIMER_UPDATE);
      }
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnKeyPress()
{
  if (this->HasObserver(vtkCommand::KeyPressEvent)) 
    {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->KeySym = this->Interactor->GetKeySym();
    this->Char = this->Interactor->GetKeyCode();  
    this->InvokeEvent(vtkCommand::KeyPressEvent, NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnKeyRelease()
{
  if (this->HasObserver(vtkCommand::KeyReleaseEvent)) 
    {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->KeySym = this->Interactor->GetKeySym();
    this->Char = this->Interactor->GetKeyCode();  

    this->InvokeEvent(vtkCommand::KeyReleaseEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnChar() 
{
  // otherwise pass the OnChar to the vtkInteractorStyle.
  if (this->HasObserver(vtkCommand::CharEvent)) 
    {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->Char = this->Interactor->GetKeyCode();  
    
    this->InvokeEvent(vtkCommand::CharEvent,NULL);
    }
  else
    {
    this->vtkInteractorStyle::OnChar();
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnRightButtonDown() 
{
  this->Button = 3;

  if (this->HasObserver(vtkCommand::RightButtonPressEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::RightButtonPressEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnRightButtonDown();
    }
}
//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnRightButtonUp() 
{
  if (this->HasObserver(vtkCommand::RightButtonReleaseEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::RightButtonReleaseEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnRightButtonUp();
    }

  if (this->Button == 3)
    {
    this->Button = 0;
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnMiddleButtonDown() 
{
  this->Button = 2;

  if (this->HasObserver(vtkCommand::MiddleButtonPressEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MiddleButtonPressEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnMiddleButtonDown();
    }
}
//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnMiddleButtonUp() 
{
  if (this->HasObserver(vtkCommand::MiddleButtonReleaseEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnMiddleButtonUp();
    }

  if (this->Button == 2)
    {
    this->Button = 0;
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnLeftButtonDown() 
{
  this->Button = 1;

  if (this->HasObserver(vtkCommand::LeftButtonPressEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnLeftButtonDown();
    }
}
//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnLeftButtonUp() 
{
  if (this->HasObserver(vtkCommand::LeftButtonReleaseEvent)) 
    {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey  = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::LeftButtonReleaseEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
  else 
    {
    this->vtkInteractorStyle::OnLeftButtonUp();
    }

  if (this->Button == 1)
    {
    this->Button = 0;
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnMouseMove() 
{
  this->vtkInteractorStyle::OnMouseMove();

  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->LastPos[0] = x;
  this->LastPos[1] = y;
  this->ShiftKey = this->Interactor->GetShiftKey();
  this->CtrlKey = this->Interactor->GetControlKey();

  if (this->HasObserver(vtkCommand::MouseMoveEvent)) 
    {
    this->InvokeEvent(vtkCommand::MouseMoveEvent,NULL);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnMouseWheelForward() 
{
  this->vtkInteractorStyle::OnMouseWheelForward();

  if (this->HasObserver(vtkCommand::MouseWheelForwardEvent)) 
    {
    this->InvokeEvent(vtkCommand::MouseWheelForwardEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnMouseWheelBackward() 
{
  this->vtkInteractorStyle::OnMouseWheelBackward();

  if (this->HasObserver(vtkCommand::MouseWheelBackwardEvent)) 
    {
    this->InvokeEvent(vtkCommand::MouseWheelBackwardEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnExpose()
{
  if (this->HasObserver(vtkCommand::ExposeEvent)) 
    {
    this->InvokeEvent(vtkCommand::ExposeEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnConfigure() 
{
  if (this->HasObserver(vtkCommand::ConfigureEvent)) 
    {
    this->InvokeEvent(vtkCommand::ConfigureEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnEnter()
{
  if (this->HasObserver(vtkCommand::EnterEvent)) 
    {
    this->LastPos[0] = this->Interactor->GetEventPosition()[0];
    this->LastPos[1] = this->Interactor->GetEventPosition()[1];
    this->InvokeEvent(vtkCommand::EnterEvent,NULL);
    }
}

//----------------------------------------------------------------------------
void vtkSlicerInteractorStyle::OnLeave()
{
  if (this->HasObserver(vtkCommand::LeaveEvent)) 
    {
    this->LastPos[0] = this->Interactor->GetEventPosition()[0];
    this->LastPos[1] = this->Interactor->GetEventPosition()[1];
    this->InvokeEvent(vtkCommand::LeaveEvent,NULL);
    }
}
