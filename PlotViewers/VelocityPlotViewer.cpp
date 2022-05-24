#include "VelocityPlotViewer.h"

VelocityPlotViewer::VelocityPlotViewer(std::vector<double> xCoordinates, std::vector<double> yCoordinates)
        : wxFrame(NULL, wxID_ANY, "Velocity plot", wxDefaultPosition, wxSize(1800, 1200)) {

    Bind(wxEVT_MENU, &VelocityPlotViewer::OnExit, this, wxID_EXIT);

    timeAxis = std::make_unique<mpScaleX>(wxT("time[s]"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    velocityAxis = std::make_unique<mpScaleY>(wxT("velocity[m/s]"), mpALIGN_LEFT, true);

    tracePlot = std::make_unique<mpWindow>(this, -1, wxPoint(900, 0), wxSize(600, 600), wxSUNKEN_BORDER );
    tracePlot->SetMargins(30, 30, 40, 60);
    wxPen vectorPen(*wxRED, 2, wxSOLID);

    positionsVectors = std::make_unique<mpFXYVector>();
    positionsVectors->SetData(xCoordinates, yCoordinates);
    positionsVectors->SetContinuity(true);
    positionsVectors->SetPen(vectorPen);
    positionsVectors->SetPen(vectorPen);
    positionsVectors->SetDrawOutsideMargins(false);

    tracePlot->AddLayer(timeAxis.get());
    tracePlot->AddLayer(velocityAxis.get());
    tracePlot->AddLayer(positionsVectors.get());
    tracePlot->SetMPScrollbars(true);

    tracePlot->SetName("VELOCITY");
    tracePlot->SetColourTheme({70,20,20}, {25,70,15}, {20, 135,90});
    wxColour wxColour(12,2,3);
    tracePlot->Fit();
}

void VelocityPlotViewer::OnExit(wxCommandEvent& event)
{
    Close(true);
}

VelocityPlotViewer::~VelocityPlotViewer() {
    tracePlot->DelLayer(timeAxis.get());
    tracePlot->DelLayer(velocityAxis.get());
    tracePlot->DelLayer(positionsVectors.get());
}

void VelocityPlotViewer::updatePlot(const std::vector<double> &xCoordinates, const std::vector<double> &yCoordinates) {

    tracePlot->DelLayer(positionsVectors.get());
    positionsVectors->SetData(xCoordinates, yCoordinates);
    tracePlot->AddLayer(positionsVectors.get());
//    tracePlot->SetMPScrollbars(true);
//    tracePlot->SetName("TRACE");
//    tracePlot->SetColourTheme({40,20,10}, {45,90,25}, {0, 135,90});
    tracePlot->Fit();
}

void VelocityPlotViewer::displayPlot() {

}
