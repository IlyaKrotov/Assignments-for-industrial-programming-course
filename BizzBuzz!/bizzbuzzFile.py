import sys
import bizzbuzzFunctions

argString = sys.argv[1]
if len(sys.argv) > 2:
	print "Sorry, you must enter no more than one argument \n"

bizzFile = open(argString, "r")
numberString = bizzFile.readline()
numbersInString, wordsInString, isNumberFirst, isMoreNumbers = bizzbuzzFunctions.argumentParsing(numberString)

resultOfReplacing = []
for number in numbersInString:
	result = bizzbuzzFunctions.mainBizzBuzzFunction(number)
	resultOfReplacing.append(result)

print bizzbuzzFunctions.finalStringIs(resultOfReplacing, numbersInString, wordsInString, isNumberFirst, isMoreNumbers)

