#ifndef SERVER_FOR_TRACKER_VELOCITYPLOTVIEWER_H
#define SERVER_FOR_TRACKER_VELOCITYPLOTVIEWER_H

#include <wx/wx.h>
#include <memory>
#include "../mathplot/mathplot.h"

class VelocityPlotViewer : public wxFrame {
public:
    VelocityPlotViewer(std::vector<double> xCoordinates, std::vector<double> yCoordinates);
    void displayPlot();
    void updatePlot(const std::vector<double>& xCoordinates, const std::vector<double>& yCoordinates);

    ~VelocityPlotViewer();

private:
    void OnExit(wxCommandEvent& event);

    std::unique_ptr<mpFXYVector> positionsVectors{nullptr};
    std::unique_ptr<mpWindow> tracePlot{nullptr};
    std::unique_ptr<mpScaleX> timeAxis{nullptr};
    std::unique_ptr<mpScaleY> velocityAxis{nullptr};
};

#endif //SERVER_FOR_TRACKER_VELOCITYPLOTVIEWER_H
