#include "quadtree.h"

float halfway(float min, float max)
{
  return min + ((max-min)/2.0);
}

void newtree(QuadTree *qt, 
             unsigned int maxsize,
             unsigned int maxdepth,
             Extent extents)
{
  qt->head     = NULL;
  qt->maxsize  = maxsize;
  qt->maxdepth = maxdepth;
  qt->extents  = extents;
    
  qt->head = calloc(1,sizeof(Qnode));
  
  if (!qt->head) {     // malloc has thrown a hizzy!
    return;
  }
  
}



#define MOVE_LEAF(to)                       \
  LeafData *next = cur->next;               \
  LeafData *tmp  = to.contents.payload;     \
  to.contents.payload = cur;                \
  cur->next = tmp;                          \
  cur = next;                               \
  to.size++;

#define BUILD_EXTENTS(xmin,ymin,xmax,ymax)  \
  (struct Extent){xmin,ymin,xmax,ymax}      

// called when a leaf has grown too big.  Creates
// a new Qnode (which branches off to 4 new leaves...)
// and places it under the current leaf.
void leafpushdown(Leaf *leaf, Extent extents)
{
  float xmid = halfway(extents.xmin, extents.xmax);
  float ymid = halfway(extents.ymin, extents.ymax);

  Qnode *newnode = malloc(sizeof(Qnode));
  bzero(newnode,sizeof(Qnode));
  LeafData *cur = leaf->contents.payload;
  while(cur) {
    if(cur->x < xmid) {
      if(cur->y < ymid) { // upper left
        MOVE_LEAF(newnode->ul);
      } else { // lower left
        MOVE_LEAF(newnode->ll);
      }
    } else {
      if(cur->y < ymid) { // upper right
        MOVE_LEAF(newnode->ur);
      } else { // lower right
        MOVE_LEAF(newnode->lr);
      }
    }
  }
  leaf->size = 0;
  leaf->contents.leaf = newnode;
}

// add new data element to the current leaf
LeafData *addleafdata(QuadTree *qt, Leaf *leaf, 
                      float x, float y, 
                      unsigned int depth,
                      Extent extents,
                      void *data)
{
  LeafData *tmp     = leaf->contents.payload;
  LeafData *newleaf = malloc(sizeof(LeafData));
  
  if (!(newleaf)){
    return NULL;
  }

  newleaf->data     = data;
  newleaf->x        = x;
  newleaf->y        = y;
  newleaf->next     = tmp;

  leaf->contents.payload = newleaf;  
  leaf->size++;
  if((leaf->size >= qt->maxsize)&&(depth < qt->maxdepth)) {
    leafpushdown(leaf,extents);
  }
  return newleaf;
}

#define FIND_CORNER(curcorner, extents)                       \
if((curcorner.size==0)&&(curcorner.contents.leaf)) {          \
  return(findleafx(curcorner.contents.leaf,extents,x,y));     \
} else {                                                      \
  return(&curcorner);                                         \
}

Leaf *findleafx(Qnode *cur, Extent extents, float x, float y)
{
  float xmid = halfway(extents.xmin, extents.xmax);
  float ymid = halfway(extents.ymin, extents.ymax);
  if(x < xmid) {    // left
    if(y < ymid) {  // upper left
      FIND_CORNER(cur->ul,BUILD_EXTENTS(extents.xmin,extents.ymin,xmid,ymid));
    } else {        // lower left
      FIND_CORNER(cur->ll,BUILD_EXTENTS(extents.xmin,ymid,xmid,extents.ymax));
    }
  } else {          // right
    if(y < ymid) {  // upper right
      FIND_CORNER(cur->ur,BUILD_EXTENTS(xmid,extents.ymin,extents.xmax,ymid));
    } else {        // lower right
      FIND_CORNER(cur->lr,BUILD_EXTENTS(xmid,ymid,extents.xmax,extents.ymax));
    }
  }
}

