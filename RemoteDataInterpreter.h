//
// Created by Admin on 23.02.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
#define SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H

#include <wx/wx.h>
#include "wx/socket.h"
#include "VelocityCalculator.h"

class PseudoServerContext;

enum class ConnectionUtils : int {
    SOCKET_ID = 101,
    SERVER_ID,
};


class RemoteDataInterpreter : public wxFrame
{
public:
    RemoteDataInterpreter();

    void OnSocketEvent(wxSocketEvent& event);
    void OnServerEvent(wxSocketEvent& event);
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStartButton(wxCommandEvent& event);
    void OnConnectButton(wxCommandEvent& event);

    void OnPaint(wxPaintEvent & event);

    void initAccepting();

    wxMenuBar *menuBar{nullptr};
    wxSocketServer *sock{nullptr};
    wxPanel *panel{nullptr};
    wxPanel* panel1{nullptr};

    int numClients{0};
    int numOfMsg{0};
    constexpr static uint16_t portNr{8000u};

    void establishServerListening();

    float actualAzimutDeg{0.0f};
    VelocityCalculator velocityCalculator;

    void refreshPanel();
};

#endif //SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
