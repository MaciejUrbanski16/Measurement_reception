//
// Created by Admin on 23.02.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
#define SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H

#include <wx/wx.h>
#include "wx/socket.h"
#include "mathplot/mathplot.h"
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

    void establishServerListening();
    void refreshPanel();

    void initAccepting();

    wxMenuBar *menuBar{nullptr};
    wxSocketServer *sock{nullptr};
    wxPanel *panel{nullptr};
    wxPanel *panel1{nullptr};
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

   // RemotePositionDrawer remotePositionDrawer;

    int numClients{0};
    int numOfMsg{0};
    constexpr static uint16_t portNr{8000u};



    float actualAzimutDeg{0.0f};
    VelocityCalculator velocityCalculator;


    int dice =1;
    bool clicked{false};


    std::vector<double> xDataToPlot{1.2,128.3,13.4,918.5,-234.7};
    std::vector<double> yDataToPlot{5.5,525.61,53.7,5.8,543.9};

    std::vector<double> timeSamples{0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0};
    std::vector<double> velocityMperS{67.6, 231.7, 190.0, 428.0, 8.0 ,456.7, 122.4, 78.9 ,22.1, 543.1};

    std::vector<double> timeStamplesForAcceleration{0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 900.0};
    std::vector<double> accelerationMperS2{67.6, 428.0, 190.0, 428.0, 428.0 ,456.7, 428.0, 428.0 ,428.0, 543.1};
};

#endif //SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
