#include "PseudoServerContext.h"
#include "RemoteDataHandler.h"
#include "RemoteDataInterpreter.h"

#include <wx/dcclient.h>
#include <wx/panel.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/notebook.h>
#include <random>
#include "mathplot/mathplot.h"


#include "boost/algorithm/cxx11/any_of.hpp"

template<class T = std::mt19937, std::size_t N = T::state_size * sizeof(typename T::result_type)>
T ProperlySeededRandomEngine () {
    std::random_device source;
    auto random_data = std::views::iota(std::size_t(), (N - 1) / sizeof(source()) + 1)
                       | std::views::transform([&](auto){ return source(); });
    std::seed_seq seeds(std::begin(random_data), std::end(random_data));
    return T(seeds);
}

RemoteDataInterpreter::RemoteDataInterpreter()
        : wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(1800, 1200)) {

    wxPoint point(30,50);
    panel = new wxPanel(this, wxID_ANY, {310, 0}, {300, 1000},0x00080000 | wxBORDER_RAISED, "PANEL" );

    this->Connect(wxEVT_PAINT, wxPaintEventHandler(RemoteDataInterpreter::OnPaint));

    menuFile = new wxMenu;
    menuFile->Append(1, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->Append(60, "Eksport to csv", "Eksport measured data to csv format file", false);
    menuFile->Append(61, "Start listening", "Start listening of incoming connection from remote device", false);
    menuFile->Append(62, "Disconnect", "Disconnect from remote device", false);
    menuFile->AppendSeparator();
    menuFile->Append(63, "Start distance measurement", "If distance measurement is not started start it at the moment of button click", false);
    menuFile->Append(64, "Stop distance measurement", "If distance measurement started stop it the moment of button click", false);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxImage::AddHandler(new wxPNGHandler);

    directionVisualisation = new wxBitmap();

    //directionVisualisation->Create(100, 300);


    panel1 = new wxPanel(this, wxID_ANY, {50, 50}, {300,1000},0x00080000 | wxBORDER_RAISED, "PANEL");

    connectButton = new wxButton(panel1, 1000, _T("Connect"), wxPoint(10, 10), wxDefaultSize, 0);
    startDistanceMeasurementButton = new wxButton(panel1, 1001, "Start distance measure", wxPoint(10, 60), wxDefaultSize, 0);
    stopDistanceMeasurementButton = new wxButton(panel1, 1002, "Stop distance measure", wxPoint(10, 110), wxDefaultSize, 0);

    showingPlotType = static_cast<uint8_t>(Plot::POSITION);

    xAxisForPositioning = new mpScaleX(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    yAxisForPositioning = new mpScaleY(wxT("Y"), mpALIGN_LEFT, true);

    timeAxisForVelocity = new mpScaleX("t[ms]", mpALIGN_BOTTOM, true, mpX_TIME);
    velocityAxis = new mpScaleY("v[m/s]", mpALIGN_LEFT, true);

    timeAxisForAcceleration = new mpScaleX("t[ms]", mpALIGN_BOTTOM, true, mpX_TIME);
    accelerationAxis = new mpScaleY("a[m/s2]", mpALIGN_LEFT, true);

    wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    xAxisForPositioning->SetFont(graphFont);
    yAxisForPositioning->SetFont(graphFont);
    xAxisForPositioning->SetDrawOutsideMargins(false);
    yAxisForPositioning->SetDrawOutsideMargins(false);

    timeAxisForVelocity->SetFont((graphFont));
    velocityAxis->SetFont(graphFont);
    timeAxisForVelocity->SetDrawOutsideMargins(false);
    velocityAxis->SetDrawOutsideMargins(false);

    timeAxisForAcceleration->SetFont((graphFont));
    accelerationAxis->SetFont(graphFont);
    timeAxisForAcceleration->SetDrawOutsideMargins(false);
    accelerationAxis->SetDrawOutsideMargins(false);

    tracePlot = new mpWindow(this, -1, wxPoint(800, 10), wxSize(600, 600), wxSUNKEN_BORDER );
    tracePlot->SetMargins(30, 30, 40, 60);

    velocityPlot = new mpWindow(this, -1, wxPoint(800, 800), wxSize(900, 600), wxSUNKEN_BORDER );

    mpFxyVector = new mpFXYVector();
    mpFxyVector->SetData(xDataToPlot,yDataToPlot);
    mpFxyVector->SetContinuity(true);
    wxPen vectorpen(*wxRED, 2, wxSOLID);
    mpFxyVector->SetPen(vectorpen);
    mpFxyVector->SetPen(vectorpen);
    mpFxyVector->SetDrawOutsideMargins(false);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);

    menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");


    numClients = 0;

    establishServerListening();

    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnHello, this, 1);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnExportCsv, this, 60);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStartListening, this, 61);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnDisconnectFromRemoteDevice, this, 62);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStartDistanceMeasurement, this, 63);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStopDistanceMeasurement, this, 64);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnStartButton, this, 101);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnConnectButton, this, wxID_YES);
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnServerEvent, this, static_cast<int>(ConnectionUtils::SERVER_ID));
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnSocketEvent, this, static_cast<int>(ConnectionUtils::SOCKET_ID));
    Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RemoteDataInterpreter::OnStartButton));
    this->Connect(wxID_ANY, wxEVT_PAINT,
                  wxPaintEventHandler(RemoteDataInterpreter::OnPaint),
                  (wxObject*)0, this);
}

void RemoteDataInterpreter::OnExportCsv(wxCommandEvent& event)
{
    std::cout<<"Eksport csv"<<std::endl;
}

