#!/bin/python

JSON_WHITESPACE = [ ' ', '\n', '\t' ]
JSON_SYNTAX = [ '[', ']', '{', '}', ':', ',' ]
JSON_QUOTE = '"'
JSON_NUMBER_CHARS = [ str(d) for d in range(0, 10) ]

def lex_null(inputstr):
    json_null = 'null'
    json_null_len = len(json_null)

    if len(inputstr) >= json_null_len and inputstr[:json_null_len] == json_null:
        rest = inputstr[json_null_len:]
        return None, rest

def lex_bool(inputstr):
    json_true = 'true'
    json_false = 'false'

    json_true_len = len(json_true)
    json_false_len = len(json_false)

    # Check if string is 'true'
    if len(inputstr) >= json_true_len and inputstr[:json_true_len] == json_true:
        rest = inputstr[json_true_len:]
        return True, rest

    # Check if string is 'false'
    if len(inputstr) >= json_false_len and inputstr[:json_false_len] == json_false:
        rest = inputstr[json_false_len:]
        return False, rest

    # string is neither 'true' nor 'false'
    return None, inputstr

def lex_number(inputstr):
    json_number = ''

    # If first c is a number keep adding all following numbers
    for c in inputstr:
        if c in JSON_NUMBER_CHARS:
            json_number += c
        else:
            break

    # If we did not find any number characters
    if not len(json_number):
        return None, inputstr

    # Remove number characters from input
    rest = inputstr[len(json_number):]

    # Return number either as float or as int
    if '.' in json_number:
        return float(json_number), rest
    else:
        return int(json_number), rest

    return None, inputstr

def lex_string(inputstr):
    json_str = ''

    # If input does not start with a quote it is not a string
    if inputstr[0] != JSON_QUOTE:
        return None, inputstr

    # Remove starting quote from input
    inputstr = inputstr[1:]

    # Everything until the closing quote is the json string
    for c in inputstr:
        if c == JSON_QUOTE:
            # Remove string characters and end quote from input
            rest = inputstr[len(json_str) + 1:]
            return json_str, rest
        else:
            json_str += c

    raise Exception('Unexpected end of input wile lexing string')
    #return None, string

def lex(string):
    print("lex()")

    tokens = []

    while len(string):
        # Check if there is a string
        json_string, string = lex_string(string)
        if json_string is not None:
            print('Found string: ' +json_string)
            tokens.append(json_string)
            continue

        # Check if there is a number
        json_number, string = lex_number(string)
        if json_number is not None:
            print('Found number: ' +str(json_number))
            tokens.append(json_number)
            continue

        # Check if there is a bool
        json_bool, string = lex_bool(string)
        if json_bool is not None:
            print('Found bool: ' +str(json_bool))
            tokens.append(json_bool)
            continue

        # Check if there is a NULL value
        json_null, string = lex_bool(string)
        if json_null is not None:
            tokens.append(json_null)
            print('Found NULL: ' +str(json_null))
            continue

        # Skip whitespaces
        if string[0] in JSON_WHITESPACE:
            string = string[1:]
        elif string[0] in JSON_SYNTAX:
            print('Found syntax char: ' +str(string[0]))
            tokens.append(string[0])
            string = string[1:]
        else:
            raise Exception('Unexpected character ' +str(string[0]))


    if tokens is not None:
        print(tokens)


def main():
    print("main()")

    with open("sprites/box.json", "r") as f:
        #print("opened file '" + str(f.name) +"'")
        content = f.read()
        #print("read file '" + str(f.name) +"'")

    #content = '{"foo": [1, 2, {"bar": 2}]}'
    content = """
        {
            "foo":
                [ 1,
                  2,
                  {
                      "bar": 2
                  }
                ]
        },
        {
            "foo": "bazboo"
        },
        ?
    """
    print("content:\n" + content)
    print("\n")
    lex(content)



if __name__ == '__main__':
    main()
