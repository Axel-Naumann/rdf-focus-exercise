#include <algorithm>
#include <vector>
#include <ROOT/RDataFrame.hxx>
#include <TSystem.h>

void snapshotFiltered(const std::vector<std::string> &sample) {
   ROOT::RDataFrame d("Events", sample);
   auto dFiltered = d.Define("isMC", [&]() {return sample.begin()->find("Run") == std::string::npos; }, {})
      .Filter("nMuon > 0 && nTau > 0 && HLT_IsoMu17_eta2p1_LooseIsoPFTau20");
   std::string outName = *sample.begin();
   outName.replace(0, strlen("samples/"), "filtered/");
   dFiltered.Snapshot("FEvents", outName);
   std::cerr << "Snapshotted " << outName << '\n';
}

auto createRDF(const std::vector<std::string> &sample) {
   auto sampleWithPath = sample;
   std::transform(sample.begin(), sample.end(), sampleWithPath.begin(), [](const std::string &s) { return "filtered/" + s; });
   std::cerr << "Checking for " << *sample.begin() << '\n';
   if (gSystem->AccessPathName(sampleWithPath.begin()->c_str())) {
      // Not found, create it.
      std::cerr << "Cannot find " << *sampleWithPath.begin() << '\n';
      auto unfilteredWithPath = sample;
      std::transform(sample.begin(), sample.end(), unfilteredWithPath.begin(), [](const std::string &s) { return "samples/" + s; });
      std::cerr << "Creating " << *sampleWithPath.begin() << " from " << *unfilteredWithPath.begin() << '\n';
      snapshotFiltered(unfilteredWithPath);
   }
   return ROOT::RDataFrame{"FEvents", sampleWithPath};
}

