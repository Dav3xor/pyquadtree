#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef struct pqt_Extent   pqt_Extent;
typedef struct pqt_LeafData pqt_LeafData;
typedef struct pqt_Leaf     pqt_Leaf;
typedef struct pqt_Qnode    pqt_Qnode;

typedef void (*pqt_LeafCallback)(pqt_LeafData *, void *arg);

struct pqt_Extent {
  float xmin;
  float ymin;
  float xmax; 
  float ymax;
};

struct pqt_LeafData {
  void *data;
  float         x;
  float         y;
  pqt_LeafData *next;
};

struct pqt_Leaf {
  union { 
    pqt_Qnode     *leaf;
    pqt_LeafData  *payload;
  }contents;

  pqt_Extent       extents;
  unsigned int     size;
};

struct pqt_Qnode {
  pqt_Leaf ul;
  pqt_Leaf ur;
  pqt_Leaf ll;
  pqt_Leaf lr;
  unsigned int depth;
};

typedef struct quadtree {
  pqt_Extent    extents;
  unsigned int  maxsize;
  unsigned int  maxdepth;
  pqt_Qnode    *head;
} pqt_QuadTree;

void newtree(pqt_QuadTree *qt,
             unsigned int  maxsize,
             unsigned int  maxdepth,
             pqt_Extent    extents);

bool addpoint(pqt_QuadTree *qt,
              float         x, 
              float         y, 
              void         *data);

bool movepoint(pqt_QuadTree *qt,
               float oldx, float oldy,
               float newx, float newy,
               void *data);

void listpoints(pqt_QuadTree *qt);

void findnearby(pqt_QuadTree *qt, float x, float y, float radius);

void maptonearby(pqt_QuadTree *qt, pqt_LeafCallback, void *arg,
                 float x, float y, float radius);

pqt_Leaf * findleaf(pqt_QuadTree *qt, float x, float y);

bool deletepoint(pqt_QuadTree *qt,
                 float x, float y,
                 void *data);


void deletetree(pqt_QuadTree *qt, pqt_LeafCallback visitor);




void deleteqnode(pqt_Qnode *node, pqt_LeafCallback visitor);
void deleteleaf(pqt_Leaf *leaf, pqt_LeafCallback visitor);


