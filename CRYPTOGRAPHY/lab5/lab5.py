""" Laboratory 5: Knapsack cryptosystem"""
import random
import math
alphabet = ' ABCDEFGHIJKLMNOPQRSTUVWXYZ' #Has length 27

# KEY GENERATION #
n = 5 # this is going to be the length of the bits

w = [] # here we'll keep a random superincreasing sequence of n positive integers
w.append(random.randint(1, 4)) # first element of our array

for i in range(1, n):
    d = 10

    # We are going to guarantee that we always work in a valid range so we don't have problems with the boundaries
    lower_bound = sum(w) + 1
    upper_bound = lower_bound + d

    w_i = random.randint(lower_bound, upper_bound)

    while w_i <= sum(w):
        d += 100
        upper_bound = lower_bound + d
        w_i = random.randint(lower_bound, upper_bound)

    w.append(w_i)


# Choose a random integer q such that q > sum(w)
q = random.randint(1, 1000)
while q <= sum(w):
    q = random.randint(1, 1000)

# Now, we have to choose another random integer r such that the gcd(r,q) = 1, i.e., r and q are coprime
r = random.randint(1, 1000)
while math.gcd(r, q) != 1:
    r = random.randint(1, 1000)

# And lastly, we create an array B where bi= rwi mod q, this will be the public key
B = []
for i in range(len(w)):
    B.append(r*w[i] % q)

# So, we get the private key (W, q, r)
# print(f"The Public Key is: {B}\nThe Private Key is: ({w},{q},{r})")

# ENCRYPTION #
def knapsack_encryp(m):
    """
    With an n-bit message, where m1 is the highest order bit
    :param m:  n-bit message
    :return: ciphertext (c)
    """
    ciphertext = 0
    m = str(m).zfill(n) # we use zfill(n) because we need to have a n-bit message, which is defined above as n=5, so we would have problems if the message doesn`t have the exact number of bits
    for i in range(n):
        ciphertext += int(m[i])*B[i]
    return ciphertext

# DECRYPTION #
def gcdExtended(a, b):
    if a == 0: # base case
        return b, 0, 1

    gcd, x1, y1 = gcdExtended(b % a, a)

    x = y1 - (b // a) * x1 # Update x and y using results of recursive
    y = x1
    return gcd, x, y


def knapsack_decryption(ciphertext):
    """
    With this function we will be able to decrypt the text we encrypt with the last 'knapscak_encryption'. To get it,
    we need to calculate the modular inverse of r mod(q), then we will have to calculate c'=cr' mod(q) and solve the
    subset sum problem. Lastly, we are going to construct the message m, where we will put a 1 in each xi bit position
    and a 0 otherwise
    :param ciphertext: encrypted text
    :return: original text
    """
    r_ = gcdExtended(r, q)[1] % q # We are going to use the Extended Euclidean Algorithm.
    c_ = (ciphertext * r_) % q

    #now, we solve the subset sum problem
    X = []

    while c_ > 0:
        wj = 0
        for i in range(w.__len__()):
            if w[i] <= c_ and w[i] > wj:
                wj = w[i]
                j = i #we want the index

        c_ -= wj
        X.append(j)

    m = 0
    for i in range(X.__len__()):
        m += pow(2, n-X[i]-1)

    # print(X)
    return bin(m)[2:].zfill(n) # we remove the first two numbers (1b) and we fill with zeros from the left until we get the len n


def word_knapsack_encryp(word):
    """
    This function will get the encryption any word into an array of numbers, using our defined function
    'knapsack_encryp', which works with binary numbers
    :param word:str, any word we want to encrypt
    :return: array of numbers which correspond with the encrypted word
    """
    wd = word.upper() # we need to put it in capital letters because we have defined our alphabet at the bottom of the project like that
    length = len(wd)
    a = [] # here we will keep the encrypted word

    for i in range(0, length):
        # print(alphabet.index(wd[i]))
        a.append(bin(alphabet.index(wd[i]))[2:]) # we get the position of every letter in the alphabet and then we convert that number to its binary number
        # print(bin(alphabet.index(wd[i]))[2:])

    for i in range(a.__len__()):
        a[i] = knapsack_encryp(int(a[i]))
        # print(a[i])

    return a

def word_knapsack_decrypt(a):
    """
    With this function we will be able to get the decryption of the word we have just encrypt, using the defined function
    'knapsack_decryption', which works with arrays and return a binary number. Lastly, we keep every letter that we have
    get from the first loop, in a string (message), removing every space and comma, in order to properly see the word.
    :param a: array of numbers
    :return: decrypted word
    """
    for i in range(len(a)):
        a[i] = knapsack_decryption(a[i])
        a[i] = int(a[i], 2) # we get the decimal number from its binary
        a[i] = alphabet[a[i]] # we get the corresponded letter from the alphabet

    message = ''
    for i in range(len(a)):
        message += a[i]

    return message



our_word = 'navidad '
print(f"We are going to use the Knapsack Cryptosystem to encrypt the word: '{our_word}'")
print(f"The Public Key is: {B}\nThe Private Key is: ({w},{q},{r})")
print(f"So using, the encryption we will get: {word_knapsack_encryp(our_word)}")
ourwd_encrypted = word_knapsack_encryp(our_word)
print(f"But, we can decrypt it again, getting: {word_knapsack_decrypt(ourwd_encrypted)}.")


"""
#This will be an example if we wanted to see how the algorithm works with a number instead of a word:
m=1001
print(f"The encryption of {m} is:\t{knapsack_encryp(m)}")
print(f"The decryption of {knapsack_encryp(m)} is:\t{knapsack_decryption(knapsack_encryp(m))}")
"""