Leaf *findleaf(QuadTree *qt, float x, float y)
{
  if(!(qt->head)){
    return NULL;
  }
  return findleafx(qt->head, qt->extents, x, y);
}

#define DO_CORNER(curcorner,extents)                                          \
if((curcorner.size==0)&&(curcorner.contents.leaf)) {                          \
  addpointx(qt,curcorner.contents.leaf,x,y,depth,extents,data);               \
} else {                                                                      \
  addleafdata(qt, &curcorner, x, y, depth, extents, data);                    \
}                                                                       

Qnode *addpointx(QuadTree * qt, Qnode *cur,
                 float x, float y, 
                 unsigned int depth,
                 Extent extents,
                 void *data)
{
  depth += 1;
  float xmid = halfway(extents.xmin, extents.xmax);
  float ymid = halfway(extents.ymin, extents.ymax);


  if(x < xmid) {    // left
    if(y < ymid) {  // upper left
      DO_CORNER(cur->ul,BUILD_EXTENTS(extents.xmin,extents.ymin,xmid,ymid));
    } else {        // lower left
      DO_CORNER(cur->ll,BUILD_EXTENTS(extents.xmin,ymid,xmid,extents.ymax));
    }
  } else {          // right
    if(y < ymid) {  // upper right
      DO_CORNER(cur->ur,BUILD_EXTENTS(xmid,extents.ymin,extents.xmax,ymid));
    } else {        // lower right
      DO_CORNER(cur->lr,BUILD_EXTENTS(xmid,ymid,extents.xmax,extents.ymax));
    }
  }
  return cur;
}

void addpoint(QuadTree *qt, 
              float x, float y, 
              void *data)
{
  qt->head = addpointx(qt, qt->head,
                       x, y,
                       0,
                       qt->extents,
                       data);
}

#define PRINT_LEAF(curnode, curcorner,extents)                  \
  printf("leaf (%f,%f) (%f,%f)\n",extents.xmin,extents.ymin,    \
                                  extents.xmax,extents.ymax);   \
  if((curcorner.contents.leaf)&&                                \
     (curcorner.size==0)) {                                     \
    printf("descending\n");                                     \
    listpointsx(curcorner.contents.leaf,extents);               \
  } else if(curcorner.contents.payload) {                       \
    printf("contents:\n");                                      \
    LeafData *curdata = curcorner.contents.payload;             \
    while(curdata){                                             \
      printf("(%4.2f,%4.2f) - %p\n",                            \
             curdata->x,                                        \
             curdata->y,                                        \
             curdata->data);                                    \
      curdata = curdata->next;                                  \
    }                                                           \
  } else {                                                      \
    printf("empty\n");                                          \
  }                                                 


void listpointsx(Qnode *cur, Extent extents)
{
  float xmid = halfway(extents.xmin,extents.xmax); 
  float ymid = halfway(extents.ymin,extents.ymax);
  
  PRINT_LEAF(cur,cur->ul,BUILD_EXTENTS(extents.xmin,extents.ymin,xmid,ymid));
  PRINT_LEAF(cur,cur->ur,BUILD_EXTENTS(xmid,extents.ymin,extents.xmax,ymid));
  PRINT_LEAF(cur,cur->ll,BUILD_EXTENTS(extents.xmin,ymid,xmid,extents.ymax));
  PRINT_LEAF(cur,cur->lr,BUILD_EXTENTS(xmid,ymid,extents.xmax,extents.ymax));
}



void listpoints(QuadTree *qt)
{
  listpointsx(qt->head, qt->extents);
}

bool overlap (Extent ext1, Extent ext2)
{
  return (!(ext1.xmin > ext2.xmax ||
            ext2.xmin > ext1.xmax ||
            ext1.ymin > ext2.ymax ||
            ext2.ymin > ext1.ymax));
}

