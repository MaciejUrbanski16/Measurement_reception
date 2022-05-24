#ifndef SERVER_FOR_TRACKER_POSITIONPLOTVIEWER_H
#define SERVER_FOR_TRACKER_POSITIONPLOTVIEWER_H

#include <wx/wx.h>
#include <memory>
#include "../mathplot/mathplot.h"

class PositionPlotViewer : public wxFrame {
public:
    PositionPlotViewer(std::vector<double> xCoordinates, std::vector<double> yCoordinates);
    void displayPlot();
    void updatePlot(const std::vector<double>& xCoordinates, const std::vector<double>& yCoordinates);

    ~PositionPlotViewer();

private:
    void OnExit(wxCommandEvent& event);

    std::unique_ptr<mpFXYVector> positionsVectors{nullptr};
    std::unique_ptr<mpWindow> tracePlot{nullptr};
    std::unique_ptr<mpScaleX> xAxisForPositioning{nullptr};
    std::unique_ptr<mpScaleY> yAxisForPositioning{nullptr};
};

#endif //SERVER_FOR_TRACKER_POSITIONPLOTVIEWER_H
