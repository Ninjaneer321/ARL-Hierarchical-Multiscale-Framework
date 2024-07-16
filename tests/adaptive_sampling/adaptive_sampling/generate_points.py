import math
import random

gridSize = 999 
numberPoints = (gridSize + 1) * (gridSize + 1)
h = 2.0 / gridSize

if __name__ == "__main__":
    
    print numberPoints
    
    #points = []

    #for i in range(0, gridSize + 1):
    #    x = -1.0 + i * h
    #    for j in range(0, gridSize + 1):
    #        y = -1.0 + j * h
    #      
    #        points.append((x, y))
            
    #random.shuffle(points)

    #for point in points:
    #    print point[0], point[1]
    for i in range(0, numberPoints):
        #x = random.uniform(-1.0, 1.0)
        #y = random.uniform(-1.0, 1.0)
        
        x = random.gauss(0.0, 1.0/3.0)
        y = random.gauss(math.sin(x*math.pi), 1.0/9.0)

        print x, y
    
