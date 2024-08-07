from subprocess import Popen, PIPE
from glob import glob

def run_command(command):
    process = Popen(command, stdout=PIPE, stderr=PIPE, shell=True)
    stdout, stderr = process.communicate()
    return stdout, stderr

def main():
    datacards = glob("/home/users/aaarora/phys/analysis/vbs-1lep/datacard/datacards/*/*.dat")
    for datacard in datacards:
        base_name = datacard.split("/")[-1]
        year = datacard.split("/")[-2]

        combine_command = f"combine -m 125 -M AsymptoticLimits -d {datacard} --run blind --name {datacard.replace('.dat', '')}"
        print(f"Running combine for {year}/{base_name}")
        stdout, stderr = run_command(combine_command)

        print(stdout.decode())
        print(stderr.decode())

    

if __name__ == "__main__":
    main()