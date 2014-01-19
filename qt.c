#include "quadtree.h"

void visit(pqt_LeafData *data, void *arg)
{
  printf("%s\n",(char *)data->data);
}

int main(void)
{
  
  pqt_QuadTree qt;
  newtree(&qt,3,7,(struct pqt_Extent){0,0,1000,1000});
  char blah [] = "abc";
  char bleh [] = "def";
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 7,8, bleh);
  addpoint(&qt, 900,900, blah);
  
  for(int i = 0; i < 10; i++){
    addpoint(&qt, 10+rand()%10, 10+rand()%10, blah);
  }
  
  
  printf("---\n");
  
  for(int i = 0; i < 10; i++){
    addpoint(&qt, 100, 100, blah);
  }
  addpoint(&qt, 100,100, bleh);
  addpoint(&qt, 101,101, bleh);
  for(int i = 0; i < 10; i++){
    addpoint(&qt, 100, 100, blah);
  }
  
  pqt_Leaf *leaf = findleaf(&qt,100,100);
  printf("leaf size = %d\n",leaf->size);
  leaf = findleaf(&qt,200,200);
  printf("leaf size = %d\n",leaf->size);
  
  movepoint(&qt, 100, 100, 200, 200, bleh);
  
  leaf = findleaf(&qt,100,100);
  printf("leaf size = %d\n",leaf->size);
  leaf = findleaf(&qt,200,200);
  printf("leaf size = %d\n",leaf->size);

  printf("---\n");
  
  //listpoints(&qt);
  findnearby(&qt,16,16,50);
  maptonearby(&qt,&visit,NULL,16,16,50);

  leaf = findleaf(&qt,5,5);
  printf("leaf size = %d\n",leaf->size);
  
  
  
  
  leaf = findleaf(&qt,7,8);
  printf("leaf size = %d\n",leaf->size);
  assert(deletepoint(&qt,7 ,8 ,&bleh));
  printf("leaf size = %d\n",leaf->size);
  
  leaf = findleaf(&qt,900,900);
  printf("leaf size = %d\n",leaf->size);
  assert(deletepoint(&qt,900,900 ,&blah));
  assert(!deletepoint(&qt,900,900 ,&blah)); 
  printf("leaf size = %d\n",leaf->size);

  deletetree(&qt, NULL);
}
