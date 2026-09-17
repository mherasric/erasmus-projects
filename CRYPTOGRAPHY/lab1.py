import random
from random import randint
import time

"""1st Algorithm: Euclidean Algorithm
   We are just going to use the Euclidean Algorithm, given in the lectures from class
"""
def euclidian(x, y):
    while y != 0:
        x, y = y, x % y
    return x

""" 2nd Algorithm:
   first we are going te search all the divisors with a function,
   then we will look for the common ones and lastly, we stick to the bigger one
"""
def finding_divisors(n):
    divisors = [] #the list where we'll keep the divisors
    for i in range(1, n+1): #we start at 0 because we can't divide by 0
        if n % i == 0:
            divisors.append(i)
    return divisors

def gcd_common_divisors(n1, n2):
    d1 = finding_divisors(n1)
    d2 = finding_divisors(n2)

    print(f"The divisors of {n1} are: {d1}\nThe divisors of {n2} are: {d2}")

    common_divisors = [] #here we'll keep all the common divisors for both numbers
    for i in d1:
        if i in d2:
            common_divisors.append(i)

    print(f"The common divisors of both numbers are: {common_divisors}")
    the_gcd = common_divisors[-1]
    return the_gcd


""" 3rd Algorithm:
    In this algorithm I'm going to decompose each number in their prime factors and keep it in a list, then I'll count 
    the times each factor appears in the set of factors. And lastly, I'll compare both sets to see how many factors they
    have in common and I'll keep the minimum of times it appears.
    
"""

def decompose(n):
    prime = [] #I'm going to create a list where I can keep all the factors for a number n
    for i in range(2, n+1):
        while n % i == 0:
            prime.append(i)
            n = n // i
    return prime

def counting_factors(factors):
    """this function will get the frequency of the factors in a list, in other words, it counts
    how many times appears each one.
"""
    count = {}
    for factor in factors:
        if factor in count:
            count[factor] += 1
        else:
            count[factor] = 1
    return count

def gcd_prime_factors(a, b):
    a_factors = decompose(a)
    b_factors = decompose(b)

    a_count = counting_factors(a_factors)
    b_count = counting_factors(b_factors)

    mcd = 1

    for factor in a_count:
        if factor in b_count:
            mcd *= factor ** min(a_count[factor], b_count[factor])

    return mcd

def generate_random_pairs(num_pairs, max_value):
    return [(random.randint(1, max_value), random.randint(1, max_value)) for _ in range(num_pairs)]


if __name__ == '__main__':
    random_pairs = generate_random_pairs(10, 10000)

    print(f"____FIRST ALGORITHM____")
    for a, b in random_pairs:
        start = time.perf_counter_ns()
        gcd1 = euclidian(a, b)
        end = time.perf_counter_ns()
        microsecs = (end-start)/1000
        print(f"The greatest common divisor of the pair ({a},{b}) is {gcd1}. And it took {microsecs} microseconds\n")


    print(f"____SECOND ALGORITHM____")
    for c, d in random_pairs:
        start = time.perf_counter_ns()
        gcd2 = gcd_common_divisors(c, d)
        end = time.perf_counter_ns()
        microsecs = (end - start) / 1000
        milisec = microsecs/1000
        print(f"The greatest common divisor of the pair ({c},{d}) is {gcd2}. And it took {milisec} miliseconds\n")

    print(f"____THIRD ALGORITHM____")
    for e, f in random_pairs:
        start = time.perf_counter_ns()
        print(f"The prime factors of the number {e} are: {decompose(e)}")
        print(f"The prime factors of the number {f} are: {decompose(f)}")
        gcd3 = gcd_prime_factors(e, f)
        end = time.perf_counter_ns()
        microsecs = (end - start) / 1000
        print(f"The greatest common divisor of the pair ({e},{f}) is {gcd3}. And it took {microsecs} microseconds\n")

