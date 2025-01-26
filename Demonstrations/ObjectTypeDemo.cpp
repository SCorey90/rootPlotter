#include "rootPlotter.h"
#include <TRandom3.h>
#include <cmath>

int main() {
    // Create a plotter instance
    Plotter plotter("type_demo", "Different ROOT Object Types");

    // Random number generator
    TRandom3 rand(12345);

    // 1. Create and fill TH1F
    TH1F* h1f = new TH1F("h1f", "Object Type Demo", 50, 0, 10);
    for(int i = 0; i < 10000; i++) {
        h1f->Fill(rand.Gaus(5, 1));
    }
    plotter.AddObject(h1f, "TH1F Gaussian");

    // 2. Create and fill TH1D
    TH1D* h1d = new TH1D("h1d", "Object Type Demo", 50, 0, 10);
    for(int i = 0; i < 10000; i++) {
        h1d->Fill(rand.Exp(2));
    }
    plotter.AddObject(h1d, "TH1D Exponential");

    // 3. Create TGraph
    const int nPoints = 100;
    double x[nPoints], y[nPoints];
    for(int i = 0; i < nPoints; i++) {
        x[i] = i * 0.1;
        y[i] = 100 * sin(x[i]) + 500;
    }
    TGraph* gr = new TGraph(nPoints, x, y);
    plotter.AddObject(gr, "TGraph Sine");

    // 4. Create TGraphErrors
    double x_err[nPoints], y_err[nPoints];
    for(int i = 0; i < nPoints; i++) {
        x_err[i] = 0.05;  // Constant x errors
        y_err[i] = 25;   // Constant y errors
        y[i] = 150 * cos(x[i]) + 500;  // Different function for variety
    }
    TGraphErrors* gre = new TGraphErrors(nPoints, x, y, x_err, y_err);
    plotter.AddObject(gre, "TGraphErrors Cosine");

    // 5. Create and fill TProfile
    TProfile* prof = new TProfile("prof", "Profile", 50, 0, 10);
    for(int i = 0; i < 10000; i++) {
        double x_val = rand.Uniform(0, 10);
        double y_val = 0.5 * x_val + rand.Gaus(0, 0.5) + 300;
        prof->Fill(x_val, y_val);
    }
    plotter.AddObject(prof, "TProfile Linear");

    // 6. Create TF1
    TF1* func = new TF1("func", "[0] + [1]*x", 0, 10);
    func->SetParameters(0, 100);  // Set parameters: offset and slope
    plotter.AddObject(func, "TF1 Linear");

    // Create the plot
    plotter.ShowStats("on", 0.1, 0.3, 0.7, 0.9);
    plotter.SetFont(102);
    // plotter.SetLegendUpperRight();
    plotter.CreatePlot();

    // Save the canvas
    plotter.GetPlot()->SaveAs("../Demonstrations/ObjectTypeDemo.png");

    return 0;
}