void RemoteDataInterpreter::OnStartDistanceMeasurement(wxCommandEvent& event)
{
    std::cout<<"Start distance measurement"<<std::endl;
}

void RemoteDataInterpreter::OnStopDistanceMeasurement(wxCommandEvent& event)
{
    std::cout<<"Stop distance measurement"<<std::endl;
}

void RemoteDataInterpreter::OnStartListening(wxCommandEvent& event)
{
    std::cout<<"Start listening"<<std::endl;
}

void RemoteDataInterpreter::OnDisconnectFromRemoteDevice(wxCommandEvent& event)
{
    std::cout<<"Disconnect remote device"<<std::endl;
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
    if(event.GetEventObject() == connectButton) {
        std::cout << "On connect connectButton" << std::endl;
        std::array<char, 100> buff{"1222 220000 3378976 446789 7 "};
        RemoteDataHandler remoteDataHandler(buff);
        auto vel{remoteDataHandler.getXAcceleration()};
        actualAzimutDeg = remoteDataHandler.getAzimut();
        std::cout << "Azimut: " << actualAzimutDeg << std::endl;
        velocityCalculator.calculateActualVelocity(remoteDataHandler.getXAcceleration(),
                                                   remoteDataHandler.getTimeIntervalMs());
        std::cout << "Actual velocity:" << velocityCalculator.getActualVelocity() << std::endl;
        refreshPanel();
    }
    else if(event.GetEventObject() == startDistanceMeasurementButton)
    {
        std::cout<<"Start distance measusre"<<std::endl;
    }
    else if(event.GetEventObject() == stopDistanceMeasurementButton)
    {
        std::cout<<"Stop distance measusre"<<std::endl;
    }
}

void RemoteDataInterpreter::OnPaint(wxPaintEvent & event){
    dc = new wxBufferedPaintDC(this);
    directionIndicator = new wxBitmap();
    bool res = directionIndicator->LoadFile("tool1.png", wxBITMAP_TYPE_PNG);
    dc->DrawBitmap(*directionIndicator,0, 0, false);
    std::cout<<"Result of load bitmap: " << res << std::endl;

    wxPaintDC dc(this);
    dc.SetTextForeground(wxColour(255, 255, 255));

    dc.DrawBitmap(*directionIndicator, {30, 20}, true);
//    dc.DrawText("HELLO it's only me!", 600, 100);
//
//    for(int i = 0 ; i < 100; i++)
//    {
//        dc.DrawPoint(i+12,i+25);
//    }
//    dc.DrawRectangle({700,100},{300,100});
//    dc.DrawPoint(500,500);
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
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 mt_rand(seed);
    int dice_roll{9};
    this->dice*=2;
    xDataToPlot.push_back(static_cast<double>(dice));
    yDataToPlot.push_back(static_cast<double>(dice*2));
    mpFxyVector->SetData(xDataToPlot,yDataToPlot);

    tracePlot->SetPos(800.0, 800.0);
    switch(showingPlotType)
    {
        case (int)Plot::POSITION:
        {
            preparePositionPlot();

            showingPlotType = static_cast<uint8_t>(Plot::VELOCITY);
            break;
        }
        case (int)Plot::VELOCITY:
        {
            prepareVelocityPlot();

            showingPlotType = static_cast<uint8_t>(Plot::ACCELERATION);
            break;
        }
        case (int)Plot::ACCELERATION:
        {
            prepareAccelerationPlot();

            showingPlotType = static_cast<uint8_t>(Plot::POSITION);
            break;
        }
        default:
        {
            std::cerr<<"Unavailable plot type " << showingPlotType <<  std::endl;
            break;
        }
    }

    std::cout<<"Added point: "<<dice<<"   "<< dice*2 << std::endl;
}

void RemoteDataInterpreter::preparePositionPlot() {
    tracePlot->DelLayer(timeAxisForAcceleration);
    tracePlot->DelLayer(accelerationAxis);
    tracePlot->DelLayer(mpFxyVector);
    mpFxyVector->SetData(xDataToPlot, yDataToPlot);
    tracePlot->AddLayer(xAxisForPositioning);
    tracePlot->AddLayer(yAxisForPositioning);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);
    tracePlot->SetName("TRACE");
    tracePlot->SetColourTheme({40,20,10}, {45,90,25}, {0, 135,90});
    wxColour wxColour(12,2,3);
    tracePlot->Fit();
}

void RemoteDataInterpreter::prepareVelocityPlot() {
    tracePlot->DelLayer(xAxisForPositioning);
    tracePlot->DelLayer(yAxisForPositioning);
    tracePlot->DelLayer(mpFxyVector);
    mpFxyVector->SetData(timeSamples, velocityMperS);
    tracePlot->AddLayer(timeAxisForVelocity);
    tracePlot->AddLayer(velocityAxis);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);
    tracePlot->SetName("VELOCITY");
    tracePlot->Fit();
}

void RemoteDataInterpreter::prepareAccelerationPlot() {
    tracePlot->DelLayer(timeAxisForVelocity);
    tracePlot->DelLayer(velocityAxis);
    tracePlot->DelLayer(mpFxyVector);
    mpFxyVector->SetData(timeStamplesForAcceleration, accelerationMperS2);
    tracePlot->AddLayer(timeAxisForAcceleration);
    tracePlot->AddLayer(accelerationAxis);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);
    tracePlot->SetName("ACCELERATION");
    tracePlot->Fit();
}
