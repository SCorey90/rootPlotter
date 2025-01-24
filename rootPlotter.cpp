#include "rootPlotter.h"
#include <TStyle.h>

// private members


bool Plotter::isPointInLegend(double x, double y) {
    if (!legend) return false;

    // Get legend coordinates in NDC
    double legendX1 = legend->GetX1NDC();
    double legendX2 = legend->GetX2NDC();
    double legendY1 = legend->GetY1NDC();
    double legendY2 = legend->GetY2NDC();

    // Get the current pad's ranges
    double xmin = canvas->GetUxmin();
    double xmax = canvas->GetUxmax();
    double ymin = canvas->GetUymin();
    double ymax = canvas->GetUymax();

    // Convert point from axis coordinates to NDC coordinates
    double xNDC = marginLeft + (1 - marginLeft - marginRight) * (x - xmin) / (xmax - xmin);
    double yNDC = marginBottom + (1 - marginBottom - marginTop) * (y - ymin) / (ymax - ymin);

    // Check if the point is inside the legend box
    return (xNDC >= legendX1 && xNDC <= legendX2 && yNDC >= legendY1 && yNDC <= legendY2);
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
    // if (!manualLegendPosition) {
    //     void (Plotter::*legendPositions[])(bool) = {
    //         &Plotter::SetLegendUpperRight,
    //         &Plotter::SetLegendUpperLeft,
    //         &Plotter::SetLegendUpperCenter,
    //         &Plotter::SetLegendLowerRight,
    //         &Plotter::SetLegendLowerLeft,
    //         &Plotter::SetLegendLowerCenter
    //     };

    //     bool foundGoodPosition = false;
    //     double currentYmin, currentYmax;
    //     double expansionFactor = 0.1; // Start with 10%
    //     int maxAttempts = 10; // Maximum number of attempts (5 up, 5 down)
    //     int attempt = 0;

    //     // Store original axis ranges
    //     if (!th1fs.empty()) {
    //         currentYmin = th1fs[0]->GetYaxis()->GetXmin();
    //         currentYmax = th1fs[0]->GetYaxis()->GetXmax();
    //     } else if (!th1ds.empty()) {
    //         currentYmin = th1ds[0]->GetYaxis()->GetXmin();
    //         currentYmax = th1ds[0]->GetYaxis()->GetXmax();
    //     } else if (!tgraphs.empty()) {
    //         currentYmin = tgraphs[0]->GetYaxis()->GetXmin();
    //         currentYmax = tgraphs[0]->GetYaxis()->GetXmax();
    //     } else if (!tgraphErrors.empty()) {
    //         currentYmin = tgraphErrors[0]->GetYaxis()->GetXmin();
    //         currentYmax = tgraphErrors[0]->GetYaxis()->GetXmax();
    //     } else if (!tprofiles.empty()) {
    //         currentYmin = tprofiles[0]->GetYaxis()->GetXmin();
    //         currentYmax = tprofiles[0]->GetYaxis()->GetXmax();
    //     } else if (!tf1s.empty()) {
    //         currentYmin = tf1s[0]->GetYaxis()->GetXmin();
    //         currentYmax = tf1s[0]->GetYaxis()->GetXmax();
    //     }

    //     // Store original ranges
    //     double originalYmin = currentYmin;
    //     double originalYmax = currentYmax;
    //     double delta = (currentYmax - currentYmin) * expansionFactor;

    //     while (!foundGoodPosition && attempt < maxAttempts) {
    //         // Try all positions with current axis range
    //         for (auto position : legendPositions) {
    //             (this->*position)(false);
    //             canvas->Update();
    //             if (!doesLegendCoverObjects()) {
    //                 foundGoodPosition = true;
    //                 break;
    //             }
    //         }

    //         if (!foundGoodPosition) {
    //             // Reset to original range first
    //             currentYmin = originalYmin;
    //             currentYmax = originalYmax;

    //             if (attempt % 2 == 0) {
    //                 // Even attempts: expand upper limit
    //                 currentYmax = originalYmax + delta * (1 + attempt/2);
    //                 std::cout << "Trying with upper limit expanded by "
    //                          << (expansionFactor * (1 + attempt/2) * 100)
    //                          << "%" << std::endl;
    //             } else {
    //                 // Odd attempts: expand lower limit
    //                 currentYmin = originalYmin - delta * (1 + attempt/2);
    //                 std::cout << "Trying with lower limit expanded by "
    //                          << (expansionFactor * (1 + attempt/2) * 100)
    //                          << "%" << std::endl;
    //             }

    //             // Set new ranges for all objects
    //             SetYAxisRange(currentYmin, currentYmax);
    //             attempt++;
    //         }
    //     }

    //     if (!foundGoodPosition) {
    //         SetLegendUpperRight(false);
    //         // Reset to original range
    //         SetYAxisRange(originalYmin, originalYmax);
    //         std::cout << "Warning: Could not find legend position even after expanding axis range" << std::endl;
    //     }
    // }

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
