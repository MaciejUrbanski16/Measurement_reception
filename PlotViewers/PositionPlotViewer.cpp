#include "PositionPlotViewer.h"

PositionPlotViewer::PositionPlotViewer(std::vector<double> xCoordinates, std::vector<double> yCoordinates)
        : wxFrame(NULL, wxID_ANY, "Sub windo for plot", wxDefaultPosition, wxSize(1800, 1200)) {

    Bind(wxEVT_MENU, &PositionPlotViewer::OnExit, this, wxID_EXIT);

    xAxisForPositioning = std::make_unique<mpScaleX>(wxT("X"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    yAxisForPositioning = std::make_unique<mpScaleY>(wxT("Y"), mpALIGN_LEFT, true);

    tracePlot = std::make_unique<mpWindow>(this, -1, wxPoint(900, 0), wxSize(600, 600), wxSUNKEN_BORDER );
    tracePlot->SetMargins(30, 30, 40, 60);
    wxPen vectorPen(*wxRED, 2, wxSOLID);

    positionsVectors = std::make_unique<mpFXYVector>();
    positionsVectors->SetData(xCoordinates, yCoordinates);
    positionsVectors->SetContinuity(true);
    positionsVectors->SetPen(vectorPen);
    positionsVectors->SetPen(vectorPen);
    positionsVectors->SetDrawOutsideMargins(false);

    tracePlot->AddLayer(xAxisForPositioning.get());
    tracePlot->AddLayer(yAxisForPositioning.get());
    tracePlot->AddLayer(positionsVectors.get());
    tracePlot->SetMPScrollbars(true);

     tracePlot->SetName("TRACE");
     tracePlot->SetColourTheme({40,20,10}, {45,90,25}, {0, 135,90});
     wxColour wxColour(12,2,3);
     tracePlot->Fit();
}

void PositionPlotViewer::updatePlot(const std::vector<double>& xCoordinates, const std::vector<double>& yCoordinates) {

    tracePlot->DelLayer(positionsVectors.get());
    positionsVectors->SetData(xCoordinates, yCoordinates);
    tracePlot->AddLayer(positionsVectors.get());
//    tracePlot->SetMPScrollbars(true);
//    tracePlot->SetName("TRACE");
//    tracePlot->SetColourTheme({40,20,10}, {45,90,25}, {0, 135,90});
    tracePlot->Fit();
}

void PositionPlotViewer::OnExit(wxCommandEvent& event)
{
   Close(true);
}

PositionPlotViewer::~PositionPlotViewer() {
    tracePlot->DelLayer(xAxisForPositioning.get());
    tracePlot->DelLayer(yAxisForPositioning.get());
    tracePlot->DelLayer(positionsVectors.get());
}
