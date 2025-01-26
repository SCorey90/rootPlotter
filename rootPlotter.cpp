#include "rootPlotter.h"
#include <TStyle.h>

// private members

std::vector<double> Plotter::getAxisLimits() {
    double xmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::lowest();
    bool rangeSet = false;

    // Check TH1F objects
    for (auto hist : th1fs) {
        if (!rangeSet) {
            xmin = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
            xmax = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
            ymin = hist->GetMinimum();
            ymax = hist->GetMaximum();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst()));
            xmax = std::max(xmax, hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast()));
            ymin = std::min(ymin, hist->GetMinimum());
            ymax = std::max(ymax, hist->GetMaximum());
        }
    }

    // Check TH1D objects
    for (auto hist : th1ds) {
        if (!rangeSet) {
            xmin = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
            xmax = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
            ymin = hist->GetMinimum();
            ymax = hist->GetMaximum();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst()));
            xmax = std::max(xmax, hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast()));
            ymin = std::min(ymin, hist->GetMinimum());
            ymax = std::max(ymax, hist->GetMaximum());
        }
    }

    // Check TGraph objects
    for (auto graph : tgraphs) {
        if (!rangeSet) {
            xmin = graph->GetXaxis()->GetXmin();
            xmax = graph->GetXaxis()->GetXmax();
            ymin = graph->GetYaxis()->GetXmin();
            ymax = graph->GetYaxis()->GetXmax();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, graph->GetXaxis()->GetXmin());
            xmax = std::max(xmax, graph->GetXaxis()->GetXmax());
            ymin = std::min(ymin, graph->GetYaxis()->GetXmin());
            ymax = std::max(ymax, graph->GetYaxis()->GetXmax());
        }
    }

    // Check TGraphErrors objects
    for (auto graph : tgraphErrors) {
        if (!rangeSet) {
            xmin = graph->GetXaxis()->GetXmin();
            xmax = graph->GetXaxis()->GetXmax();
            ymin = graph->GetYaxis()->GetXmin();
            ymax = graph->GetYaxis()->GetXmax();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, graph->GetXaxis()->GetXmin());
            xmax = std::max(xmax, graph->GetXaxis()->GetXmax());
            ymin = std::min(ymin, graph->GetYaxis()->GetXmin());
            ymax = std::max(ymax, graph->GetYaxis()->GetXmax());
        }
    }

    // Check TProfile objects
    for (auto prof : tprofiles) {
        if (!rangeSet) {
            xmin = prof->GetXaxis()->GetBinLowEdge(prof->GetXaxis()->GetFirst());
            xmax = prof->GetXaxis()->GetBinUpEdge(prof->GetXaxis()->GetLast());
            ymin = prof->GetMinimum();
            ymax = prof->GetMaximum();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, prof->GetXaxis()->GetBinLowEdge(prof->GetXaxis()->GetFirst()));
            xmax = std::max(xmax, prof->GetXaxis()->GetBinUpEdge(prof->GetXaxis()->GetLast()));
            ymin = std::min(ymin, prof->GetMinimum());
            ymax = std::max(ymax, prof->GetMaximum());
        }
    }

    // Check TF1 objects
    for (auto func : tf1s) {
        if (!rangeSet) {
            xmin = func->GetXmin();
            xmax = func->GetXmax();
            ymin = func->GetMinimum();
            ymax = func->GetMaximum();
            rangeSet = true;
        } else {
            xmin = std::min(xmin, func->GetXmin());
            xmax = std::max(xmax, func->GetXmax());
            ymin = std::min(ymin, func->GetMinimum());
            ymax = std::max(ymax, func->GetMaximum());
        }
    }

    // If no objects have been added, return default values
    if (!rangeSet) {
        return {0.0, 1.0, 0.0, 1.0};
    }

    return {xmin, xmax, ymin, ymax};
}

