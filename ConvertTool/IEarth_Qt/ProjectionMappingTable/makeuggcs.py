def getnewname(name):
    a = name
    a = "_".join([i.strip() for i in a.split("/")])
    a = "_".join([i.strip() for i in a.split(" ")])
    a = a.replace("WGS_84", "WGS_1984")
    a = a.replace("Gauss-Kruger", "GK")
    a = a.replace("3-degree", "3_Degree")
    a = a.replace("JGD2000", "JGD_2000")
    if a.startswith('"'):
        return '"GCS_%s"' % a[1:-1]
    else:
        return "GCS_"+a

f = open("gcs.csv")
line = f.readline()
fo = open("uggcs.csv", "w")

while line:
    #print line
    linearr = line.split(",")
    code = linearr[0]
    name = linearr[1]
    #print code, name, getnewname(name)
    linearr.insert(2, getnewname(name))
    fo.write(",".join(linearr))
    line = f.readline()
f.close()
fo.close()
