import math
import random

"""
The idea of this algorithm is to generate a secuence of numbers through an iterative function. And we are trying to
detect a collision between two elements of the sequence that allows to find a common factor with n.
The algorithm will follow this steps:
- Inicialitation: we star with an initial number, x0
- Generation of the sequence: We create a sequence using a function, the one that appears in our problem, which will
priduce pseudorandoms numbers
- Compare pairs: Two sequences move at different velocities, one at a simple step and the other at double step. If two
two values coincide, it would indicate a loop
- Calcullus of factors: When we find a difference between both values, the algorithm calculate the gcd of that difference 
and the number n. If the gcd it's not a trivial divisor of n, the algorithm have found a factor.
"""

def pollards_rho(n, f = None):
    """
    Finds a non-trivial factor of n using Pollard's rho algorithm.
    :param n: integer to  be factored
    :param f: function used in the iterative sequence
    :return: int, a non-trivial factor or None if no factor is found
    """

    if f is None:
        f = lambda x: (x**2 + 1) % n

    x = 2 #we initialize the variables
    j = 1

    while True:
        x = f(x) #the function that is going to be the "slowest one"
        x_2j = f(f(x)) #the one that is going to move twice fast

        d = math.gcd(abs(x_2j - x), n)
        list = []

        if 1 < d < n:
            list.append(d) #we found the factor
        elif d == n:
            return None #the algorithm failed

        j += 1 #we increment the counter
    return list


n = 8051  # A composite number
factor = pollards_rho(n)
if factor:
    print(f"A non-trivial factor of {n} is {factor}.")
else:
    print(f"No non-trivial factor found for {n} (likely prime or requires a different function).")
