#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

typedef struct Extent   Extent;
typedef struct LeafData LeafData;
typedef struct Leaf     Leaf;
typedef struct Qnode    Qnode;

typedef void (*LeafCallback)(LeafData *, void *arg);

struct Extent {
  float xmin;
  float ymin;
  float xmax; 
  float ymax;
};

struct LeafData {
  void *data;
  float x;
  float y;
  LeafData *next;
};

struct Leaf {
  union { 
    Qnode     *leaf;
    LeafData  *payload;
  }contents;
  Extent extents;
  unsigned int size;
};

struct Qnode {
  Leaf ul;
  Leaf ur;
  Leaf ll;
  Leaf lr;
  unsigned int depth;
};

typedef struct quadtree {
  Extent extents;
  unsigned int maxsize;
  unsigned int maxdepth;
  Qnode *head;
} QuadTree;

void newtree(QuadTree *qt,
             unsigned int maxsize,
             unsigned int maxdepth,
             Extent extents);

bool addpoint(QuadTree *qt,
              float x, float y, 
              void *data);

bool movepoint(QuadTree *qt,
               float oldx, float oldy,
               float newx, float newy,
               void *data);

void listpoints(QuadTree *qt);

void findnearby(QuadTree *qt, float x, float y, float radius);

void maptonearby(QuadTree *qt, LeafCallback, void *arg,
                 float x, float y, float radius);

Leaf * findleaf(QuadTree *qt, float x, float y);

bool deletepoint(QuadTree *qt,
                 float x, float y,
                 void *data);


void deletetree(QuadTree *qt, LeafCallback visitor);




void deleteqnode(Qnode *node, LeafCallback visitor);
void deleteleaf(Leaf *leaf, LeafCallback visitor);


