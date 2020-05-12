d = {
    "1”": "D(); D();\n",
"2”": "F(); F();\n",
"3”": "R(); R();\n",
"4”": "B(); B();\n",
"5”": "L(); L();\n",
"6”": "U(); U();\n",
    
"1’": "D(false);\n",
"2’": "F(false);\n",
"3’": "R(false);\n",
"4’": "B(false);\n",
"5’": "L(false);\n",
"6’": "U(false);\n",
"1": "D(true);\n",
"2": "F(true);\n",
"3": "R(true);\n",
"4": "B(true);\n",
"5": "L(true);\n",
"6": "U(true);\n"
}

while True:
    s = input()
    for i in d:
        s = s.replace(i, d[i])
    print(s)


