import os
import subprocess

OK = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'
END = '\033[0m'

def generate_architecture():

    os.system("mkdir -p architecture/lundi/mercredi/jeudi")
    os.system("mkdir -p architecture/samedi/vendredi")
    os.system("mkdir link")
    os.system("touch link/InsideSymlink; ln -s -T " + os.getcwd()
               + "/link" + " architecture/symlink")
    extensions = ["txt", "mp3", "py"]
    for i in range(3):
        for ext in extensions:
            os.system("touch architecture/" + chr(i + 97) + '.' +  ext)
            os.system("touch architecture/lundi/mercredi/" + chr(i + 99)
                        + '.' + ext)

def call_program(tests):

    success = 0
    for test in tests:
        success += make_test(test)
    print('[', OK + 'OK' if success is len(tests)
            else WARNING + 'WARNING' if success is not 0
            else FAIL + 'FAIL', end='')
    print(END + ' ]', end = ' ')
    print("Succes %d/%d" % (success, len(tests)), end = '')

def make_test(arg):

    myresult = subprocess.run(["../../myfind", arg],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    findresult = subprocess.run(["find", arg],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    is_success = (myresult.stdout == findresult.stdout) and (myresult.stderr[7:] == findresult.stderr[5:])
    if not is_success:
        print("Fail with '%s'" % arg)
        return 0
    return is_success

def run_test():

    thr_zero = [".", "", "./", "../", "lundi", "samedi", "lundi samedi"]
    call_program(thr_zero)
    print(BOLD + " -  THRESHOLD 0" + END)
    thr_one = ["-d", "-H", "-L", "-P", "-d ../", "-L samedi", "-H symlink", "symlink"]
    call_program(thr_one)
    print(BOLD + " -  THRESHOLD 1" + END)
    thr_two = ["samedi -name '*.mp3'", "-type d", "-type f", "-name '*.mp3'", "-type l"]
    call_program(thr_two)
    print(BOLD + " -  THRESHOLD 2" + END)
    thr_three = ["-name '*.mp3' -o -name '*.txt'", "-name samedi -type f", "-name samedi -a -type d",
                 "-name samedi -o -type f", "-print -a -print", "-print -o -print"]
    call_program(thr_three)
    print(BOLD + " -  THRESHOLD 3" + END)

os.chdir("tests")
generate_architecture()
os.chdir("architecture")
run_test()
