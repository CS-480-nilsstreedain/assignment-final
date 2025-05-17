i = 0
v = 0
while i < 10:
    j = 0
    while j < i:
        v = v + i * j
        j = j + 1
    i = i + 1
return_value = v
