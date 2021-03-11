#include <cmath>
#include <ROOT/RVec.hxx>

template <class RDF>
auto selectTau(RDF &&rdf)
{
   using cVIr_t = const ROOT::RVec<int> &;
   using cVFr_t = const ROOT::RVec<float> &;
   using cVBr_t = const ROOT::RVec<bool> &;


   auto goodTau = [](cVIr_t charge, cVFr_t eta, cVFr_t pt,
                     cVBr_t decayMode, cVBr_t isoTight, cVBr_t antiEleTight, cVBr_t antiMuTight) {
      return ROOT::RVec<int>{(charge != 0 && abs(eta) < 2.3 && pt > 20 && decayMode && isoTight && antiEleTight && antiMuTight)};
   };

   return rdf.Define("goodTau", goodTau, {"Tau_charge", "Tau_eta", "Tau_pt", "Tau_idDecayMode", "Tau_idIsoTight", "Tau_idAntiEleTight", "Tau_idAntiMuTight"});
}