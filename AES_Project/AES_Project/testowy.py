from array import array


a = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
b = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
for i in range(0,4):
    for j in range(0,4):
        b[j][i] = a[4*i+j]



def shift (b, i):
    sub= 0
    sub=b[i][3]
    b[i][3] = b[i][0]
    b[i][0] = b[i][1]
    b[i][1] = b[i][2]
    b[i][2] = sub

    return b
print(b)
b= shift(b,1)
print(b)
b = shift(b,2)

b = shift(b,2)
print(b)
b = shift(b,3)

b = shift(b,3)

b = shift(b,3)
print(b)
c=[]

for i in range(0,4):
    for j in range(0,4):
        c.append(b[j][i])
print(c)
