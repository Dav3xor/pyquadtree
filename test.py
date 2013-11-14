import quadtree

def f(stuff):
  print stuff

def test():
  q = quadtree.QuadTree(0,0,1000,1000)
  x = "abc"
  q.addpoint(1,1,x)
  q.addpoint(1,4,123)
  q.addpoint(1,5,(1,2,3))
  q.addpoint(1,11,[1,2,3])

  q.maptonearby(f,1,5,20)
  print "--"
  print q.listnearby(1,5,20)
  q.deletepoint(1,1,x)
  print "--"
  print q.listnearby(1,5,20)
  print "--"
  print x
test()
