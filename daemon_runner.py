import subprocess
import json
import os
import csv

def mk_data():
    fn="fleet_telemetry.csv"
    h=["id","tmp","sec","err"]
    d=[
        [101,42,2,0],
        [102,48,5,1],
        [103,59,75,14],
        [104,41,0,0],
        [105,62,88,19],
        [106,58,60,12]
    ]
    with open(fn,"w",newline="") as f:
        w=csv.writer(f)
        w.writerow(h)
        w.writerows(d)
    return fn

def comp():
    subprocess.run(["g++","-std=c++17","-pthread","daemon_core.cpp","-o","daemon_core"],check=True)

def run():
    if not os.path.exists("daemon_core"):comp()
    csv_f=mk_data()
    res=subprocess.run(["./daemon_core",csv_f],capture_output=True,text=True)
    rep={
        "pipeline_status":"success",
        "dataset":csv_f,
        "anomalies_detected":res.stdout.strip().split("\n") if res.stdout.strip() else []
    }
    print(json.dumps(rep,indent=2))

if __name__=="__main__":
    run()