"""
Carlsim 2.0 Chess Engine
Copyright (C) 2023 Sam Peterson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import itertools
import random
import sys

keywords = ["template", "endtemplate", "set", "if", "else", "elif", "endif", "use"]

templateIDs = {}
templateNames = {}

def parseCommands(code, onlyIfStatements=False, onlyTemplateStatements=False):
    codeBlockID = -1

    prefixCode = ""
    codeBlocks = []
    commands = []

    scope = 0

    for line in code.splitlines():
        stripped = line.strip()

        isCommand = False

        if stripped[0:2] == '/*' and stripped[-2:] == "*/":                
            split = stripped[2:-2].split("<")

            if split[0] == "endtemplate" or split[0] == "endif":
                scope -= 1
            
            if onlyTemplateStatements:
                isCommand = split[0] in ["template", "endtemplate"]
            else:
                isCommand = (scope == 0 and split[0] in ["if", "endif"]) or (scope == 1 and split[0] in ["elif", "else"])
                if not onlyIfStatements:
                    isCommand |= scope == 0 and split[0] in keywords
            
            if isCommand:
                if len(split) == 2:
                    commands.append([split[0], split[1][:-1]])
                else:
                    commands.append([split[0], None])

                codeBlocks.append("")
                codeBlockID += 1
            
            if split[0] == "template" or split[0] == "if":
                scope += 1
        
        if not isCommand:
            if codeBlockID == -1:
                prefixCode += line + "\n"
            else:
                codeBlocks[codeBlockID] += line + "\n"

    return (commands, codeBlocks, prefixCode)

def parseVars(vars):
    vars = vars.split(", ")

    names = []
    values = []

    for var in vars:
        tokens = var.split(":")
        names.append(tokens[0].strip())
        values.append(tokens[1].strip().split(" | "))

    return (names, values)

def expandTemplates(code):
    (commands, codeBlocks, prefixCode) = parseCommands(code, onlyTemplateStatements=True)
    processedCode = prefixCode

    for (i, command) in enumerate(commands):
        if command[0] == "template":
            processedCode += expandTemplate(codeBlocks[i], command[1])
        else:
            processedCode += codeBlocks[i]

    return processedCode

def templateIDExists(templateIDs, templateID):
    for idList in templateIDs:
        for id in idList:
            if id == templateID:
                return True
    
    return False

def generateTemplateID(templateFunctionName, template):
    global templateIDs

    templateID = ''.join(random.choice('0123456789ABCDEF') for n in range(8))

    while templateIDExists(templateIDs, templateID):
        templateID = ''.join(random.choice('0123456789ABCDEF') for n in range(8))

    templateIDs[templateFunctionName][template] = templateID

    return templateID

def expandTemplate(code, templateArgs):
    global templateNames
    global templateIDs

    (names, options) = parseVars(templateArgs)

    templateFunctionName = code.split("(")[0].split()[-1]
    if templateFunctionName not in templateNames:
        templateNames[templateFunctionName] = names
        templateIDs[templateFunctionName] = {}

        for template in itertools.product(*options):
            generateTemplateID(templateFunctionName, template)

    processedCode = ""

    for template in itertools.product(*options):
        templateID = templateIDs[templateFunctionName][template]
        processedCodeBlock = evalIfStatements(code, names, template).replace(templateFunctionName, templateFunctionName + "_t" + templateID, 1)
        processedCode += replaceFunctionCalls(processedCodeBlock, names, template)

    return processedCode

def evalCondition(condition, template, templateNames):
    isMatch = True
    (names, options) = parseVars(condition)

    for (i, name) in enumerate(names):
        valueMatches = False
        value = template[templateNames.index(name)]

        for option in options[i]:
            valueMatches |= (value == option)

        isMatch &= valueMatches
    
    return isMatch

def evalIfStatements(code, names, template):
    (commands, codeBlocks, prefixCode) = parseCommands(code, onlyIfStatements=True)
    processedCode = prefixCode

    i = 0
    while i < len(commands):
        if commands[i][0] == "if" or commands[i][0] == "elif":
            if evalCondition(commands[i][1], template, names):
                processedCode += evalIfStatements(codeBlocks[i], names, template)
                while not commands[i][0] == "endif":
                    i += 1
            else:
                i += 1
        else:
            processedCode += evalIfStatements(codeBlocks[i], names, template)
            i += 1
    
    return processedCode

def processSetCommand(args, functionName, names=None, template=None):
    global templateNames
    global templateIDs


    args = args.split(", ")
    templateValue = [""] * len(args)
    for arg in args:
        tokens = arg.split(": ")
        index = templateNames[functionName].index(tokens[0])
        
        if "$" in tokens[1]:
            tokens = tokens[1].split("$")
            value = template[names.index(tokens[0])]

            if len(tokens) == 2:
                mappings = tokens[1][1:-1].split(" | ")
                for mapping in mappings:
                    mapping = mapping.split(" -> ")
                    if value == mapping[0]:
                        value = mapping[1]
                        break
            
            templateValue[index] = value
        else:
            templateValue[index] = tokens[1]

    print(functionName)
    print(templateIDs[functionName])
    print(templateValue)

    return templateIDs[functionName][tuple(templateValue)]

def replaceFunctionCalls(code, names=None, template=None):
    (commands, codeBlocks, prefixCode) = parseCommands(code)
    processedCode = prefixCode

    functionName = ""

    for (i, command) in enumerate(commands):
        if commands[i][0] == "use":
            functionName = commands[i][1]
            processedCode += codeBlocks[i]
        elif commands[i][0] == "set":
            templateID = processSetCommand(commands[i][1], functionName, names, template) 
            processedCode += codeBlocks[i].replace(functionName + "(", functionName + "_t" + templateID + "(")
    
    return processedCode

if __name__ == "__main__":
    for arg in sys.argv[1:]:
        print(arg)
        open("Processed/" + arg, "w").write(replaceFunctionCalls(expandTemplates(open(arg).read())))