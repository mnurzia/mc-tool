line = "execute if entity @s[tag=!mcscriptStop] if entity @s[tag=loop] run function examples:mcscript/while6"

STATE_ARG = 0
STATE_SELECTOR = 0

def lexLine(line):
    curtok = ""
    tokens = []
    state = 0
    stack = 0
    for ch in line:
        if state == 0:
            if ch.isspace():
                if len(curtok):
                    tokens.append(curtok)
                curtok = ""
            elif ch == "[":
                stack += 1
                state = 1
                curtok += ch
            elif ch == "{":
                stack += 1
                state = 1
                curtok += ch
            else:
                curtok += ch
        elif state == 1:
            if ch == "]":
                stack -= 1
            elif ch == "[":
                stack += 1
            curtok += ch
            if stack == 0:
                state = 0
        elif state == 2:
            if ch == "}":
                stack -= 1
            elif ch == "{":
                stack += 1
            curtok += ch
            if stack == 0:
                state = 0
    if state == 0:
        if len(curtok):
            tokens.append(curtok)
    return tokens

print(lexLine(line))