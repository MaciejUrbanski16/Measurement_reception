#include "PseudoServerContext.h"
#include "RemoteDataHandler.h"
#include "RemoteDataInterpreter.h"

#include <wx/dcclient.h>
#include <wx/panel.h>

#include "boost/algorithm/cxx11/any_of.hpp"

RemoteDataInterpreter::RemoteDataInterpreter()
        : wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(300, 250)) {

    wxMenu *menuFile = new wxMenu;
    menuBar = new wxMenuBar();
    this->SetMenuBar(menuBar);
    wxPoint point(30,50);
    panel = new wxPanel();

    this->Connect(wxEVT_PAINT, wxPaintEventHandler(RemoteDataInterpreter::OnPaint));


    menuFile->Append(1, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxImage::AddHandler(new wxPNGHandler);

    panel1 = new wxPanel(this, wxID_ANY);
    wxButton *button;
    button = new wxButton(panel1, 1000, _T("Connect"), wxPoint(10,10), wxDefaultSize, 0);

    wxButton *button1;
    button = new wxButton(panel1, 1001, _T("Connect"), wxPoint(100,10), wxDefaultSize, 0);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");


    numClients = 0;

    establishServerListening();

    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnHello, this, 1);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnStartButton, this, 101);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnConnectButton, this, wxID_YES);
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnServerEvent, this, static_cast<int>(ConnectionUtils::SERVER_ID));
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnSocketEvent, this, static_cast<int>(ConnectionUtils::SOCKET_ID));
    Connect(1000, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RemoteDataInterpreter::OnStartButton));
}

void RemoteDataInterpreter::OnConnectButton(wxCommandEvent& event){

}

void RemoteDataInterpreter::OnServerEvent(wxSocketEvent& event)
{
    std::cout<<"On server\n";
    if(event.GetSocketEvent() == wxSOCKET_CONNECTION) {
        std::cout<<"Socet Connection\n";
        wxSocketBase *Sock = sock->Accept(false);
        if (Sock == NULL) {
            std::cout << "Fail to incoming connection\n";
            return;
        }
        Sock->SetEventHandler(*this, static_cast<int>(ConnectionUtils::SOCKET_ID));
        Sock->SetNotify( wxSOCKET_INPUT_FLAG);
        Sock->Notify(true);
    }
}

void RemoteDataInterpreter::OnSocketEvent(wxSocketEvent& event)
{

    std::cout<<"On socket\n";
    std::array<char, 100> buf{};
    std::string in{};
    char buffer[25];
    numOfMsg++;
    if(event.GetSocketEvent() == wxSOCKET_INPUT) {
        std::cout<<"On socket input\n";
        wxSocketBase *Sock = event.GetSocket();
        Sock->Read(buf.data(), buf.max_size());
        RemoteDataHandler remoteDataHandler(buf);

        for (int i = 0; i < 25; i++) {
            std::cout << buf[i];
        }
        std::cout<<"    numMsg:"<< numOfMsg << std::endl;
    }
}


void RemoteDataInterpreter::OnStartButton(wxCommandEvent& event){
    std::cout<< "On connect button" << std::endl;
    std::array<char, 100> buff{"1222 220000 3378976 446789 7 "};
    RemoteDataHandler remoteDataHandler(buff);
    auto vel{remoteDataHandler.getXAcceleration()};
    actualAzimutDeg = remoteDataHandler.getAzimut();
    std::cout<<"Azimut: "<<actualAzimutDeg<<std::endl;
    velocityCalculator.calculateActualVelocity(remoteDataHandler.getXAcceleration(), remoteDataHandler.getTimeIntervalMs());
    std::cout<<"Actual velocity:" << velocityCalculator.getActualVelocity()<<std::endl;
    refreshPanel();
}
void RemoteDataInterpreter::OnPaint(wxPaintEvent & event){
    wxPaintDC dc(this);
    dc.DrawLine(200, 200, 400, 400);
    dc.DrawText("HELLO it's only me!", 600, 100);

    for(int i = 0 ; i < 100; i++)
    {
        dc.DrawPoint(i+12,i+25);
    }
}

void RemoteDataInterpreter::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void RemoteDataInterpreter::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
void RemoteDataInterpreter::OnHello(wxCommandEvent& event)
{
    std::vector<int> path{11,22,33,44,5,6,5};
    const bool isGreaterThan4 = boost::algorithm::any_of(path, [](const int i){return i > 4;});
    wxLogMessage("Hello world from wxWidgets! %d", isGreaterThan4);
}

void RemoteDataInterpreter::initAccepting() {

}

void RemoteDataInterpreter::establishServerListening() {
    wxIPV4address addr;
    addr.AnyAddress();
    addr.Service(portNr);

    sock = new wxSocketServer(addr);

    sock->SetEventHandler(*this, static_cast<int>(ConnectionUtils::SERVER_ID));
    sock->SetNotify(wxSOCKET_CONNECTION_FLAG);
    sock->Notify(true);
}

void RemoteDataInterpreter::refreshPanel() {
    //...
}
