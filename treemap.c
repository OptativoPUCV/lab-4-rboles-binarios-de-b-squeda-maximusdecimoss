#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * map = (TreeMap *) malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;

    map -> root = NULL;
    map -> current = NULL;
    map -> lower_than = lower_than;

    return map;
}

void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (searchTreeMap(tree, key)) return;

    TreeNode *parent = NULL, *current = tree->root;

    while (current) {
        parent = current;
        current = tree->lower_than(key, current->pair->key) ? current->left : current->right;
    }

    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    Pair* newPair = (Pair*)malloc(sizeof(Pair));

    newPair->key = key;
    newPair->value = value;

    newNode->pair = newPair;
    newNode->left = newNode->right = NULL;
    newNode->parent = parent;

    if (!parent) tree->root = newNode;
    else if (tree->lower_than(key, parent->pair->key)) parent->left = newNode;
    else parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x) {
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }

    return x;
}

Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;

    while (current != NULL) {
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            tree->current = current;  
            return current->pair;     
        }
    }

    return NULL;  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

// En esta función, eliminaremos un nodo del árbol binario de búsqueda. Manejaré los tres casos posibles: un nodo sin hijos, con un hijo, o con dos hijos, ajustando los punteros para mantener la estructura del árbol.
void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    // Caso 1: Nodo sin hijos (hoja)
    if (node->left == NULL && node->right == NULL) {
        if (node == tree->root) {
            tree->root = NULL; // Si es la raíz, simplemente vacío el árbol.
        } else {
            // Ajusto el puntero del padre para que no apunte al nodo que voy a eliminar.
            if (node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    // Caso 2: Nodo con un solo hijo
    if (node->left == NULL || node->right == NULL) {
        TreeNode *child = node->left ? node->left : node->right; // Selecciono el hijo no nulo.
        if (node == tree->root) {
            tree->root = child; // Si es la raíz, el hijo se convierte en la nueva raíz.
            child->parent = NULL;
        } else {
            // Conecto el hijo directamente al padre del nodo que elimino.
            if (node->parent->left == node) node->parent->left = child;
            else node->parent->right = child;
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
        return;
    }

    // Caso 3: Nodo con dos hijos
    TreeNode *successor = minimum(node->right); // Busco el sucesor, que es el menor nodo en el subárbol derecho.
    node->pair->key = successor->pair->key; // Copio la clave y el valor del sucesor al nodo actual.
    node->pair->value = successor->pair->value;
    removeNode(tree, successor); // Elimino el sucesor recursivamente, que tiene como mucho un hijo.
}

// En esta función, buscaré el par con la clave más pequeña que sea mayor o igual a la clave dada. Esto es útil para encontrar el "límite superior" de una clave en el árbol.
Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *current = tree->root;
    TreeNode *candidate = NULL;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current; // Si encuentro la clave exacta, la retorno y actualizo current.
            return current->pair;
        } else if (tree->lower_than(key, current->pair->key)) {
            candidate = current; // Este nodo es un posible candidato, ya que su clave es mayor que la buscada.
            current = current->left; // Sigo buscando en el subárbol izquierdo por si hay una clave más cercana.
        } else {
            current = current->right; // La clave del nodo actual es menor, busco en el subárbol derecho.
        }
    }

    if (candidate != NULL) {
        tree->current = candidate; // Si encontré un candidato, lo retorno como el límite superior.
        return candidate->pair;
    }
    return NULL; // No encontré ninguna clave mayor o igual.
}

// En esta función, devolveré el par con la clave más pequeña del árbol, que corresponde al nodo más a la izquierda. Esto marca el inicio de un recorrido en orden.
Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode *min_node = minimum(tree->root); // Uso la función minimum para encontrar el nodo con la clave más pequeña.
    if (min_node == NULL) return NULL;

    tree->current = min_node; // Actualizo current para que apunte al nodo mínimo.
    return min_node->pair;
}

// En esta función, devolveré el siguiente par en orden ascendente respecto al nodo actual. Esto implica encontrar el sucesor del nodo current en el árbol.
Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode *current = tree->current;

    // Caso 1: Si el nodo actual tiene un subárbol derecho, el sucesor es el mínimo de ese subárbol.
    if (current->right != NULL) {
        current = minimum(current->right); // Encuentro el nodo más pequeño en el subárbol derecho.
        tree->current = current;
        return current->pair;
    }

    // Caso 2: Si no hay subárbol derecho, subo por los padres hasta encontrar un ancestro que sea hijo izquierdo.
    while (current->parent != NULL && current == current->parent->right) {
        current = current->parent; // Subo mientras sea hijo derecho, ya que estos nodos ya fueron visitados.
    }

    if (current->parent == NULL) {
        tree->current = NULL; // Si llego a la raíz y no hay sucesor, termino el recorrido.
        return NULL;
    }

    current = current->parent; // El sucesor es el padre que encontré.
    tree->current = current;
    return current->pair;
}