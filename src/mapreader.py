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
emptySpaces = []
foodSpaces = []
hygieneSpaces = []
trafficLightsA = []
trafficLightsB = []
trafficLightsC = []
trafficLightsD = []

x = 0
y = 0
for i in range(len(allPixels)):
  x = i % 256
  if (i != 0 and x == 0):
    y += 1
  if allPixels[i][0] < 128 and allPixels[i][1] < 128 and allPixels[i][2] < 128:
    mapGrid.append(1)
    continue
  if 128 <= allPixels[i][0]:
    mapGrid.append(0)
    emptySpaces.append([x, y])
  if 128 <= allPixels[i][1]:
    emptySpaces.append([x, y])
    foodSpaces.append([x, y])
  if 128 <= allPixels[i][2]:
    emptySpaces.append([x, y])
    hygieneSpaces.append([x, y])
  if allPixels[i][2] > 0 and allPixels[i][2] < 64:
    trafficLightsA.append([x, y])
  if allPixels[i][2] >= 64 and allPixels[i][2] < 128:
    trafficLightsB.append([x, y])
  if allPixels[i][2] >= 128 and allPixels[i][2] < 192:
    trafficLightsC.append([x, y])
  if allPixels[i][2] >= 192:
    trafficLightsD.append([x, y])

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
mapString = "#ifndef MAP_H\n#define MAP_H\n#include <array>\n#include <vector>\n#include <glm/vec2.hpp>\nstatic std::array<int,"+str(len(shuffleGrid))+">mapGrid={"
for i in range(len(allPixels)):
  secNo = i % 64
  if (i != 0 and secNo == 0):
    mapString += '\n'
  mapString += str(shuffleGrid[i])
  if (i != len(allPixels) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(emptySpaces))+"> emptySpaces{\n"
for i in range(len(emptySpaces)):
  mapString += "glm::ivec2(" + str(emptySpaces[i][0]) + ", " + str(emptySpaces[i][1]) + ")\n"
  if (i != len(emptySpaces) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(foodSpaces))+"> foodSpaces{\n"
for i in range(len(foodSpaces)):
  mapString += "glm::ivec2(" + str(foodSpaces[i][0]) + ", " + str(foodSpaces[i][1]) + ")\n"
  if (i != len(foodSpaces) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(hygieneSpaces))+"> hygieneSpaces{\n"
for i in range(len(hygieneSpaces)):
  mapString += "glm::ivec2(" + str(hygieneSpaces[i][0]) + ", " + str(hygieneSpaces[i][1]) + ")\n"
  if (i != len(hygieneSpaces) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<bool, "+str(len(emptySpaces))+"> destinationTaken{\n"
for i in range(len(emptySpaces)):
  mapString += "false\n"
  if (i != len(emptySpaces) - 1):
    mapString += ","
mapString += "};\n"


mapString += "static std::array<glm::ivec2, "+str(len(trafficLightsA))+"> trafficLightsA{\n"
for i in range(len(trafficLightsA)):
  mapString += "glm::ivec2("+str(trafficLightsA[i][0])+", "+str(trafficLightsA[i][1])+")\n"
  if (i != len(trafficLightsA) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(trafficLightsB))+"> trafficLightsB{\n"
for i in range(len(trafficLightsB)):
  mapString += "glm::ivec2("+str(trafficLightsB[i][0])+", "+str(trafficLightsB[i][1])+")\n"
  if (i != len(trafficLightsB) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(trafficLightsC))+"> trafficLightsC{\n"
for i in range(len(trafficLightsC)):
  mapString += "glm::ivec2("+str(trafficLightsC[i][0])+", "+str(trafficLightsC[i][1])+")\n"
  if (i != len(trafficLightsC) - 1):
    mapString += ","
mapString += "};\n"

mapString += "static std::array<glm::ivec2, "+str(len(trafficLightsD))+"> trafficLightsD{\n"
for i in range(len(trafficLightsD)):
  mapString += "glm::ivec2("+str(trafficLightsD[i][0])+", "+str(trafficLightsD[i][1])+")\n"
  if (i != len(trafficLightsD) - 1):
    mapString += ","
mapString += "};\n"

mapString += "\n#endif"

f = open("include/map.h", "w+")
f.write(mapString)
f.close()