bool Plotter::isPointInLegend(double x, double y) {
    if (!legend) return false;

    // Get legend coordinates in pad coordinates
    double x1NDC = legend->GetX1NDC();
    double x2NDC = legend->GetX2NDC();
    double y1NDC = legend->GetY1NDC();
    double y2NDC = legend->GetY2NDC();

    // Get pad ranges
    std::vector<double> axisLimits = getAxisLimits();
    double xmin = axisLimits[0];
    double xmax = axisLimits[1];
    double ymin = axisLimits[2];
    double ymax = axisLimits[3];

    // Convert NDC coordinates to user coordinates
    double x1 = xmin + (xmax - xmin) * x1NDC;
    double x2 = xmin + (xmax - xmin) * x2NDC;
    double y1 = ymin + (ymax - ymin) * y1NDC;
    double y2 = ymin + (ymax - ymin) * y2NDC;

    // Check if the point is inside the legend box
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

bool Plotter::doesLegendCoverObjects() {
    if (!legend) return false;

    // Check TH1F objects
    for (auto hist : th1fs) {
        for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
            double x = hist->GetXaxis()->GetBinCenter(bin);
            double y = hist->GetBinContent(bin);
            if (isPointInLegend(x, y)) return true;
        }
    }

    // Check TH1D objects
    for (auto hist : th1ds) {
        for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
            double x = hist->GetXaxis()->GetBinCenter(bin);
            double y = hist->GetBinContent(bin);
            if (isPointInLegend(x, y)) return true;
        }
    }

    // Check TGraph objects
    for (auto graph : tgraphs) {
        double* x = graph->GetX();
        double* y = graph->GetY();
        for (int i = 0; i < graph->GetN(); ++i) {
            if (isPointInLegend(x[i], y[i])) return true;
        }
    }

    // Check TGraphErrors objects
    for (auto graph : tgraphErrors) {
        double* x = graph->GetX();
        double* y = graph->GetY();
        for (int i = 0; i < graph->GetN(); ++i) {
            if (isPointInLegend(x[i], y[i])) return true;
        }
    }

    // Check TProfile objects
    for (auto prof : tprofiles) {
        for (int bin = 1; bin <= prof->GetNbinsX(); ++bin) {
            double x = prof->GetXaxis()->GetBinCenter(bin);
            double y = prof->GetBinContent(bin);
            if (isPointInLegend(x, y)) return true;
        }
    }

    // Check TF1 objects
    for (auto func : tf1s) {
        double xmin = func->GetXmin();
        double xmax = func->GetXmax();
        int nPoints = 100; // Number of points to check
        double step = (xmax - xmin) / nPoints;

        for (double x = xmin; x <= xmax; x += step) {
            double y = func->Eval(x);
            if (isPointInLegend(x, y)) return true;
        }
    }

    return false;
}

// public members
Plotter::Plotter(const std::string& canvasName, const std::string& canvasTitle, int width, int height) {
    canvas = new TCanvas(canvasName.c_str(), canvasTitle.c_str(), width, height);
    canvas->SetLeftMargin(marginLeft);
    canvas->SetRightMargin(marginRight);
    canvas->SetBottomMargin(marginBottom);
    canvas->SetTopMargin(marginTop);

    legend = new TLegend(0.7, 0.7, 0.9, 0.9);
}

Plotter::~Plotter() {
    delete canvas;
    delete legend;
    for (auto hist : th1fs) delete hist;
    for (auto hist : th1ds) delete hist;
    for (auto graph : tgraphs) delete graph;
    for (auto graph : tgraphErrors) delete graph;
    for (auto prof : tprofiles) delete prof;
    for (auto func : tf1s) delete func;
}

void Plotter::SetTitle(const std::string& title) {
    for (auto hist : th1fs) hist->SetTitle(title.c_str());
    for (auto hist : th1ds) hist->SetTitle(title.c_str());
    for (auto graph : tgraphs) graph->SetTitle(title.c_str());
    for (auto graph : tgraphErrors) graph->SetTitle(title.c_str());
    for (auto prof : tprofiles) prof->SetTitle(title.c_str());
    for (auto func : tf1s) func->SetTitle(title.c_str());
}

