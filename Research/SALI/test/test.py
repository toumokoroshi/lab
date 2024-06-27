import math

# mu = 3.003e-6

# q = [1, 0.0001]
# v = [0,0]
# k = -1
# r = [0,0]
# c = 3.000201
# r[0] = math.sqrt((q[0]+mu)**2 + q[1]**2)
# r[1] = math.sqrt((q[0]-1+mu)**2 + q[1]**2)
# V = math.sqrt(q[0]**2 + q[1]**2 +2*(1-mu)/r[0]+2*mu/r[1]  +mu * (1 - mu)-c)
# v[0] = -k*V*q[1]/r[1]
# v[1] = k*V*(q[0]-1+mu)/r[1]

# p=[0,0]
# p[0] = v[0]- q[1]
# p[1] = v[1]+ q[0]

# print(p[0],p[1])
c = (1 - math.pow(2, 1 / 3)) / (2 * (2 - math.pow(2, 1 / 3)))
print(c)