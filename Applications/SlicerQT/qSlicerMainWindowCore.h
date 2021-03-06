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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerMainWindowCore_h
#define __qSlicerMainWindowCore_h 

// Qt includes
#include <QObject>

// CTK includes
#include <ctkPimpl.h>

#include "qSlicerQTExport.h"

class qSlicerMainWindow; 
class qSlicerMainWindowCorePrivate;

class Q_SLICERQT_EXPORT qSlicerMainWindowCore : public QObject
{
  Q_OBJECT
  
public:
  typedef QObject Superclass;
  qSlicerMainWindowCore(qSlicerMainWindow *parent);
  virtual ~qSlicerMainWindowCore();

public slots: 
  /// 
  /// Handle actions - See qSlicerMainWindow::setupMenuActions
  void onFileAddDataActionTriggered();
  void onFileImportSceneActionTriggered();
  void onFileLoadSceneActionTriggered();
  void onFileAddVolumeActionTriggered();
  void onFileAddTransformActionTriggered();
  void onFileSaveSceneActionTriggered();
  void onFileCloseSceneActionTriggered();
  void onEditUndoActionTriggered();
  void onEditRedoActionTriggered();  
  void onViewLayoutConventionalActionTriggered();
  void onViewLayoutFourUpActionTriggered();
  void onViewLayoutDual3DActionTriggered();
  void onViewLayoutOneUp3DActionTriggered();
  void onViewLayoutOneUpRedSliceActionTriggered();
  void onViewLayoutOneUpYellowSliceActionTriggered();
  void onViewLayoutOneUpGreenSliceActionTriggered();
  void onViewLayoutTabbed3DActionTriggered();
  void onViewLayoutTabbedSliceActionTriggered();
  void onViewLayoutCompareActionTriggered();
  void onViewLayoutSideBySideLightboxActionTriggered();
  void onWindowPythonInteractorActionTriggered();

  void onHelpKeyboardShortcutsActionTriggered();
  void onHelpBrowseTutorialsActionTriggered();
  void onHelpInterfaceDocumentationActionTriggered();
  void onHelpSlicerPublicationsActionTriggered();
  void onHelpAboutSlicerQTActionTriggered();

  void onFeedbackReportBugActionTriggered();
  void onFeedbackReportUsabilityIssueActionTriggered();
  void onFeedbackMakeFeatureRequestActionTriggered();

  void onFeedbackCommunitySlicerVisualBlogActionTriggered();
protected:
  qSlicerMainWindow* widget() const;

protected:
  QScopedPointer<qSlicerMainWindowCorePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerMainWindowCore);
  Q_DISABLE_COPY(qSlicerMainWindowCore);
};

#endif
