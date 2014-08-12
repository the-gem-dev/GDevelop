/** \file
 *  Game Develop
 *  2008-2014 Florian Rival (Florian.Rival@gmail.com)
 */

#ifndef JSCODEEVENT_H
#define JSCODEEVENT_H
#include "GDCore/Events/Event.h"
namespace gd { class Instruction; }
namespace gd { class MainFrameWrapper; }
namespace gd { class Project; }
namespace gd { class SerializerElement; }
namespace gd { class EventsEditorItemsAreas; }
namespace gd { class EventsEditorSelection; }
namespace gd { class Layout; }
class wxWindow;

namespace gdjs
{

/**
 * \brief Builtin internal event that pick an object of a list each time it is repeated
 */
class JsCodeEvent : public gd::BaseEvent
{
public:
    JsCodeEvent();
    JsCodeEvent(const JsCodeEvent & event);
    virtual ~JsCodeEvent() {};

    JsCodeEvent& operator=(const JsCodeEvent & event);
    virtual gd::BaseEvent * Clone() const { return new JsCodeEvent(*this);}

    virtual bool IsExecutable() const {return true;}
    virtual bool CanHaveSubEvents() const {return false;}

    void EnsureAssociatedSourceFileIsUpToDate(gd::Project & project, std::string outputDir) const;

    const std::vector<std::string> & GetIncludeFiles() const { return includeFiles; };
    void SetIncludeFiles(const std::vector<std::string> & include_) { includeFiles = include_; };

    const std::string & GetInlineCode() const { return inlineCode; };
    void SetInlineCode(const std::string & code) { inlineCode = code; };

    const std::string & GetFunctionToCall() const;
    void SetFunctionToCall(const std::string & functionToCall_) { functionToCall = functionToCall_; };

    bool GetPassSceneAsParameter() const { return passSceneAsParameter; };
    void SetPassSceneAsParameter(bool passScene) { passSceneAsParameter = passScene; };

    bool GetPassObjectListAsParameter() const { return passObjectListAsParameter; };
    void SetPassObjectListAsParameter(bool passObjectList) { passObjectListAsParameter = passObjectList; };

    std::string GetObjectToPassAsParameter() const { return objectToPassAsParameter; };
    void SetObjectToPassAsParameter(std::string objectName) { objectToPassAsParameter = objectName; };

    bool IsCodeDisplayedInEditor() const { return codeDisplayedInEditor; };
    void EnableCodeDisplayedInEditor(bool enable) { codeDisplayedInEditor = enable; eventHeightNeedUpdate = true; };

    const std::string & GetDisplayedName() const { return displayedName; };
    void SetDisplayedName(const std::string & displayedName_) { displayedName = displayedName_; };

    virtual void SerializeTo(gd::SerializerElement & element) const;
    virtual void UnserializeFrom(gd::Project & project, const gd::SerializerElement & element);

    /**
     * Called by event editor to draw the event.
     */
    virtual void Render(wxDC & dc, int x, int y, unsigned int width, gd::EventsEditorItemsAreas & areas, gd::EventsEditorSelection & selection, const gd::Platform & platform);

    /**
     * Must return the height of the event when rendered
     */
    virtual unsigned int GetRenderedHeight(unsigned int width, const gd::Platform & platform) const;

    /**
     * Called when the user want to edit the event
     */
    virtual EditEventReturnType EditEvent(wxWindow* parent_, gd::Project & game_, gd::Layout & scene_, gd::MainFrameWrapper & mainFrameWrapper_);

private:
    void Init(const JsCodeEvent & event);
    std::string GenerateAssociatedFileCode() const;
    void AutogenerateFunctionName() const;

    std::vector<std::string> includeFiles;
    mutable std::string functionToCall; ///< The name of the function to call (Generated by AutogenerateFunctionName)
    std::string inlineCode; ///< Contains the Javacript code of the event, which is inserted into the associated GD-managed source file
    mutable std::string associatedGDManagedSourceFile; ///< "Inline" JS code is simulated by a call to a function containing the code, function located in a separate source file managed by GD.
    time_t lastChangeTimeStamp; ///< Time of the last change. Used to check if the associated source file must be regenerated.

    bool passSceneAsParameter;
    bool passObjectListAsParameter;
    std::string objectToPassAsParameter;

    bool codeDisplayedInEditor;
    std::string displayedName;

    bool objectsToPickSelected;
};

}

#endif // JSCODEEVENT_H