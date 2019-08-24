def seed(inp):
    hash = 0
    for char in inp:
        hash = 31 * hash + ord(char)
    return hash
        
print(seed("asd"))
print(seed("asdf"))
print(seed("asdfj"))
print(seed("asdfjj"))