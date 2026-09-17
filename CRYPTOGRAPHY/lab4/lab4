import random
import math


""" Laboratory 4, Problem RSA """

alphabet = ' ABCDEFGHIJKLMNOPQRSTUVWXYZ' #Has length 27

# KEY GENERATION #
def isPrime (n):
    for i in range(2, int(n*0.5)+1):
        if n % i == 0:
            return False
    return True

#Generation of 2 random primes p, q of approximately same size
p = random.randint(10, 100)
while isPrime(p) == False:
    p = random.randint(10, 100)

q = random.randint(10, 100)
while isPrime(q) == False:
    q = random.randint(10, 100)

n = p*q #modulus
phi = (p-1)*(q-1) #euler function

e = random.randint(1, phi) #encryption exponent
while math.gcd(e, phi) != 1:
    e = random.randint(1, phi)

d = 1 #decryption exponent
while((e*d%(phi))!=1):
    d = d+1

K_E = [n, e] #Alice's Public Key
K_D = d #Alice's private key


# ENCRYPTION #
def RSA_encryption(plaintext):
    """
    This function will give us the encryption of any word we want. To get it we set k=2 (plaintext message units are
    blocks of k letters, so we divide our word in groups of 2 letters) and l=3(ciphertext message units are blocks of l
    letters). The plaintext is completed with blanks, when necessary.
    With a loop, we divide the word in blocks of k letters and we write the numerical equivalents(i.e. the letter a
    corresponds with the number 1, then we multiply the first letter by 27(length of the alphabet) and we add the position
    of the second letter to it: al: 1*27+12)
    Then, we have to encrypt every block with m^e mod n.
    And lastly, we write the literal equivalents of the blocks in groups of l letters: With a loop we divide by 27^2,
    the quotient will represent the position of the 1st letter (encrypted), we divide the remainder by 27 and again,
    the quotient will represent the position of the 2n letter, the remainder will represent the 3rd letter.
    :param plaintext: str
    :return: ciphertext str
    """
    pl = plaintext.upper() #we are going to put our word in capital letters because we have defined our alphabet in that way
    length = len(pl)
    num = [] #here we are going to keep all the numerical equivalents
    ciphertext = "" #what we are going to return

    for i in range(0, length, 2): #we put inside our array num all the numerical equivalents
        num.append(alphabet.index(pl[i]) * 27 + alphabet.index(pl[i+1]))

    for i in range(0, len(num)): #encryption (m^e mod n)
        num[i] = (pow(num[i], e)) % n

    for i in range(0, len(num)): #this loop will get us the literal equivalents
        a = divmod(num[i], pow(27, 2))
        result1 = a[0]
        b = divmod(a[1], 27)
        result2 = b[0]
        result3 = b[1]
        #print(result1, result2, result3, '\t') #with this command we can verify everything is running as we want
        ciphertext += alphabet[result1]
        ciphertext += alphabet[result2]
        ciphertext += alphabet[result3]

    return ciphertext

# DECRYPTION #
def RSA_decryption(ciphertext):
    """
    With this function we will be able to decrypt an encrypted message. First, we need to put the numerical equivalent
    for every letter. Then, with a loop, we will do the sum of the product of the number of the first letter by 27^2,
    the second number by 27, and the 3rd number. Then, we have to the decryption by c^d mod n. And lastly, we write the
    literal equivalents by getting every number of the previous step and dividing it by 27, the quotient will be the
    position of the letter and the remainder will be the position of the second letter.
    :param ciphertext: str encrypted
    :return: str original message
    """
    ciph = ciphertext.upper()
    length = len(ciph)
    num = []  # here we are going to keep all the numerical equivalents
    pl = "" #here we will keep the plaintext

    #First we add every letter of the ciphertext as its corresponding number to a new array
    ciph_in_nums = []
    for i in range(0, length):
        ciph_in_nums.append(alphabet.index(ciph[i]))

    #Now we store the numerical equivalents in num
    for i in range(0, len(ciph_in_nums), 3):
        a = ciph_in_nums[i]*pow(27, 2) + ciph_in_nums[i+1]*27 + ciph_in_nums[i+2]
        num.append(a)

    #Now we do the decryption (c^d mod n)
    for i in range(len(num)):
        num[i] = pow(num[i], d) % n

    #We write the literal equivalents and store them in pl
    for i in range(len(num)):
        x = divmod(num[i], 27)
        pl += alphabet[x[0]] + alphabet[x[1]]

    return pl


plaintext = "bab"
encr = RSA_encryption(plaintext)
print(f"The encryption of the plaintext '{plaintext}' is:\t{encr}.")
print(f"The decription of the ciphertext '{encr}' is:\t{RSA_decryption(encr)}.")
