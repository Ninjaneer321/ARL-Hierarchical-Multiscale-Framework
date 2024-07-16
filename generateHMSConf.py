#
# generate hmsConf.ini given machinefile containing total resources and
# number of algorithm processors, adaptive sampling modules, and
# brokers - partitioning resources appropriately
#

try:
    from configparser import ConfigParser
except ImportError:
    from ConfigParser import SafeConfigParser as ConfigParser

import sys

#
# whether to write machinefiles for broker resources
#
isWriteMachinefile = False 

#
# conver list to string without braces
#
def listToString(listToOutput):
    string = ""
    for item in listToOutput:
        string += str(item) + " "
    return string[:-1]
        
#
# convert dictionary to sorted list of pairs
#
def dictToList(dictionary):
    return sorted([(key, value) for key, value in dictionary.items()])

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
# round robin scheduling
#
def roundRobin(hosts, numberProcessors):
    assignedHosts = {}
    for i in range(0, numberProcessors):
        hostIndex = i % len(hosts)
        host = hosts[hostIndex]
        hostName = host[0]
        if hostName in assignedHosts:
            assignedHosts[hostName] += 1
        else:
            assignedHosts[hostName] = 1
        hosts[hostIndex] = (hostName, host[1] - 1)
        
    assignedHosts = dictToList(assignedHosts) 
    return assignedHosts

#
# output machinefile for each host
#
def writeMachinefile(fileName, hosts):
    f = open(fileName, "w")
    for index, host in enumerate(hosts):
        for i in range(0, host[1]):
            f.write(host[0] + "\n")
           
#
# print usage information
#
def printUsage(argv):
    sys.stderr.write(argv[0] + " [hmsConf.ini] " +
                     "[machinefile of all resources] " +
                     "[number algorithm processors] " +
                     "[number adaptive sampling modules] [overload-factor]\n")
    
machinefilePrefix = ""

if __name__ == "__main__":
    if len(sys.argv) != 6:
        printUsage(sys.argv)
        sys.exit(-1)

    configFileName = sys.argv[1]
    machineFileName = sys.argv[2]
    numberAlgorithmProcessors = int(sys.argv[3])
    numberAdaptiveSamplingModules = int(sys.argv[4])
    overloadFactor = int(sys.argv[5])

    #
    # parse machinefile containing all resources
    #
    hosts = parseMachinefile(machineFileName)
    
    #
    # remove hosts for adaptive sampling
    #
    assert(numberAdaptiveSamplingModules < len(hosts))

    adaptiveSamplingHosts = []
    for i in range(0, numberAdaptiveSamplingModules):
        
        #
        # remove node from hosts
        #
        adaptiveSamplingHost = hosts.pop()[0]

        #
        # append to adaptiveSamplingHosts
        #
        adaptiveSamplingHosts.append(adaptiveSamplingHost)

    #
    # compute total number of resources left
    #
    totalNumberResources = 0
    for host, resourceAmount in hosts:
        totalNumberResources += resourceAmount

    #
    # generate algorithm hosts 
    #
    assert(numberAlgorithmProcessors < totalNumberResources) 
    algorithmHosts = roundRobin(hosts, numberAlgorithmProcessors)
    
    #
    # generate broker hosts
    #
    brokerHosts = []
    for i, host in enumerate(hosts):
        host = hosts[i]
        hostName = host[0]
        brokerHosts.append(hostName)
        hosts[i] = (hostName, host[1] - 1)

    #
    # modify hosts for overload factor
    #
    for i, host in enumerate(hosts):
        host = hosts[i]
        hosts[i] = (host[0], host[1] * overloadFactor)

    #
    # output lower scale model machinefiles (for each broker)
    #
    modelResources = []
    for i, host in enumerate(hosts):
        if isWriteMachinefile:
            fileName = "machinefile_broker" + "_" + str(i)
            writeMachinefile(fileName, [host])
            modelResources.append(fileName)
        else:
            hostName = host[0]
            resources = (hostName + ",") * (host[1] - 1) + hostName 
            modelResources.append(resources)
       
    #
    # output upper scale algorithm machinefile
    #
    writeMachinefile("machinefile_algorithm", algorithmHosts)

    #
    # read configuration file
    #
    configParser = ConfigParser()
    configParser.optionxform = str # maintain case
    configParser.read(configFileName)
    configParser.set("AdaptiveSampling",
                     "Hosts",
                     listToString(adaptiveSamplingHosts))
    configParser.set("Broker",
                     "Hosts",
                     listToString(brokerHosts))
    configParser.set("Broker",
                     "Resources",
                     listToString(modelResources))
    configParser.set("Broker",
                     "ResourceTypes",
                     "CPU " * len(brokerHosts))
    if isWriteMachinefile:
        configParser.set("Broker",
                         "ResourceListTypes",
                         "MPI " * len(brokerHosts))
    else:
        configParser.set("Broker",
                         "ResourceListTypes",
                         "List " * len(brokerHosts))
    configParser.write(sys.stdout)
    
    

