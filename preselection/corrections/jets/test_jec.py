# %%
import correctionlib

# %%
cset = correctionlib.CorrectionSet.from_file("OOG.json")
# %%

cset.get("Summer19UL16APV_V7_MC_FlavorQCD_AK4PFchs").evaluate(-5.2, 10.0)
# %%
