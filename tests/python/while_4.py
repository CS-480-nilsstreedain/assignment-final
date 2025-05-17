i = 0
v = 0
while True:
    j = 0
    while True:
        v = v + i * j
        j = j + 1
        if j >= i:
            break
    i = i + 1
    if i >= 10:
        break
return_value = v
