pyquadtree
==========

A C extension for doing quad trees (spatial indexing) in Python.

Usage
=====

First, you need to make a new tree:

    import quadtree
    quadtree = quadtree.QuadTree(0,0,1000,1000)

Then, you can add some data at different (x,y) locations:

    quadtree.addpoint(1,1,'abc')
    quadtree.addpoint(1,2,'def')
    quadtree.addpoint(1,3,'ghi')

If you want to find a list of things within 5 units of the location (1,1):

    quadtree.listnearby(1,1,5)   --> ['ghi','def','abc']
    quadtree.listnearby(1,1,2.1) --> ['def','abc']

You can also map a function to points given a location and a radius:

    def fun(arg):
      print arg[0]
    quadtree.maptonearby(fun,1,1,.1) --> (prints) abc

Lastly, you can delete a thing from the quadtree,
given the thing and it's location:

    x = 5
    quadtree.addpoint(5,5,x)
    quadtree.deletepoint(5,5,x)


