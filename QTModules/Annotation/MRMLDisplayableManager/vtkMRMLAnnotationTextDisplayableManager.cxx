// AnnotationModule includes
#include "MRMLDisplayableManager/vtkMRMLAnnotationTextDisplayableManager.h"
#include "Logic/vtkSlicerAnnotationModuleLogic.h"

// AnnotationModule/MRML includes
#include "vtkMRMLAnnotationTextNode.h"

// VTK includes
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkTextRepresentation.h>
#include <vtkTextWidget.h>
#include <vtkRenderer.h>

// std includes
#include <string>

// Convenient macro
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//---------------------------------------------------------------------------
vtkStandardNewMacro (vtkMRMLAnnotationTextDisplayableManager);
vtkCxxRevisionMacro (vtkMRMLAnnotationTextDisplayableManager, "$Revision: 1.0 $");

//---------------------------------------------------------------------------
// vtkMRMLAnnotationTextDisplayableManager Callback
class vtkAnnotationTextWidgetCallback : public vtkCommand
{
public:
  static vtkAnnotationTextWidgetCallback *New()
  { return new vtkAnnotationTextWidgetCallback; }

  virtual void Execute (vtkObject *caller, unsigned long event, void*)
  {
    if (event == vtkCommand::HoverEvent)
    {
      std::cout << "HoverEvent\n";
    }
  }
  vtkAnnotationTextWidgetCallback(){}
};

//---------------------------------------------------------------------------
// vtkMRMLAnnotationTextDisplayableManager methods

//---------------------------------------------------------------------------
void vtkMRMLAnnotationTextDisplayableManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
/// Create a new text widget.
vtkAbstractWidget * vtkMRMLAnnotationTextDisplayableManager::CreateWidget(vtkMRMLAnnotationNode* node)
{
  if (!this->IsCorrectDisplayableManager())
    {
    // jump out
    return 0;
    }

  if (!node)
    {
    vtkErrorMacro("CreateWidget: Node not set!")
    return 0;
    }

  vtkMRMLAnnotationTextNode* textNode = vtkMRMLAnnotationTextNode::SafeDownCast(node);

  if (!textNode)
    {
    vtkErrorMacro("CreateWidget: Could not get text node!")
    return 0;
    }

  vtkTextWidget* textWidget = vtkTextWidget::New();
  VTK_CREATE(vtkTextRepresentation, textRep);

  textRep->SetMoving(1);

  if (textNode->GetTextLabel())
    {
    textRep->SetText(textNode->GetTextLabel());
    }
  else
    {
    textRep->SetText("New text");
    }

  textWidget->SetRepresentation(textRep);

  textWidget->SetInteractor(this->GetInteractor());


  if (textNode->GetTextCoordinates())
    {

    textRep->SetPosition(textNode->GetTextCoordinates()[0],textNode->GetTextCoordinates()[1]);
    //textRep->SetPosition(0.01, 0.01);

    }

  textWidget->On();

  // add callback
  vtkAnnotationTextWidgetCallback *myCallback = vtkAnnotationTextWidgetCallback::New();
  textWidget->AddObserver(vtkCommand::HoverEvent, myCallback);
  myCallback->Delete();

  return textWidget;

}

//---------------------------------------------------------------------------
/// Propagate MRML properties to an existing text widget.
void vtkMRMLAnnotationTextDisplayableManager::SetWidget(vtkMRMLAnnotationNode* node)
{
  if (!this->IsCorrectDisplayableManager())
    {
    // jump out
    return;
    }

  vtkMRMLAnnotationTextNode* textNode = vtkMRMLAnnotationTextNode::SafeDownCast(node);

  vtkAbstractWidget* textWidget = this->GetWidget(textNode);
  if (!textWidget) {
    vtkErrorMacro("Widget was not found!");
    return;
  }

  // Obtain associated representation
  vtkTextRepresentation* textRepr =
      vtkTextRepresentation::SafeDownCast(textWidget->GetRepresentation());

  // Update Text
  textRepr->SetText(textNode->GetText(0));
}

//---------------------------------------------------------------------------
/// Tear down the widget creation
void vtkMRMLAnnotationTextDisplayableManager::OnWidgetCreated()
{

  if (!this->IsCorrectDisplayableManager())
    {
    // jump out
    return;
    }

  // nothing yet
}

//---------------------------------------------------------------------------
/// Create a annotationMRMLnode
void vtkMRMLAnnotationTextDisplayableManager::OnClickInThreeDRenderWindow(double x, double y)
{

  if (!this->IsCorrectDisplayableManager())
    {
    // jump out
    return;
    }

  // place the seed where the user clicked
  this->PlaceSeed(x,y);

  if (this->m_ClickCounter->HasEnoughClicks(1))
    {

    this->GetRenderer()->DisplayToNormalizedDisplay(x,y);
    this->GetRenderer()->NormalizedDisplayToViewport(x,y);
    this->GetRenderer()->ViewportToNormalizedViewport(x,y);

    double coordinates[3];
    coordinates[0]=(double)x;
    coordinates[1]=(double)y;
    coordinates[2]=0;

    vtkMRMLAnnotationTextNode *textNode = vtkMRMLAnnotationTextNode::New();
    textNode->SetTextCoordinates(coordinates);
    textNode->SetTextLabel("New text");

    textNode->Initialize(this->GetMRMLScene());

    // need a unique name since the storage node will be named from it
    if (textNode->GetScene())
      {
      textNode->SetName(textNode->GetScene()->GetUniqueNameByString("AnnotationText"));
      }
    else
      {
      textNode->SetName("AnnotationText");
      }

    textNode->Delete();

    }

  }