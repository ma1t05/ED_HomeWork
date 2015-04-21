
def editdist(P,Q):
    dist = [range(len(Q) +1)]
    for i in xrange(len(P)):
        dist.append([i + 1])
    for i in xrange(len(P)):
        for j in xrange(len(Q)):
            dist[i+1].append(min(dist[i][j+1] + 1, \
                                     dist[i+1][j] + 1, \
                                     dist[i][j] + (P[i] != Q[j])))
    for d in dist:
        print d
    return dist[-1][-1]

print editdist("POSGRADO","SUELDO")

print editdist("DIFICIL","FACIL")
