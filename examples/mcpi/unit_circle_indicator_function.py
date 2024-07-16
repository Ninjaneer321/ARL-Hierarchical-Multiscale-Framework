import sys

if __name__ == "__main__":

    if len(sys.argv) < 2:
        print("Usage: " + str(sys.argv[0]) + " <input-file-with-point>")
        sys.exit(-1)

    inputFile = sys.argv[1]
    
    f = open(inputFile)
    if not f:
        print("File containing point not found: " + inputFile)
        sys.exit(-1)

    x1 = float(f.readline())
    x2 = float(f.readline())

    squaredDistance = x1*x1 + x2*x2
    if squaredDistance < 1.0:
        print("1")
    else:
        print("0")
