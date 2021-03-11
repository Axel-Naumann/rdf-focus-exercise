#include <vector>
#include <algorithm>
#include <ROOT/RDataFrame.hxx>

#include "rdfSample.h"
#include "selectMuon.h"
#include "selectTau.h"
#include "buildPairs.h"

template <class RDF>
auto defineGood(RDF &&rdf) {
   return selectTau(selectMuon(rdf));
}

void analysis() {
   ROOT::EnableImplicitMT(); // Tell ROOT you want to go parallel
/*
   auto rdfDYJetsToLL = defineGood(createRDF({"DYJetsToLL.root"}));
   auto rdfW1JetsToLNu = defineGood(createRDF({"W1JetsToLNu.root"}));
   auto rdfW2JetsToLNu = defineGood(createRDF({"W2JetsToLNu.root"}));
   auto rdfW3JetsToLNu = defineGood(createRDF({"W3JetsToLNu.root"}));
   auto rdfTTbar = defineGood(createRDF({"TTbar.root"}));
*/
   auto rdfMC = buildPairs(defineGood(createRDF({"DYJetsToLL.root", "W1JetsToLNu.root", "W2JetsToLNu.root", "W3JetsToLNu.root", "TTbar.root"})));
   auto rdfRun2012_TauPlusX = buildPairs(defineGood(createRDF({"Run2012B_TauPlusX.root", "Run2012C_TauPlusX.root"})));

   ROOT::RDF::TH1DModel model("h", "Pair mass;M[GeV];Events", 20, 20., 120);
   rdfRun2012_TauPlusX.Histo1D(model, "pairM")->DrawClone();
   rdfMC.Histo1D(model, "pairM")->DrawClone("SAME");
   
}
