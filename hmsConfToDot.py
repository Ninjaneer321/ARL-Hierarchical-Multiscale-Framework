#
# generate graphviz dot file containing schematic of HMS setup across
# nodes for the given configuration file
#
# generated dot file is optimized for osage layout:
# python hmsConfToDot.py hmsConf.ini machinefile_algorithm | osage -Tpng -o foo.png

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import SafeConfigParser as ConfigParser

import math
import sys

#
# convert dictionary to sorted list of pairs
#
def dictToList(dictionary):
    return sorted([(key, value) for key, value in dictionary.items()])

#
# parse machinefile and return list containing (hostname, number
# processor) pairs
#
#
# parse machinefile and return list containing (hostname, number
# processor) pairs
#
def parseMachinefile(fileName):
    hosts = {}

    machinefile = open(fileName)
    for line in machinefile.readlines():
        line = line.strip()
        line = line.split()
        host = line[0]
        numHosts = 1
        for token in line[1:]:
            if token.startswith("slots="):
                numHosts = int(token[6:])
        
        if host in hosts:
            hosts[host] += numHosts
        else:
            hosts[host] = numHosts

    hosts = dictToList(hosts)
    return hosts

#
# print header of dot file
#
def printHeader():
    print("digraph hms {")
    print("\tsize = \"100,100\";")
    print("\tnode [shape=box, fontsize=12, style=filled];")
    print("\tpackmode = array_u\n")

#
# style of various node types box
#
algorithmStyle = "[fillcolor = red]"
adaptiveSamplingStyle = "[fillcolor = white]"
brokerStyle = "[fillcolor = yellow]"
modelStyle = "[fillcolor = blue]"

#
# print edge between nodes
#
def printEdge(source, dest):
    print("\t\"" + source + "\" -> \"" + dest + "\"")
    
#
# print footer of dot file
#
def printFooter():
    print("}")

#
# print usage information
#
def printUsage(argv):
    sys.stderr.write(argv[0] + " [hmsConf.ini] [algorithm machinefile]\n")

