#include <wx/wxprec.h>
#ifndef WX_PRECOMP

#include <wx/dc.h>
#include <wx/gdicmn.h>

#endif

#include "RemoteDataInterpreter.h"
#include "PseudoServerContext.h"

class ReceivedDataContainer
{

};
class RemoteDataInterpreter;

class PseudoServerTracker : public wxApp
{
public:
    virtual bool OnInit();
};

bool PseudoServerTracker::OnInit()
{
    RemoteDataInterpreter *frame = new RemoteDataInterpreter();
    frame->Show();

    return true;
}

wxIMPLEMENT_APP(PseudoServerTracker);