void testoverlap(void)
{
  assert(!overlap((struct Extent){10,10,20,20}, (struct Extent){30,30,40,40}));
  assert(!overlap((struct Extent){30,30,40,40}, (struct Extent){10,10,20,20}));
  assert( overlap((struct Extent){10,10,20,20}, (struct Extent){0,0,40,40}  ));
  assert( overlap((struct Extent){0,0,40,40},   (struct Extent){10,10,20,20}));
}



float getdistance(float x1,float y1,float x2,float y2) {
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}



#define FIND_IN_LEAF(curleaf,extents)                                     \
  if(overlap(extents,                                                     \
             (struct Extent){x-radius,y-radius,                           \
                             x+radius,y+radius})) {                       \
    if((curleaf.contents.leaf)&&(curleaf.size == 0)) {                    \
      maptonearbyx(curleaf.contents.leaf, visitor, arg,                   \
                   extents,                                               \
                   x, y, radius);                                         \
    } else if ((curleaf.contents.payload)&&(curleaf.size > 0)) {          \
      LeafData *cur = curleaf.contents.payload;                           \
      while(cur) {                                                        \
        if(getdistance(x,y,cur->x,cur->y) <= radius) {                    \
          visitor(cur, arg);                                              \
        }                                                                 \
        cur = cur->next;                                                  \
      }                                                                   \
    }                                                                     \
  } 

void printlocation(LeafData *cur, void *arg)
{
  printf ("found -- (%f,%f)\n",cur->x,cur->y);                 
}

void maptonearbyx(Qnode *node, LeafCallback visitor, void *arg,
                 Extent extents,
                 float x, float y, float radius)
{
  float midx = halfway(extents.xmin,extents.xmax); 
  float midy = halfway(extents.ymin,extents.ymax);
  FIND_IN_LEAF(node->ul,BUILD_EXTENTS(extents.xmin, extents.ymin, midx,         midy));
  FIND_IN_LEAF(node->ur,BUILD_EXTENTS(midx,         extents.ymin, extents.xmax, midy));
  FIND_IN_LEAF(node->ll,BUILD_EXTENTS(extents.xmin, midy,         midx,         extents.ymax));
  FIND_IN_LEAF(node->lr,BUILD_EXTENTS(midx,         midy,         extents.xmax, extents.xmax));
}

void findnearby(QuadTree *qt, float x, float y, float radius)
{
  maptonearbyx(qt->head, &printlocation, NULL, qt->extents, x, y, radius);  
}

void maptonearby(QuadTree *qt, LeafCallback visitor, void *arg,
                 float x, float y, float radius)
{
  maptonearbyx(qt->head, visitor, arg, qt->extents, x, y, radius);  
}

bool deletepoint(QuadTree *qt,
                 float x, float y,
                 void *data)
{
  Leaf *leaf = findleaf(qt, x, y);
  if(leaf->size){
    LeafData *cur = leaf->contents.payload;
    LeafData *prev = NULL;
    while(cur){
      if(cur->data == data){
        if(prev){
          prev->next = cur->next;
        } else {
          leaf->contents.payload = cur->next;
        }
        free(cur);
        leaf->size -= 1;
        return true;
      }
      prev = cur;
      cur  = cur->next;
    }
  }
  return false;
}  

void deleteqnode(Qnode *node, LeafCallback visitor)
{
  deleteleaf(&node->ul,visitor);
  deleteleaf(&node->ur,visitor);
  deleteleaf(&node->ll,visitor);
  deleteleaf(&node->lr,visitor);
  free(node);
}

void deleteleaf(Leaf *leaf, LeafCallback visitor)
{
  if(leaf->size) {
    LeafData *cur = leaf->contents.payload; 
    while(cur){
      LeafData *next = cur->next;
      if(visitor){
        visitor(cur, NULL);
      }
      free(cur);
      cur = next;
    }
  } else if(leaf->contents.leaf){
    deleteqnode(leaf->contents.leaf,visitor);
  }
    
}

void deletetree(QuadTree *qt, LeafCallback visitor)
{
  deleteqnode(qt->head, visitor);
  qt->head = NULL;
}




