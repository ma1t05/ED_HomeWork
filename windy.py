from random import randint,uniform
import numpy
import math

costo_min = 5
costo_max = 10
beneficio_min = 1
beneficio_max = 50

class Edge:
    def __init__(self,a,b):
        if(a > b):
            (a,b) = (b,a)
        self.i = a
        self.j = b
        self.c_ij = uniform(costo_min, costo_max)
        self.c_ji = uniform(costo_min, costo_max)
        self.beneficio = uniform(beneficio_min, beneficio_max)
        self.interaccion = 0.0

class Node:
    def __init__(self,label):
        self.label = label
        self.position = numpy.array([uniform(-10,10),uniform(-10,10)])
        self.delta = numpy.array([0,0])
        self.an = set()
    def __str__(self):
        return str(self.position[0]) + ' ' + str(self.position[1])

def crea_grafo_conexo(nodos,aristas):
    E = dict()
    uf = range(nodos);
    while(sum(uf) > 0):
        i = randint(0,nodos-1)
        j = randint(0,nodos-1)
        if(i != j and uf[i] != uf[j]):
#            print "Agrega arco (",i,j,")",uf
            a = Edge(i,j)
            menor = min(uf[i],uf[j])
            mayor = max(uf[i],uf[j])
            E[a.i * nodos + a.j] = a;
            for pos in xrange(nodos):
                if(uf[pos] == mayor):
                    uf[pos] = menor
#    print "Se agregaron n - 1 aristas"
    aristas -= nodos - 1
    while(aristas > 0):
        i = randint(0,nodos-1)
        j = randint(0,nodos-1)
        if(i > j):
            (i,j) = (j,i)
        if(i != j and (i * nodos + j) not in E):
#            print "Agrega arco (",i,j,")"
            a = Edge(i,j)
            E[a.i * nodos + a.j] = a
            aristas-=1
    return E            

def plot_graph(nodes,E,num):
    nodes_file = open('nodes.dat','w')
    edges_file = open('edges.dat','w')
    for n in nodes:
        i = nodes[n]
        nodes_file.write(str(i))
        nodes_file.write('\n')
        for j in i.an:
            m = j.label
            if n < m :
                j = nodes[m]
                diff = j.position - i.position
                s = str(i) + ' ' + str(diff[0]) + ' ' + str(diff[1]) + '\n'
                edges_file.write(s)

    nodes_file.close()
    edges_file.close()

    # GnuPlot commands
    tmp = open('tmp.gp','w')
    tmp.write('set term pngcairo \n')
    s = 'set output \'graph_' +str(num)+'.png\'\n'
    tmp.write(s)
    s = 'set title "Grafo con '+str(len(nodes))+' nodos y '+str(len(E))+' aristas"\n'
    tmp.write(s)
    tmp.write('unset key \n')
    tmp.write('unset border \n')
    tmp.write('unset yzeroaxis \n')
    tmp.write('unset xtics \n')
    tmp.write('unset ytics \n')
    tmp.write('unset ztics \n')
    tmp.write('set style fill transparent solid 1.0 \n')
    tmp.write('plot \'edges.dat\' using 1:2:3:4 with vectors nohead linecolor rgb "black", \'nodes.dat\' using 1:2 with points pt 7 lc rgb "blue" \n')
    tmp.close()

    from os import system, remove
    system('gnuplot tmp.gp')
    remove('nodes.dat')
    remove('edges.dat')
    remove('tmp.gp')

def interaccion(i,j):
    return 1-len(i.an.intersection(j.an))/len(i.an.union(j.an))

# SPRING Force Layout
def spring_force_layout(E,nodos):
    nodes = dict()
    for e in E:
        edge = E[e]
        #    print e,"(",edge.i,edge.j,")"
        if edge.i not in nodes :
            i = Node(edge.i)
            nodes[edge.i] = i
        else:
            i = nodes[edge.i]
        if edge.j not in nodes :
            j = Node(edge.j)
            nodes[edge.j] = j
        else:
            j = nodes[edge.j]
        i.an.add(j)
        j.an.add(i)
        
    for e in E:
        edge = E[e]
        i = nodes[edge.i]
        j = nodes[edge.i]
        edge.interaccion = interaccion(i,j)

    c4 = 0.1
    r1 = 1.0
    r2 = 3.0
    r3 = 5.0
    Iteraciones = 1000
    for x in xrange(Iteraciones):
        for n in nodes:
            i = nodes[n]
            i.delta = numpy.array([0,0])
            for m in nodes:
                if m > n :
                    j = nodes[m]
                    diff = i.position - j.position
                    d = numpy.linalg.norm(diff) 
                    inter = interaccion(i,j)
                    if j not in i.an:
                        if d < r3 :
                            i.delta += inter * r2 * diff / d
                            j.delta -= inter * r2 * diff / d
                    else:
                        if d < r1 * inter:
                            i.delta += inter * r2 * diff / d
                            j.delta -= inter * r2 * diff / d
                        else :
                            if d > r3 :
                                i.delta -= inter * r2 *diff / (2*d)
                                j.delta += inter * r2 *diff / (2*d)
        for n in nodes:
            i = nodes[n]
            i.position += c4*i.delta
    return nodes

num = 1
for nodos in xrange(5):
    n = 10 * nodos + 10
    aristas = n + randint(1,n*(n-3)/2)
    E = crea_grafo_conexo(n,aristas)
    nodes = spring_force_layout(E,n)
    plot_graph(nodes,E,num)
    print "Termina grafo ",num
    num+=1
