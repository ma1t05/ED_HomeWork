from random import choice

class Vertice:
    def __init__(self,etiqueta):
        self.etiqueta = etiqueta
        self.vecinos = set()
        self.marcado = False
    def __str__(self):
        return self.etiqueta
    def __repr__(self):
        return str(self)
    

def dfs_iter(inicio):
    pila = [inicio]
    while len(pila) > 0:
        actual = pila.pop(0)
        if actual.marcado:
            continue
        print "Visita en", actual
        actual.marcado = True
        for vecino in actual.vecinos:
            pila.insert(0,vecino)
    return

def dfs_rec(actual):
    if actual.marcado:
        return
    print "Visita en", actual
    actual.marcado = True
    for vecino in actual.vecinos:
        dfs_rec(vecino)
    
def dfs_rec(actual):
    if actual.marcado:
        return
    print "Visita en", actual
    actual.marcado = True
    for vecino in actual.vecinos:
        dfs_rec(vecino)
    return

aristas = ["ad","ac","ae","bc","bf","be","de","fc"]
vertices = dict()

for arista in aristas:
    inicio = arista[0]
    final = arista[1]
    v = None
    w = None
    if inicio not in vertices:
        v = Vertice(inicio)
        vertices[inicio] = v
    else:
        v = vertices[inicio]
    if final not in vertices:
        w = Vertice(final)
        vertices[final] = w
    else:
        w = vertices[final]
    v.vecinos.add(w)
    w.vecinos.add(v)

print "DFS Iterativo"
dfs_iter(choice(vertices.values()))

for inicio in vertices:
    v = vertices[inicio]
    v.marcado = False

print "DFS Recursivo"
dfs_rec(choice(vertices.values()))
