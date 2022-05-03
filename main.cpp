#include <wx/wxprec.h>
#include "boost/asio.hpp"
#ifndef WX_PRECOMP
#include <wx/gdicmn.h>
#endif
#include "RemoteDataInterpreter.h"

class RemoteDataInterpreter;

class PseudoServerTracker : public wxApp
{
public:
    virtual bool OnInit();
};

bool PseudoServerTracker::OnInit()
{
    RemoteDataInterpreter *remoteDataInterpreter = new RemoteDataInterpreter();
    remoteDataInterpreter->Show();

    return true;
}

wxIMPLEMENT_APP(PseudoServerTracker);