void Plotter::SetXAxisTitle(const std::string& title) {
    for (auto hist : th1fs) hist->GetXaxis()->SetTitle(title.c_str());
    for (auto hist : th1ds) hist->GetXaxis()->SetTitle(title.c_str());
    for (auto graph : tgraphs) graph->GetXaxis()->SetTitle(title.c_str());
    for (auto graph : tgraphErrors) graph->GetXaxis()->SetTitle(title.c_str());
    for (auto prof : tprofiles) prof->GetXaxis()->SetTitle(title.c_str());
    for (auto func : tf1s) func->GetXaxis()->SetTitle(title.c_str());
}

void Plotter::SetYAxisTitle(const std::string& title) {
    for (auto hist : th1fs) hist->GetYaxis()->SetTitle(title.c_str());
    for (auto hist : th1ds) hist->GetYaxis()->SetTitle(title.c_str());
    for (auto graph : tgraphs) graph->GetYaxis()->SetTitle(title.c_str());
    for (auto graph : tgraphErrors) graph->GetYaxis()->SetTitle(title.c_str());
    for (auto prof : tprofiles) prof->GetYaxis()->SetTitle(title.c_str());
    for (auto func : tf1s) func->GetYaxis()->SetTitle(title.c_str());
}

void Plotter::SetFont(int font) {
    gStyle->SetTextFont(font);
    gStyle->SetLabelFont(font, "x");
    gStyle->SetLabelFont(font, "y");
    gStyle->SetLabelFont(font, "z");
    gStyle->SetTitleFont(font, "x");
    gStyle->SetTitleFont(font, "y");
    gStyle->SetTitleFont(font, "z");
    gStyle->SetLegendFont(font);
    gStyle->SetStatFont(font);

    // Apply to existing objects
    for (auto hist : th1fs) {
        hist->GetXaxis()->SetLabelFont(font);
        hist->GetYaxis()->SetLabelFont(font);
        hist->GetXaxis()->SetTitleFont(font);
        hist->GetYaxis()->SetTitleFont(font);
        hist->SetTitleFont(font);
    }

    for (auto hist : th1ds) {
        hist->GetXaxis()->SetLabelFont(font);
        hist->GetYaxis()->SetLabelFont(font);
        hist->GetXaxis()->SetTitleFont(font);
        hist->GetYaxis()->SetTitleFont(font);
        hist->SetTitleFont(font);
    }

    for (auto graph : tgraphs) {
        graph->GetXaxis()->SetLabelFont(font);
        graph->GetYaxis()->SetLabelFont(font);
        graph->GetXaxis()->SetTitleFont(font);
        graph->GetYaxis()->SetTitleFont(font);
        graph->GetHistogram()->SetTitleFont(font);
    }

    for (auto graph : tgraphErrors) {
        graph->GetXaxis()->SetLabelFont(font);
        graph->GetYaxis()->SetLabelFont(font);
        graph->GetXaxis()->SetTitleFont(font);
        graph->GetYaxis()->SetTitleFont(font);
        graph->GetHistogram()->SetTitleFont(font);
    }

    for (auto prof : tprofiles) {
        prof->GetXaxis()->SetLabelFont(font);
        prof->GetYaxis()->SetLabelFont(font);
        prof->GetXaxis()->SetTitleFont(font);
        prof->GetYaxis()->SetTitleFont(font);
        prof->SetTitleFont(font);
    }

    for (auto func : tf1s) {
        func->GetXaxis()->SetLabelFont(font);
        func->GetYaxis()->SetLabelFont(font);
        func->GetXaxis()->SetTitleFont(font);
        func->GetYaxis()->SetTitleFont(font);
        func->GetHistogram()->SetTitleFont(font);
    }

    // Apply to legend
    legend->SetTextFont(font);

    // Update the canvas to show changes
    canvas->Update();
}

