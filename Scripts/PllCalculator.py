import sys
import argparse
import math

# Stern-Brocot tree.
# Source: https://stackoverflow.com/questions/5124743/algorithm-for-simplifying-decimal-to-fractions
def float_to_fraction(x, error=0.01):
    n = int(math.floor(x))
    x -= n
    if x < error:
        return (n, 1)
    elif 1 - error < x:
        return (n+1, 1)

    # The lower fraction is 0/1
    lower_n = 0
    lower_d = 1
    # The upper fraction is 1/1
    upper_n = 1
    upper_d = 1

    while True:
        # The middle fraction is (lower_n + upper_n) / (lower_d + upper_d)
        middle_n = lower_n + upper_n
        middle_d = lower_d + upper_d
        # If x + error < middle
        if middle_d * (x + error) < middle_n:
            # middle is our new upper
            upper_n = middle_n
            upper_d = middle_d
        # Else If middle < x - error
        elif middle_n < (x - error) * middle_d:
            # middle is our new lower
            lower_n = middle_n
            lower_d = middle_d
        # Else middle is our best fraction
        else:
            return (n * middle_d + middle_n, middle_d)

def calculate_pll(source, target):
    pllp = 2
    plln = 50
    pllm = 2

    simple_fraction = float_to_fraction(target/source)
    print ("Original fraction: {}/{}".format(simple_fraction[0], simple_fraction[1]))

    fraction = [simple_fraction[0], simple_fraction[1]]
    i = 1
    while (fraction[0] < 50 or fraction[1] < 4 or (fraction[1] % 2 != 0 and fraction[1] % 3 != 0)):
        i += 1
        fraction[0] = simple_fraction[0] * i
        fraction[1] = simple_fraction[1] * i

    if fraction[1] % 2 != 0:
        pllp = 3
    plln = fraction[0]
    pllm = int(fraction[1]/pllp)

    print ("Final fraction: {}/{}".format(fraction[0], fraction[1]))
    print ("Iterations: " + str(i-1))

    vco = source*plln/pllm
    actual = vco/pllp

    pllq = 1
    actual_secondary = 0
    while pllq <= 63:
        pllq += 1
        actual_secondary = vco/pllq
        if actual_secondary <= 48000000:
            break

    print ("Source: {}MHz".format(source/1000000))
    print ("Target: {}MHz".format(target/1000000))
    print ("PLLP: {}".format(pllp))
    print ("PLLN: {}".format(plln))
    print ("PLLM: {}".format(pllm))
    print ("PLLQ: {}".format(pllq))
    print ("VCO: {}MHz".format(vco/1000000))
    print ("Actual {}MHz".format(actual/1000000))
    print ("Actual secondary {}MHz".format(actual_secondary/1000000))

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Calculate PLL register values for given frequency and source.")
    parser.add_argument("-s", "--source", type=int, dest="source", required=True, help="Source frequency in Hz.")
    parser.add_argument("-t", "--target", type=int, dest="target", required=True, help="Target frequency in Hz.")
    arguments = parser.parse_args()

    source = arguments.source
    target = arguments.target
    calculate_pll(source, target)
