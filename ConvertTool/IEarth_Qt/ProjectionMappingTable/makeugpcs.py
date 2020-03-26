def getnewname(name):
    a = name[1:-1]
    a = "_".join([i.strip() for i in a.split("/")])
    a = "_".join([i.strip() for i in a.split(" ")])
    a = a.replace("WGS_84", "WGS_1984")
    a = a.replace("Gauss-Kruger", "GK")
    a = a.replace("3-degree", "3_Degree")
    a = a.replace("JGD2000", "JGD_2000")
    return a

f = open("pcs.csv")
line = f.readline()
fo = open("ugpcs.csv", "w")

while line:
    #print line
    linearr = line.split(",")
    code = linearr[0]
    name = linearr[1]
    #print code, name, getnewname(name)
    linearr.insert(2, '"'+getnewname(name)+'"')
    fo.write(",".join(linearr))
    line = f.readline()
f.close()
fo.close()
