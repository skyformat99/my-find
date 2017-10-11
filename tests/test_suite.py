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
    extensions = ["txt", "mp3", "py"]
    for i in range(3):
        for ext in extensions:
            os.system("touch architecture/" + chr(i + 97) + '.' +  ext)
            os.system("touch architecture/lundi/mercredi/" + chr(i + 99)
                        + '.' + ext)

def call_program(tests):

    print("Running %d tests .." % len(tests))
    success = 0
    for test in tests:
        success += make_test(test)
    print('[' + OK + 'OK' if success is len(tests) 
            else WARNING + 'WARNING' if success is not 0 
            else FAIL + 'FAIL', end='')
    print(END + ']', end = ' ')
    print("Succes %d/%d" % (success, len(tests)))

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

    thr_one = [".", "", "./", "../", "lundi", "samedi", "lundi samedi"]
    print(BOLD + "[*] THRESHOLD 1" + END)
    call_program(thr_one)

os.chdir("tests")
generate_architecture()
os.chdir("architecture")
run_test()
