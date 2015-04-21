
/*
  Implementacion de un 'dict' de python
  mediante arboles AVL
 */

#include"dict.h"

dict* crea_dict(){
  dict *hash;
  hash = (dict*)malloc(sizeof(dict));
  hash->elements = 0;
  hash->root = NULL;
  return hash;
}

int agrega_clave(dict *hash,int key,void *value){
  avl *leaf;
  leaf = _agrega_clave(hash->root,key);
  if(leaf != NULL){
    leaf->value = value;
    hash->elements++;
    return TRUE;
  }
  return FALSE;
}

int es_elemento(dict* hash,int key){
  return _es_elemento(*(hash->root),key);
}

avl* busca_clave(dict* hash,int key){ 
  return _busca_clave(*(hash->root),key);
}

void* elimina_clave(dict *hash,int key){
  void *value;
  avl *p,*s,*g;
  avl *t = busca_clave(hash,key);
  if(t != NULL){
    value = t->value;
    hash->elements--;
    if(t->p != NULL){
      p = t->p;
      s = (p->left == t ? p->right : p->left);
      free(t);
      if(p->p != NULL){
	g = p->p;
	s->p = g;
	if(g->left == p){
	  g->left = s;
	}
	else {
	  g->right = s;
	}
	free(p);
      }
      else {
	s->p = NULL;
	*(hash->root) = s;
      }
    }
    else {
      free(t);
      hash->root = NULL;
    }
  }
}

int es_hoja(avl *tree){
  return (tree->left == NULL ? TRUE : FALSE);
}

int _es_elemento(avl* tree,int key){
  if(es_hoja(tree)){
    return (tree->key == key ? TRUE : FALSE);
  }
  else {
    return _es_elemento((tree->key > key ? tree->left : tree->right),key);
  }
}

avl* _busca_clave(avl* tree,int key){
  if(es_hoja(tree)){
    return (tree->key == key ? tree : NULL);
  }
  else {
    return _busca_clave((tree->key > key ? tree->left : tree->right),key);
  }
}

avl* _agrega_clave(avl** tree,int key){
  avl *t,*left,*right;
  if(tree == NULL){
    tree = (avl**)malloc(sizeof(avl*));
    t = avl_crea_hoja(NULL);
    t->key = key;
    *tree = t;
    return t;
  }
  t = *tree;
  if(!es_hoja(t)){
    return _agrega_clave((key < t->key ? &(t->left) : &(t->right)),key);
  }
  else {
    if(key == t->key){
      return NULL;
    }
    else {
      left = avl_crea_hoja(t);
      right = avl_crea_hoja(t);
      t->left = left;
      t->right = right;
      if(key < t->key){
	left->key = key;
	right->key = t->key;
	right->value = t->value;
	t->value = NULL;
	return left;
      }
      else {
	left->key = t->key;
	left->value = t->value;
	t->value = NULL;
	right->key = key;
	return right;
      }
    }
  }
}

avl* avl_crea_hoja(avl* p){
  avl *leaf;
  leaf = (avl*)malloc(sizeof(avl));
  leaf->left = NULL;
  leaf->right = NULL;
  leaf->p = p;
  return leaf;
}
