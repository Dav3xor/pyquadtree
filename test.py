import quadtree
import unittest
import sys
from contextlib import contextmanager
from StringIO import StringIO

class TestSequenceFunctions(unittest.TestCase):

  def setUp(self):
    self.quadtree = quadtree.QuadTree(0,0,1000,1000)

  def test_addpoint(self):
    x = "abc"
    self.quadtree.addpoint(1,1,x)
    self.assertEqual(self.quadtree.listnearby(1,1,5),['abc'])

  def test_listnearby(self):
    self.quadtree.addpoint(100,100,"123")
    self.quadtree.addpoint(100,105,"456")
    self.assertEqual(self.quadtree.listnearby(1,50,5),[])
    self.assertEqual(self.quadtree.listnearby(101,101,5),['456','123'])
    self.assertEqual(self.quadtree.listnearby(101,101,3),['123'])
    self.assertEqual(self.quadtree.listnearby(101,106,3),['456'])
    
  def test_mapnearby(self):
    def testmap(stuff):
      stuff.append(11)
    self.quadtree.addpoint(500,500,[1,5,7])
    self.quadtree.maptonearby(testmap,500,500,1)
    self.assertEqual(self.quadtree.listnearby(500,500,1000),[[1,5,7,11]])
  
  def test_deletenearby(self):
    x = 1
    y = 2
    self.quadtree.addpoint(500,500,x)
    self.quadtree.addpoint(500,500,y)
    self.assertEqual(self.quadtree.listnearby(500,500,5),[2,1])
    self.quadtree.deletepoint(500,500,x)
    self.assertEqual(self.quadtree.listnearby(500,500,5),[2])

    
"""
  def test_choice(self):
    element = random.choice(self.seq)
    self.assertTrue(element in self.seq)

  def test_sample(self):
    with self.assertRaises(ValueError):
      random.sample(self.seq, 20)
    for element in random.sample(self.seq, 5):
      self.assertTrue(element in self.seq)
"""
if __name__ == '__main__':
  unittest.main() 

"""
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
"""
