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

    this->Connect(wxEVT_PAINT, wxPaintEventHandler(RemoteDataInterpreter::OnPaint));

    menuFile = new wxMenu;
    menuFile->Append(1, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu statisticOfMeasurement");
    menuFile->Append(60, "Eksport to csv", "Eksport measured data to csv format file", false);
    menuFile->Append(61, "Start listening", "Start listening of incoming connection from remote device", false);
    menuFile->Append(62, "Disconnect", "Disconnect from remote device", false);
    menuFile->AppendSeparator();
    menuFile->Append(63, "Start distance measurement", "If distance measurement is not started start it at the moment of button click", false);
    menuFile->Append(64, "Stop distance measurement", "If distance measurement started stop it the moment of button click", false);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    menuPlot = new wxMenu;
    menuPlot->Append(65, "Change plot", "Change displayed plot to next one", false);
    menuPlot->Append(66, "Choose plot", "Choose plot from available plots", false);
    menuPlot->Append(67, "Fit", "Fit all plot to size of window plotting", false);
    submenuPlotChoose = new wxMenu;
    submenuPlotChoose->Append(68, "Position plot");
    submenuPlotChoose->Append(69, "Acceleration plot");
    submenuPlotChoose->Append(70, "Velocity plot");
    menuPlot->AppendSubMenu(submenuPlotChoose, "Choose plot", "Choose plot to show");
    menuPlot->AppendSeparator();
    menuPlot->Append(71, "Set interval measurement", "Set time interval of measurement in the remote device ms", false);
    //show window of setup??

    menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuPlot, "&Plot");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    setupTimeInterval = new wxTextCtrl(this, wxID_ANY, "value", {300, 900}, {180, 60}, 0);

    listOfMeasuredDistances = new wxListCtrl(this, 901, {300, 0}, {400, 600}, wxLC_REPORT);
    listOfMeasuredDistances->InsertColumn(0, "Start time", wxLIST_FORMAT_LEFT);
    listOfMeasuredDistances->InsertColumn(1, "Stop time", wxLIST_FORMAT_LEFT);
    listOfMeasuredDistances->InsertColumn(2, "Total time", wxLIST_FORMAT_LEFT);
    listOfMeasuredDistances->InsertColumn(3, "Distance", wxLIST_FORMAT_LEFT);

    statisticOfMeasurement     = new wxListItem();

    statisticOfMeasurement->SetBackgroundColour(*wxRED);
    statisticOfMeasurement->SetText(wxT("Basic statistics"));
    statisticOfMeasurement->SetId(0);

    listOfMeasuredDistances->InsertItem(0, *statisticOfMeasurement);
    listOfMeasuredDistances->InsertItem(1, *statisticOfMeasurement);
    listOfMeasuredDistances->InsertItem(2, *statisticOfMeasurement);
    listOfMeasuredDistances->InsertItem(3, *statisticOfMeasurement);
    listOfMeasuredDistances->InsertItem(4, *statisticOfMeasurement);

    listOfMeasuredDistances->SetItem(0, 0, wxT("1"), -1);
    listOfMeasuredDistances->SetItem(0, 1, wxT("1"), -1);
    listOfMeasuredDistances->SetItem(0, 2, wxT("23"), -1);
    listOfMeasuredDistances->SetItem(0, 3, wxT("8:34"), -1);
    listOfMeasuredDistances->SetItem(1, 0, wxT("2"), -1);
    listOfMeasuredDistances->SetItem(1, 1, wxT("2"), -1);
    listOfMeasuredDistances->SetItem(2, 0, wxT("3"), -1);
    listOfMeasuredDistances->SetItem(2, 1, wxT("3"), -1);
    listOfMeasuredDistances->SetItem(3, 0, wxT("4"), -1);
    listOfMeasuredDistances->SetItem(3, 1, wxT("4"), -1);
    listOfMeasuredDistances->SetItem(4, 1, wxT("Maciej"), -1);
    actualIndexInMeasurementTable = 5u;


    wxImage::AddHandler(new wxPNGHandler);

    directionVisualisation = new wxBitmap();


    mainControlPanel = new wxPanel(this, wxID_ANY, {0, 0}, {300, 600}, 0x00080000 | wxBORDER_RAISED, "PANEL");

    connectButton = new wxButton(mainControlPanel, 1000, _T("Connect"), wxPoint(10, 10), wxDefaultSize, 0);
    startDistanceMeasurementButton = new wxButton(mainControlPanel, 1001, "Start distance measure", wxPoint(10, 60), {160, 30}, 0);
    stopDistanceMeasurementButton = new wxButton(mainControlPanel, 1002, "Stop distance measure", wxPoint(10, 110), {160, 30}, 1);

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

    tracePlot = new mpWindow(this, -1, wxPoint(700, 0), wxSize(600, 600), wxSUNKEN_BORDER );
    tracePlot->SetMargins(30, 30, 40, 60);

    mpFxyVector = new mpFXYVector();
    mpFxyVector->SetData(xCoordinatesOfRelativePosition, yCoordinatesOfRelativePosition);
    mpFxyVector->SetContinuity(true);
    wxPen vectorpen(*wxRED, 2, wxSOLID);
    mpFxyVector->SetPen(vectorpen);
    mpFxyVector->SetPen(vectorpen);
    mpFxyVector->SetDrawOutsideMargins(false);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);

    numClients = 0;

    prepareVelocityPlot();
    establishServerListening();

    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnHello, this, 1);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnChangeToPositionPlot, this, 59);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnExportCsv, this, 60);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStartListening, this, 61);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnDisconnectFromRemoteDevice, this, 62);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStartDistanceMeasurement, this, 63);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnStopDistanceMeasurement, this, 64);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnSetTimeInterval, this, 71);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &RemoteDataInterpreter::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnStartButton, this, 101);
    Bind(wxEVT_BUTTON, &RemoteDataInterpreter::OnConnectButton, this, wxID_YES);
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnServerEvent, this, static_cast<int>(ConnectionUtils::SERVER_ID));
    Bind(wxEVT_SOCKET, &RemoteDataInterpreter::OnSocketEvent, this, static_cast<int>(ConnectionUtils::SOCKET_ID));
    Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RemoteDataInterpreter::OnStartButton));
    Connect(64, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RemoteDataInterpreter::OnStopDistanceMeasurement));
    this->Connect(wxID_ANY, wxEVT_PAINT,
                  wxPaintEventHandler(RemoteDataInterpreter::OnPaint),
                  (wxObject*)0, this);
}