void Plotter::AddObject(TH1F* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    th1fs.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = th1fDrawOption;
    }
    th1fDrawOptions.push_back(drawOption);

    th1fs.back()->SetLineColor(color);
    th1fs.back()->SetLineWidth(lineWidth);

    th1fs.back()->SetFillColorAlpha(color, fillAlpha);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::AddObject(TH1D* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    th1ds.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = th1dDrawOption;
    }
    th1dDrawOptions.push_back(drawOption);

    th1ds.back()->SetLineColor(color);
    th1ds.back()->SetLineWidth(lineWidth);

    th1ds.back()->SetFillColorAlpha(color, fillAlpha);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::AddObject(TGraph* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    tgraphs.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = tgraphDrawOption;
    }
    tgraphDrawOptions.push_back(drawOption);

    tgraphs.back()->SetMarkerColorAlpha(color, markerAlpha);
    tgraphs.back()->SetMarkerStyle(markerStyle);
    tgraphs.back()->SetMarkerSize(markerSize);

    tgraphs.back()->SetLineColor(color);
    tgraphs.back()->SetLineWidth(lineWidth);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::AddObject(TGraphErrors* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    tgraphErrors.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = tgraphErrorsDrawOption;
    }
    tgraphErrorsDrawOptions.push_back(drawOption);

    tgraphErrors.back()->SetMarkerColorAlpha(color, markerAlpha);
    tgraphErrors.back()->SetMarkerStyle(markerStyle);
    tgraphErrors.back()->SetMarkerSize(markerSize);

    tgraphErrors.back()->SetLineColor(color);
    tgraphErrors.back()->SetLineWidth(lineWidth);

    tgraphErrors.back()->SetFillColorAlpha(color, fillAlpha);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::AddObject(TProfile* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    tprofiles.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = tprofileDrawOption;
    }
    tprofileDrawOptions.push_back(drawOption);

    tprofiles.back()->SetMarkerColorAlpha(color, markerAlpha);
    tprofiles.back()->SetMarkerStyle(markerStyle);
    tprofiles.back()->SetMarkerSize(markerSize);

    tprofiles.back()->SetLineColor(color);
    tprofiles.back()->SetLineWidth(lineWidth);

    tprofiles.back()->SetFillColorAlpha(color, fillAlpha);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::AddObject(TF1* obj, const std::string& name, bool addLegend, bool newColor, std::string drawOption) {
    tf1s.push_back(obj);

    int color;
    if (newColor) {
        color = plotColors[objectCounter % plotColors.size()];
        objectCounter++;
    } else {
        color = plotColors[(objectCounter - 1) % plotColors.size()];
    }

    if (drawOption == "") {
        drawOption = tf1DrawOption;
    }
    tf1DrawOptions.push_back(drawOption);

    tf1s.back()->SetLineColor(color);
    tf1s.back()->SetLineWidth(lineWidth);

    tf1s.back()->SetNpx(nPixels);

    if (addLegend) {
        legend->AddEntry(obj, name.c_str());
    }
}

void Plotter::ShowStats(const std::string& on_off, double xmin, double xmax, double ymin, double ymax) {
    if (on_off == "on") {
        statsBox = true;
        statsXmin = xmin;
        statsXmax = xmax;
        statsYmin = ymin;
        statsYmax = ymax;
    }
    else if (on_off == "off") {
        statsBox = false;
    }
    else {
        std::cerr << "Invalid option for stats box. Use 'on' or 'off'." << std::endl;
        return;
    }
}

void Plotter::SetLegendPosition(double xmin, double xmax, double ymin, double ymax, bool hold) {

    legend->SetX1NDC(xmin);
    legend->SetX2NDC(xmax);
    legend->SetY1NDC(ymin);
    legend->SetY2NDC(ymax);

    if (hold) { manualLegendPosition = true; }
}

