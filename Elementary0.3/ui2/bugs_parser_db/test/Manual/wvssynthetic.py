import random
file = open("test.data", "w")
n = 10
nations = 87
file.write("list(N=")
file.write(str(n) + ",nations = 87, nation1 = c(")

linebreak = 0
for r in xrange(n):
	file.write(str(random.randint(1,nations)))
	if(r != n-1):
		file.write(", ")
	linebreak = linebreak + 1
	if linebreak == 25 :
		file.write("\n")
		linebreak = 0
	#print "r = " + str(r+1)
file.write("), lifesat = c(")

linebreak = 0
for x in xrange(n):
	file.write(str(random.randint(1,10)))
	if(x != n-1):
		file.write(", ")
	linebreak = linebreak + 1
	if linebreak == 25:
		file.write("\n");
		linebreak = 0
	#print "x = " + str(x+1)

file.write("), proud1 = c(")

linebreak = 0
for y in xrange(n):
	file.write(str(random.randint(1,4)))
	if(y != n-1):
		file.write(", ")
	linebreak = linebreak + 1
	if linebreak == 25:
		file.write("\n");
		linebreak = 0

file.write(")\n")
file.write(")")
file.close()
