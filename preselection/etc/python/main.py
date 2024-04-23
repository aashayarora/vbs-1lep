import ROOT
# from cluster import *
from selections import *
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--use_condor", action="store_true")
    parser.add_argument("--config", type=str, default="configs/input.yaml")
    args = parser.parse_args()

    # in the future perhaps :( 
    # client = get_client(condor=args.use_condor)
    # spec = ROOT.DataSpecFromJSON(args.config)
    # RDataFrame = ROOT.RDF.Experimental.Distributed.Dask.RDataFrame

    # ROOT.EnableImplicitMT(32)
    # df = ROOT.RDF.Experimental.FromSpec(args.config)
    # ROOT.RDF.Experimental.AddProgressBar(df)

    # df = df.DefinePerSample("xsec_weight", 'rdfsampleinfo_.GetD("xsec_weight")')
    # df = df.DefinePerSample("sample_category", 'rdfsampleinfo_.GetS("sample_category")')
    # df = df.DefinePerSample("sample_type", 'rdfsampleinfo_.GetS("sample_type")')
    # df = df.DefinePerSample("sample_year", 'rdfsampleinfo_.GetS("sample_year")')

    # df1 = selections(df)

    # df.Report().Print()