#include "quadtree.h"

void visit(LeafData *data, void *arg)
{
  printf("%s\n",data->data);
}

int main(void)
{
  
  QuadTree qt;
  newtree(&qt,5,2,(struct Extent){0,0,1000,1000});
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
  
  listpoints(&qt);
  findnearby(&qt,16,16,50);
  maptonearby(&qt,&visit,NULL,16,16,50);

  Leaf *leaf = findleaf(&qt,5,5);
  printf("leaf size = %d\n",leaf->size);
  
  
  
  
  leaf = findleaf(&qt,7,8);
  printf("leaf size = %d\n",leaf->size);
  deletepoint(&qt,7 ,8 ,&bleh); 
  printf("leaf size = %d\n",leaf->size);
  
  leaf = findleaf(&qt,900,900);
  printf("leaf size = %d\n",leaf->size);
  deletepoint(&qt,900,900 ,&blah); 
  deletepoint(&qt,900,900 ,&blah); 
  deletepoint(&qt,900,900 ,&blah); 
  deletepoint(&qt,900,900 ,&blah); 
  deletepoint(&qt,900,900 ,&blah); 
  printf("leaf size = %d\n",leaf->size);

  deletetree(&qt, NULL);
}
