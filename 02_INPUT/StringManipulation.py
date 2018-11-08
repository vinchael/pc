def substring_after(string, delim):
    return string.partition(delim)[2]

def getchar(string, n):
    if len(string) > n:
        return str(string)[n]
    else:
        return ''

def bin_to_dec(bin_val):
    return int(bin_val, 2)
