#include "weights.h"

RNode goodRun(lumiMask golden, RNode df){
    auto goldenjson = [golden](unsigned int &run, unsigned int &luminosityBlock){return golden.accept(run, luminosityBlock);};
    return df.Define("goodRun", goldenjson, {"run", "luminosityBlock"})
             .Filter("(!isData) || (isData && goodRun)", "PASSES GOLDEN JSON");
}