#!/usr/bin/python
import os, sys, getopt, shutil, errno, re, subprocess


def copyFile(src, dst):
    try:
        shutil.copytree(src, dst)
    # Depend what you need here to catch the problem
    except OSError as exc: 
        # File already exist
        if exc.errno == errno.EEXIST:
            shutil.copytree(src, dst)
        # The dirtory does not exist
        if exc.errno == errno.ENOENT:
            shutil.copytree(src, dst)
        else:
            raise

def createProj(name):
    if not name.isalpha():
        print(name + ": name must only contain letters ")
        return
    path = os.path.abspath(os.path.join(os.getcwd(), os.pardir))
    data = path+"/.ProjectData"
    dir = path+"/"+name+"/"+name
    src = dir + "/Source/"
    copyFile(data+"/Android/", src+"Android/")
    copyFile(data+"/iOS/", src+"iOS/")
    copyFile(data+"/Content/", dir+"/Content/")
    copyFile(data+"/Data/", dir+"/Data/")
    copyFile(data+"/AndroidStudio/",  path+"/"+name+"/AndroidStudio")
    shutil.copy2(path+"/.ProjectData/GenerateProjectVS19.bat", path+"/"+name+"/")
    shutil.copy2(path+"/.ProjectData/GenerateProjectVS19_Android.bat", path+"/"+name+"/")
    shutil.copy2(path+"/.ProjectData/GenerateProjectXC4.command", path+"/"+name+"/")
    shutil.copy2(path+"/.ProjectData/GenerateProjectXC4_iOS.command", path+"/"+name+"/")
    shutil.copy2(path+"/.ProjectData/Podfile_ios", path+"/"+name+"/")
    d = ''
    with open (data+"/premake5.lua", "r") as file:
        d = file.read().replace("sandbox", name)
    if os.path.exists(path+"/"+name+"/BuildTarget.lua"):
        os.remove(path+"/"+name+"/BuildTarget.lua")
    file = open(path+"/"+name+"/BuildTarget.lua", 'w')
    file.write(d)
    file.close

    with open(path+"/"+name+"/AndroidStudio/app/src/main/res/values/strings.xml") as file:
        d = file.read().replace("sandbox", name)
    file = open(path+"/"+name+"/AndroidStudio/app/src/main/res/values/strings.xml", 'w')
    file.write(d)
    file.close    

    with open(path+"/"+name+"/AndroidStudio/app/src/main/AndroidManifest.xml") as file:
        d = file.read().replace("sandbox", name)
    file = open(path+"/"+name+"/AndroidStudio/app/src/main/AndroidManifest.xml", 'w')
    file.write(d)
    file.close    

    with open(path+"/"+name+"/AndroidStudio/settings.gradle") as file:
        d = file.read().replace("sandbox", name)
    file = open(path+"/"+name+"/AndroidStudio/settings.gradle", 'w')
    file.write(d)
    file.close    

    with open(path+"/"+name+"/AndroidStudio/app/build.gradle") as file:
        d = file.read().replace("sandbox", name)
    file = open(path+"/"+name+"/AndroidStudio/app/build.gradle", 'w')
    file.write(d)
    file.close    
    

    with open (data+"/ReloadProject.py", "r") as file:
        d = file.read().replace("FILENAME", name)
    file = open(path+"/"+name+"/ReloadProject.py", 'w')
    file.write(d)
    file.close


    with open (data+"/main.cpp", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+"main.cpp", 'w')
    file.write(d)
    file.close

    with open (data+"/MainGameScene.h", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+"MainGameScene.h", 'w')
    file.write(d)
    file.close

    with open (data+"/SplashScreenScene.h", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+"SplashScreenScene.h", 'w')
    file.write(d)
    file.close

    with open (data+"/Sandbox.h", "r") as file:
        d = file.read().replace("Sandbox", name)
        
    file = open(src+name+".h", 'w')
    file.write(d)
    file.close

    with open (data+"/Sandbox.cpp", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+name+".cpp", 'w')
    file.write(d)
    file.close

    with open (data+"/DebugLayer.cpp", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+"DebugLayer.cpp", 'w')
    file.write(d)
    file.close

    with open (data+"/DebugLayer.h", "r") as file:
        d = file.read().replace("Sandbox", name)
    file = open(src+"DebugLayer.h", 'w')
    file.write(d)
    file.close

    with open (data+"/ImGuiLayer.h", "r") as file:
        d = file.read().replace("Sandbox", name)

    file = open(src+"ImGuiLayer.h", 'w')
    file.write(d)
    file.close

    print("Project Created: " + path+"/"+name)


def main(argv):
    name = ''
    if (len(argv) <= 0):
        print ('CreateProject.py --name <name>')
        return
    try:
        opts, argv = getopt.getopt(argv,"",["name="])
    except getopt.GetoptError:
        print ('CreateProject.py --name <name>')
        return
    for opt, arg in opts:
        if opt == '-h':
            print ('CreateProject.py --name <name>')
            return
        elif opt in ("-n", "--name"):
            name = arg
    createProj(name)

if __name__ == "__main__":
   main(sys.argv[1:])




