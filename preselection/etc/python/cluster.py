from dask_jobqueue.htcondor import HTCondorCluster
from dask.distributed import Client, LocalCluster

_CLUSTER = None
_CLIENT = None

def get_cluster(condor=False):
    global _CLUSTER
    if _CLUSTER is None:
        if condor:
            print("Using HTCondorCluster")
            _CLUSTER = HTCondorCluster(cores=2, 
                        memory="1GB", 
                        disk="1GB",
                        death_timeout='60',
                        nanny=False,
                        job_extra_directives={
                            "+DESIRED_Sites": "T2_US_UCSD",
                        },
                        log_directory="./log")
            _CLUSTER.scale(jobs=2)
        else:  
            print("Using LocalCluster")
            _CLUSTER = LocalCluster(n_workers=1, threads_per_worker=1)
    return _CLUSTER

def get_client(condor=False):
    global _CLIENT
    if _CLIENT is None:
        _CLIENT = Client(get_cluster(condor=condor))
    return _CLIENT