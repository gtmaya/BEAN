from PIL import Image
import sys
import os

if len(sys.argv) > 1:
  if sys.argv[1].endswith(('.png', '.jpg')):
    if os.path.isfile("images/" + sys.argv[1]):
      i = Image.open("images/" + sys.argv[1])
    else:
      print 'The file "' + sys.argv[1] + '" does not exist.'
      sys.exit(1)
  else:
    print 'The file "' + sys.argv[1] + '" is not a .png or .jpg.'
    sys.exit(1)
else:
  print 'No file given for map.'
  sys.exit(1)

pixels = i.load()
width, height = i.size

if width != 256 and height != 256:
  print 'The file "' + sys.argv[1] + '" does not have dimensions of 256 * 256.'
  sys.exit(1)

#Read data
allPixels = []
for x in range(width):
    for y in range(height):
        cpixel = pixels[x, y]
        allPixels.append(cpixel)

#Convert data
mapGrid = []
for i in range(len(allPixels)):
    if allPixels[i][0] < 128:
      mapGrid.append(1)
      continue
    elif 128 <= allPixels[i][0]:
      mapGrid.append(0)
      continue

#Reshuffle data
shuffleGrid = []
currentSection = 0
for secX in range(32):
  for secY in range(32):
    for x in range(8):
      for y in range(8):
        shuffleGrid.append(mapGrid[y * 256 + secY * 256 * 8 + x + secX * 8])


#Put data into map.h
secNo = 0
mapString = "#ifndef MAP_H\n#define MAP_H\n#include <array>\nstatic std::array<int,"+str(len(shuffleGrid))+">mapGrid={"
for i in range(len(allPixels)):
  secNo = i % 64
  if (i != 0 and secNo == 0):
    mapString += '\n'
  mapString += str(shuffleGrid[i])
  if (i != len(allPixels) - 1):
    mapString += ","
mapString += "};\n#endif"

f = open("include/map.h", "w+")
f.write(mapString)
f.close()