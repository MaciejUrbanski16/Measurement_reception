#ifndef SERVER_FOR_TRACKER_PLOTVIEWERVISITOR_H
#define SERVER_FOR_TRACKER_PLOTVIEWERVISITOR_H

#include "PositionPlotViewer.h"
#include "VelocityPlotViewer.h"

class PlotViewerVisitor {
public:
    void operator()(PositionPlotViewer& positionPlotViewer, std::vector<double> xCoordinates, std::vector<double> yCoordinates) const
    {
        positionPlotViewer.updatePlot(xCoordinates, yCoordinates);
    }

    void operator()(VelocityPlotViewer& velocityPlotViewer, std::vector<double> timeStamps, std::vector<double> velocityMperS) const
    {
        velocityPlotViewer.updatePlot(timeStamps, velocityMperS);
    }
};


#endif //SERVER_FOR_TRACKER_PLOTVIEWERVISITOR_H
