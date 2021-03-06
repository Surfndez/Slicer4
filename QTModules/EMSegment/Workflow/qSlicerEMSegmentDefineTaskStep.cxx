/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by
    Danielle Pace and Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QSignalMapper>

// CTK includes
#include <ctkWorkflow.h>

// EMSegment includes
#include "qSlicerEMSegmentDefineTaskStep.h"
#include "ui_qSlicerEMSegmentDefineTaskStep.h"
#include "qSlicerEMSegmentDefineTaskStep_p.h"
#include "qSlicerEMSegmentModuleWidget.h"

// EMSegment/MRML includes
#include <vtkEMSegmentMRMLManager.h>
#include <vtkMRMLEMSNode.h>

//-----------------------------------------------------------------------------
// qSlicerEMSegmentDefineTaskStepPrivate methods

//-----------------------------------------------------------------------------
qSlicerEMSegmentDefineTaskStepPrivate::qSlicerEMSegmentDefineTaskStepPrivate(qSlicerEMSegmentDefineTaskStep& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
void qSlicerEMSegmentDefineTaskStepPrivate::setupUi(qSlicerEMSegmentWorkflowWidgetStep* step)
{
  this->Ui_qSlicerEMSegmentDefineTaskStep::setupUi(step);

  // Connect TaskSelector
  connect(this->TaskSelectorComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this,
          SLOT(selectTask(vtkMRMLNode*)));

  // Each button is mapped with the corresponding branch name
  this->Mapper.setMapping(this->SimpleModeButton, qSlicerEMSegmentModuleWidget::SimpleMode);
  this->Mapper.setMapping(this->AdvancedModeButton, qSlicerEMSegmentModuleWidget::AdvancedMode);

  // Connect buttons
  connect(this->SimpleModeButton, SIGNAL(clicked()), &this->Mapper, SLOT(map()));
  connect(this->AdvancedModeButton, SIGNAL(clicked()), &this->Mapper,SLOT(map()));

  connect(&this->Mapper, SIGNAL(mapped(const QString)), this, SIGNAL(modeChanged(const QString&)));
}

//-----------------------------------------------------------------------------
void qSlicerEMSegmentDefineTaskStepPrivate::selectTask(vtkMRMLNode* mrmlNode)
{
  Q_Q(qSlicerEMSegmentDefineTaskStep);
  Q_ASSERT(q->mrmlManager());
  q->mrmlManager()->SetNode(vtkMRMLEMSNode::SafeDownCast(mrmlNode));
}

//-----------------------------------------------------------------------------
// qSlicerEMSegmentDefineTaskStep methods

//-----------------------------------------------------------------------------
const QString qSlicerEMSegmentDefineTaskStep::StepId = "DefineTask";

//-----------------------------------------------------------------------------
qSlicerEMSegmentDefineTaskStep::qSlicerEMSegmentDefineTaskStep(
  ctkWorkflow* newWorkflow, QWidget* newWidget)
  : Superclass(newWorkflow, qSlicerEMSegmentDefineTaskStep::StepId, newWidget)
  , d_ptr(new qSlicerEMSegmentDefineTaskStepPrivate(*this))
{
  Q_D(qSlicerEMSegmentDefineTaskStep);
  d->setupUi(this);

  this->setName("1/9. Define Task");
  this->setDescription("Select a (new) task.");
  this->setButtonBoxHints(ctkWorkflowWidgetStep::ButtonBoxHidden);
}

//-----------------------------------------------------------------------------
qSlicerEMSegmentDefineTaskStep::~qSlicerEMSegmentDefineTaskStep()
{
}

//-----------------------------------------------------------------------------
void qSlicerEMSegmentDefineTaskStep::createUserInterface()
{
  Q_D(qSlicerEMSegmentDefineTaskStep);
  connect(d, SIGNAL(modeChanged(const QString&)), this->workflow(), SLOT(goForward(const QString&)));

  this->createUserInterfaceComplete();
}
