import os, subprocess

path = os.path.abspath(os.path.join(os.getcwd(), os.pardir))
d = ''
with open (path+"/FILENAME/BuildTarget.lua", "r") as file:
    d = file.read()
    if os.path.exists(path+"/premake5.lua"):
        os.remove(path+"/premake5.lua")
    file = open(path+"/premake5.lua", 'w')
    file.write(d)
    file.close