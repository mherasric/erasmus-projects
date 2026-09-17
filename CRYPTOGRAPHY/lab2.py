"""
The aim of this algorithm is to Find all bases b (1 < b < n) for which n is a pseudoprime.
The repeated squaring modular exponentiation method it's an efficient method to calculate power of numbers in the
context of modular arithmetic. What we do is decompose the calculus of a^b mod m in steps, where we multiply only the
neccessary part and reduce the size of the numbers in each step.
First, we are going to explain what is the concept of pseudoprimes: A composite number n is a pseudoprime to a base b if
it satisfies Fermat's Little Theorem for b, even though n is not a prime. (b^(n-1) congruent with 1 mod n)
To calculate b^(n-1) mod n efficiently we use the repeated squaring modular exponentiation method because it reduces
significantly the number of multiplies needed
"""

def modular_exponentiation(b, e, m):
    """
    Calculate b^e % m using the repeated squaring method.
    - This function calculates b^e mod m using repeated squaring, which is efficient for large exponents.
    - We iterate through each bit in e(the exponent), squaring the base and reducing it mod m.
    - If the current bit in e is 1, we multiply the result by the base.
    """
    result = 1 #we initialize in 1. That's the value of b^e mod m as it increases the calcullus
    base = b % m  # Ensure base is within the range of modulus (always <m)

    while e > 0:
        if e % 2 == 1: #(e odd)
            result = (result * base) % m #we ensure that only the powers needed will be in the result
        # Square the base and reduce the exponent by half
        base = (base * base) % m
        e //= 2 #we move to next bit in the binary representation of e
        #this will continue until e=0, when the result contains the value of b^e mod m

    return result

def find_pseudoprime_bases(n):
    """
    Find all bases b (1 < b < n) for which n is a pseudoprime.
    - This function iterates through each potential base b from 2 to n-1 and checks if b^n-1 mod n=1.
    - If the condition is met, we add b to the list of bases for which n is a pseudoprime.
    """
    #We have to verify if the number is not prime and odd
    if n % 2 == 0 or is_prime(n):
        return "The number must be odd and composite "

    pseudoprime_bases = []

    # Iterate over all possible bases from 2 to n-1, because b=1 always verifies the condition and b=n it's not a valid case
    for b in range(2, n):
        # Check if b^(n-1) ≡ 1 (mod n)
        if modular_exponentiation(b, n - 1, n) == 1:
            pseudoprime_bases.append(b)

    return pseudoprime_bases

def is_prime(n):
    """
    This function verifies if a number it's prime
    """
    if n <= 1:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

if __name__ == '__main__':
    n = 97  # Replace with any odd composite number
    pseudoprime_bases = find_pseudoprime_bases(n)
    print(f"Bases for which {n} is a pseudoprime: {pseudoprime_bases}")
