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
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 5, 5, blah);
  addpoint(&qt, 7,8, blah);
  addpoint(&qt, 900,900, blah);
  
  for(int i = 0; i < 10; i++){
    addpoint(&qt, 10+rand()%10, 10+rand()%10, blah);
  }
  
  listpoints(&qt);
  findnearby(&qt,16,16,50);
  maptonearby(&qt,&visit,NULL,16,16,50);

  deletetree(&qt, NULL);
}
