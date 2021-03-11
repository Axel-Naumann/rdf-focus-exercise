#include <cmath>
#include <ROOT/RVec.hxx>

template <class RDF>
auto selectMuon(RDF &&rdf)
{
   using cVFr_t = const ROOT::RVec<float> &;
   using cVBr_t = const ROOT::RVec<int> &;

   auto goodMuon = [](cVFr_t eta, cVFr_t pt, cVBr_t tightId) {
      // Return RVec<bool> and adjust buildPairs(), and hell breaks loose.
      return ROOT::RVec<int>{(abs(eta) < 2.1 && pt > 17 && tightId)};
   };
   return rdf.Define("goodMuons", goodMuon, {"Muon_eta", "Muon_pt", "Muon_tightId"});
}
