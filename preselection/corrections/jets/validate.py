import json

with open("2017_jerc.json") as f:
    data = json.load(f)

with open("OOGA.json", "w+") as g:
    g.write(json.dumps(data, indent=4))