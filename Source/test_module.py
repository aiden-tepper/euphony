def generate_list():
    result = []
    for i in range(26):
        letter = chr(ord('A') + i)
        number = i + 1
        element = f"{letter}{number}"
        result.append(element)
    return result

def generate_helloworld():
    return "Hello World!"