void RemoteDataInterpreter::OnSetTimeInterval(wxCommandEvent& event)
{
    const char *const text = static_cast<const char *>(setupTimeInterval->GetValue());
    if(not isNumber(text)) {
        wxMessageBox("The given value is not integral. Write value of correct type, please",
                     "About Hello World", wxOK | wxICON_ERROR);
    }
    else {
        updateTimeMeasurementIntervalMs(text);
        std::cout << timeMeasurementIntervalMs << std::endl;
    }
}

void RemoteDataInterpreter::OnExportCsv(wxCommandEvent& event)
{
    std::cout<<"Eksport csv"<<std::endl;
}

void RemoteDataInterpreter::OnStartDistanceMeasurement(wxCommandEvent& event)
{
    startDistanceMeasurement();
    std::cout<<"Start distance measurement"<<std::endl;
}

void RemoteDataInterpreter::OnStopDistanceMeasurement(wxCommandEvent& event)
{
    stopDistanceMeasurement();
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
        if (Sock == nullptr) {
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
        //this is implementation of first step remote data handling
        std::cout << "On connect connectButton" << std::endl;
        std::array<char, 100> buff{"1800 220 33 89 7 "};
        RemoteDataHandler remoteDataHandler(buff);
        actualAzimutDeg = remoteDataHandler.getAzimut();
        updateDataToPlotAcceleration(remoteDataHandler);
        velocityCalculator.calculateActualVelocity(remoteDataHandler.getXAcceleration(),
                                                   remoteDataHandler.getTimeIntervalMs());
        updateDataToPlotVelocity(remoteDataHandler);

        relativePositionCalculator.setPreviousRelativePosition({0.0,16.0});
        relativePositionCalculator.calculateActualRelativePosition(velocityCalculator.getActualVelocity(),
                                                                   remoteDataHandler.getTimeIntervalMs(),
                                                                   remoteDataHandler.getAzimut());
        updateDataToPlotRelativePosition(remoteDataHandler);
        if(isStartedMeasurementDistance){
            updateMeasuredDistance(velocityCalculator.getActualVelocity());
        }
        refreshPanel();
    }
    else if(event.GetEventObject() == startDistanceMeasurementButton)
    {
        startDistanceMeasurement();
        std::cout<<"Start distance measusre"<<std::endl;
    }
    else if(event.GetEventObject() == stopDistanceMeasurementButton)
    {
        stopDistanceMeasurement();
        std::cout<<"Stop distance measusre"<<std::endl;
    }
}