// Set default legend positions
void Plotter::SetLegendUpperRight(bool hold) {
    double xmax = 1 - marginRight - 0.02;
    double ymax = 1 - marginTop - 0.02;

    double xmin = xmax - legendWidth;
    double ymin = ymax - legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetLegendUpperCenter(bool hold) {
    double x_center = marginLeft + (1 - marginLeft - marginRight - legendWidth) / 2;

    double xmin = x_center;
    double xmax = xmin + legendWidth;

    double ymax = 1 - marginTop - 0.02;
    double ymin = ymax - legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetLegendUpperLeft(bool hold) {
    double xmin = marginLeft + 0.02;
    double ymax = 1 - marginTop - 0.02;

    double xmax = xmin + legendWidth;
    double ymin = ymax - legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetLegendLowerRight(bool hold) {
    double xmax = 1 - marginRight - 0.02;
    double ymin = marginBottom + 0.02;

    double xmin = xmax - legendWidth;
    double ymax = ymin + legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetLegendLowerCenter(bool hold) {
    double x_center = marginLeft + (1 - marginLeft - marginRight - legendWidth) / 2;

    double xmin = x_center;
    double xmax = xmin + legendWidth;

    double ymin = marginBottom + 0.02;
    double ymax = ymin + legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetLegendLowerLeft(bool hold) {
    double xmin = marginLeft + 0.02;
    double ymin = marginBottom + 0.02;

    double xmax = xmin + legendWidth;
    double ymax = ymin + legendHeight;

    Plotter::SetLegendPosition(xmin, xmax, ymin, ymax, false);

    canvas->Update();

    if (hold) { manualLegendPosition = true; }
}

void Plotter::SetXAxisRange(double xmin, double xmax) {
    for (auto hist : th1fs) hist->GetXaxis()->SetRangeUser(xmin, xmax);
    for (auto hist : th1ds) hist->GetXaxis()->SetRangeUser(xmin, xmax);
    for (auto graph : tgraphs) graph->GetXaxis()->SetRangeUser(xmin, xmax);
    for (auto graph : tgraphErrors) graph->GetXaxis()->SetRangeUser(xmin, xmax);
    for (auto prof : tprofiles) prof->GetXaxis()->SetRangeUser(xmin, xmax);
    for (auto func : tf1s) func->GetXaxis()->SetRangeUser(xmin, xmax);
}

void Plotter::SetYAxisRange(double ymin, double ymax) {
    for (auto hist : th1fs) hist->GetYaxis()->SetRangeUser(ymin, ymax);
    for (auto hist : th1ds) hist->GetYaxis()->SetRangeUser(ymin, ymax);
    for (auto graph : tgraphs) graph->GetYaxis()->SetRangeUser(ymin, ymax);
    for (auto graph : tgraphErrors) graph->GetYaxis()->SetRangeUser(ymin, ymax);
    for (auto prof : tprofiles) prof->GetYaxis()->SetRangeUser(ymin, ymax);
    for (auto func : tf1s) func->GetYaxis()->SetRangeUser(ymin, ymax);
}

void Plotter::CreatePlot() {
    if (objectCounter == 0) {
        std::cout << "Nothing to draw!" << std::endl;
        return;
    }

    // Draw all objects on the same canvas
    bool first = true;

    TPaveStats* stats = nullptr;

    gStyle->SetImageScaling(3.0);

    // Draw TH1F objects
    for (int i=0; i<th1fs.size(); i++) {
        th1fs[i]->Draw(first ? th1fDrawOptions[i].c_str() : (th1fDrawOptions[i]+drawSame).c_str() );
        th1fs[i]->SetTitleSize(titleSize);
        th1fs[i]->SetTitleSize(axisSize, "x");
        th1fs[i]->SetTitleSize(axisSize, "y");
        th1fs[i]->SetLabelSize(axisLabelSize, "x");
        th1fs[i]->SetLabelSize(axisLabelSize, "y");

        if (first && statsBox) {
            canvas->Update();
            stats = (TPaveStats*)th1fs[i]->GetListOfFunctions()->FindObject("stats");
            gStyle->SetOptFit( 1111 );
            if (stats) {
                stats->SetX1NDC(statsXmin);
                stats->SetX2NDC(statsXmax);
                stats->SetY1NDC(statsYmin);
                stats->SetY2NDC(statsYmax);
            }
        } else if (!statsBox) {
            th1fs[i]->SetStats(0);
        }
        first = false;
    }

    // Draw TH1D objects
    for (int i=0; i<th1ds.size(); i++) {
        th1ds[i]->Draw(first ? th1dDrawOptions[i].c_str() : (th1dDrawOptions[i]+drawSame).c_str() );
        th1ds[i]->SetTitleSize(titleSize);
        th1ds[i]->SetTitleSize(axisSize, "x");
        th1ds[i]->SetTitleSize(axisSize, "y");
        th1ds[i]->SetLabelSize(axisLabelSize, "x");
        th1ds[i]->SetLabelSize(axisLabelSize, "y");

        if (first && statsBox) {
            canvas->Update();
            stats = (TPaveStats*)th1ds[i]->FindObject("stats");
            gStyle->SetOptFit( 1111 );
            if (stats) {
                stats->SetX1NDC(statsXmin);
                stats->SetX2NDC(statsXmax);
                stats->SetY1NDC(statsYmin);
                stats->SetY2NDC(statsYmax);
            }
            canvas->Update();
        } else if (!statsBox) {
            th1ds[i]->SetStats(0);
        }

        first = false;
    }

    // Draw TGraph objects
    for (int i=0; i<tgraphs.size(); i++) {
        tgraphs[i]->Draw(first ? (drawAxes+tgraphDrawOptions[i]).c_str() : (tgraphDrawOptions[i]+drawSame).c_str() );
        tgraphs[i]->GetHistogram()->SetTitleSize(titleSize);
        tgraphs[i]->GetHistogram()->SetTitleSize(axisSize, "x");
        tgraphs[i]->GetHistogram()->SetTitleSize(axisSize, "y");
        tgraphs[i]->GetHistogram()->SetLabelSize(axisLabelSize, "x");
        tgraphs[i]->GetHistogram()->SetLabelSize(axisLabelSize, "y");

        first = false;
    }

    // Draw TGraphErrors objects
    for (int i=0; i<tgraphErrors.size(); i++) {
        tgraphErrors[i]->Draw(first ? (drawAxes+tgraphErrorsDrawOptions[i]).c_str() : (tgraphErrorsDrawOptions[i]+drawSame).c_str() );
        tgraphErrors[i]->GetHistogram()->SetTitleSize(titleSize);
        tgraphErrors[i]->GetHistogram()->SetTitleSize(axisSize, "x");
        tgraphErrors[i]->GetHistogram()->SetTitleSize(axisSize, "y");
        tgraphErrors[i]->GetHistogram()->SetLabelSize(axisLabelSize, "x");
        tgraphErrors[i]->GetHistogram()->SetLabelSize(axisLabelSize, "y");

        first = false;
    }

    // Draw TProfile objects
    for (int i=0; i<tprofiles.size(); i++) {
        tprofiles[i]->Draw(first ? (tprofileDrawOptions[i]).c_str() : (tprofileDrawOptions[i]+drawSame).c_str() );
        tprofiles[i]->SetTitleSize(titleSize);
        tprofiles[i]->SetTitleSize(axisSize, "x");
        tprofiles[i]->SetTitleSize(axisSize, "y");
        tprofiles[i]->SetLabelSize(axisLabelSize, "x");
        tprofiles[i]->SetLabelSize(axisLabelSize, "y");

        first = false;
    }

    // Draw TF1 objects
    for (int i=0; i<tf1s.size(); i++) {
        tf1s[i]->Draw(first ? tf1DrawOptions[i].c_str() : (tf1DrawOptions[i]+drawSame).c_str() );
        tf1s[i]->GetHistogram()->SetTitleSize(titleSize);
        tf1s[i]->GetHistogram()->SetTitleSize(axisSize, "x");
        tf1s[i]->GetHistogram()->SetTitleSize(axisSize, "y");
        tf1s[i]->GetHistogram()->SetLabelSize(axisLabelSize, "x");
        tf1s[i]->GetHistogram()->SetLabelSize(axisLabelSize, "y");

        first = false;
    }

    // Automatically place legend if it hasn't been manually positioned
    if (!manualLegendPosition) {
        // Split legend positions into upper and lower
        std::vector<std::function<void(bool)>> upperLegendPositions = {
            [this](bool h) { this->SetLegendUpperRight(h); },
            [this](bool h) { this->SetLegendUpperLeft(h); },
            [this](bool h) { this->SetLegendUpperCenter(h); }
        };
        std::vector<std::function<void(bool)>> lowerLegendPositions = {
            [this](bool h) { this->SetLegendLowerRight(h); },
            [this](bool h) { this->SetLegendLowerLeft(h); },
            [this](bool h) { this->SetLegendLowerCenter(h); }
        };

        // Get the current pad's y range
        std::vector<double> axisLimits = getAxisLimits();
        double originalYmin = axisLimits[2];
        double originalYmax = axisLimits[3];
        double ymin = originalYmin;
        double ymax = originalYmax;

        bool legendCoversObjects = true;
        int positionAttempts = 0;
        int rangeAttempts = 0;

        // First try different positions at original scale
        for (auto& setPosition : upperLegendPositions) {
            setPosition(false);
            legendCoversObjects = doesLegendCoverObjects();
            if (!legendCoversObjects) break;
        }
        if (legendCoversObjects) {
            for (auto& setPosition : lowerLegendPositions) {
                setPosition(false);
                legendCoversObjects = doesLegendCoverObjects();
                if (!legendCoversObjects) break;
            }
        }

        // If no suitable position found, start alternating between increasing ymax and ymin
        while (legendCoversObjects && rangeAttempts < 10) {
            if (rangeAttempts % 2 == 0) {
                // Try increasing ymax
                ymax = originalYmax * (1.0 + (0.1 * ((rangeAttempts/2) + 1)));
                ymin = originalYmin;
                SetYAxisRange(ymin, ymax);

                // Check upper positions
                for (auto& setPosition : upperLegendPositions) {
                    setPosition(false);
                    legendCoversObjects = doesLegendCoverObjects();
                    if (!legendCoversObjects) {
                        break;
                    }
                }
            } else {
                // Try increasing ymin
                ymin = originalYmin * (1.0 + (0.1 * ((rangeAttempts/2) + 1)));
                ymax = originalYmax;
                SetYAxisRange(ymin, ymax);

                // Check lower positions
                for (auto& setPosition : lowerLegendPositions) {
                    setPosition(false);
                    legendCoversObjects = doesLegendCoverObjects();
                    if (!legendCoversObjects) {
                        break;
                    }
                }
            }

            if (!legendCoversObjects) break;
            rangeAttempts++;
        }
    }

    if (showLegend) {
        auto * legendtodraw = new TLegend(legend->GetX1NDC(), legend->GetY1NDC(), legend->GetX2NDC(), legend->GetY2NDC());

        TList* entries = legend->GetListOfPrimitives();
        if (!entries) {
            std::cerr << "Error: Source legend has no entries!" << std::endl;
            return;
        }

        // Iterate through the entries and add them to the target legend
        for (TObject* obj : *entries) {
            TLegendEntry* entry = dynamic_cast<TLegendEntry*>(obj);
            if (entry) {
                // Retrieve the object, label, and option from the entry
                TObject* object = entry->GetObject();
                const char* label = entry->GetLabel();
                const char* option = entry->GetOption();

                // Add the entry to the target legend
                legendtodraw->AddEntry(object, label, option);
            }
        }

        legendtodraw->Draw();
    }

    if (stats) {
        stats->Draw();
    }

    canvas->Update();

}
