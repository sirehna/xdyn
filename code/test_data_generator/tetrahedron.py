from sympy import *

x, y, z, z1 = symbols('x y z z1')
a, l = symbols('a l', nonnegative = True)

P = lambda i:Matrix([[x + l * cos(2 * i * pi / 3), y + l * sin(2 * i * pi / 3), z1]])

A = P(1)
B = P(2)
C = P(3)
D = Matrix([[x, y, z]]);
n = [(A - B).norm(), (C - B).norm(), (D - B).norm(), (A - C).norm(), (A - C).norm(), (A - D).norm()]

possible_z1 = solve(Eq(n[2], n[3]), z1)
possible_l = solve(Eq(n[1], a), l)

P = map(lambda x:x.subs(z1, possible_z1[1]).subs(l, possible_l[0]), (A, B, C, D))