if __name__ == "__main__":
    
    if len(sys.argv) != 3:
        printUsage(sys.argv)
        sys.exit(-1)
        
    configFileName = sys.argv[1]
    machineFileName = sys.argv[2]

    #
    # parse config file and machinefile to obtain list of resources
    # for algorithm, adaptive sampling, brokers, and models
    #
    configParser = ConfigParser()
    configParser.read(configFileName)
    
    algorithmResources = parseMachinefile(machineFileName)

    adaptiveSamplingHosts = (
        configParser.get("AdaptiveSampling", "Hosts").split())

    brokerHosts = configParser.get("Broker", "Hosts").split()

    brokerResourceList = configParser.get("Broker", "Resources").split()

    brokerResourceTypes = configParser.get("Broker", "ResourceListTypes").split()

    if (len(brokerHosts) != len(brokerResourceList) and
        len(brokerResourceList)  == 1) :
        brokerResourceList = [brokerResourceList[0] for i in brokerHosts]

    if (len(brokerHosts) != len(brokerResourceTypes) and
        len(brokerResourceTypes)  == 1) :
        brokerResourceTypes = [brokerResourceTypes[0] for i in brokerHosts]  
        
    if (len(brokerHosts) != len(brokerResourceList) or 
        len(brokerHosts) != len(brokerResourceTypes)):            
        sys.stderr.write("Error in file - number of broker resource files does "
                         "not equal number of brokers\n")
        sys.exit(-1)

    brokerResources = []
    for resource, resourceType in zip(brokerResourceList, brokerResourceTypes):
        if resourceType == "MPI":
            brokerResources.append(parseMachinefile(resource))
        elif resourceType == "List":
            resourceDict = {}
            for splitResource in resource.split(","):
                if splitResource in resourceDict:
                    resourceDict[splitResource] += 1
                else:
                    resourceDict[splitResource] = 1
            brokerResources.append(dictToList(resourceDict))
        elif resourceType == "Localhost":
            continue
        else:
            sys.stderr.write("Invalid resource type in file.\n")
            sys.exit(-1)
            
    #
    # get all hosts for entire problem
    #
    allHosts = set()

    for algorithmResource in algorithmResources:
        allHosts.add(algorithmResource[0])
    
    for adaptiveSamplingHost in adaptiveSamplingHosts:
        allHosts.add(adaptiveSamplingHost)

    for brokerHost in brokerHosts:
        allHosts.add(brokerHost)

    for brokerResource in brokerResources:
        for resource in brokerResource:
            allHosts.add(resource[0])

    #
    # print header of dot file
    #
    printHeader()

    #
    # print each host as subgraph
    #
    allHosts = sorted(allHosts)
    algIndex = 0
    asIndex = 0
    brokerIndex = 0
    lsIndex = 0
    for index, host in enumerate(allHosts):
        print("\tsubgraph cluster" + str(index) + " {")
        print("\t\tsortv = " + str(index)+ ";")
        print("\t\tpackmode = \"node\";")
        print("\t\tlabel = \"" + host + "\";")
        
        # handle algorithm hosts
        print("\t\tsubgraph cluster" + str(index) + "0 {")
        print("\t\t\tstyle = \"invis\";")
        for algorithmResource in algorithmResources:
            if algorithmResource[0] == host:
                for i in range(0, algorithmResource[1]):
                    print ("\t\t\t\"Algorithm " + str(algIndex) + "\" " +
                           algorithmStyle + " [sortv = " + str(algIndex) + "]")
                    algIndex += 1
        print("\t\t}")

        # handle adaptive sampling hosts
        print("\t\tsubgraph cluster" + str(index) + "1 {")
        print("\t\t\tstyle = \"invis\";")
        print("\t\t\tpackmode = array_u;")
        for adaptiveSamplingHost in adaptiveSamplingHosts:
            if adaptiveSamplingHost == host:
                print ("\t\t\t\"Adaptive Sampling " + str(asIndex) + "\" " +
                       adaptiveSamplingStyle + " [sortv = " + str(asIndex) 
                       + "]")
                asIndex += 1
        print("\t\t}")

        # handle broker hosts
        print("\t\tsubgraph cluster" + str(index) + "2 {")
        print("\t\t\tstyle = \"invis\";")
        print("\t\t\tpackmode = array_u;")
        for brokerHost in brokerHosts:
            if brokerHost == host:
                print ("\t\t\t\"Broker " + str(brokerIndex) + "\" " + 
                       brokerStyle + " [sortv = " + str(brokerIndex) + "]")  
                brokerIndex += 1
        print("\t\t}")

        # handle lower scale models
        print("\t\tsubgraph cluster" + str(index) + "3 {")
        print("\t\t\tstyle = \"invis\";")
        print("\t\t\tpackmode = array_u;")
        for brokerResource in brokerResources:
            for resource in brokerResource:
                if resource[0] == host:
                    for i in range(0, resource[1]):
                        print ("\t\t\t\"Model " + str(lsIndex) + "\" " + 
                               modelStyle + " [sortv = " + str(lsIndex) + "]")
                        lsIndex += 1
        print("\t\t}")

        print("\t}")
    
    #
    # print edges
    #

    numberAlgorithmProcessors = 0
    for algorithmResource in algorithmResources:
        numberAlgorithmProcessors += algorithmResource[1]
    
    #
    # adaptive sampling to brokers
    #
    if len(adaptiveSamplingHosts) > 0:

        #
        # algorithm to adaptive sampling
        #
        if numberAlgorithmProcessors % len(adaptiveSamplingHosts) != 0:
            sys.stderr.write("Cannot equally divide " + 
                             str(numberAlgorithmProcessors) +
                             " algorithm processors among " + 
                             str(len(adaptiveSamplingHosts)) + " adaptive" +
                             " sampling modules - unequal partitioning is" +
                             " currently unsupported in the code\n")
            sys.exit(-1)
        
        numberAlgorithmProcessorsPerAdaptiveSampling = int(
            numberAlgorithmProcessors / len(adaptiveSamplingHosts))

        algIndex = 0
        for algorithmResource in algorithmResources:
            for i in range(0, algorithmResource[1]):
                printEdge("Algorithm " + str(algIndex),
                          ("Adaptive Sampling " + 
                           str(int(algIndex / 
                                   numberAlgorithmProcessorsPerAdaptiveSampling))))
                algIndex += 1

        #
        # adaptive sampling to brokers
        # 
        if len(brokerHosts) % len(adaptiveSamplingHosts) != 0:
            sys.stderr.write("Cannot equally divide " + str(len(brokerHosts)) + 
                             " brokers among " + 
                             str(len(adaptiveSamplingHosts)) + " adaptive" +
                             " sampling modules - unequal partitioning is" +
                             " currently unsupported in the code\n")
            sys.exit(-1)

        numberBrokersPerAdaptiveSampling = int(
            len(brokerHosts) / len(adaptiveSamplingHosts))
        
        brokerIndex = 0
        for i in range(0, len(adaptiveSamplingHosts)):
            for j in range(0, numberBrokersPerAdaptiveSampling):
                printEdge("Adaptive Sampling " + str(i),
                          "Broker " + str(brokerIndex))
                brokerIndex += 1

    else :

        #
        # algorithm to brokers
        #        
        if numberAlgorithmProcessors % len(brokerHosts) != 0:
            sys.stderr.write("Cannot equally divide " + 
                             str(numberAlgorithmProcessors) + " algorithm"
                             " processors among " + str(len(brokerHosts)) + 
                             " brokers - unequal partitioning is currently" +
                             " unsupported in the code\n")
            sys.exit(-1)
        
        numberAlgorithmProcessorsPerBroker = int(
            numberAlgorithmProcessors / len(brokerHosts))

        algIndex = 0
        for algorithmResource in algorithmResources:
            for i in range(0, algorithmResource[1]):
                printEdge("Algorithm " + str(algIndex),
                          ("Broker " + 
                           str(int(algIndex / 
                                    numberAlgorithmProcessorsPerBroker))))
                algIndex += 1

    #
    # brokers to lower scale models
    #
    lsIndex = 0
    for index, brokerResource in enumerate(brokerResources):
        for resource in brokerResource:
            for i in range(0, resource[1]):
                printEdge("Broker " + str(index),
                          "Model " + str(lsIndex))
                lsIndex += 1
        
    #
    # print footer
    #
    printFooter()
