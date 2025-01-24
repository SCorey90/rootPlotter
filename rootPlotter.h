#ifndef ROOT_PLOTTER_H
#define ROOT_PLOTTER_H

#include "TColor.h"
#include <TH1F.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TF1.h>

#include <TCanvas.h>
#include <TLegend.h>
#include "TLegendEntry.h"
#include "TList.h"
#include <TPaveStats.h>

#include <string>
#include <vector>

class Plotter {
public:
    // Constructor
    Plotter(const std::string& canvasName = "canvas", const std::string& canvasTitle = "", int width = 800, int height = 600);
    // Destructor
    ~Plotter();

    // Get color vector
    std::vector<int> GetColors() { return plotColors; }

    // Titles
    void SetTitle(const std::string& title);
    void SetXAxisTitle(const std::string& title);
    void SetYAxisTitle(const std::string& title);

    void SetTitleSize(double size) { titleSize = size; }
    void SetAxisSize(double size) { axisSize = size; }
    void SetAxisLabelSize(double size) { axisLabelSize = size; }

    //Set Font
    void SetFont(int font=102);

    // Method declarations (moved to source file)
    void AddObject(TH1F* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");
    void AddObject(TH1D* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");
    void AddObject(TGraph* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");
    void AddObject(TGraphErrors* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");
    void AddObject(TProfile* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");
    void AddObject(TF1* obj, const std::string& name, bool addLegend = true, bool newColor = true, std::string drawOption = "");

    // Methods to set style properties
    void SetMarker(int style, int size, double alpha) { markerStyle = style; markerSize = size; markerAlpha = alpha; }
    void SetLineWidth(int width) { lineWidth = width; }
    void SetFillAlpha(double alpha) { fillAlpha = alpha; }
    void SetNPixels(int pixels) { nPixels = pixels; }

    // Method to set draw options
    void SetTH1FDrawOption(const std::string& option) { th1fDrawOption = option; }
    void SetTH1DDrawOption(const std::string& option) { th1dDrawOption = option; }
    void SetTGraphDrawOption(const std::string& option) { tgraphDrawOption = option; }
    void SetTGraphErrorsDrawOption(const std::string& option) { tgraphErrorsDrawOption = option; }
    void SetTProfileDrawOption(const std::string& option) { tprofileDrawOption = option; }
    void SetTF1DrawOption(const std::string& option) { tf1DrawOption = option; }

    // Method to interact with stats box
    void ShowStats(const std::string& on_off="off", double xmin=0.7, double xmax=0.9, double ymin=0.6, double ymax=0.9);

    // Method to set legend position
    void ShowLegend(bool show) { showLegend = show; }
    void SetLegendPosition(double xmin, double xmax, double ymin, double ymax, bool hold=true);
    void SetLegendSize(double width, double height) { legendWidth = width; legendHeight = height; }

    // Legend default locations
    void SetLegendUpperRight(bool hold=true);
    void SetLegendUpperCenter(bool hold=true);
    void SetLegendUpperLeft(bool hold=true);
    void SetLegendLowerRight(bool hold=true);
    void SetLegendLowerCenter(bool hold=true);
    void SetLegendLowerLeft(bool hold=true);

    // Method to set axis ranges
    void SetXAxisRange(double xmin, double xmax);
    void SetYAxisRange(double ymin, double ymax);

    // Method to create the plot
    void CreatePlot();

    // Method to get the plot
    TCanvas* GetPlot() { return canvas; }

private:
    int objectCounter = 0;
    bool incrementColor = true;

    // Colors
    std::vector<int> plotColors = {
        kPink-3, kAzure-7, kOrange+7, kGreen+1, kBlue+2, kViolet, kGray+3, kAzure+7, kYellow-4, kCyan-3, kMagenta-9, kRed, kTeal-8, kOrange+10, kRed-6
    };

    //Axis and title text size defaults
    double titleSize = 0.07;
    double axisSize = 0.05;
    double axisLabelSize = 0.03;

    // Margin Defaults
    double marginLeft = 0.12;
    double marginRight = 0.05;
    double marginBottom = 0.16;
    double marginTop = 0.07;

    // Marker and line styles
    int markerStyle = 20;
    double markerSize = 1;
    double markerAlpha = 0.95;

    int lineWidth = 2;

    double fillAlpha = 0.5;

    double nPixels = 2800;

    // vectors to hold histograms and other objects
    std::vector<TH1F*> th1fs;
    std::vector<TH1D*> th1ds;
    std::vector<TGraph*> tgraphs;
    std::vector<TGraphErrors*> tgraphErrors;
    std::vector<TProfile*> tprofiles;
    std::vector<TF1*> tf1s;

    // draw option strings
    std::string drawSame = " SAME";
    std::string drawAxes = "A";
    std::string th1fDrawOption = "EH";
    std::string th1dDrawOption = "EH";
    std::string tgraphDrawOption = "PL";
    std::string tgraphErrorsDrawOption = "PL E3";
    std::string tprofileDrawOption = "PL E3";
    std::string tf1DrawOption = "";

    std::vector<std::string> th1fDrawOptions;
    std::vector<std::string> th1dDrawOptions;
    std::vector<std::string> tgraphDrawOptions;
    std::vector<std::string> tgraphErrorsDrawOptions;
    std::vector<std::string> tprofileDrawOptions;
    std::vector<std::string> tf1DrawOptions;

    // Stats box settings
    bool statsBox = false;
    double statsXmin = 0.7;
    double statsXmax = 0.9;
    double statsYmin = 0.6;
    double statsYmax = 0.9;

    // Canvas for the plotter
    TCanvas* canvas = nullptr;

    // Legend for the plotter
    bool showLegend = true;
    bool manualLegendPosition = false;
    TLegend* legend = nullptr;

    // Legend location options
    double legendWidth = 0.3;
    double legendHeight = 0.2;

    // Private methods
    bool isPointInLegend(double x, double y);
    bool doesLegendCoverObjects();
};

#endif
