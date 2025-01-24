#include "rootPlotter.h"
#include <TFile.h>
#include <TRandom3.h>
#include <string>
#include <vector>

int main() {
    // Create a plotter instance
    Plotter plotter("test_canvas", "Test Functions");

    std::cout << "Plotter created." << std::endl;

    // Vector to store our histograms for cleanup
    std::vector<TH1F*> histograms;

    // Random number generator
    TRandom3 rand(12345); // seed for reproducibility

    // Create different histograms in a loop
    for(int i = 0; i < plotter.GetColors().size(); i++) {
        std::string histName = "h" + std::to_string(i);
        // Create histogram with 100 bins from -5 to 5
        TH1F* h = new TH1F(histName.c_str(), "", 100, -5, 30);

        // Fill histogram with Gaussian distribution
        // Mean shifts by 0.5 for each histogram
        // Sigma is constant at 0.8
        double mean = -2.0 + i * 2;
        double sigma = 0.8;

        // Fill with 10000 random points
        for(int j = 0; j < 10000; j++) {
            h->Fill(rand.Gaus(mean, sigma));
        }

        histograms.push_back(h);
        plotter.AddObject(h, "Gaussian " + std::to_string(i));
    }

    std::cout << "Histograms added to plotter." << std::endl;

    // Create the plot
    plotter.SetTitle("Changed Font");
    plotter.SetXAxisTitle("x-axis");
    plotter.SetYAxisTitle("y-axis");
    plotter.SetFont(102);
    plotter.ShowStats("off");
    plotter.SetLegendLowerCenter();
    plotter.CreatePlot();

    std::cout << "Plot created." << std::endl;

    // Save the canvas
    plotter.GetPlot()->SaveAs("../Demonstrations/ColorPaletteDemo.png");

    std::cout << "Plot saved." << std::endl;

    return 0;
}
