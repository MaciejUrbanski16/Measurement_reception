//
// Created by Admin on 23.02.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
#define SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H

#include <wx/wx.h>
#include "wx/socket.h"
#include <wx/dcbuffer.h>
#include "mathplot/mathplot.h"
#include "VelocityCalculator.h"

class PseudoServerContext;

enum class ConnectionUtils : int {
    SOCKET_ID = 101,
    SERVER_ID,
};

enum class Plot : uint8_t {
    POSITION = 200,
    ACCELERATION,
    VELOCITY,
    NONE
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
    void OnExportCsv(wxCommandEvent& event);
    void OnStartListening(wxCommandEvent &event);
    void OnDisconnectFromRemoteDevice(wxCommandEvent &event);
    void OnStartDistanceMeasurement(wxCommandEvent &event);
    void OnStopDistanceMeasurement(wxCommandEvent &event);

    void OnPaint(wxPaintEvent & event);

    void establishServerListening();
    void refreshPanel();

    void initAccepting();

    wxMenuBar *menuBar{nullptr};
    wxMenu *menuFile{nullptr};
    wxMenu *menuHelp{nullptr};
  //  wxMenuBar *menuBar{nullptr};
    wxSocketServer *sock{nullptr};
    wxPanel *panel{nullptr};
    wxPanel *panel1{nullptr};
    wxButton *connectButton{nullptr};
    wxButton *startDistanceMeasurementButton{nullptr};
    wxButton *stopDistanceMeasurementButton{nullptr};
    wxBitmap *directionIndicator{nullptr};
    wxBufferedPaintDC *dc{nullptr};
    wxBitmap *directionVisualisation{nullptr};
    mpWindow *actualPlot{nullptr};
    mpWindow *tracePlot{nullptr};
    mpWindow *velocityPlot{nullptr};
    mpWindow *accelerationPlot{nullptr};
    mpInfoCoords *nfo{nullptr};
    mpFXYVector *mpFxyVector{nullptr};

    mpScaleX* xAxisForPositioning{nullptr};
    mpScaleY* yAxisForPositioning{nullptr};

    mpScaleX* timeAxisForVelocity{nullptr};
    mpScaleY* velocityAxis{nullptr};

    mpScaleX* timeAxisForAcceleration{nullptr};
    mpScaleY* accelerationAxis{nullptr};

   // RemotePositionDrawer remotePositionDrawer;

    int numClients{0};
    int numOfMsg{0};
    constexpr static uint16_t portNr{8000u};

    float actualAzimutDeg{0.0f};
    VelocityCalculator velocityCalculator;


    int dice =1;
    bool clicked{false};
    uint8_t showingPlotType{static_cast<uint8_t>(Plot::NONE)};

    std::vector<double> xDataToPlot{1.2,128.3,13.4,918.5,-234.7};
    std::vector<double> yDataToPlot{5.5,525.61,53.7,5.8,543.9};

    std::vector<double> timeSamples{0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0};
    std::vector<double> velocityMperS{67.6, 231.7, 190.0, 428.0, 8.0 ,456.7, 122.4, 78.9 ,22.1, 543.1};

    std::vector<double> timeStamplesForAcceleration{0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0};
    std::vector<double> accelerationMperS2{67.6, 428.0, 190.0, 428.0, 428.0 ,456.7, 428.0, 428.0 ,428.0, 543.1};

    void preparePositionPlot();

    void prepareVelocityPlot();

    void prepareAccelerationPlot();

};

#endif //SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
