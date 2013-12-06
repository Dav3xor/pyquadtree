#!/usr/bin/python

import quadtree
import unittest

class TestSequenceFunctions(unittest.TestCase):

  def setUp(self):
    self.quadtree = quadtree.QuadTree(0,0,1000,1000)

  def test_addpoint(self):
    x = "abc"
    self.quadtree.addpoint(1,1,x)
    self.assertEqual(self.quadtree.listnearby(1,1,5),['abc'])
  
  def test_movepoint(self):
    x = "abc"
    self.assertEqual(self.quadtree.listnearby(200,200,1),[])
    self.quadtree.addpoint(200,200,x)
    self.assertEqual(self.quadtree.listnearby(200,200,1),['abc'])
    self.assertEqual(self.quadtree.listnearby(205,205,1),[])
    self.quadtree.movepoint(200,200,205,205,x)
    self.assertEqual(self.quadtree.listnearby(200,200,1),[])
    self.assertEqual(self.quadtree.listnearby(205,205,1),['abc'])

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
  
  def test_deletepoint(self):
    x = 1
    y = 2
    self.quadtree.addpoint(500,500,x)
    self.quadtree.addpoint(500,500,y)
    self.assertEqual(self.quadtree.listnearby(500,500,5),[2,1])
    self.quadtree.deletepoint(500,500,x)
    self.assertEqual(self.quadtree.listnearby(500,500,5),[2])

if __name__ == '__main__':
  unittest.main()