void RemoteDataInterpreter::OnPaint(wxPaintEvent & event){
    dc = new wxBufferedPaintDC(this);
    directionIndicator = new wxBitmap();
    bool res = directionIndicator->LoadFile("tool1.png", wxBITMAP_TYPE_PNG);
    dc->DrawBitmap(*directionIndicator,800, 900, false);
    std::cout<<"Result of load bitmap: " << res << std::endl;

    wxPaintDC dc(this);
    dc.SetTextForeground(wxColour(255, 255, 255));

    dc.DrawBitmap(*directionIndicator, {600, 900}, true);
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

void RemoteDataInterpreter::OnChangeToPositionPlot(wxCommandEvent& event) {
    showingPlotType = static_cast<uint8_t>(Plot::POSITION);
    refreshPanel();
    std::cout<<"OnChangeToPositionPlot"<<std::endl;
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
    mpFxyVector->SetData(xCoordinatesOfRelativePosition, yCoordinatesOfRelativePosition);
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
}

void RemoteDataInterpreter::preparePositionPlot() {
    tracePlot->DelLayer(timeAxisForAcceleration);
    tracePlot->DelLayer(accelerationAxis);
    tracePlot->DelLayer(mpFxyVector);
    mpFxyVector->SetData(xCoordinatesOfRelativePosition, yCoordinatesOfRelativePosition);
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
    mpFxyVector->SetData(timeSamples, accelerationMperS2);
    tracePlot->AddLayer(timeAxisForAcceleration);
    tracePlot->AddLayer(accelerationAxis);
    tracePlot->AddLayer(mpFxyVector);
    tracePlot->SetMPScrollbars(true);
    tracePlot->SetName("ACCELERATION");
    tracePlot->Fit();
}

bool RemoteDataInterpreter::isNumber(const std::string& s)
{
    return !s.empty() &&
            std::find_if(s.begin(),
                    s.end(), [](unsigned char c)
                         {return !std::isdigit(c); }) == s.end();
}

void RemoteDataInterpreter::updateTimeMeasurementIntervalMs(const char *const text) {
    timeMeasurementIntervalMs = atoi(text);
}

void RemoteDataInterpreter::updateMeasuredDistance(long long int velocity) {
    measuredDistance += velocity/timeMeasurementIntervalMs;
}

void RemoteDataInterpreter::updateMeasurementsTable(std::string startTime, std::string stopTime, std::string totalTime,
                                                    std::string distance) {
    listOfMeasuredDistances->InsertItem(actualIndexInMeasurementTable, *statisticOfMeasurement);

    listOfMeasuredDistances->SetItem(actualIndexInMeasurementTable, 0, startTime, -1);
    listOfMeasuredDistances->SetItem(actualIndexInMeasurementTable, 1, stopTime, -1);
    listOfMeasuredDistances->SetItem(actualIndexInMeasurementTable, 2, totalTime, -1);
    listOfMeasuredDistances->SetItem(actualIndexInMeasurementTable, 3, distance, -1);
    actualIndexInMeasurementTable++;
}

void RemoteDataInterpreter::startDistanceMeasurement() {
    if(isStartedMeasurementDistance){
        wxMessageBox("Function distance measurement has already been started!",
                     "About Measurement distance", wxOK | wxICON_INFORMATION);
    }
    else {
        isStartedMeasurementDistance = true;
    }
}

void RemoteDataInterpreter::stopDistanceMeasurement() {
    if(not isStartedMeasurementDistance)
    {
        wxMessageBox("Function distance measurement has already been stopped!",
                     "About Measurement distance", wxOK | wxICON_INFORMATION);
    }
    else{
        std::string startTime{"Maciej"};
        std::string stopTime{"Maciej"};
        std::string totalTime{"Maciej"};
        std::string distance{"Maciej"};
        updateMeasurementsTable(startTime, stopTime, totalTime, distance);

        isStartedMeasurementDistance = false;
    }

}

void RemoteDataInterpreter::updateDataToPlotAcceleration(const RemoteDataHandler &remoteDataHandler) {
    if(timeSamples.empty()){
        const auto actualTime{remoteDataHandler.getTimeIntervalMs()};
        timeSamples.push_back(actualTime);
    }
    else{
        const auto actualTime{timeSamples.back() + remoteDataHandler.getTimeIntervalMs()};
        timeSamples.push_back(actualTime);
    }
    accelerationMperS2.push_back(remoteDataHandler.getXAcceleration());
}

void RemoteDataInterpreter::updateDataToPlotVelocity(const RemoteDataHandler &remoteDataHandler) {
    velocityMperS.push_back(velocityCalculator.getActualVelocity());
    std::cout << "Actual velocity:" << velocityCalculator.getActualVelocity() << std::endl;
}

void RemoteDataInterpreter::updateDataToPlotRelativePosition(const RemoteDataHandler &handler) {
    const auto actualRelativePosition = relativePositionCalculator.getCalculatedRelativePosition();
    xCoordinatesOfRelativePosition.push_back(actualRelativePosition.first);
    yCoordinatesOfRelativePosition.push_back(actualRelativePosition.second);
    std::cout<< "DEB Calculated relative position x: " << actualRelativePosition.first
             << " y: " << actualRelativePosition.second << "Azimut: " << handler.getAzimut() << std::endl;
}
