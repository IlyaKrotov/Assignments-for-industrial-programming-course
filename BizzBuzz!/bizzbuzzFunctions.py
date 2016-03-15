import sys

def argumentParsing(argString):
	""" This function converts a string to an array of numbers and characters. 
		Example: "123abc456def" -> [['1', '2', '3'], ['4', '5', '6']] and
		[['a', 'b', 'c'], ['d', 'e', 'f']]

	"""
	numbersInString = []
	buff = []
	buffForStrings = []
	wordsInStrings = []

	if list(argString)[0].isdigit():
		isNumberFirst = 1
	else:
		isNumberFirst = 0

	for element in argString:
		if element.isdigit():
			buff.append(element)
			if len(buffForStrings) != 0:
				wordsInStrings.append(buffForStrings)
				buffForStrings = []
		else:
			buffForStrings.append(element)
			if len(buff) != 0:
				numbersInString.append(buff)
				buff = []
	if len(buff) != 0:
		numbersInString.append(buff)
	if len(buffForStrings) != 0:
		wordsInStrings.append(buffForStrings)

	isMoreNumbers = 1
	maxLength = len(numbersInString)
	if len(wordsInStrings) > maxLength:
		maxLength = len(wordsInStrings)
		isMoreNumbers = 0
	elif len(wordsInStrings) == len(numbersInString):
		isMoreNumbers = 2

	if len(numbersInString) == 0:
		result = ""
		for elem in wordsInStrings:
			result += "".join(elem)
		print result
		sys.exit(1)

	return numbersInString, wordsInStrings, isNumberFirst, isMoreNumbers

def modulo3(number):
	""" The function checks the divisibility of 3.
		Example: ['3'] -> "divided"
				 ['2'] -> "not divided"

	"""
	summ = 0
	for n in number:
		summ += int(n)
	if summ > 9:
		modulo3(list(str(summ)))
	elif summ == 0:
		return False
	else:
		if summ == 3 or summ == 6 or summ == 9:
			return True
		

def modulo5(number):
	""" The function checks the divisibility of 5.
		Example: ['5'] -> "divided"
				 ['4'] -> "not divided"

	"""
	return ((number[-1] == "0") or (number[-1] == "5"))
		

def mainBizzBuzzFunction(number):
	""" The function returns "bizzbuzz!" if number divisible by 3 and 5.
		If divisivle only by 3, function returns "bizz".
		If divisivle only by 5, function returns "buzz".
		If number is not divisible by 3 and 5, function returns this number.

	"""
	
	if modulo5(number) == True and modulo3(number) == True:
		return "Bizzbuzz"
	elif modulo3(number) == True:
		return "Bizz"
	elif modulo5(number) == True:
		return "Buzz"
	else:
		return number

def finalStringIs(masRes, numbersInString, wordsInString, isNumberFirst, isMoreNumbers):
	"""The final composition of characters and replaced numbers

	"""
	maxLength = len(numbersInString)
	if len(wordsInString) > maxLength:
		maxLength = len(wordsInString)
	k = []
	resultString = ""

	for i in range(maxLength):
		if (i != (maxLength - 1) and isNumberFirst == 1) or (isMoreNumbers == 2 and isNumberFirst == 1):
			k.append(list(masRes[i]) + wordsInString[i])
		elif isMoreNumbers == 1:
			k.append(list(masRes[i]))
		elif isMoreNumbers == 0:
			k.append(wordsInString[i])
		else:
			k.append(wordsInString[i] + list(masRes[i]))

	for part in k:
		resultString += "".join(part)

	return resultString