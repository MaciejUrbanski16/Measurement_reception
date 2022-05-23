#ifndef SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
#define SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H

#include <wx/wx.h>
#include "wx/socket.h"
#include <wx/dcbuffer.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>
#include "mathplot/mathplot.h"
#include "VelocityCalculator.h"
#include "RelativePositionCalculator.h"
#include "ManualMeasurements.h"
#include "PositionPlotViewer.h"

#include <variant>

class PseudoServerContext;
class RemoteDataHandler;

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
    void OnChangeToPositionPlot(wxCommandEvent &event);
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

    std::variant<std::monostate> plotViewer;
    PositionPlotViewer *plotViewer1;

    wxMenuBar *menuBar{nullptr};
    wxMenu *menuFile{nullptr};
    wxMenu *menuPlot{nullptr};
    wxMenu *submenuPlotChoose{nullptr};
    wxMenu *submenuPlotInNewWindow{nullptr};
    wxMenu *menuHelp{nullptr};
    wxSocketServer *sock{nullptr};
    wxPanel *panel{nullptr};
    wxPanel *mainControlPanel{nullptr};
    wxTextCtrl *setupTimeInterval{nullptr};
    wxListCtrl *listOfMeasuredDistances{nullptr};
    wxButton *connectButton{nullptr};
    wxButton *startDistanceMeasurementButton{nullptr};
    wxButton *stopDistanceMeasurementButton{nullptr};
    wxListItem* statisticOfMeasurement{nullptr};
    wxBitmap *directionIndicator{nullptr};
    wxBufferedPaintDC *dc{nullptr};
    wxBitmap *directionVisualisation{nullptr};
    wxFilePickerCtrl *openFileWithMeasurements{nullptr};
    mpWindow *actualPlot{nullptr};
    mpWindow *tracePlot{nullptr};
    mpWindow *velocityPlot{nullptr};
    mpWindow *accelerationPlot{nullptr};
    mpInfoCoords *nfo{nullptr};
    mpFXYVector *mpFxyVector{nullptr};
    wxTimer   *m_timer;
    std::optional<wxString> pathToSaveMeasurements{std::nullopt};

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
    RelativePositionCalculator relativePositionCalculator;
    ManualMeasurements manualMeasurements{};
    std::vector<ManualMeasurements> collectedManualMeasurements{};

    int dice =1;
    bool isStartedMeasurementDistance{false};
    uint64_t measuredDistance{0};

    uint64_t measuredDistanceInOnePeriodOfManualMeasurements{0};
    uint64_t measuredVelocityInOnePeriondOfManualMeasurements{0};
    uint32_t counterOfManualMeasurementsSamples{0};

    uint8_t showingPlotType{static_cast<uint8_t>(Plot::NONE)};
    uint32_t timeMeasurementIntervalMs{100u};
    uint32_t actualIndexInMeasurementTable{0u};

    std::vector<double> xCoordinatesOfRelativePosition{};
    std::vector<double> yCoordinatesOfRelativePosition{};

    std::vector<double> timeSamples{};
    std::vector<double> velocityMperS{};

    std::vector<double> timeSamplesForAcceleration{};
    std::vector<double> accelerationMperS2{};

    void preparePositionPlot();

    void prepareVelocityPlot();

    void prepareAccelerationPlot();

    void OnSetTimeInterval(wxCommandEvent &event);

    bool isNumber(const std::string &s);

    void updateTimeMeasurementIntervalMs(const char *const text);

    void updateMeasuredDistance(long long int velocity);

    void updateMeasurementsTable();


    void startDistanceMeasurement();

    void stopDistanceMeasurement();

    void updateDataToPlotVelocity(const RemoteDataHandler &handler);

    void updateDataToPlotRelativePosition(const RemoteDataHandler &handler);

    void updateDataToPlotAcceleration(const RemoteDataHandler &handler);

    void OnTimer(wxTimerEvent &event);

    void OnPositionPlotChoose(wxCommandEvent &event);

    void OnAccelerationPlotChoose(wxCommandEvent &event);

    void OnVelocityPlotChoose(wxCommandEvent &event);

    void OnBrowseFile(wxFileDirPickerEvent& event);

    void OnReadManualMeasurements(wxCommandEvent &event);

    void OnWriteManualMeasurements(wxCommandEvent &event);

    bool canMeasurementsBeSaved();

    bool canMeasurementsBeRead();

    void
    concatenateReadMeasurementsWithPreviouslyDoneOnes(const std::vector<ManualMeasurements> &readMeasurementsFromFile);

    bool areDataReceived{false};

    void updateMeasurementsTableWhenReadMeasurements();

    void OnVelocityPlotInNewWindow(wxCommandEvent& event);

    void OnPositionPlotInNewWindow(wxCommandEvent &event);

    void OnAccelerationPlotInNewWindow(wxCommandEvent &event);
};

#endif //SERVER_FOR_TRACKER_REMOTEDATAINTERPRETER_H
