from distutils.core import setup, Extension
setup(name="quadtree", version="1.0",
      ext_modules=[Extension("quadtree", ["quadtree.c","pyquadtree.c"])])
