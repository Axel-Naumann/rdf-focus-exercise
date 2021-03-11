#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PtEtaPhiM4D.h>
#include <Math/GenVector/VectorUtil.h>

template <class RDF>
auto buildPairs(RDF &&rdf)
{
   using cVIr_t = const ROOT::RVec<int> &;
   using cVFr_t = const ROOT::RVec<float> &;
   using cVBr_t = const ROOT::RVec<bool> &;
   using LV = ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float>>;
   auto matchMuTau = [](cVIr_t goodMu, cVFr_t muPhi, cVFr_t muEta, cVFr_t muPt, cVFr_t muM, cVIr_t muCharge,
                        cVIr_t goodTau, cVFr_t tauPhi, cVFr_t tauEta, cVFr_t tauPt, cVFr_t tauM, cVIr_t tauCharge, cVFr_t tauRelIso_all)
   {
      struct Pair {
         float fMuPt;
         float fTauIso;
         LV fPair4V;
         Pair(float pt, float iso, const LV &pair4v):
         fMuPt(pt), fTauIso(iso), fPair4V(pair4v) {}
         bool operator<(const Pair &other) const {
            return fMuPt < other.fMuPt || (fMuPt == other.fMuPt && fTauIso > other.fTauIso);
         }
      };

      std::vector<Pair> pairs;
      for (int iMu = 0, nMu = muPhi.size(); iMu < nMu; ++iMu) {
         if (!goodMu[iMu])
            continue;
         LV mu4(muPt[iMu], muEta[iMu], muPhi[iMu], muM[iMu]);
         for (int iTau = 0, nTau =tauPhi.size(); iTau < nTau; ++iTau) {
            if (!goodTau[iTau])
               continue;
            if (std::isnan(tauRelIso_all[iTau])) // missing isnan(RVec)
               continue;
            if (muCharge[iMu] == tauCharge[iTau])
               continue;
            LV tau4(tauPt[iTau], tauEta[iTau], tauPhi[iTau], tauM[iTau]);
            if (ROOT::Math::VectorUtil::DeltaR(tau4, mu4) <= 0.5)
               continue;
            pairs.emplace_back(muPt[iMu], tauRelIso_all[iTau], mu4 + tau4);
         }
      }
      ROOT::RVec<float> pairM;
      if (!pairs.empty()) {
         std::sort(pairs.begin(), pairs.end());
         pairM = ROOT::RVec<float>{pairs.back().fPair4V.M()};
      }
      return pairM;
   };
   return rdf.Define("pairM", matchMuTau,
      {"goodMuons", "Muon_phi", "Muon_eta", "Muon_pt", "Muon_mass", "Muon_charge",
       "goodTau", "Tau_phi", "Tau_eta", "Tau_pt", "Tau_mass", "Tau_charge", "Tau_relIso_all"});
}